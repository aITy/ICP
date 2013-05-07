/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"

//FIXME check all methods called from this file if the have at
//  the begining isRunning() or isReplaying() check

void App::prepareNewGame(void) {
  if (g != NULL) delete g;

  g = new Game();
  connect(g, SIGNAL(refresh(void)), this, SLOT(schedule_refresh(void)));
}

App::App(QCoreApplication *_parent) :
  QObject(_parent),
  qtout(stdout, QIODevice::WriteOnly | QIODevice::Text),
  qterr(stderr, QIODevice::WriteOnly | QIODevice::Text),
  qtin(stdin, QIODevice::ReadOnly),
  g(NULL) {

  /** ignore arguments */
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //qtout << i.toLocal8Bit().constData() << endl;
  //  qtout << (*i).toLocal8Bit().constData() << endl;
  //}

  /** simulate that we are waiting for new connection */
  new_conn_handled = true;
  server = new QTcpServer(this);
  /** we will react on new connections in a user-loop */
  connect(server, SIGNAL(newConnection()), this, SLOT(gotConnection()));

  /** listen on all interfaces; set "random" port (everything else has
      default values - e.g. max connections 30) */
  if (! server->listen(QHostAddress::Any, 0)) {
    qterr << "ERR: TCP server not listening." << endl;
    schedule_refresh();
  }

  notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
  /** it does line buffering after \n character => called after one line given
      for immediate reaction use setbuf(stdin, _IOFBF) */
  connect(notifier, SIGNAL(activated(int)), SLOT(handleInput(void)));
  // not needed (notifier is set enabled by default)
  // notifier->setEnabled(true);

  prepareNewGame();
}

App::~App(void) {
  if (g        != NULL) delete g;
  if (server   != NULL) delete server;
  if (notifier != NULL) delete notifier;
}

