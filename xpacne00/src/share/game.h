/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __GAME_H__
#define __GAME_H__

#include <QtCore>
#include <QtNetwork>
#include <QtXml>

//#if (__cplusplus > 199711L)
///** deal with C++11 */
//#define CONST_STR(name, value) static char (name)[sizeof(value)] = (value)
//#else
#define CONST_STR(name, value) const char (name)[sizeof(value)] = (value)
//#endif

#define CONST_STR_LEN(value) (sizeof(value) -1)

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
    static pair_uint_t strCoordToUInt(QString, bool *);
    static pair_str_t intToStrCoord(unsigned int, unsigned int);
};

namespace TOK {
  CONST_STR( INVITE       , "INVITE "        );
  CONST_STR( INVITE_ACCEPT, "INVITE_ACCEPT " );
  CONST_STR( INVITE_REJECT, "INVITE_REJECT"  );
  CONST_STR( GAME         , "GAME "          );
  CONST_STR( WHITE        , "WHITE "         );
  CONST_STR( BLACK        , "BLACK "         );
  CONST_STR( NEW          , "NEW"            );
  CONST_STR( LOAD         , "LOAD "          );
  CONST_STR( GAME_ACCEPT  , "GAME_ACCEPT"    );
  CONST_STR( GAME_REJECT  , "GAME_REJECT"    );
  CONST_STR( MOVE         , "MOVE "          );
  CONST_STR( EXIT         , "EXIT"           );
}

class NetCmdParser {
  public:
    typedef enum {
      NONE         ,
      INVITE       ,
      INVITE_ACCEPT,
      INVITE_REJECT,
      GAME         ,
      WHITE        ,
      BLACK        ,
      NEW          ,
      LOAD         ,
      GAME_ACCEPT  ,
      GAME_REJECT  ,
      MOVE         ,
      EXIT         ,
    } tok_t;

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
    const Game &par;
    int kicked;
  public:
    typedef enum {
      COLOR_DONT_KNOW,
      COLOR_WHITE,
      COLOR_BLACK,
    } color_t;

    QString name;
    bool local;

    Player(const Game &);
    bool incKicked(void);
    bool decKicked(void);
    int getKicked(void);
};

