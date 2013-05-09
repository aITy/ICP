/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "game.h"

#define PRINT_STATE(st) if (game_state == (st)) qDebug(#st);
#define PRINT_CUR_STATE \
  PRINT_STATE(STATE_PRE_INIT            ); \
  PRINT_STATE(STATE_CAN_START           ); \
  PRINT_STATE(STATE_WHITE               ); \
  PRINT_STATE(STATE_BLACK               ); \
  PRINT_STATE(STATE_WAIT_FOR_CONNECTION ); \
  PRINT_STATE(STATE_WAIT_FOR_REMOTE     ); \
  PRINT_STATE(STATE_INVITE_DISPATCH     ); \
  PRINT_STATE(STATE_INVITE_RECEIVED     ); \
  PRINT_STATE(STATE_INVITE_ANSWERED     ); \
  PRINT_STATE(STATE_REPLAY_STEP         ); \
  PRINT_STATE(STATE_REPLAY_TIMED        ); \
  PRINT_STATE(STATE_REPLAY_STOP         ); \
  PRINT_STATE(STATE_END                 );

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

QTimerImproved::QTimerImproved(QObject *par) :
  QTimer(par), begin(0), end(0), paused(false) {
  connect(this, SIGNAL(timeout(void)), this, SLOT(resendTimeout(void)));
}

void QTimerImproved::start(void) {
  paused = false;
  QDateTime dt;
  begin = dt.toMSecsSinceEpoch();
  end = begin;
  period = interval();
  QTimer::start();
}

void QTimerImproved::start(int msec) {
  paused = false;
  QDateTime dt;
  begin = dt.toMSecsSinceEpoch();
  end = begin;
  period = msec;
  QTimer::start(msec);
}

void QTimerImproved::stop(void) {
  QTimer::stop();
  paused = false;
}

void QTimerImproved::pause(void) {
  if (isActive()) {
    QTimer::stop();
    QDateTime dt;
    end = dt.toMSecsSinceEpoch();
    singleShotScheduledInternally = false;
    paused = true;
  }
}

void QTimerImproved::resume(void) {
  if (paused) {
    paused = false;
    QDateTime dt;
    begin = dt.toMSecsSinceEpoch();
    singleShotScheduledInternally = true;
    singleShot((end - begin) % period, this, SLOT(resendTimeout));
  }
}

bool QTimerImproved::isPaused(void) {
  return paused;
}

/** x ~ a-h; y ~ 0-8 */
IcpSyntaxParser::pair_uint_t IcpSyntaxParser::strCoordToUInt(QString s, bool *ok) {
  /** allow loading only 8x8 sized boards */
  if (! (s.at(0) >= 'a' && s.at(0) <= 'h' &&
         s.at(1) >= '1' && s.at(1) <= '8') ) {
    if (ok != NULL) *ok = false;

    return pair_uint_t(0, 0);
  }

  pair_uint_t p(s.at(0).toLatin1() - 'a', s.at(1).toLatin1() - '1');

  if (ok != NULL) *ok = true;

  return p;
}

/** x ~ a-h; y ~ 0-8 */
IcpSyntaxParser::pair_str_t IcpSyntaxParser::intToStrCoord(unsigned int x, unsigned int y) {
  Q_ASSERT(x + 'a' <= 'h');
  pair_str_t p(
      QString(static_cast<char>(x + 'a')),
      QString::number(y +1));
  return p;
}

NetCmdParser::NetCmdParser(QString _s) : last_cmd(NONE), s(_s) { }

NetCmdParser::tok_t NetCmdParser::getNextCmd() {
  if (s.startsWith(TOK::INVITE)) {
    s.remove(0, CONST_STR_LEN(TOK::INVITE));
    return INVITE;
  }
  else if (s.startsWith(TOK::INVITE_ACCEPT)) {
    s.remove(0, CONST_STR_LEN(TOK::INVITE_ACCEPT));
    return INVITE_ACCEPT;
  }
  else if (s.startsWith(TOK::INVITE_REJECT)) {
    s.remove(0, CONST_STR_LEN(TOK::INVITE_REJECT));
    return INVITE_REJECT;
  }
  else if (s.startsWith(TOK::GAME)) {
    s.remove(0, CONST_STR_LEN(TOK::GAME));

    if (s.startsWith(TOK::WHITE)) {
      s.remove(0, CONST_STR_LEN(TOK::WHITE));
      return WHITE;
    }
    else if (s.startsWith(TOK::BLACK)) {
      s.remove(0, CONST_STR_LEN(TOK::BLACK));
      return BLACK;
    }

    if (s.startsWith(TOK::NEW)) {
      s.remove(0, CONST_STR_LEN(TOK::NEW));
      return NEW;
    }
    else if (s.startsWith(TOK::LOAD)) {
      s.remove(0, CONST_STR_LEN(TOK::LOAD));
      return LOAD;
    }
  }
  else if (s.startsWith(TOK::GAME_ACCEPT)) {
    return GAME_ACCEPT;
  }
  else if (s.startsWith(TOK::GAME_REJECT)) {
    return GAME_REJECT;
  }
  else if (s.startsWith(TOK::MOVE)) {
    s.remove(0, CONST_STR_LEN(TOK::MOVE));
    return MOVE;
  }
  else if (s.startsWith(TOK::EXIT)) {
    return EXIT;
  }

  return NONE;
}

QString NetCmdParser::getRest(void) {
  return s;
}

Player::Player(const Game &_parent) :
  par(_parent), kicked(0), name("Player Alias"), local(true) { }

/**
 * @return true if game can be played further
 */