void App::refresh(void) {
  /** header and content are not needed for user answers */
  qtout << "server running on " << server->serverAddress().toString() <<
    " port " << QString::number(server->serverPort()) << endl;

  qtout << "current game: ";
  if (g->isRunning()) {
    if (g->isLocal())
      qtout << "local";
    else
      qtout << "remote: " <<
        g->getRemoteAddr().toString() << ":" <<
        QString::number(g->getRemotePort()) << endl;
  }
  else {
    qtout << "none";
  }
  qtout << endl;

  qtout <<
    TERM_FG_MEN_WHITE << "o" << TERM_C_RESET <<
    " kicked out: " << QString::number(g->getPlayerWhite()->getKicked()) <<
    " [" << g->getPlayerWhite()->name << "]" << endl <<
    TERM_FG_MEN_BLACK << "o" << TERM_C_RESET <<
    " kicked out: " << QString::number(g->getPlayerBlack()->getKicked()) <<
    " [" << g->getPlayerBlack()->name << "]" << endl <<
    endl;

  if (g->isRunning()) {
    bool first_run = true;

    for (int i = 0; i < g->board.size(); ++i) {
      if (first_run) {
        qtout << "  ";
        /** first line is header with a-h */
        for(int j = 0; j < g->board[i].size(); ++j)
          qtout << IcpSyntaxParser::intToStrCoord(j, 0).first;
        qtout << endl;
        first_run = false;
      }

      /** first column (except of the first header line) starts with index 1-8 */
      qtout << IcpSyntaxParser::intToStrCoord(0, i).second << " ";

      for(int j = 0; j < g->board[i].size(); ++j) {
        if (g->isBlackBox(j, i))
          qtout << TERM_BG_BLACK;
        else
          qtout << TERM_BG_WHITE;

        switch (g->board[i][j]) {
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
      }

      qtout << TERM_BG_BLACK << endl;
    }

    qtout << endl;
  }
  else {
    qtout << "The game is not running..." << endl;
    QString tmp = g->getError();
    if (! tmp.isEmpty())
      qterr << tmp << endl;
    qtout << endl;
  }

  /** in CLI we can handle only 1 game at a time */
  if (! g->isRunning() && ! new_conn_handled) {
    new_conn_handled = true;

    for (;;) {
      qtout << "Connection request from " <<
        g->getRemoteAddr().toString() << ":" <<
        QString::number(g->getRemotePort()) << ", accept? [y/n] " << flush;

      /** read one line of user input */
      line = qtin.readLine();
      if (line == "y") {
        schedule_refresh();
        return;
      }
      else if (line == "n") {
        prepareNewGame();
        schedule_refresh();
        return;
      }
    }
  }

  qtout << "for help type h" << endl << ">>> " << flush;

  if (cmd_l.isEmpty()) {
    /** <CR> => refresh */
  }
  else if (cmd_l.at(0) == "h") {
    qtout << endl <<
      "BASIC COMMANDS" << endl <<
      "  h                     this help" << endl <<
      "  q                     quit game only" << endl <<
      "  q!                    quit program" << endl <<
      "  aw                    alias of white player" << endl <<
      "  ab                    alias of black player" << endl <<
      "  n                     new game" << endl <<
      "  nn <host> <port> w|b  new network game" << endl <<
      "  nc w|b                new game against computer AI (w ~ I want to be" << endl <<
      "                           the white player and AI will be black; b ~ vice versa)" << endl <<
      "  l <file> [w|b]        load & play game from file; w ~ white, b ~ black" << endl <<
      "                          (color choice is mandatory for a network game)" << endl <<
      "  r <file>              replay game from file" << endl <<
      "  s x|i [<file>]        save game to file (x ~ in XML syntax, i ~ in ICP syntax)" << endl <<
      "  m c3 b4               droughtsmen/king move" << endl <<
      "  pm c3                 show possible moves" << endl <<
      "  hm                    help with move (using AI)" << endl <<
      "  <CR>                  refresh (needed e.g. for seeing new connection requests)" << endl <<
      "REPLAY COMMANDS" << endl <<
      "  bw [<N steps>]        backwards (by default 1 step)" << endl <<
      "  fw [<N steps>]        forwards (by default 1 step)" << endl <<
      "  p                     pause/play" << endl <<
      "  d [<delay>]           set delay in ms (if no number given, set to default)" << endl <<
      "  st                    stop running timed replay (if any)" << endl;
  }
  else if (cmd_l.at(0) == "q") {
    prepareNewGame();
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "q!") {
    qtout << endl;
    Q_EMIT finished();
  }
  else if (cmd_l.at(0) == "aw") {
    cmd_l.removeFirst();
    g->getPlayerWhite()->name = cmd_l.join(" ");
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "ab") {
    cmd_l.removeFirst();
    g->getPlayerBlack()->name = cmd_l.join(" ");
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "n") {
    g->gameLocal();
  }
  else if (cmd_l.at(0) == "nn") {
    if (cmd_l.size() == 4) {
      Player::color_t cl = Player::COLOR_DONT_KNOW;

      if (cmd_l.at(3) == "w") {
        cl = Player::COLOR_WHITE;
      }
      else if (cmd_l.at(3) == "b") {
        cl = Player::COLOR_BLACK;
      }
      else {
        qterr << "ERR: Invalid player color: " << cmd_l.at(3) << endl;
        return;
      }

      QHostAddress addr;
      if (addr.setAddress(cmd_l.at(1))) {
        if (! g->gameRemote(addr, QString(cmd_l.at(2)).toUInt(), cl))
          qterr << g->getError() << endl;
      }
      else {
        qterr << "ERR: Invalid IP addr/hostname: " << cmd_l.at(1) << endl;
      }
    }
  }
  else if (cmd_l.at(0) == "nn") {
    if (cmd_l.size() == 2) {
      if (cmd_l.at(1) == "w")
        g->gameLocal(true);
      else if (cmd_l.at(1) == "b")
        g->gameLocal(false);
    }
  }
  else if (cmd_l.at(0) == "l") {
    Player::color_t cl = Player::COLOR_DONT_KNOW;

    if (cmd_l.size() == 3) {
      if (cmd_l.at(2) == "w") {
        cl = Player::COLOR_WHITE;
      }
      else if (cmd_l.at(2) == "b") {
        cl = Player::COLOR_BLACK;
      }
      else {
        qterr << "ERR: Invalid player color: " << cmd_l.at(2) << endl;
        return;
      }
    }

    if (cmd_l.size() == 2 || cmd_l.size() == 3) {
      if (! g->gameFromFile(cmd_l.at(1), cl))
        qterr << g->getError() << endl;
    }
  }
  else if (cmd_l.at(0) == "r") {
    if (cmd_l.size() == 2) {
      if (! g->gameFromFile(cmd_l.at(1), false))
        qterr << g->getError() << endl;
    }
  }
  else if (cmd_l.at(0) == "rt") {
    if (cmd_l.size() == 2) {
      if (! g->gameFromFile(cmd_l.at(1), true))
        qterr << g->getError() << endl;
    }
  }
  else if (cmd_l.at(0) == "s") {
    if (cmd_l.size() >= 2) {
      bool want_xml;

      if (cmd_l.at(1) == "x") {
        want_xml = true;
      }
      else if (cmd_l.at(1) == "i") {
        want_xml = false;
      }
      else {
        qterr << "ERR: Unknown file type specified: " << cmd_l.at(1) << endl;
        return;
      }

      QString fpath;
      cmd_l.removeFirst();
      cmd_l.removeFirst();

      if (! cmd_l.isEmpty()) {
        fpath.append(cmd_l.join(" "));
      }
      else if (! g->getFilePath().isEmpty()) {
        fpath.append(g->getFilePath());
      }
      else {
        qterr << "ERR: No file to save to." << endl;
        return;
      }

      QFile f(fpath);
      /** overwrite file */
      f.open(QIODevice::WriteOnly | QIODevice::Truncate);
      QTextStream fs(&f);
      fs << ((want_xml) ? g->getXmlStr() : g->getIcpSyntaxStr());
      /** tmp gets synced and closed after its destroyal */
      schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "m") {
    if (cmd_l.size() == 3) {
      bool ok;
      IcpSyntaxParser::pair_uint_t coord_src =
        IcpSyntaxParser::strCoordToUInt(cmd_l.at(1), &ok);

      if (! ok)
        qterr << "ERR: bad coordinates given: " << cmd_l.at(1) << endl;

      IcpSyntaxParser::pair_uint_t coord_dst =
        IcpSyntaxParser::strCoordToUInt(cmd_l.at(2), &ok);

      if (! ok)
        qterr << "ERR: bad coordinates given: " << cmd_l.at(1) << endl;

      if (g->move(coord_src.first, coord_src.second,
                  coord_dst.first, coord_dst.second)) {
        qterr << g->getError() << endl;
      }
      else {
        schedule_refresh();
      }
    }
  }
  else if (cmd_l.at(0) == "pm") {
    if (cmd_l.size() == 2) {
      bool ok;
      IcpSyntaxParser::pair_uint_t coord =
        IcpSyntaxParser::strCoordToUInt(cmd_l.at(1), &ok);

      if (! ok)
        qterr << "ERR: bad coordinates given: " << cmd_l.at(1) << endl;

      g->showPossibleMoves(coord.first, coord.second);
      schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "hm") {
    g->adviceMove();
  }
  else if (cmd_l.at(0) == "bw") {
    int steps = 1;

    if (cmd_l.size() == 2) {
      bool ok;
      steps = QString(cmd_l.at(1)).toUInt(&ok);

      if (! ok) {
        qterr << "ERR: Unsigned integer expected." << endl;
        return;
      }
    }

    g->replayMove(steps, true);
  }
  else if (cmd_l.at(0) == "fw") {
    int steps = 1;

    if (cmd_l.size() == 2) {
      bool ok;
      steps = QString(cmd_l.at(1)).toUInt(&ok);

      if (! ok) {
        qterr << "ERR: Unsigned integer expected." << endl;
        return;
      }
    }

    g->replayMove(steps, false);
  }
  else if (cmd_l.at(0) == "p") {
    g->replayMoveToggle();
  }
  else if (cmd_l.at(0) == "d") {
    int timeout = -1;  /** will set the default value */

    if (cmd_l.size() == 2) {
      bool ok;
      timeout = QString(cmd_l.at(1)).toUInt(&ok);

      if (! ok) {
        qterr << "ERR: Unsigned integer expected." << endl;
        return;
      }
    }

    g->setReplayTimeout(timeout);
  }
  else if (cmd_l.at(0) == "st") {
    g->replayMoveStop();
  }
  else {
    qterr << "ERR: Unknown command(s) given. Use `h' for help." << endl;
  }

  /** make sure the current commands will not be processed more than once */
  cmd_l.clear();
}

void App::schedule_refresh(void) {
  QTimer::singleShot(0, this, SLOT(refresh(void)));
}

void App::handleInput(void) {
  line = qtin.readLine();

  if (! line.isEmpty())
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    cmd_l = line.split(" ", QString::SkipEmptyParts);

  schedule_refresh();
}

/** usually this slot must prepare a solely new game, but in CLI, this game
    already exists because we can have only one game at a time */
void App::gotConnection() {
  qDebug("gotConnection()"); //FIXME remove this debug

  /** user did not handled the previous connection */
  if (! new_conn_handled) return;

  if (server->hasPendingConnections())
    g->gameRemote(server->nextPendingConnection());

  /** the user has not handled it yet */
  new_conn_handled = false;
}