namespace XML {
  CONST_STR( STR_DRAUGHTS     , "draughts"      );
  CONST_STR( STR_GAME         , "game"          );
  CONST_STR( STR_TYPE         , "type"          );
  CONST_STR( STR_LOCAL        , "local"         );
  CONST_STR( STR_NETWORK      , "network"       );
  CONST_STR( STR_HOST         , "host"          );
  CONST_STR( STR_PORT         , "port"          );
  CONST_STR( STR_PLAYERS      , "players"       );
  CONST_STR( STR_WHITE        , "white"         );
  CONST_STR( STR_WHITE_KING   , "white_king"    );
  CONST_STR( STR_BLACK        , "black"         );
  CONST_STR( STR_BLACK_KING   , "black_king"    );
  CONST_STR( STR_NONE         , "none"          );
  CONST_STR( STR_MOVES        , "moves"         );
  CONST_STR( STR_MOVE         , "move"          );
  CONST_STR( STR_SRCX         , "srcx"          );
  CONST_STR( STR_SRCY         , "srcy"          );
  CONST_STR( STR_DSTX         , "dstx"          );
  CONST_STR( STR_DSTY         , "dsty"          );
  CONST_STR( STR_KICKEDX      , "kickedx"       );
  CONST_STR( STR_KICKEDY      , "kickedy"       );
  CONST_STR( STR_KICKED       , "kicked"        );
  CONST_STR( STR_BECAME_A_KING, "became-a-king" );
  CONST_STR( STR_TRUE         , "true"          );
  CONST_STR( STR_FALSE        , "false"         );
}

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
      /** common states */
      STATE_PRE_INIT,
      STATE_CAN_START,
      STATE_WHITE,
      STATE_BLACK,
      /** network game specific states */
      STATE_WAIT_FOR_CONNECTION,
      STATE_WAIT_FOR_REMOTE,
      STATE_INVITE_DISPATCH,
      STATE_INVITE_RECEIVED,
      STATE_INVITE_ANSWERED,
      /** replay game specific states */
      STATE_REPLAY_STEP,  /**< accepting user steps */
      STATE_REPLAY_TIMED,
      STATE_REPLAY_STOP,
      STATE_END,  /**< disconnected, exit, error, etc. */
    } state_t;

    const int DEFAULT_TIMEOUT;  /**< miliseconds */

    /**
     * the top left corner is always white
     * convention: board(ROWS, QVector<Player>(COLUMNS))
     */
    QVector< QVector<men_t> > board;

    Game(QTcpServer *);  /**< port number of the running QTcpServer */
    ~Game(void);
    bool gameRemote(QHostAddress, int, Player::color_t);  /**< locally initiated */
    bool gameRemote(QTcpSocket *);  /**< remotely initiated */
    bool gameLocal(void);
    bool gameLocal(bool);
    bool gameFromFile(QString, Player::color_t);
    //FIXME some flag, that the game can not be user-modified while replaying
    bool gameFromFile(QString);  /**< game replay (switches immediately to stopped state) */
    bool isInNetworkMeantime(void);
    bool isRunning(void);  /**< is the game running in either mode? (e.g. replaying loaded game; playing network game etc.) */
    bool isLocal(void);
    state_t getState(void);//FIXME not needed?

    err_t move(unsigned int, unsigned int, unsigned int, unsigned int, bool do_emit = true);
    bool showPossibleMoves(unsigned int, unsigned int, bool do_emit = true);
    void hidePossibleMoves(bool do_emit = true);
    void hidePossibleMoves(int, int, bool do_emit = true);
    void adviceMove(bool do_emit = true);

    void setReplayTimeout(int);
    bool replayMove(unsigned int, bool);
    bool replayMoveToggle(void);  /**< toggle pause/play */
    bool replayMoveStop(void);
    /** used e.g. for user input locking */
    bool isReplaying(void);
    bool isBlackBox(unsigned int, unsigned int);

    Player *getPlayerWhite(void);
    Player *getPlayerBlack(void);

    //FIXME update variables inside (player names etc.)
    QString getXmlStr(void);
    QString getIcpSyntaxStr(bool to_current_move_index = false);
    QString getError(void);
    QHostAddress getRemoteAddr(void);
    int getRemotePort(void);
    QString getFilePath(void);
    void setFilePath(QString);

  private:
    /** the whole game XML tree including history */
    QDomDocument *doc;
    QTcpSocket *socket;
    QTcpServer *server;  /** only to read the port server is listening on */
    Player *player_white;
    Player *player_black;
    QString filepath;
    /** queue for errors which can not be output immediately */
    QList<QString> err_queue;
    state_t game_state;

    /** used for checking of possible moves before a user a move is processed */
    QPair<int, int> last_move_dst;
    IcpSyntaxParser::pair_uint_t possible_jump;
    QPair<int, int> possible_move_src;
    QPair<int, int> possible_move_dst;
    /** for replay mode: index of item with the currently applied move */
    int current_move_index;
    Player::color_t game_ai;

    QTimerImproved *replay_timer;
    int replay_delay;
    int replay_stop_index;

    /** needed for game loading from XML, because the connection port
       is temporary */
    int remote_server_port;
    /** we are loading game from file and the remote side will load our game */
    bool remote_will_load;

    /** create a new document template if none is existing yet */
    void initXml(void);
    void appendMoveToXml(unsigned int, unsigned int, unsigned int, unsigned int,
        int, int, men_t, bool);
    bool moveFromXml(bool forward = true);
    void syncXml(void);
    bool loadFromIcpSyntax(QString);
    void prepareNewSocket(QHostAddress, int);
    void prepareNewTimer(void);
    Player *getPlayerFromCoord(unsigned int, unsigned int);
    bool isInBoundaries(int, int);
    bool isBecomingAKing(men_t, unsigned int);

  public Q_SLOTS:
    //QMessageBox::critical(this, tr("Network error"), socket->errorString());
    void gotConnected(void);
    void gotNewData(void);
    void gotDisconnected(void);  /**< disconnect, error */
    void gotTimeout(void);

    void dispatchUserResponseInvite(bool);
    void dispatchUserResponseExit(void);

  Q_SIGNALS:
    /** emitted whenever a board has changed (thus redraw is needed) */
    void refresh(void);
    /** let the user accept the new connection */
    void gotInvite(Player::color_t, QString);
    void gotExit(void);
};

#endif