bool Player::incKicked(void) {
  /** compute the total number of men of one player */
  if ( ( (par.board.size() / 2) *
        ((par.board.size() / 2) -1) ) <= (kicked +1))
    /** all men kicked :-( */
    return false;

  kicked++;
  /** at least one men is on the board :-) */
  return true;
}

bool Player::decKicked(void) {
  if (kicked <= 0) return false;

  kicked--;
  return true;
}

int Player::getKicked(void) {
  return kicked;
}

void Game::initXml(void) {
  /** it is yet initialized */
  if (! doc->firstChild().isNull()) return;

  /** special XML characters are escaped automatically */
  if (! doc->setContent(
      QString("<?xml version='1.0' encoding='UTF-8' standalone='yes'?>"
      "<!-- standalone because we have no DTD for it -->"
      "<draughts version=\"1.0\">"
      "<game type=\"network/local\" host=\"hostname/IPv4/IPv6\" port=\"7564\"/>"
      "<players>"
      "<black>") + QString(player_black->name) + QString("</black>"
      "<white>") + QString(player_white->name) + QString("</white>"
      "</players>"
      "<moves>"
      "</moves>"
      "</draughts>")))
    qDebug("setContent() failed");
}

// http://www.qtforum.org/article/27624/how-to-read-a-xml-file.html
// http://qt-project.org/forums/viewthread/13723
// http://www.qtcentre.org/threads/12313-remove-node-in-xml-file
void Game::appendMoveToXml(unsigned int srcx, unsigned int srcy,
    unsigned int dstx, unsigned int dsty,
    int kickedx, int kickedy, men_t kicked, bool became_a_king) {
  QDomElement el_move = doc->createElement(XML::STR_MOVE);

  QDomAttr attr = doc->createAttribute(XML::STR_SRCX);
  attr.setValue(QString::number(srcx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_SRCY);
  attr.setValue(QString::number(srcy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_DSTX);
  attr.setValue(QString::number(dstx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_DSTY);
  attr.setValue(QString::number(dsty));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_KICKEDX);
  attr.setValue(QString::number(kickedx));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_KICKEDY);
  attr.setValue(QString::number(kickedy));
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_KICKED);
  switch (kicked) {
    case MEN_WHITE:
      attr.setValue(XML::STR_WHITE);
      break;
    case MEN_WHITE_KING:
      attr.setValue(XML::STR_WHITE_KING);
      break;
    case MEN_BLACK:
      attr.setValue(XML::STR_BLACK);
      break;
    case MEN_BLACK_KING:
      attr.setValue(XML::STR_BLACK_KING);
      break;
    // MEN_POSSIBLE_MOVE, MEN_NONE
    default:
      attr.setValue(XML::STR_NONE);
      break;
  }
  el_move.setAttributeNode(attr);

  attr = doc->createAttribute(XML::STR_BECAME_A_KING);
  attr.setValue(became_a_king ? XML::STR_TRUE : XML::STR_FALSE);
  el_move.setAttributeNode(attr);

  doc->elementsByTagName(XML::STR_MOVES).at(0).appendChild(el_move);
}

/**
 * proceed a move by reading the inner XML
 * @param true: forward; false: backward
 * @return true if the move was successful
 */
