/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"

App::App(QCoreApplication *_parent) :
  QObject(_parent),
  qtout(stdout, QIODevice::WriteOnly | QIODevice::Text),
  qter(stderr, QIODevice::WriteOnly | QIODevice::Text),
  qtin(stdin, QIODevice::ReadOnly)
{
  /** ignore arguments */
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
    schedule_refresh();
  }

  out << "addr " << server->serverAddress().toString() << " port " <<
    QString::number(server->serverPort()) << endl;

  notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
  /** it does line buffering after \n character => called after one line given
      for immediate reaction use setbuf(stdin, _IOFBF) */
  connect(notifier, SIGNAL(activated(int)), SLOT(handleInput(void)));
  // not needed (notifier is set enabled by default)
  // notifier->setEnabled(true);

  g = new Game();
  connect(g, SIGNAL(refresh(void)), this, SLOT(schedule_refresh(void)));
}

App::~App() {
  if (g        != NULL) delete g;
  if (server   != NULL) delete server;
  if (notifier != NULL) delete notifier;
}

void App::refresh(void) {
  /** header and content are not needed for user answers */
  qtout << "server running on " << QString::number(port) <<
    " port " << QString::number(port) << endl;

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
    " kicked out: " << QString::number(g->getPlayerWhite()->getKickedCount()) <<
    " [" << p1.name << "]" << endl <<
    TERM_FG_MEN_BLACK << "o" << TERM_C_RESET <<
    " kicked out: " << QString::number(g->getPlayerBlack()->getKickedCount()) <<
    " [" << p1.name << "]" << endl <<
    endl;

  if (g->isRunning()) {
    bool first_run = true;
    /** the top left corner is white */
    bool dim = false;

    for (int i = 0; i < g->board.size(); ++i) {
      /** first column (except of the first header line) starts with index 1-8 */
      if (! first_run)
        qtout << IcpSyntaxParser::intToStrCoord(0, i).second << " ";

      for(int j = 0; j < g->board[i].size(); ++j) {
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
    qtout << "The game is not running->.." << endl;
    QString tmp = g->getError();
    if (! tmp.isEmpty())
      qterr << tmp << endl;
    qtout << endl;
  }

  /** in CLI we can handle only 1 game at a time */
  if (! g->isRunning() && server.hasPendingConnections()) {
    g->socket = server.nextPendingConnection();
    for (;) {
      qtout << "Connection request from " <<
        socket->peerAddress().toString() << ":" <<
        socket->peerPort().toString() << ", accept? [y/n] " << flush;

      /** read one line of user input */
      line = qtin.readLine();
      if (line == "y") {
        schedule_refresh();
        return;
      }
      else if (line == "n") {
        g->socket->deleteLater();
        //FIXME
        g->socket = NULL;
        schedule_refresh();
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

  if (cmd_l.at(0) == "h") {
    qtout <<
      "BASIC COMMANDS" << endl <<
      "  h                this help" << endl <<
      "  q                quit" << endl <<
      "  aw               alias of white player" << endl <<
      "  ab               alias of black player" << endl <<
      "  n                new game" << endl <<
      "  nn <host> <port> new network game (white)" << endl <<
      "  l <file>         load & play game from file (note it could be local or network game)" << endl <<
      "  r <file>         replay game from file" << endl <<
      "  rt <file>        replay timed game from file" << endl <<
      "  s [<file>]       save game to file" << endl <<
      "  m c3 b4          droughtsmen/king move" << endl <<
      "  pm c3            show possible moves" << endl <<
      "  hm               help with move (using AI)" << endl <<
      "  <CR>             refresh (needed e.g-> for seeing new connection requests)" << endl <<
      "REPLAY COMMANDS" << endl <<
      "  bw               backwards" << endl <<
      "  fw               forwards" << endl <<
      "  p                pause/play" << endl <<
      "  d                set delay" << endl <<
      "  q                quit and close this game" << endl;
  }
  else if (cmd_l.at(0) == "q") {
    /** are we in the replay-mode? */
    if (g->isReplaying())
      delete g;
    else
      QTimer::singleShot(0, this, SLOT(finished(void)));
  }
  else if (cmd_l.at(0) == "aw") {
    g->getPlayerWhite()->name = cmd_l.at(1);
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "ab") {
    g->getPlayerBlack()->name = cmd_l.at(1);
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "n") {
    g->gameLocal();
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "nn") {
    if (cmd_l.size() == 3) {
      QHostAddress addr;
      if (addr.setAddress(cmd_l.at(1))) {
        if (! g->gameRemote(addr, std::stoul(cmd_l.at(2))))
          qterr << "ERR: " << g->getError() << endl;
      }
      else {
        qterr << "ERR: Invalid IP addr/hostname: " << cmd_l.at(1) << endl;
      }
    }
  }
  else if (cmd_l.at(0) == "l") {
    if (cmd_l.size() == 2) {
      if (! g->gameFromFile(cmd_l.at(1)))
        qterr << "ERR: " << g->getError() << endl;
      schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "r") {
    if (cmd_l.size() == 2) {
      if (! g->gameFromFile(cmd_l.at(1), REPLAY_STEP))
        qterr << "ERR: " << g->getError() << endl;
      schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "rt") {
    if (cmd_l.size() == 2) {
      if (! g->gameFromFile(cmd_l.at(1), REPLAY_TIMED))
        qterr << "ERR: " << g->getError() << endl;
      schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "s") {
    if (cmd_l.size() == 2) {
      QFile f(cmd_l.at(1));
    }
    else if (! g->getFilePath().isEmpty()) {
      QFile f(g->getFilePath());
    }
    else {
      qterr << "ERR: No file to save to." << endl;
      return;
    }

    /** overwrite file */
    QTextStream fs(f, QIODevice::WriteOnly | QIODevice::Truncate);
    fs << g->getXmlStr();
    /** f gets synced and closed after destroying it */
    schedule_refresh();
  }
  else if (cmd_l.at(0) == "m") {
    if (cmd_l.size() == 3) {
      IcpSyntaxParser::pair_uint_t coord_src =
        IcpSyntaxParser::strCoordToUint(cmd_l.at(1));
      IcpSyntaxParser::pair_uint_t coord_dst =
        IcpSyntaxParser::strCoordToUint(cmd_l.at(2));

      if (g->move(coord_src.first, coord_src.second, coord_dst.first,
            coord_dst.second)) {
        qterr << g->getError() << endl;
      }
      else {
        schedule_refresh();
      }
    }
  }
  else if (cmd_l.at(0) == "pm") {
    if (cmd_l.size() == 2) {
    IcpSyntaxParser::pair_uint_t coord =
      IcpSyntaxParser::strCoordToUint(cmd_l.at(1));
    g->showPossibleMoves(coord.first, coord.second);
    schedule_refresh();
    }
  }
  else if (cmd_l.at(0) == "hm") {
    g->adviceMove();
    schedule_refresh();
  }
  /** <CR> => refresh */
  else if (! cmd_l.at(0).isEmpty()) {
    qterr << "ERR: Unknown command(s) given. Use `h' for help." << endl;
  }

  //cmd_l.clear(); //FIXME needed?
}

App::schedule_refresh(void) {
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

void App::gotConnection() {
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
