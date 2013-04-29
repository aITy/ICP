/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

//FIXME remove <iostream> !!!!!
#include "app.h"
#include "../share/board.h"

App::App(QCoreApplication *_parent) : QObject(_parent) {}

void App::run(void) {
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //qtout << i.toLocal8Bit().constData() << endl;
  //  qtout << (*i).toLocal8Bit().constData() << endl;
  //}

  QTcpServer server;

  if (! server.listen(QHostAddress::Any, 0)) {
    qterr << "ERR: TCP server not listening."  << endl;
    Q_EMIT finished();
  }

  QFile file_stdin;
  file_stdin.open(stdin, QIODevice::ReadOnly);
  QTextStream qtin(&file_stdin);

  QFile file_stdout;
  file_stdout.open(stdout, QIODevice::WriteOnly | QIODevice::Text);
  QTextStream qtout(&file_stdout);

  QFile file_stderr;
  file_stderr.open(stderr, QIODevice::WriteOnly | QIODevice::Text);
  QTextStream qterr(&file_stderr);

  QString line;
  Game g;

  for (;;) {
    qtout << "server running on " << std::to_string(port) <<
      " port " << std::to_string(port) << endl;

    qtout << "current game: ";
    if (g.isRunning()) {
      if (g.isLocal())
        qtout << "local";
      else
        qtout << "remote: " <<
          g.getRemoteAddr().toString() << ":" <<
          std::to_string(g.getRemotePort()) << endl;
    }
    else {
      qtout << "none";
    }
    qtout << endl;

    qtout <<
      TERM_FG_MEN_WHITE << "o" << TERM_C_RESET <<
      " kicked out: " << std::to_string(g.getPlayerWhite()->getKickedCount()) <<
      " [" << p1.name << "]" << endl <<
      TERM_FG_MEN_BLACK << "o" << TERM_C_RESET <<
      " kicked out: " << std::to_string(g.getPlayerBlack()->getKickedCount()) <<
      " [" << p1.name << "]" << endl <<
      endl;

    if (g.isRunning()) {
      bool first_run = true;
      /** the top left corner is white */
      bool dim = false;

      for (int i = 0; i < g.board.size(); ++i) {
        /** first column (except of the first header line) starts with index 1-8 */
        if (! first_run)
          qtout << IcpSyntaxParser::intToStrCoord(0, i).second << " ";

        for(int j = 0; j < g.board[i].size(); ++j) {
          /** first line is header with a-h */
          if (first_run)
            qtout << IcpSyntaxParser::intToStrCoord(j, 0).first << endl << endl;

          qtout << (dim) ? TERM_BG_BLACK : TERM_BG_WHITE;

          switch (board[i][j]) {
            case Game::MEN_NONE:
              qtout << " ";
              break;
            case Game::MEN_POSSIBLE_MOVE:
              qtout << TERM_BG_TIP << " " << TERM_C_RESET;
              break;
            case Game::MEN_WHITE:
              qtout << TERM_FG_MEN_WHITE << "o" << TERM_C_RESET;
              break;
            case Game::MEN_WHITE_KING:
              qtout << TERM_FG_KING_WHITE << "?" << TERM_C_RESET;
              break;
            case Game::MEN_BLACK:
              qtout << TERM_FG_MEN_BLACK << "o" << TERM_C_RESET;
              break;
            case Game::MEN_BLACK_KING:
              qtout << TERM_FG_KING_BLACK << "?" << TERM_C_RESET;
              break;
          }

          dim = ! dim;
        }
        qtout << endl;
      }
      qtout << endl;
    }
    else {
      qtout << "The game is not running ..." << endl;
      QString tmp = g.getError();
      if (! tmp.isEmpty())
        qterr << tmp << endl;
      qtout << endl;
    }

    /** using CLI we can handle only 1 game */
    if (! g.isRunning() && server.hasPendingConnections()) {
      g.socket = server.nextPendingConnection();

      for (;;) {
        qtout << "Connection request from " <<
          socket->peerAddress().toString() << ":" <<
          socket->peerPort().toString() << ", accept? [y/n] " << flush;

        line = qtin.readLine();
        if (line == "y") {
          break;
        }
        else if (line == "n") {
          g.socket->deleteLater();
          g.socket = NULL;
          break;
        }
      }
    }

    qtout << "for help type h" << endl << ">>> ";

    /** read one line of user input */
    line = qtin.readLine();
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    QStringList cmd_l = line.split(" ", QString::SkipEmptyParts);

    if (cmd.at(0) == "h") {
      qtout <<
        "  h                this help" << endl <<
        "  q                quit" << endl <<
        "  a                alias of player1" << endl <<
        "  aa               alias of player2" << endl <<
        "  n                new game" << endl <<
        "  nn <host> <port> new network game (white)" << endl <<
        "  l <file>         load & play game from file (note it could be local or network game)" << endl <<
        "  r <file>         replay game from file (note it could be local or network game)" << endl <<
        "  rt <file>        replay timed game from file (note it could be local or network game)" << endl <<
        "  s <file>         save game to file" << endl <<
        "  m c3 b4          droughtsmen/king move" << endl <<
        "  pm c3            show possible moves" << endl <<
        "  hm               help with move (using AI)" << endl <<
        "  <CR>             refresh" << endl;
        qtin.readLine();
    }
    else if (cmd.at(0) == "q") {
      break;
    }
    else if (cmd.at(0) == "a") {
      g.getPlayerWhite()->name = cmd.at(1);
    }
    else if (cmd.at(0) == "aa") {
      g.getPlayerBlack()->name = cmd.at(1);
    }
    else if (cmd.at(0) == "n") {
      g.gameLocal();
    }
    else if (cmd.at(0) == "nn") {
      QHostAddress addr;
      if (addr.setAddress(cmd.at(1))) {
        if (! g.setRemoteHost(addr, std::stoul(cmd.at(2)))) {
          qterr << "ERR: " << g.getError() << endl;
          qtin.readLine();
        }
      }
      else {
        qterr << "ERR: Invalid IP addr: " << strAddr << endl;
        qtin.readLine();
      }
    }
    else if (cmd.at(0) == "l") {
      //FIXME
      QString s = read_full_file(cmd.at(1));
      g.readAndInterpret(s);
    }
    else if (cmd.at(0) == "r") {
      //FIXME
    }
    else if (cmd.at(0) == "rt") {
      //FIXME
    }
    else if (cmd.at(0) == "s") {
      //FIXME overwrite file! (maybe inform user?)
      QFile f(cmd.at(1))
        QString s(g.getXmlStr());
    }
    else if (cmd.at(0) == "m") {
      IcpSyntaxParser::pair_uint_t coord_src =
        IcpSyntaxParser::strCoordToUint(cmd.at(1));
      IcpSyntaxParser::pair_uint_t coord_dst =
        IcpSyntaxParser::strCoordToUint(cmd.at(2));

      if (g.move(coord_src.first, coord_src.second, coord_dst.first,
            coord_dst.second)) {
        qterr << g.getError() << endl;
      }
    }
    else if (cmd.at(0) == "pm") {
      IcpSyntaxParser::pair_uint_t coord =
        IcpSyntaxParser::strCoordToUint(cmd.at(1));
      g.showPossibleMoves(coord.first, coord.second);
    }
    else if (cmd.at(0) == "hm") {
      g.adviceMove();
    }
    // refresh
    else if (cmd.at(0) != "") {
      qterr << "ERR: Unknown command(s) given. Use `h' for help" << endl;
      qtin.readLine();
    }
  }

  //file.close();  // not needed
  Q_EMIT finished();
}