bool Game::moveFromXml(bool forward) {
  /** we can't go more backwards than before the first move */
  if (! forward && current_move_index < 0) {
    /** switch back to the absolute beginning of the game */
    if (! isReplaying()) game_state = STATE_CAN_START;

    return false;
  }

  QDomNode mv(doc->documentElement().firstChildElement(XML::STR_MOVES).
      firstChildElement(XML::STR_MOVE));

  if (mv.isNull()) {
    if (! isReplaying()) game_state = STATE_CAN_START;

    return false;
  }

  int i;
  /** get to the current index */
  for (i = 0; i < current_move_index; ++i)
    mv = mv.nextSibling();

  if (forward) {
    /** do not jump over the first item */
    if (current_move_index >= 0) {
      mv = mv.nextSibling();

      /** we can't do any move further */
      if (mv.isNull()) return false;
    }

    current_move_index++;
  }
  else {
    current_move_index--;
  }

  QDomNamedNodeMap map = mv.attributes();

  unsigned int srcx = QString(map.namedItem(XML::STR_SRCX).nodeValue()).toUInt();
  unsigned int srcy = QString(map.namedItem(XML::STR_SRCY).nodeValue()).toUInt();
  unsigned int dstx = QString(map.namedItem(XML::STR_DSTX).nodeValue()).toUInt();
  unsigned int dsty = QString(map.namedItem(XML::STR_DSTY).nodeValue()).toUInt();
  unsigned int kickedx = QString(map.namedItem(XML::STR_KICKEDX).nodeValue()).toUInt();
  unsigned int kickedy = QString(map.namedItem(XML::STR_KICKEDY).nodeValue()).toUInt();

  men_t kicked_men = MEN_NONE;
  if (map.namedItem(XML::STR_KICKED).nodeValue() == XML::STR_WHITE)
    kicked_men = MEN_WHITE;
  else if (map.namedItem(XML::STR_KICKED).nodeValue() == XML::STR_WHITE_KING)
    kicked_men = MEN_WHITE_KING;
  else if (map.namedItem(XML::STR_KICKED).nodeValue() == XML::STR_BLACK)
    kicked_men = MEN_BLACK;
  else if (map.namedItem(XML::STR_KICKED).nodeValue() == XML::STR_BLACK_KING)
    kicked_men = MEN_BLACK_KING;

  bool became_a_king = (map.namedItem(XML::STR_BECAME_A_KING).nodeValue() == XML::STR_TRUE) ? true : false;

  if (kicked_men != MEN_NONE) {
    if (forward) {
      if (! getPlayerFromCoord(kickedx, kickedy)->incKicked())
        if (! isReplaying())
          game_state = STATE_END;

      board[kickedy][kickedx] = MEN_NONE;
    }
    else {
      board[kickedy][kickedx] = kicked_men;

      if (! getPlayerFromCoord(kickedx, kickedy)->decKicked())
        if (! isReplaying())
          game_state = STATE_CAN_START;
    }
  }

  if (forward) {
    if (! isReplaying()) {
      if (board[srcy][srcx] == MEN_WHITE ||
          board[srcy][srcx] == MEN_WHITE_KING)
        game_state = STATE_WHITE;
      else
        game_state = STATE_BLACK;
    }

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
    if (! isReplaying()) {
      if (board[dsty][dstx] == MEN_WHITE ||
          board[dsty][dstx] == MEN_WHITE_KING)
        game_state = STATE_WHITE;
      else
        game_state = STATE_BLACK;
    }

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
bool Game::loadFromIcpSyntax(QString s) {
  /** thanks to QIODevice::Text we have always \n as line delimiter */
  QListIterator<QString> it(s.split("\n", QString::SkipEmptyParts));

  while (it.hasNext()) {
    QListIterator<QString> itt(it.next().split(" ", QString::SkipEmptyParts));

    while (itt.hasNext()) {
      QString tmp = itt.next();

      IcpSyntaxParser::pair_uint_t pair1 =
        IcpSyntaxParser::strCoordToUInt(tmp, NULL);
      tmp.remove(0, 2);  /**< remove c3 */

      while (! tmp.isEmpty()) {
        tmp.remove(0, 1);  /**< remove x - */
        IcpSyntaxParser::pair_uint_t pair2 =
          IcpSyntaxParser::strCoordToUInt(tmp, NULL);

        if (move(pair1.first, pair1.second, pair2.first, pair2.second, true))
          return false;

        pair1 = pair2;
        tmp.remove(0, 2);  /**< remove c3 */
      }
    }
  }

  return true;
}

Player *Game::getPlayerFromCoord(unsigned int x, unsigned int y) {
  Q_ASSERT(isInBoundaries(x, y));

  if (board[y][x] == MEN_WHITE ||
      board[y][x] == MEN_WHITE_KING) {
    return player_white;
  }
  else if (board[y][x] == MEN_BLACK ||
           board[y][x] == MEN_BLACK_KING) {
    return player_black;
  }
  else {
    qDebug("ERR: Game::getPlayerFromCoord() returns NULL.");
    return NULL;
  }
}

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
bool Game::isInBoundaries(int x, int y) {
  return x >= 0 && x < board[0].size() && y >= 0 && y < board.size();
}

/**
 * @return true if the men is becoming a king
 */
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

void Game::prepareNewTimer(void) {
  if (replay_timer != NULL) delete replay_timer;

  replay_timer = new QTimerImproved(this);
  connect(replay_timer, SIGNAL(newTimeout()), this, SLOT(gotTimeout()));
  replay_timer->setInterval(replay_delay);
}

Game::Game(QTcpServer *_server) :
  DEFAULT_TIMEOUT(600),
  board(8, QVector<men_t>(8, MEN_NONE)),
  socket(NULL),
  server(_server),
  game_state(STATE_PRE_INIT),
  last_move_dst(-1, -1),
  possible_move_present(-1, -1),
  current_move_index(-1),
  game_ai(Player::COLOR_DONT_KNOW),
  replay_timer(NULL),
  replay_delay(DEFAULT_TIMEOUT)
{
  player_white = new Player(*this);
  player_white->name = "Player White";
  player_black = new Player(*this);
  player_black->name = "Player Black";
  doc = new QDomDocument("ICP_draughts_game_XML_document");
  remote_will_load = false;

  /** init board with the default MEN distribution */
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board[i].size(); ++j) {
      if (isBlackBox(j, i)) {
        if (i < (board.size() / 2 -1))
          board[i][j] = MEN_WHITE;
        else if (i >= (board.size() - (board.size() / 2 -1)))
          board[i][j] = MEN_BLACK;
      }
    }
  }
}

Game::~Game(void) {
  if (doc          != NULL)
    delete doc;
  /** if we are not the parent, the socket was not newed by us */
  if (socket       != NULL && socket->parent() == this)
    delete socket;
  if (player_white != NULL)
    delete player_white;
  if (player_black != NULL)
    delete player_black;
  if (replay_timer != NULL)
    delete replay_timer;
}

/**
 * we are founding a completely new game and we have 2 cases:
 *   the user starts completely new game
 *   the user loads network game from file
 * @return true if OK
 */
bool Game::gameRemote(QHostAddress addr, int port, Player::color_t color) {
  if (! remote_will_load &&  /**< do we call it internally? */
      isRunning()) return false;

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
  game_state = STATE_WAIT_FOR_CONNECTION;

  return true;
}

/**
 * we are connecting to a "running" game
 * @return true if OK
 */
bool Game::gameRemote(QTcpSocket *_sock) {
  if (isRunning()) return false;

  socket = _sock;
  connect(socket, SIGNAL(readyRead()), this, SLOT(gotNewData()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(gotDisconnected()));
  connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
      this, SLOT(gotDisconnected()));

  game_state = STATE_WAIT_FOR_REMOTE;
  Q_EMIT refresh();
  return true;
}

/** @return true if OK */
bool Game::gameLocal(void) {
  if (isRunning()) return false;

  initXml();
  game_state = STATE_CAN_START;
  Q_EMIT refresh();

  return true;
}

