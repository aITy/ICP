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
    singleShot((end - begin) % period, this, SLOT(resendTimeout));
  }
}

//TODO OK
bool QTimerImproved::isPaused(void) {
  return paused;
}

//TODO OK
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

//TODO OK
/** x ~ a-h; y ~ 0-8 */
IcpSyntaxParser::pair_str_t IcpSyntaxParser::intToStrCoord(unsigned int x, unsigned int y) {
  Q_ASSERT(x + 'a' <= 'h');
  pair_str_t p(
      QString(static_cast<char>(x + 'a')),
      QString::number(y +1));
  return p;
}

//TODO OK
NetCmdParser::NetCmdParser(QString _s) : last_cmd(NONE), s(_s) { }

//TODO OK
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
  return true;
}

//TODO OK
int Player::getKicked(void) {
  return kicked;
}

//TODO OK
void Game::initXml(void) {
  /** it is yet initialized */
  if (! doc->firstChild().isNull()) return;

  /** special XML characters are escaped automatically */
  if (! doc->setContent(
      QString("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
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

  attr = doc->createAttribute(XML::STR_BECAME_A_KING);
  attr.setValue(became_a_king ? XML::STR_TRUE : XML::STR_FALSE);
  el_move.setAttributeNode(attr);

  doc->elementsByTagName(XML::STR_MOVES).at(0).appendChild(el_move);
  //doc->elementsByTagName(XML::STR_MOVES).at(0).toElement().appendChild(el_move);
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

  qDebug("calling........................");//FIXME

  int i;
  /** get to the current index */
  for (i = 0; i < current_move_index; ++i) {
    if (mv.nextSibling().isNull()) {
      break;
    }
    else {
      mv = mv.nextSibling();
    }
  }

  /** the last performed move is the current one (but
      do not include the first move) */
  if (forward && current_move_index > 0) {
    mv = mv.nextSibling();
  }

  if (mv.isNull()) {
    //if (! isReplaying()) game_state = STATE_END;
    return false;
  }

  if (forward)
    current_move_index++;
  else
    current_move_index--;

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
    qDebug("howk00");//FIXME
    if (forward) {
      if (! getPlayerFromCoord(kickedx, kickedy)->incKicked())
        if (! isReplaying())
          game_state = STATE_END;

      board[kickedy][kickedx] = MEN_NONE;
    }
    else {
      if (! getPlayerFromCoord(kickedx, kickedy)->decKicked())
        if (! isReplaying())
          game_state = STATE_CAN_START;

      board[kickedy][kickedx] = kicked_men;
    }
  }

  qDebug("srcx " + QString::number(srcx).toLocal8Bit());//FIXME
  qDebug("srcy " + QString::number(srcy).toLocal8Bit());//FIXME
  qDebug("dstx " + QString::number(dstx).toLocal8Bit());//FIXME
  qDebug("dsty " + QString::number(dsty).toLocal8Bit());//FIXME

  if (forward) {
    if (! isReplaying()) {
      if (board[srcy][srcx] == MEN_WHITE ||
          board[srcy][srcx] == MEN_WHITE_KING)
        game_state = STATE_WHITE;
      else
        game_state = STATE_BLACK;
    }

    if (became_a_king) {
    qDebug("howk02");//FIXME
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
    qDebug("howk03");//FIXME
      board[dsty][dstx] = board[srcy][srcx];
      last_move_dst.first  = dstx;
      last_move_dst.second = dsty;
    }

    board[srcy][srcx] = MEN_NONE;
  }
  else {
    qDebug("howk04");//FIXME
    if (! isReplaying()) {
    qDebug("howk05");//FIXME
      if (board[dsty][dstx] == MEN_WHITE ||
          board[dsty][dstx] == MEN_WHITE_KING)
        game_state = STATE_WHITE;
      else
        game_state = STATE_BLACK;
    }

    if (became_a_king) {
    qDebug("howk06");//FIXME
      men_t tmp;

      if (board[dsty][dstx] == MEN_WHITE_KING)
        tmp = MEN_WHITE;
      else
        tmp = MEN_BLACK;

      board[srcy][srcx] = tmp;
    }
    else {
    qDebug("howk07");//FIXME
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
Game::Game(void) :
  DEFAULT_TIMEOUT(300),
  board(8, QVector<men_t>(8, MEN_NONE)),
  socket(NULL),
  game_state(STATE_PRE_INIT),
  remote_server_port(-1),
  last_move_dst(-1, -1),
  possible_move_present(-1, -1),
  current_move_index(-1),
  game_ai(Player::COLOR_DONT_KNOW),
  replay_timer(NULL),
  replay_delay(DEFAULT_TIMEOUT)
{
  player_white = new Player(this);
  player_white->name = "Player White";
  player_black = new Player(this);
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
/**
 * we are founding a completely new game
 * @return true if OK
 */
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

/**
 * we are connecting to a "running" game
 * @return true if OK
 */
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
 * @param if true, the computer AI will black; otherwise AI will be white
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
 * @return true if OK
 */
//TODO OK
bool Game::gameFromFile(QString s, Player::color_t color) {
  if (isRunning()) return false;

  filepath = s;
  QFile f(filepath);

  /** gets closed automatically while destructing */
  if (! f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

  QTextStream stream(&f);
  QString err; int line, column;

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
  else if (f.seek(0) && doc->setContent(&f, false, &err, &line, &column)) {
    /** local game */
    if (doc->documentElement().firstChildElement(XML::STR_GAME).
        attributeNode(XML::STR_TYPE).value() == XML::STR_LOCAL) {
      game_state = STATE_CAN_START;

      /** interpret all available moves */
      while (moveFromXml(true));
      //FIXME
      #define PRINT_STATE(st) if (game_state == (st)) qDebug(#st);
      PRINT_STATE(STATE_PRE_INIT            );
      PRINT_STATE(STATE_CAN_START           );
      PRINT_STATE(STATE_RUNNING             );
      PRINT_STATE(STATE_WHITE               );
      PRINT_STATE(STATE_BLACK               );
      PRINT_STATE(STATE_WAIT_FOR_CONNECTION );
      PRINT_STATE(STATE_INVITE_DISPATCH     );
      PRINT_STATE(STATE_INVITE_RECEIVE      );
      PRINT_STATE(STATE_GAME_DISPATCH       );
      PRINT_STATE(STATE_GAME_RECEIVE        );
      PRINT_STATE(STATE_MOVE_DISPATCH       );
      PRINT_STATE(STATE_MOVE_RECEIVE        );
      PRINT_STATE(STATE_EXIT_DISPATCH       );
      PRINT_STATE(STATE_EXIT_RECEIVE        );
      PRINT_STATE(STATE_REPLAY_STEP         );
      PRINT_STATE(STATE_REPLAY_TIMED        );
      PRINT_STATE(STATE_REPLAY_STOP         );
      PRINT_STATE(STATE_END                 );
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
        qDebug("WTF00");//FIXME
        return false;
      }
    }
    else {
      err_queue.append("ERR: Bad XML format (can't decide whether it is local/network game)!");
      return false;
    }
  }
  else {
    qDebug("ABC00: " + err.toLocal8Bit() + "; line " +
        QString::number(line).toLocal8Bit() + "; column " +
        QString::number(column).toLocal8Bit());//FIXME
    err_queue.append("ERR: Bad file format (only XML or ICP syntax supported)!");
    return false;
  }

  Q_EMIT refresh();
  return true;
}

/** replay loaded game
 * @return true if OK
 */
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

  /** in network game color matters */
  if (! isLocal()) {
    /** am I allowed to move this men? */
    if (! ((player_white->local && white_is_playing) ||
          (player_black->local && black_is_playing)) ) {
      /** do nothing and be silent */
      return ERR_OK;
    }
  }

  if ((loading || game_state == STATE_CAN_START) && board[srcy][srcx] != MEN_WHITE) {
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

  /** force to alternate moves */
  if ((white_is_playing && game_state == STATE_WHITE) ||
      (black_is_playing && game_state == STATE_BLACK)) {
    err_queue.append("ERR: Players must alternate.");
    return ERR_INVALID_MOVE;
  }

  //FIXME
//  if (game_state == STATE_WHITE && game_ai == Player::COLOR_BLACK) {
//    adviceMove();
//    if (can make another move)
//      call move();
//    else
//      game_state = state_white
//
//    Q_EMIT refresh();
//    return ERR_OK;
//  }
//  else if (game_state == STATE_BLACK && game_ai == Player::COLOR_BLACK) {
//  }

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

    if (kickedx != -1) {
      if (! getPlayerFromCoord(kickedx, kickedy)->incKicked())
        game_state = STATE_END;
      board[kickedy][kickedx] = MEN_NONE;
    }
  }

  hidePossibleMoves(false);

  bool became_a_king = isBecomingAKing(board[srcy][srcx], dsty);
  board[dsty][dstx] = board[srcy][srcx];
  board[srcy][srcx] = MEN_NONE;
  //FIXME add support for network game!
  appendMoveToXml(srcx, srcy, dstx, dsty, kickedx, kickedy, became_a_king);

  //FIXME what about network game?
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

    QPair<int, int> contra_color;

    if (board[y][x] == MEN_BLACK_KING)
      contra_color = QPair<int, int>(MEN_WHITE, MEN_WHITE_KING);
    else
      contra_color = QPair<int, int>(MEN_BLACK, MEN_BLACK_KING);

    bool tl_can_jump = false;
    bool tr_can_jump = false;
    bool bl_can_jump = false;
    bool br_can_jump = false;

    /** find out if it is oponnent's men/king and set the can_jump flag */
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
 * @param number of moves/steps to proceed
 * @return true if OK;
 */
//TODO OK
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

//TODO OK
bool Game::replayMoveToggle(void) {
  if (! isReplaying()) return false;

  if (replay_timer == NULL) return false;

  if (replay_timer->isPaused())
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

  QDomElement e = doc->documentElement().firstChildElement(XML::STR_GAME);
  /** /draughts/game[@type] */
  e.setAttribute(XML::STR_TYPE,
      (socket == NULL) ? XML::STR_LOCAL : XML::STR_NETWORK);
  /** /draughts/game[@host] */
  e.setAttribute(XML::STR_HOST,
      (socket == NULL) ? "" : socket->peerAddress().toString());
  /** /draughts/game[@port] */
  e.setAttribute(XML::STR_PORT,
      (socket == NULL) ? "" : remote_server_port);

  e = doc->documentElement().firstChildElement(XML::STR_PLAYERS);
  /** /draughts/players/black */
  e.firstChildElement(XML::STR_WHITE).toText().setData(player_white->name);
  /** /draughts/players/white */
  e.firstChildElement(XML::STR_BLACK).toText().setData(player_black->name);
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
  to_current_move_index = to_current_move_index;//FIXME delete
  QString res;
  //FIXME only for debug
  return res;

//  if (! isRunning()) return res;
//
//  Q_ASSERT(doc != NULL);
//  syncXml();
//  QDomElement mv(doc->documentElement().
//  firstChildElement(XML::STR_MOVES).firstChildElement(XML::STR_MOVE));
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
//        srcx = QString(map.namedItem(XML::STR_SRCX).nodeValue()).toUInt();
//        srcy = QString(map.namedItem(XML::STR_SRCY).nodeValue()).toUInt();
//        dstx = QString(map.namedItem(XML::STR_DSTX).nodeValue()).toUInt();
//        dsty = QString(map.namedItem(XML::STR_DSTY).nodeValue()).toUInt();
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

  return s;
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

/**
 * @return filepath of the last opened/saved file in this game
 */
QString Game::getFilePath(void) {
  return filepath;
}

/** received only once */
//TODO OK
void Game::gotConnected(void) {
  /** initiate communication */
  //FIXME toLocal8Bit().constData()
  socket->write(QString(
        TOK::INVITE +
        QString::number(qobject_cast<QTcpServer *>(socket->parent())->serverPort()) +
        " " + ((player_white->local) ? player_white->name : player_black->name)
        ).toLocal8Bit());
  socket->flush();
  game_state = STATE_INVITE_DISPATCH;
}

void Game::gotNewData(void) {
  qDebug("UNIMPLEMENTED: gotNewData()");//FIXME
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
  qDebug("UNIMPLEMENTED: gotDisconnected()");//FIXME
  //QTcpSocket* socket = qobject_cast<QTcpSocket*>( this->sender() );
  //if (this->sender()->parent != this)

  socket->deleteLater();
  game_state = STATE_END;
  Q_EMIT refresh();
}

void Game::gotTimeout(void) {
  qDebug("UNIMPLEMENTED: gotTimeout()");//FIXME
  //FIXME move forward by 1; refresh
  Q_EMIT refresh();
}
