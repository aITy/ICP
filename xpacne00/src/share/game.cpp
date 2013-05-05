/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "game.h"

//TODO OK
void QTimerImproved::resendTimeout(void) {
  if (singleShotScheduledInternally) {
    singleShotScheduledInternally = false;

    if (originalWasSingleShot)
      setSingleShot(true);
    else
      setSingleShot(false);

    setInterval(period);
    start();
  }

  Q_EMIT newTimeout();
}

//TODO OK
QTimerImproved::QTimerImproved(QObject *par) :
  QTimer(par), begin(0), end(0), paused(false) {
  connect(this, SIGNAL(timeout(void)), this, SLOT(resendTimeout(void)));
}

//TODO OK
void QTimerImproved::start(void) {
  paused = false;
  QDateTime dt;
  begin = dt.toMSecsSinceEpoch();
  end = begin;
  period = interval();
  QTimer::start();
}

//TODO OK
void QTimerImproved::start(int msec) {
  paused = false;
  QDateTime dt;
  begin = dt.toMSecsSinceEpoch();
  end = begin;
  period = msec;
  QTimer::start(msec);
}

//TODO OK
void QTimerImproved::stop(void) {
  QTimer::stop();
  paused = false;
}

//TODO OK
void QTimerImproved::pause(void) {
  if (isActive()) {
    QTimer::stop();
    QDateTime dt;
    end = dt.toMSecsSinceEpoch();
    singleShotScheduledInternally = false;
    paused = true;
  }
}

//TODO OK
void QTimerImproved::resume(void) {
  if (paused) {
    paused = false;
    QDateTime dt;
    begin = dt.toMSecsSinceEpoch();
    singleShotScheduledInternally = true;
    singleShot(end - begin, this, SLOT(resendTimeout));
  }
}

//TODO OK
bool QTimerImproved::isPaused(void) {
  return paused;
}

//TODO OK
/** x ~ a-h; y ~ 0-8 */
IcpSyntaxParser::pair_uint_t IcpSyntaxParser::strCoordToUInt(QString s) {
  /** allow loading only 8x8 sized boards */
  Q_ASSERT(s.at(0) >= 'a' && s.at(0) <= 'h' &&
      s.at(1) >= '1' && s.at(1) <= '8');
  pair_uint_t p(s.at(0).toLatin1() - 'a', s.at(1).toLatin1() - '1');
  return p;
}

//TODO OK
/** x ~ a-h; y ~ 0-8 */
IcpSyntaxParser::pair_str_t IcpSyntaxParser::intToStrCoord(unsigned int x, unsigned int y) {
  Q_ASSERT(x + 'a' <= 'h');
  pair_str_t p(
      //FIXME static_cast<char>
      QString(static_cast<char>(x + 'a')),
      QString::number(y +1));
  return p;
}

//TODO OK
//NetCmdParser::NetCmdParser(QString _s) : last_cmd(NONE), s(_s) { }

//TODO OK
NetCmdParser::tok_t NetCmdParser::getNextCmd() {
  if (s.startsWith(NetCmdParser::TOK_INVITE)) {
    s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_INVITE));
    return INVITE;
  }
  else if (s.startsWith(NetCmdParser::TOK_INVITE_ACCEPT)) {
    s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_INVITE_ACCEPT));
    return INVITE_ACCEPT;
  }
  else if (s.startsWith(NetCmdParser::TOK_INVITE_REJECT)) {
    s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_INVITE_REJECT));
    return INVITE_REJECT;
  }
  else if (s.startsWith(NetCmdParser::TOK_GAME)) {
    s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_GAME));

    if (s.startsWith(NetCmdParser::TOK_WHITE)) {
      s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_WHITE));
      return WHITE;
    }
    else if (s.startsWith(NetCmdParser::TOK_BLACK)) {
      s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_BLACK));
      return BLACK;
    }

    if (s.startsWith(NetCmdParser::TOK_NEW)) {
      s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_NEW));
      return NEW;
    }
    else if (s.startsWith(NetCmdParser::TOK_LOAD)) {
      s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_LOAD));
      return LOAD;
    }
  }
  else if (s.startsWith(NetCmdParser::TOK_GAME_ACCEPT)) {
    return GAME_ACCEPT;
  }
  else if (s.startsWith(NetCmdParser::TOK_GAME_REJECT)) {
    return GAME_REJECT;
  }
  else if (s.startsWith(NetCmdParser::TOK_MOVE)) {
    s.remove(0, CONST_STR_LEN(NetCmdParser::TOK_MOVE));
    return MOVE;
  }
  else if (s.startsWith(NetCmdParser::TOK_EXIT)) {
    return EXIT;
  }

  return NONE;
}