/**
 * play against AI
 * @param if true, the computer AI will be the black player
 *        otherwise AI will be white
 * @return true if OK
 */
bool Game::gameLocal(bool ai_is_black) {
  if (isRunning()) return false;

  initXml();
  game_state = STATE_CAN_START;

  if (ai_is_black) {
    game_ai = Player::COLOR_BLACK;
  }
  else {
    game_ai = Player::COLOR_WHITE;
    adviceMove();
    game_state = STATE_BLACK;
  }

  Q_EMIT refresh();
  return true;
}

/**
 * new game from file => jump to end (interpret moves between, of course)
 * @param filepath
 * @param color of the local player in the network game (for solely local game use Player::COLOR_DONT_KNOW)
 * @return true if OK
 */
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
  else if (f.seek(0) && doc->setContent(&f)) {
    /** local game */
    if (doc->documentElement().firstChildElement(XML::STR_GAME).
        attributeNode(XML::STR_TYPE).value() == XML::STR_LOCAL) {
      game_state = STATE_CAN_START;

      /** interpret all available moves */
      while (moveFromXml(true));
    }
    /** network game */
    else if (doc->documentElement().firstChildElement(XML::STR_GAME).
        attributeNode(XML::STR_TYPE).value() == XML::STR_NETWORK) {
      /** the remote side will have to load my config */
      remote_will_load = true; //FIXME zohlednit v gotNewData()
      game_state = STATE_CAN_START;

      /** interpret all available moves */
      while (moveFromXml(true));

      if (! gameRemote(
            QHostAddress(doc->documentElement().
              firstChildElement(XML::STR_GAME).
              attributeNode(XML::STR_HOST).value()),
            QString(doc->documentElement().
              firstChildElement(XML::STR_GAME).
              attributeNode(XML::STR_HOST).value()).toInt(), color)) {
        remote_will_load = false;
        game_state = STATE_PRE_INIT;
        doc->clear();  /**< tidy up garbage from setContent() */
        return false;
      }
    }
    else {
      err_queue.append("ERR: Bad XML format (can't decide whether it is local/network game)!");
      return false;
    }
  }
  else {
    err_queue.append("ERR: Bad file format (only XML or ICP syntax supported)!");
    return false;
  }

  Q_EMIT refresh();
  return true;
}

/** replay loaded game
 * @return true if OK
 */
bool Game::gameFromFile(QString s) {
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

    game_state = STATE_REPLAY_STEP;
  }
  /** try to parse the file as XML */
  else if (f.seek(0) && doc->setContent(&f)) {
    game_state = STATE_REPLAY_STEP;

    /** interpret all moves */
    if (! replayMove(99999999, true)) {
      game_state = STATE_PRE_INIT;
      doc->clear();
      err_queue.append("ERR: Could not interpret all moves from the given XML.");
      return false;
    }
  }
  else {
    err_queue.append("ERR: Bad file format (only XML or ICP syntax supported)!");
    return false;
  }

  prepareNewTimer();
  return true;
}

bool Game::isInNetworkMeantime(void) {
  return game_state == STATE_WAIT_FOR_CONNECTION ||
         game_state == STATE_WAIT_FOR_REMOTE     ||
         game_state == STATE_INVITE_DISPATCH     ||
         game_state == STATE_INVITE_RECEIVED     ||
         game_state == STATE_INVITE_ANSWERED;
}

bool Game::isRunning(void) {
  return game_state != STATE_PRE_INIT && game_state != STATE_END;
}

bool Game::isLocal(void) {
  return socket == NULL;
}

