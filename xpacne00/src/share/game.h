/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __GAME_H__
#define __GAME_H__

#include <QtCore>
#include <QtNetwork>
#include <QtXml>

#if (__cplusplus > 199711L)
/** deal with C++11 */
#define CONST_STR(name, value) \
  static constexpr char (name)[sizeof(value)] = (value)
#else
#define CONST_STR(name, value) \
  static const char (name)[sizeof(value)] = (value)
#endif

#define CONST_STR_LEN(value) \
  (sizeof(value) -1)

/**
 * timer supporting pause
 */
class QTimerImproved : public QTimer {
  Q_OBJECT

  private:
    qint64 begin, end;
    int period;
    bool paused;
    bool singleShotScheduledInternally;
    bool originalWasSingleShot;
  private Q_SLOTS:
    void resendTimeout(void);
  public:
    QTimerImproved(QObject *par = 0);
    void start(void);
    void start(int);
    void stop(void);
    void pause(void);
    void resume(void);
    bool isPaused(void);
  Q_SIGNALS:
    void newTimeout();
};

class IcpSyntaxParser {
  public:
    typedef QPair<unsigned int, unsigned int> pair_uint_t;
    typedef QPair<QString, QString> pair_str_t;
    static pair_uint_t strCoordToUInt(QString);
    static pair_str_t intToStrCoord(unsigned int, unsigned int);
};

class NetCmdParser {
  public:
    typedef enum {
      NONE         ,
      INVITE       ,
      INVITE_ACCEPT,
      INVITE_REJECT,
      //GAME         ,  //FIXME
      WHITE        ,
      BLACK        ,
      NEW          ,
      LOAD         ,
      GAME_ACCEPT  ,
      GAME_REJECT  ,
      MOVE         ,
      EXIT         ,
    } tok_t;

    CONST_STR( TOK_INVITE       , "INVITE "        );
    CONST_STR( TOK_INVITE_ACCEPT, "INVITE_ACCEPT " );
    CONST_STR( TOK_INVITE_REJECT, "INVITE_REJECT"  );
    CONST_STR( TOK_GAME         , "GAME "          );
    CONST_STR( TOK_WHITE        , "WHITE "         );
    CONST_STR( TOK_BLACK        , "BLACK "         );
    CONST_STR( TOK_NEW          , "NEW "           );
    CONST_STR( TOK_LOAD         , "LOAD "          );
    CONST_STR( TOK_GAME_ACCEPT  , "GAME_ACCEPT"    );
    CONST_STR( TOK_GAME_REJECT  , "GAME_REJECT"    );
    CONST_STR( TOK_MOVE         , "MOVE "          );
    CONST_STR( TOK_EXIT         , "EXIT"           );

    NetCmdParser(QString);
    tok_t getNextCmd();
    QString getRest();

  private:
    tok_t last_cmd;
    QString s;
};

class Game;

class Player {
  private:
    Game *par;
    int kicked;
  public:
    typedef enum {
      COLOR_DONT_KNOW,
      COLOR_WHITE,
      COLOR_BLACK,
    } color_t;

    QString name;
    bool local;

    Player(Game *);
    bool incKicked(void);
    bool decKicked(void);
    int getKicked(void);
};

/**
 * class for one game counting 2 players
 */
class Game : public QObject {
  Q_OBJECT

  public:
    typedef enum {
      ERR_OK = 0,
      ERR_INVALID_MOVE,
      ERR_WHITE_MUST_START,
      ERR_PREV_MOVE_NOT_FINISHED,
    } err_t;

    typedef enum {
      /** empty boxes on the board */
      MEN_NONE,
      /** denotes boxes where the given draughtsmen can possibly move */
      MEN_POSSIBLE_MOVE,
      MEN_WHITE,
      MEN_WHITE_KING,
      MEN_BLACK,
      MEN_BLACK_KING,
    } men_t;

    typedef enum {
      NOTATION_XML,
      NOTATION_ICP,
    } notation_t;

    typedef enum {
      STATE_PRE_INIT,
      STATE_CAN_START,
      STATE_RUNNING,
      STATE_WHITE,
      STATE_BLACK,
      STATE_WAIT_FOR_CONNECTION,
      STATE_INVITE_DISPATCH,  /**< question */
      STATE_INVITE_RECEIVE,  /**< answer */
      STATE_GAME_DISPATCH,
      STATE_GAME_RECEIVE,
      STATE_MOVE_DISPATCH,
      STATE_MOVE_RECEIVE,
      STATE_EXIT_DISPATCH,  /**< FIXME redundant??? */
      STATE_EXIT_RECEIVE,
      STATE_REPLAY_STEP,  /**< accepting user steps */
      STATE_REPLAY_TIMED,
      STATE_REPLAY_STOP,
      STATE_END,  /**< disconnected, exit, error, etc. */
    } state_t;

