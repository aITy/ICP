/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

//TODO OK
/** x ~ a-h; y ~ 0-8 */
pair_uint_t IcpSyntaxParser::strCoordToUint(QString s) {
  /** allow loading only 8x8 sized boards */
  Q_ASSERT(s.at(0) >= 'a' && s.at(0) <= 'h' &&
      s.at(1) >= '1' && s.at(1) <= '8');
  pair_uint_t p(s.at(0) - 'a', s.at(1) - '1');
  return p;
}

//TODO OK
/** x ~ a-h; y ~ 0-8 */
pair_str_t IcpSyntaxParser::intToStrCoord(unsigned int x, unsigned int y) {
  /** allow saving of bigger boards than 8x8 */
  Q_ASSERT(x + 'a' <= 'Z');
  pair_str_t p(
      //FIXME static_cast<char>
      QString(const_cast<char>(x + 'a')),
      QString::number(y +1));
  return p;
}

//TODO OK
NetCmdParser::NetCmdParser(QString _s) : last_cmd(CMD_NONE), s(_s) { }

//TODO OK
cmd_t NetCmdParser::getNextCmd() {
  if (s.startswith(NetCmdParser::TOK_INVITE)) {
    s.remove(0, sizeof(NetCmdParser::TOK_INVITE) -1);
    return INVITE;
  }
  else if (s.startswith(NetCmdParser::TOK_INVITE_ACCEPT)) {
    s.remove(0, sizeof(NetCmdParser::TOK_INVITE_ACCEPT) -1);
    return INVITE_ACCEPT;
  }
  else if (s.startswith(NetCmdParser::TOK_INVITE_REJECT)) {
    s.remove(0, sizeof(NetCmdParser::TOK_INVITE_REJECT) -1);
    return INVITE_REJECT;
  }
  else if (s.startswith(NetCmdParser::TOK_GAME)) {
    s.remove(0, sizeof(NetCmdParser::TOK_GAME) -1);

    if (s.startswith(NetCmdParser::TOK_WHITE)) {
      s.remove(0, sizeof(NetCmdParser::TOK_WHITE) -1);
      return TOK_WHITE;
    }
    else if (s.startswith(NetCmdParser::TOK_BLACK)) {
      s.remove(0, sizeof(NetCmdParser::TOK_BLACK) -1);
      return TOK_BLACK;
    }

    if (s.startswith(NetCmdParser::TOK_NEW)) {
      s.remove(0, sizeof(NetCmdParser::TOK_NEW) -1);
      return NEW;
    }
    else if (s.startswith(NetCmdParser::TOK_LOAD)) {
      s.remove(0, sizeof(NetCmdParser::TOK_LOAD) -1);
      return LOAD;
    }
  }
  else if (s.startswith(NetCmdParser::TOK_GAME_ACCEPT)) {
    return GAME_ACCEPT;
  }
  else if (s.startswith(NetCmdParser::TOK_GAME_REJECT)) {
    return GAME_REJECT;
  }
  else if (s.startswith(NetCmdParser::TOK_MOVE)) {
    s.remove(0, sizeof(NetCmdParser::TOK_MOVE) -1);
    return MOVE;
  }
  else if (s.startswith(NetCmdParser::TOK_EXIT)) {
    return EXIT;
  }

  return NONE;
}

//TODO OK
QString NetCmdParser::getRest() {
  return s;
}

//TODO OK
Player::Player(QString _name = "Player Alias") : kicked(0), name(_name), local(true) { }

//TODO OK
int Player::getKickedCount() {
  return kicked;
}

//TODO OK
void Game::initXml() {
  /** it is yet initialized */
  if (doc->firstChild) return;

  /** special XML characters are escaped automatically */
  if (! doc->setContent(
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
      "<!-- standalone because we have no DTD for it -->"
      "<draughts version=\"1.0\">"
      "<game type=\"network/local\" ?host=\"hostname/IPv4/IPv6\">"
      "<players>"
      "<black>" << player_black.name << "</black>"
      "<white>" << player_white.name << "</white>"
      "</players>"
      "<moves type=\"XML/ICP\">"
      "</moves>"
      "</draughts>"))
    qDebug() << "setContent() failed";

  /*
  QDomProcessingInstruction header = doc->createProcessingInstruction();
  QDomComment comment = doc->createComment();
  QDomElement draughts = doc->createElement();
  QDomAttr attr = doc->createAttribute();
  QDomText text_content = doc->createTextNode(p.name);
  */
}