/**
 * men/king move (used both by user and internally => the argument "loading")
 * @param
 * @param
 * @param
 * @param
 * @param true if used internally for loading from ICP syntax
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
    err_queue.append("ERR: Given coordinates are not on black boxes.");
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

  /** in network_game and local_game_with_AI color matters */
  if (! isLocal() || game_ai != Player::COLOR_DONT_KNOW) {
    /** am I allowed to move this men? */
    if (! ((player_white->local && white_is_playing) ||
           (player_black->local && black_is_playing) ||
           (white_is_playing && game_ai == Player::COLOR_BLACK) ||
           (black_is_playing && game_ai == Player::COLOR_WHITE)
           ) ) {
      err_queue.append("ERR: You can move only with your men/king!.");
      return ERR_WHITE_MUST_START;
    }
  }

  if (game_state == STATE_CAN_START && board[srcy][srcx] != MEN_WHITE) {
    err_queue.append("ERR: White men must start the game.");
    return ERR_WHITE_MUST_START;
  }

  /** current src is different from the last_move_dst */
  if (int(srcx) != last_move_dst.first || int(srcy) != last_move_dst.second) {
    /** return if the previous move wasn't complete */
    if (! loading && last_move_dst.first != -1 &&
        /** make sure, the last move has kicked some men/king out */
        doc->documentElement().firstChildElement(XML::STR_MOVES).
        lastChild().attributes().namedItem(XML::STR_KICKED).
        nodeValue() != XML::STR_NONE &&
        showPossibleMoves(last_move_dst.first, last_move_dst.second, false)) {
      hidePossibleMoves(false);
      err_queue.append("ERR: Previous move is not complete.");
      return ERR_PREV_MOVE_NOT_FINISHED;
    }

    hidePossibleMoves(false);
  }

  if (game_state != STATE_CAN_START) {
    QPair<int, int> color;

    if (white_is_playing)
      color = QPair<int, int>(MEN_WHITE, MEN_WHITE_KING);
    else
      color = QPair<int, int>(MEN_BLACK, MEN_BLACK_KING);

    /** try to find any source for a move the current player should do
      (i.e. must jump) instead of the move he's currently requesting */
    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board[i].size(); ++j) {
        if (isBlackBox(j, i) &&
            (board[i][j] == color.first || board[i][j] == color.second) &&
            j != int(srcx) && i != int(srcy) &&
            showPossibleMoves(j, i, false)) {
          err_queue.append("ERR: Some jump is necessary!");
          hidePossibleMoves(false);
          return ERR_INVALID_MOVE;
        }
      }
    }

    hidePossibleMoves(false);
  }

  /** check presence in dst */
  if (board[dsty][dstx] != MEN_NONE) {
    err_queue.append("ERR: Destination not empty!");
    return ERR_INVALID_MOVE;
  }

  /** check if the dst is on one of 2 or 4 allowed diagonals and in the right distance */
  bool can_jump = showPossibleMoves(srcx, srcy, false);

  if (board[dsty][dstx] != MEN_POSSIBLE_MOVE) {
    hidePossibleMoves(false);
    err_queue.append("ERR: Destination not on the same diagonal or too far from source or jumping over too much men!");
    return ERR_INVALID_MOVE;
  }

  int kickedx = -1, kickedy = -1;
  men_t kicked = MEN_NONE;

  /** find the men who will be kicked (if any) */
  if (can_jump) {
    /** get the right direction vector */
    int mulx = (dstx < srcx) ? -1 : 1;
    int muly = (dsty < srcy) ? -1 : 1;

    /** find nearest non-possible-move box on the particular diagonal
        (board.size() for sure to prevent infinite looping) */
    for (int i = 1; i < board.size(); ++i) {
      int _x = int(srcx) + (i * mulx);
      int _y = int(srcy) + (i * muly);

      /** just for case the player does not want to jump */
      if (_x == int(dstx) && _y == int(dsty)) break;

      if (isInBoundaries(_x, _y) && board[_y][_x] != MEN_POSSIBLE_MOVE) {
        kickedx = _x;
        kickedy = _y;
        break;
      }
    }

    /** current player has both the possibility of a simple move(s) and
        jump(s), but prefers the move => be angry on him */
    if (kickedx == -1) {
      err_queue.append("ERR: Some jump is necessary!");
      hidePossibleMoves(false);
      return ERR_INVALID_MOVE;
    }
    else {
      if (! getPlayerFromCoord(kickedx, kickedy)->incKicked())
        game_state = STATE_END;
      kicked = board[kickedy][kickedx];
      board[kickedy][kickedx] = MEN_NONE;
    }
  }
  else {
    /** force to alternate moves */
    if ((white_is_playing && game_state == STATE_WHITE) ||
        (black_is_playing && game_state == STATE_BLACK)) {
      hidePossibleMoves(false);
      err_queue.append("ERR: Players must alternate.");
      return ERR_INVALID_MOVE;
    }
  }

  hidePossibleMoves(false);

  bool became_a_king = isBecomingAKing(board[srcy][srcx], dsty);
  if (became_a_king)
    board[dsty][dstx] = (white_is_playing) ? MEN_WHITE_KING : MEN_BLACK_KING;
  else
    board[dsty][dstx] = board[srcy][srcx];

  board[srcy][srcx] = MEN_NONE;
  appendMoveToXml(srcx, srcy, dstx, dsty,kickedx, kickedy, kicked, became_a_king);

  if (! isLocal()) {
    if (socket->write(QString(TOK::MOVE +
            QString::number(srcx) + " " +
            QString::number(srcy) + " " +
            QString::number(dstx) + " " +
            QString::number(dsty)).toLocal8Bit()) == -1)
      err_queue.append(socket->errorString());
  }

  if (game_state != STATE_END) {
    if (white_is_playing)
      game_state = STATE_WHITE;
    else
      game_state = STATE_BLACK;
  }

  if (became_a_king) {
    last_move_dst.first  = -1;
    last_move_dst.second = -1;
  }
  else {
    last_move_dst.first  = dstx;
    last_move_dst.second = dsty;
  }

//  /** AI is enabled */
//  if (game_ai != Player::COLOR_DONT_KNOW &&
//      (
//       /** current player is not AI */
//       ((white_is_playing && game_ai == Player::COLOR_BLACK) ||
//        (black_is_playing && game_ai == Player::COLOR_WHITE)) &&
//       /** current player can not make another move */
//       doc->documentElement().firstChildElement(XML::STR_MOVES).
//       lastChild().attributes().namedItem(XML::STR_KICKED).
//       nodeValue() == XML::STR_NONE ||
//       ! showPossibleMoves(last_move_dst.first, last_move_dst.second, false)
//      ) ||
//      (
//       /** current player is AI */
//       ((white_is_playing && game_ai == Player::COLOR_WHITE) ||
//        (black_is_playing && game_ai == Player::COLOR_BLACK)) &&
//       /** current player not make another move */
//       doc->documentElement().firstChildElement(XML::STR_MOVES).
//       lastChild().attributes().namedItem(XML::STR_KICKED).
//       nodeValue() != XML::STR_NONE &&
//       showPossibleMoves(last_move_dst.first, last_move_dst.second, false)
//      ) ) {
//    // generate move coordinates
//    move(..., )
//
//    hidePossibleMoves(false);
//
//    //FIXME
//    //  if (game_state == STATE_WHITE && game_ai == Player::COLOR_BLACK) {
//    //    adviceMove();
//    //    if (can make another move)
//    //      call move();
//    //    else
//    //      game_state = state_white
//    //
//    //        Q_EMIT refresh();
//    //    return ERR_OK;
//    //  }
//    //  else if (game_state == STATE_BLACK && game_ai == Player::COLOR_WHITE) {
//    //  }
//  }

  if (! loading) Q_EMIT refresh();

  return ERR_OK;
}

