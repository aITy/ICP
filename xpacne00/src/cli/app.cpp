/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"

class CliGame : Game {
  public:
    CliGame() : Game() {};
    bool replayMoveForward();  //FIXME return true if all moves were done
    bool replayMoveBackward();  //FIXME return true if all moves were done
    bool replayMovePause();  //FIXME return true if all moves were done
    bool replayMoveStop();  //FIXME return true if all moves were done
}

App::App(QCoreApplication *_parent) :
  QObject(_parent),
  qtout(stdout, QIODevice::WriteOnly | QIODevice::Text),
  qter(stderr, QIODevice::WriteOnly | QIODevice::Text),
  qtin(stdin, QIODevice::ReadOnly)
{
  //QStringList sl = par->arguments();
  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //qtout << i.toLocal8Bit().constData() << endl;
  //  qtout << (*i).toLocal8Bit().constData() << endl;
  //}

  server = new QTcpServer(this);
  /** we will react on new connections in a user-loop
      connect(server, SIGNAL(newConnection()), this, SLOT(gotConnection())); */

  /** listen on all interfaces; set "random" port (everything else has
      default values - e.g. max connections 30) */
  if (! server->listen(QHostAddress::Any, 0)) {
    eout << "ERR: TCP server not listening." << endl;
    QTimer::singleShot(0, this, SLOT(finished()));
  }

  out << "addr " << server->serverAddress().toString() << " port " <<
    QString::number(server->serverPort()) << endl;

  notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
  /** it does line buffering after \n character => called after one line given
      for immediate reaction use setbuf(stdin, _IOFBF) */
  connect(notifier, SIGNAL(activated(int)), SLOT(handleInput(void)));
  // not needed (notifier is set enabled by default)
  // notifier->setEnabled(true);

  //FIXME
  //Game g = new Game();
}

App::~App() {
  if (server   != NULL) delete server;
  if (notifier != NULL) delete notifier;
}

void App::refresh(void) {
  /** header and content are not needed for user answers */
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
    qtout << "The game is not running..." << endl;
    QString tmp = g.getError();
    if (! tmp.isEmpty())
      qterr << tmp << endl;
    qtout << endl;
  }

  if (g.getState() == Game::STATE_REPLAYING) {
    if (! g.replayMove())
      /** we don't need extra accuracy => rescheduling doesn't matter */
      QTimer::singleShot(500, this, SLOT(refresh()));
    return;
  }

  /* FIXME
  socket->write()
  socket->flush()
  */

  /** in CLI we can handle only 1 game at a time */
  if (! g.isRunning() && server.hasPendingConnections()) {
    g.socket = server.nextPendingConnection();
    for (;;) {
      qtout << "Connection request from " <<
        socket->peerAddress().toString() << ":" <<
        socket->peerPort().toString() << ", accept? [y/n] " << flush;

      /** read one line of user input */
      line = qtin.readLine();
      if (line == "y") {
        QTimer::singleShot(0, this, SLOT(refresh()));
        return;
      }
      else if (line == "n") {
        g.socket->deleteLater();
        //FIXME
        g.socket = NULL;
        QTimer::singleShot(0, this, SLOT(refresh()));
        return;
      }
    }
  }

  qtout << "for help type h" << endl << ">>> ";

  /* FIXME
  QStringListIterator it(cmd_l);
  while (it.hasNext()) {
    out << it.next().toLocal8Bit().constData() << endl;
  }
  */

  if (cmd.at(0) == "h") {
    qtout <<
      "  h                this help" << endl <<
      "  q                quit" << endl <<
      "  aw               alias of white player" << endl <<
      "  ab               alias of black player" << endl <<
      "  n                new game" << endl <<
      "  nn <host> <port> new network game (white)" << endl <<
      "  l <file>         load & play game from file (note it could be local or network game)" << endl <<
      "  r <file>         replay game from file" << endl <<
      "  rt <file>        replay timed game from file" << endl <<
      "  s <file>         save game to file" << endl <<
      "  m c3 b4          droughtsmen/king move" << endl <<
      "  pm c3            show possible moves" << endl <<
      "  hm               help with move (using AI)" << endl <<
      "  <CR>             refresh (needed e.g. for seeing new connection requests)" << endl <<
      "REPLAY COMMANDS" << endl <<
      "  bw               backwards" << endl <<
      "  fw               forwards" << endl <<
      "  p                pause/play" << endl <<
      "  d                set delay" << endl <<
      "  q                quit and close this game" << endl;
  }
  else if (cmd.at(0) == "q") {
    QTimer::singleShot(0, this, SLOT(finished()));
  }
  else if (cmd.at(0) == "aw") {
    g.getPlayerWhite()->name = cmd.at(1);
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "ab") {
    g.getPlayerBlack()->name = cmd.at(1);
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "n") {
    //FIXME does it suffice?
    g.gameLocal();
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "nn") {
    QHostAddress addr;
    if (addr.setAddress(cmd.at(1))) {
      if (! g.gameRemote(addr, std::stoul(cmd.at(2))))
        qterr << "ERR: " << g.getError() << endl;
    }
    else {
      qterr << "ERR: Invalid IP addr/hostname: " << cmd.at(1) << endl;
    }
  }
  else if (cmd.at(0) == "l") {
    //FIXME it can be a network game!
    g.gameFromFile(cmd.at(1));
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "r") {
    //FIXME tady se musi cist stav a chovat se podle toho
    g.gameFromFile(cmd.at(1), REPLAY_STEP);
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "rt") {
    //FIXME handle the replay_t while refreshing
    g.gameFromFile(cmd.at(1), REPLAY_STEP);
    QTimer::singleShot(0, this, SLOT(refresh()));
  }
  else if (cmd.at(0) == "s") {
    //FIXME overwrite file! (maybe inform user?)
    QFile f(cmd.at(1));
    f.write(g.getXmlStr());
    f.close();
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
    qterr << "ERR: Unknown command(s) given. Use `h' for help." << endl;
  }

  //cmd_l.clear(); //FIXME needed?
}

void App::parseLine(void) {
  if (! line.isEmpty()) {
    //QString word;
    //qtin >> word;  // read a word (separated by space)
    cmd_l = line.split(" ", QString::SkipEmptyParts);
    out << "cmd_l.size(): " << cmd_l.size() << endl;
    if (cmd_l.size()) {
      out << "at(0): " << cmd_l.at(0) << endl;
      if (cmd_l.at(0) == "q")
        QTimer::singleShot(0, this, SLOT(finished()));
    }
  }
}

void App::handleInput(void) {
  line = qtin.readLine();
  parseLine();
  QTimer::singleShot(0, this, SLOT(refresh()));
}

void App::gotConnection() {
  // not needed in this case; only for demonstration
  // FIXME
  //QTcpServer* ser = qobject_cast<QTcpServer *>(this->sender());
  QTcpServer* ser = server;

  out << "gotConnection()" << endl;

  if (ser->hasPendingConnections()) {
    out << "some pending connections, connecting..." << endl;
    ser->nextPendingConnection()->deleteLater();
    out << "manually disconnected" << endl;
  }
  else {
    // this could occur when the initiator disconnects really fast
    eout << "no pending connections" << endl;
  }
}