// http://www.qtforum.org/article/27624/how-to-read-a-xml-file.html
// http://qt-project.org/forums/viewthread/13723
// http://www.qtcentre.org/threads/12313-remove-node-in-xml-file
//TODO OK
void Game::appendMoveToXml(Player &p, unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty, bool kicked = false) {
  QDomElement el_move = doc->createElement("move");

  attr = doc->createAttribute("srcx");
  attr.setValue(QString::number(srcx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("srcy");
  attr.setValue(QString::number(srcy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("dstx");
  attr.setValue(QString::number(dstx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("dsty");
  attr.setValue(QString::number(dsty));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute("kicked");
  attr.setValue(kicked ? "true" : "false");
  el_move.setAttributeNode(attr);

  doc->elementsByTagName("moves").at(0).appendChild(el_move);
  //doc->elementsByTagName("moves").at(0).toElement().appendChild(el_move);
}

/** returns true if the move was successful */
bool Game::moveFromXml(move_xml_dir_t t) {
}

/**
 * read ICP syntax moves and save them to DOM
 * @return true if OK
 */
//TODO OK
bool Game::loadFromIcpSyntax(QString s) {
  /** thanks to QIODevice::Text we have always \n as line delimiter */
  QListIterator<QString> it(stream.split("\n", QString::SkipEmptyParts));

  while (it.hasNext()) {
    //FIXME toLocal8Bit().constData()

    QListIterator<QString> itt(it.next().split(" ", QString::SkipEmptyParts));
    while (itt.hasNext()) {
      QString tmp = itt.next();

      IcpSyntaxParser::pair_uint_t pair1 = IcpSyntaxParser::strCoordToUint(itt.next());
      tmp.remove(0, 2);  /**< remove c3 */

      while (! tmp.empty()) {
        tmp.remove(0, 1);  /**< remove - x */
        IcpSyntaxParser::pair_uint_t pair2 =
          IcpSyntaxParser::strCoordToUint(itt.next());
        appendMoveToXml(pair1.first, pair1.second, pair2.first, pair2.second);
        pair1 = pair2;
        tmp.remove(0, 2);  /**< remove c3 */
      }
    }
  }

  return true;
}

//TODO OK
void Game::prepareNewSocket(QHostAddress addr, int port) {
  Q_ASSERT(socket == NULL);

  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()), this, SLOT(gotConnected()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
  //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
  //    SLOT(gotError(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(gotDisconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(handleReply()));

  socket->connectToHost(addr, port);
}

//TODO OK
void Game::Game(void) : socket(NULL), game_state(STATE_PRE_INIT),
  possible_move_present(false), replay_delay(300), remote_server_port(-1) {
  player_white = new Player("Player White");
  player_black = new Player("Player Black");
  doc = new QDomDocument("ICP_draughts_game_XML_document");
  replay_timer = QTimer(this);
}

//TODO OK
void Game::~Game() {
  if (doc          != NULL)
    free(doc);
  if (socket       != NULL && socket->parent() == this)
    free(socket);
  if (player_white != NULL)
    free(player_white);
  if (player_black != NULL)
    free(player_black);
}

//TODO OK
/** we are founding a completely new game */
void Game::gameRemote(QHostAddress addr, int port, bool white) {
  if (isRunning()) return;

  initXml();
  newSocket(addr, port);

  /** set who will be white and who black */
  if (white) {
    player_white->local = true;
    player_black->local = false;
  }
  else {
    player_white->local = false;
    player_black->local = true;
  }

  /** initiate communication */
  socket->write(NetCmdParser::TOK_INVITE + QString::number(port) +
      " " + player_white->name);
  game_state = STATE_INVITE_DISPATCH;
}

/** we are connecting to a "running" game */
void Game::gameRemote(QTcpSocket *socket) {
  if (isRunning()) return;

  //FIXME we should receive XML
  /** handle newly arrived msg */
  connect(socket, SIGNAL(readyRead()), this, SLOT(readReply()));
  /** handle disconnect */
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
}

//TODO OK
void Game::gameLocal() {
  if (isRunning()) return;

  initXml();
  game_state = STATE_CAN_START;
}

/** new game from file => jump to end (interpret moves between, of course) */
bool Game::gameFromFile(QString s) {
  if (isRunning()) return false;

  filepath = s;
  QFile f(filepath);
  QTextStream stream(&f, QIODevice::ReadWrite | QIODevice::Text);

  /** try to parse the file as ICP syntax (first character must be '1') */
  if (! stream.atEnd() && stream.seek(0) && stream.read(1) == "1") {
    stream.seek(0);
    initXml();
    if (! loadFromIcpSyntax(stream.readAll()))
      return false;

    gameLocal();
  }
  /** try to parse the file as XML */
  else if (f.open() && doc->setContent(&f)) {
    /** local game */
    if (doc->documentElement().firstChildElement(XML_STR_DRAUGHTS).
        firstChildElement(XML_STR_GAME).
        attributeNode(XML_STR_TYPE) == XML_STR_LOCAL) {
      gameLocal();
    }
    /** network game */
    else {
      //FIXME zalozi jakoby "novou" hru, ale skoci
      //  na jiny game_state, protoze bude na tahu
      //  nekdo jiny nez pri nove hre
      t
    }
    //FIXME get the type ()
    game_state = ...;
    f.close();
  }
  else {
    //FIXME
    err_queue.append("");
  }
}

/** replay loaded game */
bool Game::gameFromFile(QString s, replay_t replay) {
  if (isRunning()) return false;

  replay == REPLAY_TIMED;
  t
}

//TODO OK
bool Game::isRunning() {
  return game_state != STATE_PRE_INIT &&
         game_state != STATE_END;
}

//TODO OK
bool Game::isLocal() {
  return socket == NULL;
}

//FIXME check who is on move if starting the game
err_t Game::move(unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty) {
  /** do nothing and be silent */
  if (! isRunning()) return ERR_OK;

  /** check boundaries */
  if (! (_x >= 0 && _x < board[0].size() && _y >= 0 && _y < board.size()) ) {
    err_queue.append("ERR: Coordinates out of board size.");
    return ERR_INVALID_MOVE;
  }

  /** in network game color matters */
  if (! isLocal()) {
    /** am I allowed to move this men? */
    if (! (
        (player_white->local &&
         (board[srcy][srcx] == MEN_WHITE ||
          board[srcy][srcx] == MEN_WHITE_KING)
        ) ||
        (player_black->local &&
         (board[srcy][srcx] == MEN_BLACK ||
          board[srcy][srcx] == MEN_BLACK_KING)
        ) )
       ) {
      /** do nothing and be silent */
      return ERR_OK;
    }
  }

  if (game_state == STATE_CAN_START && board[srcy][srcx] == MEN_WHITE) {
    err_queue.append("ERR: White men must start the game.");
    return ERR_WHITE_MUST_START;
  }

  /** return if the previous move wasn't complete */
  if (game_state != STATE_CAN_START && (
        srcx != last_move_dst.first ||
        srcy != last_move_dst.second) &&
      showPossibleMoves(last_move_dst.first, last_move_dst.second, false)) {
    hidePossibleMoves(false);
    err_queue.append("ERR: Previous move is not complete.");
    return ERR_PREV_MOVE_NOT_FINISHED;
  }

  hidePossibleMoves(false);

  /** check presence in dst */
  if (board[dsty][dstx] == MEN_NONE) {
    err_queue.append("ERR: Destination not empty!");
    return ERR_INVALID_MOVE;
  }

  //FIXME
  /*
    check
      the placement (can be only black square)
      the presence of src draughtsmen
      the distance in conjunction with jumping over

    hraje se pouze na cernych
    kamen jen dopredu, jen po diagonale
    dama muze i dozadu

    dama
      assert:
        dst musi byt volne
        musi byt na 1 ze 4 diagonal
        na diagonale mezi musi byt pouze souperovy kameny
      move(dst)
      foreach diagonal in 4_possible_diagonals {
        if ...
      }

    brani (kicked++)
   */

  //diagonal -> the diff(x) and diff(y) must be same for src and dst
  board[dsty][dstx] = board[srcy][srcx];
  board[srcy][srcx] = MEN_NONE;
  appendMoveToXml(srcx, srcy, dstx, dsty);

  last_move_dst.first  = dstx;
  last_move_dst.second = dsty;

  if (board[dsty][dstx] == MEN_WHITE ||
      board[dsty][dstx] == MEN_WHITE_KING)
    game_state = MOVE_WHITE;
  else
    game_state = MOVE_BLACK;

  return ERR_OK;
}

/**
 * right now it can only show diagonals (i.e. no AI)
 * @return true if some jump is possible
 */
bool showPossibleMoves(unsigned int _x, unsigned int _y, bool do_emit = true) {
  /** check boundaries */
  if (! (_x >= 0 && _x < board[0].size() && _y >= 0 && _y < board.size()) )
    return false;

  hidePossibleMoves();

  /** check presence */
  if (board[y][x] == MEN_NONE)
    return false;

  int x = _x;
  int y = _y;
  //FIXME vycist tohle z qpair
  bool can_jump = false;
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
          possible_move_present = true;
        }
        // some men/king
        else {
          if (x -2 >= 0 && y +2 < board.size() &&
              board[y +2][x -2] == MEN_NONE) {
            board[y +2][x -2] = MEN_POSSIBLE_MOVE;
            possible_move_present = true;
            can_jump = true;
          }
        }
      }
      // bottom right
      if (x +1 < board[0].size() && y +1 < board.size()) {
        if (board[y +1][x +1] == MEN_NONE) {
          board[y +1][x +1] = MEN_POSSIBLE_MOVE;
          possible_move_present = true;
        }
        // some men/king
        else {
          if (x +2 < board[0].size() && y +2 < board.size() &&
              board[y +2][x +2] == MEN_NONE) {
            board[y +2][x +2] = MEN_POSSIBLE_MOVE;
            possible_move_present = true;
            can_jump = true;
          }
        }
      }
    }
    // men_black => direction top
    else if (board[y][x] == MEN_BLACK) {
      // tl
      if (x -1 >= 0 && y -1 >= 0 ) {
        if (board[y -1][x -1] == MEN_NONE) {
          board[y -1][x -1] = MEN_POSSIBLE_MOVE;
          possible_move_present = true;
        }
        // some men/king
        else {
          if (x -2 >= 0 && y -2 >= 0 &&
              board[y -2][x -2] == MEN_NONE) {
            board[y -2][x -2] = MEN_POSSIBLE_MOVE;
            possible_move_present = true;
            can_jump = true;
          }
        }
      }
      // tr
      if (x +1 < board[0].size() && y -1 >= 0 ) {
        if (board[y -1][x +1] == MEN_NONE) {
          board[y -1][x +1] = MEN_POSSIBLE_MOVE;
          possible_move_present = true;
        }
        // some men/king
        else {
          if (x +2 < board[0].size() && y -2 >= 0 &&
              board[y -2][x +2] == MEN_NONE) {
            board[y -2][x +2] = MEN_POSSIBLE_MOVE;
            possible_move_present = true;
            can_jump = true;
          }
        }
      }
    }
  }

  return can_jump;
}

//TODO OK
void hidePossibleMoves(bool do_emit = true) {
  for (int i = 0; i < board.size(); ++i) {
    for(int j = 0; j < board[i].size(); ++j) {
      if (board[i][j] == MEN_POSSIBLE_MOVE)
        board[i][j] = MEN_NONE;
    }
  }

  possible_move_present = false;
}

void Game::adviceMove() {
  /** the previous move wasn't complete (jump necessary) */
  if (showPossibleMoves(last_move_dst.first, last_move_dst.second)) {
    /** make the first found possible move the advice */
    bool first_iter = true;
    for (int i = 0; i < board.size(); ++i) {
      for(int j = 0; j < board[i].size(); ++j) {
        if (board[i][j] == MEN_POSSIBLE_MOVE)
          /** keep the first found */
          if (first_iter)
            first_iter = false;
          /** remove the others */
          else
            board[i][j] = MEN_NONE;
      }
    }
  }
  else {
    men_t tmp;

    if (board[last_move_dst.first][last_move_dst.second] == MEN_WHITE ||
        board[last_move_dst.first][last_move_dst.second] == MEN_WHITE_KING)
      tmp = black;
    else
      tmp = white;

    hledej prvniho opacneho
    for (int i = 0; i < board.size(); ++i) {
      t
        for(int j = 0; j < board[i].size(); ++j) {
          if (board[i][j] != MEN_NONE &&
              board[i][j] != board[last_move_dst.first][last_move_dst.second] &&
              board[i][j] != MEN_&& )
        }
    }
  }
}

//TODO OK
void Game::setReplayTimeout(int t) {
  replay_delay = t;

  if (replay_timer.isActive())
    /** start or restart timer */
    replay_timer.start(replay_delay);
}

bool Game::replayMoveForward(int i) {
  while (i < )
}

bool Game::replayMoveBackward(int i) {
}

bool Game::replayMovePause(void) {
}

bool Game::replayMoveStop(void) {
}

//TODO OK
bool Game::isReplaying(void) {
  switch (game_state) {
      case STATE_REPLAY_STEP:
      case STATE_REPLAY_TIMED:
      case STATE_REPLAY_PAUSE:
      case STATE_REPLAY_STOP:
        return true;
      default:
        return false;
  }
}

//TODO OK
Player *Game::getPlayerWhite() {
  return player_white;
}

//TODO OK
Player *Game::getPlayerBlack() {
  return player_black;
}

//TODO OK
/** synchronize XML tree with content of the Game class */
void Game::syncXml(void) {
  Q_ASSERT(doc != NULL);

  //FIXME test if the prefix documentElement() returns directly "draughts"
  QDomElement e(doc->documentElement().firstChildElement(XMS_STR_DRAUGHTS));

  /** /draughts/game[@type] */
  e.firstChildElement(XML_STR_GAME).setAttribute(XML_STR_TYPE,
      (socket == NULL) ? XML_STR_LOCAL : XML_STR_NETWORK);
  /** /draughts/game[@host] */
  e.firstChildElement(XML_STR_GAME).setAttribute(XML_STR_HOST,
      (socket == NULL) ? "" : socket->peerAddress().toString());
  /** /draughts/game[@port] */
  e.firstChildElement(XML_STR_GAME).setAttribute(XML_STR_PORT,
      (socket == NULL) ? "" : remote_server_port);
  /** /draughts/players/black */
  e.firstChildElement(XML_STR_PLAYERS).firstChildElement(XML_STR_WHITE).
    toText().setData(player_white->name);
  /** /draughts/players/white */
  e.firstChildElement(XML_STR_PLAYERS).firstChildElement(XML_STR_BLACK).
    toText().setData(player_black->name);
}

//TODO OK
QString Game::getXmlStr() {
  Q_ASSERT(doc != NULL);

  syncXml();
  return doc->toString();
}

QString Game::getIcpSyntaxStr(void) {
  Q_ASSERT(doc != NULL);

  syncXml();
  l = doc->getList("move");

  while (it.hasNext()) {
    t
  }

  return ;
}

QString Game::getError() {
  QString s;
  bool first_iter = true;

  while (! err_queue.isEmpty()) {
    if (first_iter)
      first_iter = false;
    else
      /** should work on Windows too if QIODevice::Text is used in
        QTextStream */
      s.append('\n');

    s.append(err_queue.takeFirst());
  }
}

//TODO OK
QHostAddress Game::getRemoteAddr() {
  Q_ASSERT(socket != NULL);
  return socket->peerAddress();
}

//TODO OK
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

void Game::refresh(void) {
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
}