/**
 * right now it can only show direct diagonals (i.e. no AI)
 * @return true if some jump is possible
 */
bool Game::showPossibleMoves(unsigned int x, unsigned int y, bool do_emit) {
  if (! isInBoundaries(x, y) || ! isBlackBox(x, y)) return false;

  hidePossibleMoves(false);

  /** check presence */
  if (board[y][x] == MEN_NONE)
    return false;

  bool can_jump = false;
  possible_move_present = QPair<int, int>(-1, -1);

  QPair<int, int> contra_color;

  if (board[y][x] == MEN_BLACK_KING || board[y][x] == MEN_BLACK)
    contra_color = QPair<int, int>(MEN_WHITE, MEN_WHITE_KING);
  else
    contra_color = QPair<int, int>(MEN_BLACK, MEN_BLACK_KING);

  /** is it a king? */
  if (board[y][x] == MEN_BLACK_KING || board[y][x] == MEN_WHITE_KING) {
    /**
     * it works like following:
     *   1) read only consecutive empty boxes until exception occurs
     *   2) if the current box is an opponent's men,
     *   3) read further consecutive empty boxes until exception occurs
     */

    /** first non-MEN_NONE box in the particular direction */
    QPair<int, int> tl(-1, -1);
    QPair<int, int> tr(-1, -1);
    QPair<int, int> bl(-1, -1);
    QPair<int, int> br(-1, -1);

    #define FIND_FIRST_NON_MEN_NONE(_x, _y, dir) \
      /** prevent out of boundary and rewrite of the last value */ \
      if (isInBoundaries((_x), (_y)) && dir.first == -1) { \
        if (board[(_y)][(_x)] == MEN_NONE) { \
          board[(_y)][(_x)] = MEN_POSSIBLE_MOVE; \
          possible_move_present = QPair<int, int>((_x), (_y)); \
        } \
        else { \
          dir = QPair<int, int>((_x), (_y)); \
        } \
      }

    /** traverse all 4 diagonals (board.size() to prevent infinite loop) */
    for (int i = 1; i < board.size(); ++i) {
      FIND_FIRST_NON_MEN_NONE(x - i, y - i, tl);
      FIND_FIRST_NON_MEN_NONE(x + i, y - i, tr);
      FIND_FIRST_NON_MEN_NONE(x - i, y + i, bl);
      FIND_FIRST_NON_MEN_NONE(x + i, y + i, br);
    }

    bool tl_can_jump = false;
    bool tr_can_jump = false;
    bool bl_can_jump = false;
    bool br_can_jump = false;

    /** find out if it is opponent's men/king and set the can_jump flag */
    #define MAYBE_CAN_JUMP(dir) \
      if (isInBoundaries(dir.first, dir.second) && \
          (board[dir.second][dir.first] == contra_color.first || \
           board[dir.second][dir.first] == contra_color.second)) \
        dir ## _can_jump = true;

    MAYBE_CAN_JUMP(tl);
    MAYBE_CAN_JUMP(tr);
    MAYBE_CAN_JUMP(bl);
    MAYBE_CAN_JUMP(br);

    /** make sure we can really jump in that particular direction */
    #define DECIDE_THE_JUMP(_x, _y, dir) \
      if (dir ## _can_jump) { \
        for (int i = 1; i < board.size(); ++i) { \
          if (isInBoundaries((_x), (_y))) { \
            if (board[(_y)][(_x)] == MEN_NONE) { \
              board[(_y)][(_x)] = MEN_POSSIBLE_MOVE; \
              possible_move_present = QPair<int, int>((_x), (_y)); \
            } \
            else { \
              if (i == 1) \
                dir ## _can_jump = false; \
              break; \
            } \
          } \
        } \
      }

    DECIDE_THE_JUMP(tl.first - i, tl.second - i, tl);
    DECIDE_THE_JUMP(tr.first + i, tr.second - i, tr);
    DECIDE_THE_JUMP(bl.first - i, bl.second + i, bl);
    DECIDE_THE_JUMP(br.first + i, br.second + i, br);

    if (tl_can_jump || tr_can_jump || bl_can_jump || br_can_jump)
      can_jump = true;
  }
  else {
    /**
     * nearx, neary ~ the first-hand boxes
     * farx, fary ~ the boxes after the first-hand ones in the diagonal direction
     */
    #define FIND_POSSIBLE_MOVE_FOR_MEN(nearx, neary, farx, fary) \
      if (isInBoundaries(nearx, neary)) { \
        /** the nearest box is empty */ \
        if (board[neary][nearx] == MEN_NONE) { \
          board[neary][nearx] = MEN_POSSIBLE_MOVE; \
          possible_move_present = QPair<int, int>(nearx, neary); \
        } \
        else { \
          /** we can only jump over the opponent's men */ \
          if ((board[neary][nearx] == contra_color.first || \
               board[neary][nearx] == contra_color.second) && \
              isInBoundaries(farx, fary) && \
              board[fary][farx] == MEN_NONE) { \
            board[fary][farx] = MEN_POSSIBLE_MOVE; \
            possible_move_present = QPair<int, int>(farx, fary); \
            can_jump = true; \
          } \
        } \
      }

    // men_white => direction bottom
    if (board[y][x] == MEN_WHITE) {
      // bottom left
      FIND_POSSIBLE_MOVE_FOR_MEN(x -1, y +1, x -2, y +2);
      // bottom right
      FIND_POSSIBLE_MOVE_FOR_MEN(x +1, y +1, x +2, y +2);
    }
    // men_black => direction top
    else if (board[y][x] == MEN_BLACK) {
      // top left
      FIND_POSSIBLE_MOVE_FOR_MEN(x -1, y -1, x -2, y -2);
      // top right
      FIND_POSSIBLE_MOVE_FOR_MEN(x +1, y -1, x +2, y -2);
    }
  }

  if (do_emit) Q_EMIT refresh();

  return can_jump;
}