    CONST_STR( XML_STR_DRAUGHTS     , "draughts"      );
    CONST_STR( XML_STR_GAME         , "game"          );
    CONST_STR( XML_STR_TYPE         , "type"          );
    CONST_STR( XML_STR_LOCAL        , "local"         );
    CONST_STR( XML_STR_NETWORK      , "network"       );
    CONST_STR( XML_STR_HOST         , "host"          );
    CONST_STR( XML_STR_PORT         , "port"          );
    CONST_STR( XML_STR_PLAYERS      , "players"       );
    CONST_STR( XML_STR_WHITE        , "white"         );
    CONST_STR( XML_STR_WHITE_KING   , "white_king"    );
    CONST_STR( XML_STR_BLACK        , "black"         );
    CONST_STR( XML_STR_BLACK_KING   , "black_king"    );
    CONST_STR( XML_STR_MOVES        , "moves"         );
    CONST_STR( XML_STR_MOVE         , "move"          );
    CONST_STR( XML_STR_SRCX         , "srcx"          );
    CONST_STR( XML_STR_SRCY         , "srcy"          );
    CONST_STR( XML_STR_DSTX         , "dstx"          );
    CONST_STR( XML_STR_DSTY         , "dsty"          );
    CONST_STR( XML_STR_KICKEDX      , "kickedx"       );
    CONST_STR( XML_STR_KICKEDY      , "kickedy"       );
    CONST_STR( XML_STR_KICKED       , "kicked"        );
    CONST_STR( XML_STR_BECAME_A_KING, "became-a-king" );
    CONST_STR( XML_STR_TRUE         , "true"          );
    CONST_STR( XML_STR_FALSE        , "false"         );

    const int DEFAULT_TIMEOUT = 300;  /**< miliseconds */

    /**
     * the top left corner is always white
     * convention: board(ROWS, QVector<Player>(COLUMNS))
     */
    QVector< QVector<men_t> > board;

    Game(void);
    ~Game(void);
    /** @return true if OK */
    bool gameRemote(QHostAddress, int, Player::color_t);  /**< locally initiated */
    /** @return true if OK */
    bool gameRemote(QTcpSocket *);  /**< remotely initiated */
    /** @return true if OK */
    bool gameLocal(void);
    /** @return true if OK */
    bool gameFromFile(QString, Player::color_t);
    //FIXME some flag, that the game can not be user-modified while replaying
    //  tohle prepne do stavu PAUSE
    /** @return true if OK */
    bool gameFromFile(QString, bool);  /**< game replay */
    //FIXME adjust to state_t!!!
    bool isRunning(void);  /**< is the game running in either mode? (e.g. replaying loaded game; playing network game etc.) */
    bool isLocal(void);
    state_t getState(void);//FIXME not needed?

    err_t move(unsigned int, unsigned int, unsigned int, unsigned int, bool loading = false);
    bool showPossibleMoves(unsigned int, unsigned int, bool do_emit = true);
    void hidePossibleMoves(bool do_emit = true);
    void hidePossibleMoves(int, int, bool do_emit = true);
    void adviceMove(void);

    void setReplayTimeout(int);
    bool replayMove(unsigned int, bool);
    bool replayMoveToggle(void);  /**< toggle pause/play */
    bool replayMoveStop(void);
    /** used e.g. for user input locking */
    bool isReplaying(void);

    Player *getPlayerWhite(void);
    Player *getPlayerBlack(void);

    //FIXME update variables inside (player names etc.)
    QString getXmlStr(void);
    QString getIcpSyntaxStr(bool to_current_move_index = false);
    QString getError(void);
    QHostAddress getRemoteAddr(void);
    int getRemotePort(void);
    QString getFilePath(void);

  private:
    /** the whole game XML tree including history */
    QDomDocument *doc;
    QTcpSocket *socket;
    Player *player_white;
    Player *player_black;
    QString filepath;
    /** queue for errors which can not be output immediately */
    QList<QString> err_queue;
    state_t game_state;
    /** needed for game loading from XML, because the connection port
       is temporary */
    QString remote_server_port;
    /** we are loading game from file and the remote side will load our game */
    bool remote_will_load;

    /** used for checking of possible moves before a user a move is processed */
    QPair<int, int> last_move_dst;
    IcpSyntaxParser::pair_uint_t possible_jump;
    QPair<int, int> possible_move_present;
    /** for replay mode: index of item with the currently applied move */
    int current_move_index;

    QTimerImproved *replay_timer;
    int replay_delay;
    int replay_stop_index;

    /** create a new document template if none is existing yet */
    void initXml(void);
    void appendMoveToXml(unsigned int, unsigned int, unsigned int, unsigned int, int, int, bool);
    bool moveFromXml(bool forward = true);
    void syncXml(void);
    bool loadFromIcpSyntax(QString);
    void prepareNewSocket(QHostAddress, int);
    void prepareNewTimer(void);
    Player *getPlayerFromCoord(unsigned int, unsigned int);
    bool isBlackBox(unsigned int, unsigned int);
    bool isInBoundaries(int, int);
    bool isBecomingAKing(men_t, unsigned int);

  public Q_SLOTS:
    //QMessageBox::critical(this, tr("Network error"), socket->errorString());
    void gotConnected(void);
    void gotNewData(void);
    void gotDisconnected(void);  /**< disconnect, error */
    void gotTimeout(void);

  Q_SIGNALS:
    /** emitted whenever a board has changed (thus redraw is needed) */
    void refresh(void);
};

#endif