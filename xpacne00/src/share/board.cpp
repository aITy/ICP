/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

Player::Player(bool _is_black; QString &_name) :
  is_black(_is_black), name(_name), kicked(0) {}

int Player::getKickedCount() {
  return kicked;
}

void Game::initXml() {
  // FIXME read and use XML in Qt
  // http://www.qtforum.org/article/27624/how-to-read-a-xml-file.html

  if (! doc->setContent(
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
      "<!-- standalone because we have no DTD for it -->"
      "<draughts version=\"1.0\">"
      "<game type=\"network/local\" ?host=\"hostname/IPv4/IPv6\">"
      "<players>"
      // FIXME substitute XML characters
      "<black>" << player_black.name << "</black>"
      "<white>" << player_white.name << "</white>"
      "</players>"
      "<moves type=\"XML/ICP\">"
      "</moves>"
      "</draughts>"))
    qDebug() << "setContent() failed";

  // QDomProcessingInstruction header = doc->createProcessingInstruction();
  // QDomComment comment = doc->createComment();
  // QDomElement draughts = doc->createElement();
  // QDomAttr attr = doc->createAttribute();
  // QDomText text_content = doc->createTextNode(p.name);
}

void Game::appendMoveToXml(Player &p, unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty, kicked = false) {
  QDomElement el_move = doc->createElement("move");

  attr = doc->createAttribute("player");
  attr.setValue("black");
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("srcx");
  attr.setValue(std::stoul(srcx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("srcy");
  attr.setValue(std::stoul(srcy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("dstx");
  attr.setValue(std::stoul(dsty));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("dsty");
  attr.setValue(std::stoul(dsty));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("kicked");
  attr.setValue(std::stoul(dsty));
  el_move.setAttributeNode(attr);

  // how to traverse DOM
  // http://qt-project.org/forums/viewthread/13723
  doc->elementsByTagName("moves").at(0).appendChild(el_move);
  //doc->elementsByTagName("moves").at(0).toElement().appendChild(el_move);
}

void Game::Game(void) : socket(NULL), game_state(state_pre_init) {
  player_white = new Player();
  player_black = new Player();
  doc = new QDomDocument("draughts-document");

  initXml();
  //FIXME initialize the board with mens

  game_state = ...;
}

void Game::~Game() {
  if (doc          != NULL)
    free(doc);
  if (socket       != NULL && socket->parent() == this)
    free(socket);
  if (player_white != NULL)
    free(player_white);
  if (player_black != NULL)
    free(player_black);
  if (filepath     != NULL)
    free(filepath);
}

void Game::gameLocal() {
  if (game_state != STATE_PRE_INIT)
    //FIXME
    // next move will have the white player
    game_state = STATE_CAN_START;
}

/** we are founding a completely new game */
void Game::gameRemote(QHostAddress addr, int port) {
  Q_ASSERT(socket == NULL);
  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()),
      this, SLOT(gotConnected()));
  connect(socket, SIGNAL(disconnected()),
      this, SLOT(gotDisconnected()));

  //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
  //    SLOT(gotError(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(gotDisconnected()));
  connect(socket, SIGNAL(readyRead()),
      this, SLOT(handleReply()));

  socket->connectToHost(addr, port);

  socket->write("");
}

/** we are connecting to a "running" game */
void Game::gameRemote(QTcpSocket *socket) {
  /** handle newly arrived msg */
  connect(socket, SIGNAL(readyRead()), this, SLOT(readReply()));
  /** handle disconnect */
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
}

/** new game from file => jump to end (interpret moves between, of course) */
void Game::gameFromFile(QString s) {
  QFile f(filename);

  if (! f.open(QIODevice::ReadOnly)) return;

  /** try to parse the file as XML */
  if (! doc->setContent(&f)) {
    /** try to parse the file as ICP syntax */
    foreach (line, )
      .close();
  }
  f.close();
}

/** replay loaded game */
void Game::gameFromFile(QString s, replay_t replay) {
  if (replay == REPLAY_TIMED) {
    lockUserInput();
  }
  //FIXME
  t
}

bool Game::isRunning() {
  return game_state != state_pre_init &&
         game_state != state_end;
}

bool Game::isLocal() {
  return socket == NULL;
}

err_t Game::move(unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty) {
  /** check boundaries */
  if (! (_x >= 0 && _x < board[0].size() && _y >= 0 && _y < board.size()) )
    return ERR_INVALID_MOVE;

  /** check presence */
  if (board[y][x] == MEN_NONE)
    return false;

  /** return if the previous move wasn't complete */
  if ((srcx != last_move_dst.first || srcy != last_move_dst.second) &&
      //FIXME implement somehow
      showPossibleMoves(last_move_dst.first, last_move_dst.second)) {
    hidePossibleMoves();
    return ERR_PREV_MOVE_NOT_FINISHED;
  }

  hidePossibleMoves();

  //FIXME
  /** check
   *   the presence of some draughtsmen
   *   the ownership of the draughtsmen (must conform to the player)
   *   the distance in conjunction with jumping over
   *   the placement (can be only black square)
   */
  // a1 == cerna barva!
  // hraje se pouze na cernych
  // kamen jen dopredu; jen po diagonale
  // dama muze i dozadu

  //kamen;
  //  t
  //dama;
  //  assert;
  //    dst musi byt volne
  //    musi byt na 1 ze 4 diagonal
  //    na diagonale mezi musi byt pouze souperovy kameny
  //  move(dst)
  //  foreach diagonal in 4_possible_diagonals {
  //    if t
  //  }

  //FIXME howto update QDomDocument
  // http://www.qtcentre.org/threads/12313-remove-node-in-xml-file

  //diagonal -> the diff(x) and diff(y) must be same for src and dst
  board[dstx][dsty] = board[srcx][srcy];
  board[srcx][srcy] = MEN_NONE;
  appendMoveToXml(srcx, srcy, dstx, dsty);

  last_move_dst.first  = dstx;
  last_move_dst.second = dsty;

  return ERR_OK;
}

/**
 * right now it can only show diagonals (i.e. no AI)
 * @return true if some jump is possible
 */
bool showPossibleMoves(unsigned int _x, unsigned int _y) {
  /** check boundaries */
  if (! (_x >= 0 && _x < board[0].size() && _y >= 0 && _y < board.size()) )
    return false;

  hidePossibleMoves();

  /** check presence */
  if (board[y][x] == MEN_NONE)
    return false;

  int x = _x;
  int y = _y;
  int i = 1;

  if (board[y][x] == MEN_BLACK_KING || board[y][x] == MEN_WHITE_KING) {
    /** indicators if the diagonal can be processed further */
    bool tl = true, tr = true, bl = true, br = true;

    //FIXME
    ///** gradually increase the "ring" */
    //while (tl || tr || bl || br) {
    //  if (tl && x - i >= 0 && y - i >= 0 &&
    //      board[x - i][y - i] == MEN_NONE)
    //    board[x - i][y - i] = MEN_POSSIBLE_MOVE;
    //  else
    //    tl = false;

    //  ++i;
    //}
  }
  else {
    // men_white => direction bottom
    if (board[y][x] == MEN_WHITE) {
      // bottom left
      if (x -1 >= 0 && y +1 < board.size()) {
        if (board[y +1][x -1] == MEN_NONE) {
            board[y +1][x -1] = MEN_POSSIBLE_MOVE;
        }
        // some men/king
        else {
          if (x -2 >= 0 && y +2 < board.size() &&
              board[y +2][x -2] == MEN_NONE)
              board[y +2][x -2] = MEN_POSSIBLE_MOVE;
        }
      }
      // bottom right
      if (x +1 < board[0].size() && y +1 < board.size()) {
        if (board[y +1][x +1] == MEN_NONE) {
            board[y +1][x +1] = MEN_POSSIBLE_MOVE;
        }
        // some men/king
        else {
          if (x +2 < board[0].size() && y +2 < board.size() &&
              board[y +2][x +2] == MEN_NONE)
              board[y +2][x +2] = MEN_POSSIBLE_MOVE;
        }
      }
    }
    // men_black => direction top
    else if (board[y][x] == MEN_BLACK) {
      // tl
      if (x -1 >= 0 && y -1 >= 0 ) {
        if (board[y -1][x -1] == MEN_NONE) {
            board[y -1][x -1] = MEN_POSSIBLE_MOVE;
        }
        // some men/king
        else {
          if (x -2 >= 0 && y -2 >= 0 &&
              board[y -2][x -2] == MEN_NONE)
              board[y -2][x -2] = MEN_POSSIBLE_MOVE;
        }
      }
      // tr
      if (x +1 < board[0].size() && y -1 >= 0 ) {
        if (board[y -1][x +1] == MEN_NONE) {
            board[y -1][x +1] = MEN_POSSIBLE_MOVE;
        }
        // some men/king
        else {
          if (x +2 < board[0].size() && y -2 >= 0 &&
              board[y -2][x +2] == MEN_NONE)
              board[y -2][x +2] = MEN_POSSIBLE_MOVE;
        }
      }
    }
  }
}

void hidePossibleMoves(unsigned int srcx, unsigned int srcy) {
  for (int i = 0; i < board.size(); ++i) {
    for(int j = 0; j < board[i].size(); ++j) {
      if (board[i][j] == MEN_POSSIBLE_MOVE)
        board[i][j] = MEN_NONE;
    }
  }
}

Game::getPlayerWhite() {
  return player_white;
}

Game::getPlayerBlack() {
  return player_black;
}

QString Game::getXmlStr() {
  Q_ASSERT(doc != NULL);
  return doc->toString();
}

QString Game::getError() {
  QString s;
  bool first_iter = true;
  while (! err_queue.isEmpty()) {
    if (first_iter)
      first_iter = false;
    else
      s.append('\n');  //FIXME add windows support

    err_t e = err_queue.takeFirst();
    switch (e) {
      //FIXME
      default:
        s.append("Unknown error.");
    }
  }
}

QHostAddress Game::getRemoteAddr() {
  Q_ASSERT(socket != NULL);
  return socket->peerAddress();
}

int Game::getRemotePort() {
  Q_ASSERT(socket != NULL);
  return socket->peerPort();
}

void Game::gotDisconnected() {
  //QTcpSocket* socket = qobject_cast<QTcpSocket*>( this->sender() );
  //if (this->sender()->parent != this)
  //FIXME this should also work for locally newed socket
  socket->deleteLater();
  running = false;
}

void Game::gotConnected() { }

void Game::readReply() {
  // get the socket from the sender object
  QTcpSocket* soc = qobject_cast<QTcpSocket*>(this->sender());
  QString s(socket->readAll());
  NetCmdParser parser(s);

  for (;;) {
    switch (game_state) {
    }

    switch (parser.getNextCmd()) {
      case NetCmdParser::TOK_:
        break;
    }
  }

  soc->write(QByteArray);
  soc->flush();
  soc->errorString();
}

IcpSyntaxParser::IcpSyntaxParser(QString _s) : s(_s) { }

/** x ~ a-h; y ~ 0-8 */
QPair<unsigned int, unsigned int> IcpSyntaxParser::strCoordToInt(QString s) {
  /** allow loading only 8x8 sized boards */
  Q_ASSERT(s.at(0) >= 'a' && s.at(0) <= 'h' &&
      s.at(1) >= '1' && s.at(1) <= '8');
  QPair<unsigned int, unsigned int> p(s.at(0) - 'a', s.at(1) - '1');
  return p;
}

/** x ~ a-h; y ~ 0-8 */
QPair<QString, QString> IcpSyntaxParser::intToStrCoord(unsigned int x, unsigned int y) {
  /** allow saving of bigger boards than 8x8 */
  Q_ASSERT(x + 'a' <= 'Z');
  QPair<QString, QString> p(
      //FIXME static_cast<char>
      const_cast<char>(x + 'a'),
      std::to_string(y +1));
  return p;
}

NetCmdParser::NetCmdParser(QString) : last_cmd(CMD_NONE) { }

cmd_t NetCmdParser::getNextCmd() {
  if (s.startswith(NET_TOK_INVITE)) {
    s.remove(0, sizeof(NET_TOK_INVITE) -1);
    return INVITE;
  }
  else if (s.startswith(NET_TOK_INVITE_ACCEPT)) {
    s.remove(0, sizeof(NET_TOK_INVITE_ACCEPT) -1);
    return INVITE_ACCEPT;
  }
  else if (s.startswith(NET_TOK_INVITE_REJECT)) {
    s.remove(0, sizeof(NET_TOK_INVITE_REJECT) -1);
    return INVITE_REJECT;
  }
  else if (s.startswith(NET_TOK_GAME)) {
    s.remove(0, sizeof(NET_TOK_GAME) -1);

    //// currently there is no support for color choosing
    //if (s.startswith(NET_TOK_WHITE)) {
    //  s.remove(0, sizeof(NET_TOK_WHITE) -1);
    //}
    //else if (s.startswith(NET_TOK_BLACK)) {
    //  s.remove(0, sizeof(NET_TOK_BLACK) -1);
    //}

    if (s.startswith(NET_TOK_NEW)) {
      s.remove(0, sizeof(NET_TOK_NEW) -1);
      return NEW;
    }
    else if (s.startswith(NET_TOK_LOAD)) {
      s.remove(0, sizeof(NET_TOK_LOAD) -1);
      return LOAD;
    }
  }
  else if (s.startswith(NET_TOK_GAME_ACCEPT)) {
    return GAME_ACCEPT;
  }
  else if (s.startswith(NET_TOK_GAME_REJECT)) {
    return GAME_REJECT;
  }
  else if (s.startswith(NET_TOK_MOVE)) {
    s.remove(0, sizeof(NET_TOK_MOVE) -1);
    return MOVE;
  }
  else if (s.startswith(NET_TOK_EXIT)) {
    return EXIT;
  }

  return NONE;
}

QString NetCmdParser::getRest() {
  return s;
}