//TODO OK
QString NetCmdParser::getRest(void) {
  return s;
}

//TODO OK
Player::Player(Game *_parent) :
  par(_parent), kicked(0), name("Player Alias"), local(true) { }

/**
 * @return true if game can be played further
 */
//TODO OK
bool Player::incKicked(void) {
  /** compute the total number of men of one player */
  if ( ( (par->board.size() / 2) *
        ((par->board.size() / 2) -1) ) <= (kicked +1))
    /** all men kicked :-( */
    return false;

  kicked++;
  /** at least one men is on the board :-) */
  return true;
}

//TODO OK
bool Player::decKicked(void) {
  if (kicked <= 0) return false;

  kicked--;
}

//TODO OK
int Player::getKicked(void) {
  return kicked;
}

//TODO OK
void Game::initXml(void) {
  /** it is yet initialized */
  if (doc->firstChild().isNull()) return;

  /** special XML characters are escaped automatically */
  if (! doc->setContent(
      QString("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
      "<!-- standalone because we have no DTD for it -->"
      "<draughts version=\"1.0\">"
      "<game type=\"network/local\" host=\"hostname/IPv4/IPv6\" port=\"7564\">"
      "<players>"
      "<black>") + QString(player_black->name) + QString("</black>"
      "<white>") + QString(player_white->name) + QString("</white>"
      "</players>"
      "<moves>"
      "</moves>"
      "</draughts>")))
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
void Game::appendMoveToXml(unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty,
    int kickedx, int kickedy, bool became_a_king) {
  QDomElement el_move = doc->createElement(XML_STR_MOVE);

  QDomAttr attr = doc->createAttribute(XML_STR_SRCX);
  attr.setValue(QString::number(srcx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_SRCY);
  attr.setValue(QString::number(srcy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_DSTX);
  attr.setValue(QString::number(dstx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_DSTY);
  attr.setValue(QString::number(dsty));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_KICKEDX);
  attr.setValue(QString::number(kickedx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_KICKEDY);
  attr.setValue(QString::number(kickedy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML_STR_BECAME_A_KING);
  attr.setValue(became_a_king ? XML_STR_TRUE : XML_STR_FALSE);
  el_move.setAttributeNode(attr);

  doc->elementsByTagName(XML_STR_MOVES).at(0).appendChild(el_move);
  //doc->elementsByTagName(XML_STR_MOVES).at(0).toElement().appendChild(el_move);
}

/**
 * proceed a move by reading the inner XML
 * @param true: forward; false: backward
 * @return true if the move was successful
 */
//TODO OK
bool Game::moveFromXml(bool forward) {
  /** we can't go more backwards than before the first move */
  if (! forward && current_move_index < 0) {
    if (! isReplaying()) game_state = STATE_CAN_START;

    return false;
  }

  QDomNode mv(doc->documentElement().firstChildElement(XML_STR_DRAUGHTS)
  .firstChildElement(XML_STR_MOVES).firstChildElement(XML_STR_MOVE));

  if (mv.isNull()) {
    if (! isReplaying()) game_state = STATE_CAN_START;

    return false;
  }

  int i;
  /** get to the current index */
  for (i = 0; i < current_move_index; ++i) {
    if (mv.nextSibling().isNull())
      break;
    else
      mv = mv.nextSibling();
  }

  /** the last performed move is the current one */
  if (forward) mv = mv.nextSibling();

  if (mv.isNull()) {
    if (! isReplaying()) game_state = STATE_END;

    return false;
  }

  if (forward)
    current_move_index++;
  else
    current_move_index--;

  QDomNamedNodeMap map = mv.attributes();

  unsigned int srcx = QString(map.namedItem(XML_STR_SRCX).nodeValue()).toUInt();
  unsigned int srcy = QString(map.namedItem(XML_STR_SRCY).nodeValue()).toUInt();
  unsigned int dstx = QString(map.namedItem(XML_STR_DSTX).nodeValue()).toUInt();
  unsigned int dsty = QString(map.namedItem(XML_STR_DSTY).nodeValue()).toUInt();
  unsigned int kickedx = QString(map.namedItem(XML_STR_KICKEDX).nodeValue()).toUInt();
  unsigned int kickedy = QString(map.namedItem(XML_STR_KICKEDY).nodeValue()).toUInt();

  men_t kicked_men = MEN_NONE;
  if (map.namedItem(XML_STR_KICKED).nodeValue() == XML_STR_WHITE)
    kicked_men = MEN_WHITE;
  else if (map.namedItem(XML_STR_KICKED).nodeValue() == XML_STR_WHITE_KING)
    kicked_men = MEN_WHITE_KING;
  else if (map.namedItem(XML_STR_KICKED).nodeValue() == XML_STR_BLACK)
    kicked_men = MEN_BLACK;
  else if (map.namedItem(XML_STR_KICKED).nodeValue() == XML_STR_BLACK_KING)
    kicked_men = MEN_BLACK_KING;

  bool became_a_king = (map.namedItem(XML_STR_BECAME_A_KING).nodeValue() == XML_STR_TRUE) ? true : false;

  if (kicked_men != MEN_NONE) {
    if (forward) {
      if (getPlayerFromCoord(kickedx, kickedy)->incKicked()) {
        if (! isReplaying()) game_state = STATE_RUNNING;
      }
      else {
        if (! isReplaying()) game_state = STATE_END;
      }

      board[kickedy][kickedx] = MEN_NONE;
    }
    else {
      if (getPlayerFromCoord(kickedx, kickedy)->decKicked()) {
        if (! isReplaying()) game_state = STATE_RUNNING;
      }
      else {
        if (! isReplaying()) game_state = STATE_CAN_START;
      }

      board[kickedy][kickedx] = kicked_men;
    }
  }

  if (forward) {
    if (became_a_king) {
      men_t tmp;

      if (board[srcy][srcx] == MEN_WHITE)
        tmp = MEN_WHITE_KING;
      else
        tmp = MEN_BLACK_KING;

      board[dsty][dstx] = tmp;
      /** last_move_dst changes are not really needed for replaying,
          but for loading XML */
      last_move_dst.first  = -1;
      last_move_dst.second = -1;
    }
    else {
      board[dsty][dstx] = board[srcy][srcx];
      last_move_dst.first  = dstx;
      last_move_dst.second = dsty;
    }

    board[srcy][srcx] = MEN_NONE;
  }
  else {
    if (became_a_king) {
      men_t tmp;

      if (board[dsty][dstx] == MEN_WHITE_KING)
        tmp = MEN_WHITE;
      else
        tmp = MEN_BLACK;

      board[srcy][srcx] = tmp;
    }
    else {
      board[srcy][srcx] = board[dsty][dstx];
    }

    last_move_dst.first  = srcx;
    last_move_dst.second = srcy;
    board[dsty][dstx] = MEN_NONE;
  }

  return true;
}

/**
 * read ICP syntax moves and save them to DOM
 * @return true if OK
 */
//TODO OK
bool Game::loadFromIcpSyntax(QString s) {
  /** thanks to QIODevice::Text we have always \n as line delimiter */
  QListIterator<QString> it(s.split("\n", QString::SkipEmptyParts));

  while (it.hasNext()) {
    //FIXME toLocal8Bit().constData()
    QListIterator<QString> itt(it.next().split(" ", QString::SkipEmptyParts));

    while (itt.hasNext()) {
      QString tmp = itt.next();

      IcpSyntaxParser::pair_uint_t pair1 =
        IcpSyntaxParser::strCoordToUInt(tmp);
      tmp.remove(0, 2);  /**< remove c3 */

      while (! tmp.isEmpty()) {
        tmp.remove(0, 1);  /**< remove x - */
        IcpSyntaxParser::pair_uint_t pair2 =
          IcpSyntaxParser::strCoordToUInt(tmp);

        if (move(pair1.first, pair1.second, pair2.first, pair2.second, true))
          return false;

        pair1 = pair2;
        tmp.remove(0, 2);  /**< remove c3 */
      }
    }
  }

  return true;
}

//TODO OK
Player *Game::getPlayerFromCoord(unsigned int x, unsigned int y) {
  Q_ASSERT(isInBoundaries(x, y));

  if (board[y][x] == MEN_WHITE ||
      board[y][x] == MEN_WHITE_KING)
    return player_white;
  else if (board[y][x] == MEN_BLACK ||
           board[y][x] == MEN_BLACK_KING)
    return player_black;
  else
    return NULL;
}

//TODO OK
bool Game::isBlackBox(unsigned int x, unsigned int y) {
  /** top left box is white */
  if (y % 2 == 0) {
    if (x % 2 == 0)
      return false;
    else
      return true;
  }
  else {
    if (x % 2 == 0)
      return true;
    else
      return false;
  }
}

/**
 * check boundaries
 * @return true if in boundaries
 */
//TODO OK
bool Game::isInBoundaries(int x, int y) {
  return x >= 0 && x < board[0].size() && y >= 0 && y < board.size();
}

/**
 * @return true if the men is becoming a king
 */
//TODO OK
bool Game::isBecomingAKing(men_t src_men, unsigned int y) {
  Q_ASSERT(isInBoundaries(0, y));

  if (src_men == MEN_WHITE_KING || src_men == MEN_BLACK_KING)
    return false;

  if (
      /** top half of the board belongs to the white player */
      (src_men == MEN_BLACK && y == 0) ||
      /** bottom half of the board belongs to the black player */
      (src_men == MEN_WHITE && int(y) == board.size() -1)
     )
    return true;

  return false;
}

//TODO OK
void Game::prepareNewSocket(QHostAddress addr, int port) {
  Q_ASSERT(socket == NULL);

  socket = new QTcpSocket(this);

  connect(socket, SIGNAL(connected()), this, SLOT(gotConnected()));
  /** handle newly arrived data */
  connect(socket, SIGNAL(readyRead()), this, SLOT(gotNewData()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
  //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
  //    SLOT(gotError(QAbstractSocket::SocketError)));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(gotDisconnected()));

  socket->connectToHost(addr, port);
}

//TODO OK
void Game::prepareNewTimer(void) {
  if (replay_timer != NULL) delete replay_timer;

  replay_timer = new QTimerImproved(this);
  connect(replay_timer, SIGNAL(newTimeout()), this, SLOT(gotTimeout()));
  replay_timer->setInterval(replay_delay);
}

//TODO OK
//Game::Game(void) :
//  board(8, QVector<men_t>(8, MEN_NONE)),
//  socket(NULL),
//  game_state(STATE_PRE_INIT),
//  remote_server_port(-1),
//  last_move_dst(-1, -1),
//  possible_move_present(-1, -1),
//  current_move_index(-1),
//  replay_timer(NULL),
//  replay_delay(DEFAULT_TIMEOUT)
//{
//  player_white = new Player("Player White");
//  player_black = new Player("Player Black");
//  doc = new QDomDocument("ICP_draughts_game_XML_document");
//  remote_will_load = false;
//}

//TODO OK
Game::~Game(void) {
  if (doc          != NULL)
    free(doc);
  /** if we are not the parent, the socket was not newed by us */
  if (socket       != NULL && socket->parent() == this)
    free(socket);
  if (player_white != NULL)
    free(player_white);
  if (player_black != NULL)
    free(player_black);
  if (replay_timer != NULL)
    free(replay_timer);
}

//TODO OK
/** we are founding a completely new game */
bool Game::gameRemote(QHostAddress addr, int port, Player::color_t color) {
  if (! remote_will_load && isRunning()) return false;

  /** set who will be white and who black */
  if (color == Player::COLOR_WHITE) {
    player_white->local = true;
    player_black->local = false;
  }
  else if (color == Player::COLOR_BLACK) {
    player_white->local = false;
    player_black->local = true;
  }
  else {
    err_queue.append("ERR: No color given!");
    return false;
  }

  initXml();
  prepareNewSocket(addr, port);

  //FIXME v gotNewData() menit stav!
  if (! remote_will_load) game_state = STATE_WAIT_FOR_CONNECTION;

  return true;
}

/** we are connecting to a "running" game */
//TODO OK
bool Game::gameRemote(QTcpSocket *socket) {
  if (isRunning()) return false;

  connect(socket, SIGNAL(readyRead()), this, SLOT(gotNewData()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(gotDisconnected()));

  game_state = STATE_WAIT_FOR_CONNECTION;
  Q_EMIT refresh();
  return true;
}

//TODO OK
bool Game::gameLocal(void) {
  if (isRunning()) return false;

  initXml();
  game_state = STATE_CAN_START;
  Q_EMIT refresh();

  return true;
}

/** new game from file => jump to end (interpret moves between, of course) */
//TODO OK
bool Game::gameFromFile(QString s, Player::color_t color) {
  if (isRunning()) return false;

  filepath = s;
  QFile f(filepath);

  /** gets closed automatically while destructing */
  if (! f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

  QTextStream stream(&f);

  /** try to parse the file as ICP syntax (first character must be '1') */
  if (! stream.atEnd() && stream.read(1) == "1") {
    stream.seek(0);
    initXml();
    /** ICP syntax can store only local games */
    game_state = STATE_CAN_START;

    if (! loadFromIcpSyntax(stream.readAll())) {
      doc->clear();  /**< tidy up garbage from initXml() */
      return false;
    }
  }
  /** try to parse the file as XML */
  else if (doc->setContent(&f)) {
    /** local game */
    if (doc->documentElement().firstChildElement(XML_STR_DRAUGHTS).
        firstChildElement(XML_STR_GAME).
        attributeNode(XML_STR_TYPE).value() == XML_STR_LOCAL) {
      game_state = STATE_CAN_START;

      /** interpret all available moves */
      while (moveFromXml(true));
    }
    /** network game */
    else {
      /** the remote side will have to load my config */
      remote_will_load = true; //FIXME zohlednit v gotNewData()
      game_state = STATE_CAN_START;

      /** interpret all available moves */
      while (moveFromXml(true));

      if (! gameRemote(
            QHostAddress(doc->documentElement().
              firstChildElement(XML_STR_DRAUGHTS).
              firstChildElement(XML_STR_GAME).
              attributeNode(XML_STR_HOST).value()),
            QString(doc->documentElement().
              firstChildElement(XML_STR_DRAUGHTS).
              firstChildElement(XML_STR_GAME).
              attributeNode(XML_STR_HOST).value()).toInt(), color)) {
        remote_will_load = false;
        doc->clear();  /**< tidy up garbage from setContent() */
        return false;
      }
    }
  }
  else {
    err_queue.append("ERR: Bad file format (only XML or ICP syntax supported)!");
    return false;
  }

  Q_EMIT refresh();
  return true;
}

/** replay loaded game */
//TODO OK
bool Game::gameFromFile(QString s, bool timed) {
  if (isRunning()) return false;

  filepath = s;
  QFile f(filepath);
  f.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream stream(&f);

  /** try to parse the file as ICP syntax (first character must be '1') */
  if (! stream.atEnd() && stream.read(1) == "1") {
    stream.seek(0);
    initXml();

    if (! loadFromIcpSyntax(stream.readAll())) {
      doc->clear();  /**< tidy up garbage from initXml() */
      return false;
    }
  }
  /** try to parse the file as XML */
  else if (doc->setContent(&f)) {
    /** interpret all moves */
    if (! replayMove(99999999, true)) {
      err_queue.append("ERR: Could not interpret all moves from the given XML.");
      return false;
    }
  }
  else {
    err_queue.append("ERR: Bad file format (only XML or ICP syntax supported)!");
    return false;
  }

  prepareNewTimer();

  if (timed)
    replay_timer->start();

  game_state = STATE_REPLAY_STEP;
  return true;
}

//TODO OK
bool Game::isRunning(void) {
  return game_state != STATE_PRE_INIT &&
         game_state != STATE_END;
}

//TODO OK
bool Game::isLocal(void) {
  return socket == NULL;
}

/**
 * men/king move (used both by user and internally => the argument "loading")
 */
Game::err_t Game::move(unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty, bool loading) {
  bool white_is_playing;
  bool black_is_playing;

  /** do nothing and be silent */
  if (! loading && ! isRunning()) return ERR_OK;

  /** check boundaries */
  if (! isInBoundaries(srcx, srcy) || ! isInBoundaries(dstx, dsty)) {
    err_queue.append("ERR: Coordinates out of board size.");
    return ERR_INVALID_MOVE;
  }

  /** check placement (can be only black square) */
  if (! isBlackBox(srcx, srcy) || ! isBlackBox(dstx, dsty)) {
    err_queue.append("ERR: Given coordinates are on black boxes.");
    return ERR_INVALID_MOVE;
  }

  white_is_playing = board[srcy][srcx] == MEN_WHITE ||
                     board[srcy][srcx] == MEN_WHITE_KING;
  black_is_playing = board[srcy][srcx] == MEN_BLACK ||
                     board[srcy][srcx] == MEN_BLACK_KING;

  /** check the presence of src draughtsmen */
  if (! white_is_playing && ! black_is_playing) {
    err_queue.append("ERR: Invalid move source (neither black nor white)!");
    return ERR_INVALID_MOVE;
  }

  /** in network game color matters */
  if (! isLocal()) {
    /** am I allowed to move this men? */
    if (! ((player_white->local && white_is_playing) ||
          (player_black->local && black_is_playing)) ) {
      /** do nothing and be silent */
      return ERR_OK;
    }
  }

  if ((loading || game_state == STATE_CAN_START) && board[srcy][srcx] == MEN_WHITE) {
    err_queue.append("ERR: White men must start the game.");
    return ERR_WHITE_MUST_START;
  }

  /** return if the previous move wasn't complete */
  if (! loading && last_move_dst.first != -1 &&
      (int(srcx) != last_move_dst.first || int(srcy) != last_move_dst.second) &&
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

  /** check if the dst is on one of 2 or 4 allowed diagonals and in the right distance */
  bool can_jump = showPossibleMoves(srcx, srcy, false);
  can_jump = can_jump;//FIXME delete

  if (board[dsty][dstx] != MEN_POSSIBLE_MOVE) {
    hidePossibleMoves(false);
    err_queue.append("ERR: Destination not on the same diagonal!");
    return ERR_INVALID_MOVE;
  }

  hidePossibleMoves(false);

  //FIXME
  /*
  if (can_jump) {
    if (KING) {
      //try all 4 diagonals for dst[y][x]
      // na diagonale mezi src a dst musi byt pouze souperovy kameny
      //diagonal => the diff(x) and diff(y) must be same for src and dst
    }
    else if (white_is_playing) {
      // try direction bottom (2 diagonals)
    }
    else {
      // try direction top (2 diagonals)
    }

    int kickedx = (srcx > dstx) ? srcx -1 : srcx +1;
    int kickedy = (srcy > dsty) ? dsty -1 : dsty +1;
    if (! getPlayerFromCoord(kickedx, kickedy)->incKicked())
      game_state = STATE_END;
    board[kickedy][kickedx] = MEN_NONE;
  }
  */

  bool became_a_king = isBecomingAKing(board[srcy][srcx], dsty);

  board[dsty][dstx] = board[srcy][srcx];
  board[srcy][srcx] = MEN_NONE;

  //FIXME add support for network game!
  int kickedx = -1, kickedy = -1;
  appendMoveToXml(srcx, srcy, dstx, dsty, kickedx, kickedy, became_a_king);

  //FIXME what about network game?
  if (game_state != STATE_END) game_state = STATE_RUNNING;

  if (became_a_king) {
    last_move_dst.first  = -1;
    last_move_dst.second = -1;
  }
  else {
    last_move_dst.first  = dstx;
    last_move_dst.second = dsty;
  }

  if (! loading) Q_EMIT refresh();

  return ERR_OK;
}

/**
 * right now it can only show direct diagonals (i.e. no AI)
 * @return true if some jump is possible
 */
bool Game::showPossibleMoves(unsigned int x, unsigned int y, bool do_emit) {
  if (! isInBoundaries(x, y) || ! isBlackBox(x, y)) return false;

  hidePossibleMoves(do_emit);

  /** check presence */
  if (board[y][x] == MEN_NONE)
    return false;

  bool can_jump = false;
  possible_move_present = QPair<int, int>(-1, -1);

  if (board[y][x] == MEN_BLACK_KING || board[y][x] == MEN_WHITE_KING) {
    /** indicators if the diagonal can be processed further */
    //bool tl = true, tr = true, bl = true, br = true;

    //FIXME
    ///** gradually increase the "ring" */
    //int i = 1;
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
      if (isInBoundaries(x -1, y +1)) {
        if (board[y +1][x -1] == MEN_NONE) {
          board[y +1][x -1] = MEN_POSSIBLE_MOVE;
          possible_move_present = QPair<int, int>(x -1, y +1);
        }
        // some men/king
        else {
          if (isInBoundaries(x -2, y +2) &&
              board[y +2][x -2] == MEN_NONE) {
            board[y +2][x -2] = MEN_POSSIBLE_MOVE;
            possible_move_present = QPair<int, int>(x -2, y +2);
            can_jump = true;
          }
        }
      }

      // bottom right
      if (isInBoundaries(x +1, y +1)) {
        if (board[y +1][x +1] == MEN_NONE) {
          board[y +1][x +1] = MEN_POSSIBLE_MOVE;
          possible_move_present = QPair<int, int>(x +1, y +1);
        }
        // some men/king
        else {
          if (isInBoundaries(x +2, y +2) &&
              board[y +2][x +2] == MEN_NONE) {
            board[y +2][x +2] = MEN_POSSIBLE_MOVE;
            possible_move_present = QPair<int, int>(x +2, y +2);
            can_jump = true;
          }
        }
      }
    }
    // men_black => direction top
    else if (board[y][x] == MEN_BLACK) {
      // tl
      if (isInBoundaries(x -1, y -1)) {
        if (board[y -1][x -1] == MEN_NONE) {
          board[y -1][x -1] = MEN_POSSIBLE_MOVE;
          possible_move_present = QPair<int, int>(x -1, y -1);
        }
        // some men/king
        else {
          if (isInBoundaries(x -2, y -2) &&
              board[y -2][x -2] == MEN_NONE) {
            board[y -2][x -2] = MEN_POSSIBLE_MOVE;
            possible_move_present = QPair<int, int>(x -2, y -2);
            can_jump = true;
          }
        }
      }
      // tr
      if (isInBoundaries(x +1, y -1)) {
        if (board[y -1][x +1] == MEN_NONE) {
          board[y -1][x +1] = MEN_POSSIBLE_MOVE;
          possible_move_present = QPair<int, int>(x +1, y -1);
        }
        // some men/king
        else {
          if (isInBoundaries(x +2, y -2) &&
              board[y -2][x +2] == MEN_NONE) {
            board[y -2][x +2] = MEN_POSSIBLE_MOVE;
            possible_move_present = QPair<int, int>(x +2, y -2);
            can_jump = true;
          }
        }
      }
    }
  }

  if (do_emit) Q_EMIT refresh();

  return can_jump;
}

//TODO OK
void Game::hidePossibleMoves(bool do_emit) {
  hidePossibleMoves(-1, -1, do_emit);
}

/**
 * @param x coordinate of cell which will be completely ignored
 * @param y coordinate of cell which will be completely ignored
 * @param if true, do call refresh() at the end
 */
//TODO OK
void Game::hidePossibleMoves(int x, int y, bool do_emit) {
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board[i].size(); ++j) {
      if (x >= 0 && y >= 0) {
        if ((i != y || j != x) && board[i][j] == MEN_POSSIBLE_MOVE)
          board[i][j] = MEN_NONE;
      }
      else if (board[i][j] == MEN_POSSIBLE_MOVE) {
        board[i][j] = MEN_NONE;
      }
    }
  }

  possible_move_present = QPair<int, int>(-1, -1);

  if (do_emit) Q_EMIT refresh();
}

//TODO OK
void Game::adviceMove(void) {
  if (! isRunning() || isReplaying()) return;

  /** the previous move wasn't complete (jump necessary) */
  if (game_state != STATE_CAN_START &&
      showPossibleMoves(last_move_dst.first, last_move_dst.second, false)) {
    /** make the last found possible move the advice */
    hidePossibleMoves(possible_move_present.first,
                      possible_move_present.second, false);
  }
  else {
    bool white_on_move;

    if (game_state == STATE_CAN_START) {
      white_on_move = true;
    }
    else {
      if (board[last_move_dst.second][last_move_dst.first] == MEN_WHITE ||
          board[last_move_dst.second][last_move_dst.first] == MEN_WHITE_KING)
        white_on_move = false;
      else
        white_on_move = true;
    }

    bool possible_move_found = false;
    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board[i].size(); ++j) {
        /** is it white/black men/king? */
        if (isBlackBox(i, j) && board[j][i] != MEN_NONE) {
          showPossibleMoves(i, j, false);

          if (possible_move_present.first != -1) {
            hidePossibleMoves(possible_move_present.first,
                              possible_move_present.second, false);
            possible_move_found = true;
            break;
          }

          hidePossibleMoves(false);
        }
      }

      if (possible_move_found) break;
    }

    /** none of the two players can make any move */
    if (! possible_move_found) game_state = STATE_END;
  }

  Q_EMIT refresh();
}

/**
 * @param timeout in ms; (if < 0, the default timeout will be set)
 */
//TODO OK
void Game::setReplayTimeout(int t) {
  if (t < 0)
    replay_delay = DEFAULT_TIMEOUT;
  else
    replay_delay = t;

  if (replay_timer != NULL)
    /** setting interval takes immediate effect (also on an active timer) */
    replay_timer->setInterval(replay_delay);
}

/**
 * @return true if OK;
 */
//TODO OK
bool Game::replayMove(unsigned int i, bool forward) {
  if (! isReplaying()) return false;

  for (int x = 0; x < i; ++x) {
    if (! moveFromXml(forward))
      /** prevent empty looping */
      break;
  }

  Q_EMIT refresh();
  return true;
}

//TODO OK
bool Game::replayMoveToggle(void) {
  if (! isReplaying()) return false;

  if (replay_timer == NULL) return false;

  if replay_timer->isPaused()
    replay_timer->resume();
  else
    replay_timer->pause();

  game_state = STATE_REPLAY_TIMED;
  return true;
}

//TODO OK
bool Game::replayMoveStop(void) {
  if (! isReplaying()) return false;

  if (replay_timer == NULL) return false;

  replay_timer->stop();
  game_state = STATE_REPLAY_STEP;
  return true;
}

//TODO OK
bool Game::isReplaying(void) {
  switch (game_state) {
      case STATE_REPLAY_STEP:
      case STATE_REPLAY_TIMED:
      case STATE_REPLAY_STOP:
        return true;
      default:
        return false;
  }
}

//TODO OK
Player *Game::getPlayerWhite(void) {
  return player_white;
}

//TODO OK
Player *Game::getPlayerBlack(void) {
  return player_black;
}

//TODO OK
/** synchronize XML tree with content of the Game class */
void Game::syncXml(void) {
  Q_ASSERT(doc != NULL);

  //FIXME test if the prefix documentElement() returns directly "draughts"
  QDomElement e(doc->documentElement().firstChildElement(XML_STR_DRAUGHTS));

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
QString Game::getXmlStr(void) {
  Q_ASSERT(doc != NULL);
  syncXml();
  return doc->toString();
}

/**
 * @param true: get everything from beginning to the current move index
 *        false: get all moves
 */
QString Game::getIcpSyntaxStr(bool to_current_move_index) {
  QString res;
  //FIXME only for debug
  return res;

//  if (! isRunning()) return res;
//
//  Q_ASSERT(doc != NULL);
//  syncXml();
//  QDomElement mv(doc->documentElement().firstChildElement(XML_STR_DRAUGHTS)
//  .firstChildElement(XML_STR_MOVES).firstChildElement(XML_STR_MOVE));
//
//  if (mv.isNull()) return res;
//
//  if (to_current_move_index) {
//    int srcx, srcy, dstx, dsty;
//    int last_dstx = -1, last_dsty = -1;
//    int i;
//    /** get to the current index */
//    for (i = 0; i <= current_move_index; ++i) {
//      if (mv.isNull()) {
//        break;
//      }
//      else {
//        QDomNamedNodeMap map = mv.attributes();
//        srcx = QString(map.namedItem(XML_STR_SRCX).nodeValue()).toUInt();
//        srcy = QString(map.namedItem(XML_STR_SRCY).nodeValue()).toUInt();
//        dstx = QString(map.namedItem(XML_STR_DSTX).nodeValue()).toUInt();
//        dsty = QString(map.namedItem(XML_STR_DSTY).nodeValue()).toUInt();
//        IcpSyntaxParser::pair_str_t tmp = IcpSyntaxParser::intToStrCoord(
//              );
//        mv = mv.nextSibling();
//      }
//    }
//  }
//  else {
//    while (! mv.isNull()) {
//      res.append(mv.);
//    }
//  }
//
//  return res;
}

//TODO OK
QString Game::getError(void) {
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
QHostAddress Game::getRemoteAddr(void) {
  Q_ASSERT(socket != NULL);
  return socket->peerAddress();
}

//TODO OK
int Game::getRemotePort(void) {
  Q_ASSERT(socket != NULL);
  return socket->peerPort();
}

/** received only once */
//TODO OK
void Game::gotConnected(void) {
  /** initiate communication */
  socket->write(NetCmdParser::TOK_INVITE + QString::number(port) + " " +
      (player_white->local) ? player_white->name : player_black->name);
  socket->flush();
  game_state = STATE_INVITE_DISPATCH;
}

void Game::gotNewData(void) {
  qDebug() << "UNIMPLEMENTED: gotNewData()" << endl;
  //// get the socket from the sender object
  //QTcpSocket* soc = qobject_cast<QTcpSocket*>(this->sender());
  //QString s(socket->readAll());
  //NetCmdParser parser(s);

  //for (;;) {
  //  switch (game_state) {
  //  }

  //  switch (parser.getNextCmd()) {
  //    case NetCmdParser::TOK_:
  //      break;
  //  }
  //}

  //soc->write(QByteArray);
  //soc->flush();
  //soc->errorString();
}

void Game::gotDisconnected(void) {
  qDebug() << "UNIMPLEMENTED: gotDisconnected()" << endl;
  //QTcpSocket* socket = qobject_cast<QTcpSocket*>( this->sender() );
  //if (this->sender()->parent != this)

  socket->deleteLater();
  game_state = STATE_END;
  Q_EMIT refresh();
}

void Game::gotTimeout(void) {
  qDebug() << "UNIMPLEMENTED: gotTimeout()" << endl;
  //FIXME move forward by 1; refresh
  Q_EMIT refresh();
}