void Game::hidePossibleMoves(bool do_emit) {
  hidePossibleMoves(-1, -1, do_emit);
}

/**
 * @param x coordinate of cell which will be completely ignored
 * @param y coordinate of cell which will be completely ignored
 * @param if true, do call refresh() at the end
 */
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

void Game::adviceMove(void) {
  if (! isRunning() || isReplaying()) return;

  /** look at this genius AI */
  if (game_state == STATE_CAN_START) {
    board[3][2] = MEN_POSSIBLE_MOVE;
    return;
  }

  /** the previous move wasn't complete (jump necessary) */
  if (last_move_dst.first != -1 &&
      showPossibleMoves(last_move_dst.first, last_move_dst.second, false)) {
    /** make the last found possible move the advice */
    hidePossibleMoves(possible_move_present.first,
                      possible_move_present.second, false);
  }
  else {
    bool possible_move_found = false;
    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board[i].size(); ++j) {
        /** is it white/black men/king? */
        if (isBlackBox(i, j) && board[j][i] != MEN_NONE) {
          if (game_state == STATE_WHITE) {
            if (board[j][i] == MEN_BLACK ||
                board[j][i] == MEN_BLACK_KING)
              continue;
          }
          else {
            if (board[j][i] == MEN_WHITE ||
                board[j][i] == MEN_WHITE_KING)
              continue;
          }

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
 * @param number of moves/steps to proceed
 * @return true if OK;
 */
bool Game::replayMove(unsigned int i, bool forward) {
  if (! isReplaying()) return false;

  for (int x = 0; x < int(i); ++x) {
    if (! moveFromXml(forward))
      /** prevent empty looping */
      break;
  }

  Q_EMIT refresh();
  return true;
}

bool Game::replayMoveToggle(void) {
  if (! isReplaying()) return false;

  Q_ASSERT(replay_timer != NULL);

  if (replay_timer->isPaused()) {
    replay_timer->resume();
    game_state = STATE_REPLAY_TIMED;
  }
  else if (replay_timer->isActive()) {
    replay_timer->pause();
    game_state = STATE_REPLAY_STEP;
  }
  else {
    replay_timer->start();
    game_state = STATE_REPLAY_TIMED;
  }

  return true;
}

bool Game::replayMoveStop(void) {
  if (! isReplaying()) return false;

  if (replay_timer == NULL) return false;

  replay_timer->stop();
  game_state = STATE_REPLAY_STEP;
  return true;
}

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

Player *Game::getPlayerWhite(void) {
  return player_white;
}

Player *Game::getPlayerBlack(void) {
  return player_black;
}

/** synchronize XML tree with content of the Game class */
void Game::syncXml(void) {
  Q_ASSERT(doc != NULL);

  QDomElement e = doc->documentElement().firstChildElement(XML::STR_GAME);
  /** /draughts/game[@type] */
  e.setAttribute(XML::STR_TYPE,
      (socket == NULL) ? XML::STR_LOCAL : XML::STR_NETWORK);
  /** /draughts/game[@host] */
  e.setAttribute(XML::STR_HOST,
      (socket == NULL) ? "" : socket->peerAddress().toString());
  /** /draughts/game[@port] */
  e.setAttribute(XML::STR_PORT,
      (socket == NULL) ? "" : QString::number(remote_server_port));

  e = doc->documentElement().firstChildElement(XML::STR_PLAYERS);
  /** /draughts/players/black */
  e.firstChildElement(XML::STR_WHITE).toText().setData(player_white->name);
  /** /draughts/players/white */
  e.firstChildElement(XML::STR_BLACK).toText().setData(player_black->name);
}

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
  if (doc == NULL) return "";

  QString res;
  syncXml();
  QDomNode mv(doc->documentElement().
      firstChildElement(XML::STR_MOVES).firstChildElement(XML::STR_MOVE));

  int srcx, srcy, dstx, dsty;
  QString last_coord;
  bool processing_white = false;
  bool first_run = true;

  int i = 1;
  while (! mv.isNull()) {
    if (to_current_move_index && (i -2) >= current_move_index)
      break;

    QDomNamedNodeMap map = mv.attributes();
    srcx = map.namedItem(XML::STR_SRCX).nodeValue().toUInt();
    srcy = map.namedItem(XML::STR_SRCY).nodeValue().toUInt();
    dstx = map.namedItem(XML::STR_DSTX).nodeValue().toUInt();
    dsty = map.namedItem(XML::STR_DSTY).nodeValue().toUInt();
    bool kicked = (map.namedItem(XML::STR_KICKED).nodeValue() ==
        XML::STR_NONE) ? false : true;

    IcpSyntaxParser::pair_str_t tmp =
      IcpSyntaxParser::intToStrCoord(srcx, srcy);
    QString src = tmp.first + tmp.second;

    tmp = IcpSyntaxParser::intToStrCoord(dstx, dsty);
    QString dst = tmp.first + tmp.second;

    if (src == last_coord) {
      /** append the dst to the current - it can be only jump, nothing else */
      res.append("x" + dst);
    }
    else {
      if (first_run) {
        first_run = false;
        res.append(QString::number(i) + ".");
        ++i;
      }
      else if (! processing_white) {
        res.append("\n");
        res.append(QString::number(i) + ".");
        ++i;
      }

      res.append(" " + src + ((kicked) ? "x" : "-") + dst);
      processing_white = (processing_white) ? false : true;
    }

    last_coord = dst;
    mv = mv.nextSibling();
  }

  return res;
}

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

  return s;
}

QHostAddress Game::getRemoteAddr(void) {
  Q_ASSERT(socket != NULL);
  return socket->peerAddress();
}

int Game::getRemotePort(void) {
  Q_ASSERT(socket != NULL);
  return socket->peerPort();
}

/**
 * @return filepath of the last opened/saved file in this game
 */
QString Game::getFilePath(void) {
  return filepath;
}

/**
 * @param filepath for saving
 */
void Game::setFilePath(QString fpath) {
  filepath = fpath;
}

/** received only once */
void Game::gotConnected(void) {
  Q_ASSERT(game_state == STATE_WAIT_FOR_CONNECTION);

  /** initiate communication */
  if (socket->write(QString(
        TOK::INVITE +
        QString((player_white->local) ? TOK::WHITE : TOK::BLACK) +
        QString::number(server->serverPort()) + " " +
        ((player_white->local) ? player_white->name : player_black->name)
        ).toLocal8Bit()) == -1) {
    err_queue.append(socket->errorString());
  }

  socket->flush();
  game_state = STATE_INVITE_DISPATCH;
}

void Game::gotNewData(void) {
  Q_ASSERT(socket != NULL);
  QString s(socket->readAll());
  NetCmdParser parser(s);

  switch (parser.getNextCmd()) {
    case NetCmdParser::INVITE:
      if (game_state == STATE_WAIT_FOR_REMOTE) {
        if (parser.getNextCmd() == NetCmdParser::BLACK) {
          player_black->local = false;
          player_white->local = true;
        }
        else {
          player_black->local = true;
          player_white->local = false;
        }

        /** keep empty parts */
        QStringList tmp = parser.getRest().split(" ");
        remote_server_port = tmp.at(0).toInt();
        tmp.removeFirst();
        game_state = STATE_INVITE_RECEIVED;

        if (player_black->local)
          player_white->name = tmp.join(" ");
        else
          player_black->name = tmp.join(" ");

        /** ask user if he wants to proceed */
        Q_EMIT gotInvite(Player::COLOR_BLACK, tmp.join(" "));
      }
      break;

    case NetCmdParser::INVITE_ACCEPT:
      if (game_state == STATE_INVITE_DISPATCH) {
        if (socket->write(QString(
                TOK::GAME +
                ((remote_will_load) ? (TOK::LOAD + getXmlStr()) : TOK::NEW)
                ).toLocal8Bit()) == -1) {
          err_queue.append(socket->errorString());
        }

        /** thanks to TCP, the other side has received this message and
            thus we can start the game itself */
        game_state = STATE_CAN_START;
      }
      break;

    /** we can start playing */
    case NetCmdParser::GAME:
      if (game_state == STATE_INVITE_ANSWERED) {
        if (parser.getNextCmd() == NetCmdParser::NEW) {
          initXml();
          game_state = STATE_CAN_START;
        }
        /** load */
        else {
          if (! doc->setContent(parser.getRest())) {
            qDebug("setContent() from network failed");
            game_state = STATE_END;
            return;
          }

          game_state = STATE_CAN_START;

          /** interpret all available moves */
          while (moveFromXml(true));
        }

        Q_EMIT refresh();
      }

      break;
    case NetCmdParser::MOVE:
      //FIXME check state?
      {
        QStringList coord = parser.getRest().split(" ", QString::SkipEmptyParts);
        move(coord.at(0).toUInt(), coord.at(1).toUInt(),
            coord.at(2).toUInt(), coord.at(3).toUInt(), false);
        break;
      }
    case NetCmdParser::INVITE_REJECT:
    case NetCmdParser::EXIT:
      game_state = STATE_END;
      socket->disconnectFromHost();
      Q_EMIT gotExit();
      break;
    default:
      break;
  }
}

void Game::gotDisconnected(void) {
  socket->deleteLater();
  game_state = STATE_END;
  Q_EMIT refresh();
}

void Game::gotTimeout(void) {
  /** move by 1 forward */
  if (! replayMove(1, true))
    replay_timer->stop();
  Q_EMIT refresh();
}

/** called after the user do or do not accepts the invite */
void Game::dispatchUserResponseInvite(bool yes) {
  Q_ASSERT(socket != NULL);

  /** this slot must be called only after invite was received */
  if (game_state != STATE_INVITE_RECEIVED) return;

  if (yes) {
    if (socket->write(QString(TOK::INVITE_ACCEPT +
            ((player_white->local) ? player_white->name : player_black->name))
          .toLocal8Bit()) == -1)
      err_queue.append(socket->errorString());

    game_state = STATE_INVITE_ANSWERED;
  }
  else {
    if (socket->write(QString(TOK::INVITE_REJECT).toLocal8Bit()) == -1)
      err_queue.append(socket->errorString());

    game_state = STATE_END;
    socket->disconnectFromHost();
  }
}

/** user wants to exit */
void Game::dispatchUserResponseExit(void) {
  if (isLocal()) return;

  if (socket->write(QString(TOK::EXIT).toLocal8Bit()) == -1)
    err_queue.append(socket->errorString());

  socket->disconnectFromHost();
  game_state = STATE_END;
}
