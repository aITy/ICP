/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __GAME_H__
#define __GAME_H__

#include <QtCore>
#include <QtNetwork>

/**
 * timer supporting pause
 */
class QTimerImproved : public QTimer {
  private:
    qint64 begin;
    int period;
    bool paused;
    bool singleShotScheduledInternally;
    bool originalWasSingleShot;
  private Q_SLOTS:
    resendTimeout(void);
  public:
    void QTimerImproved(void);
    void start(void);
    void start(int);
    void stop(void);
    void pause(void);
    void resume(void);
    bool isPaused(void);
  Q_SIGNALS:
    void newTimeout(void);
}

class IcpSyntaxParser {
  public:
    typedef QPair<unsigned int, unsigned int> pair_uint_t;
    typedef QPair<QString, QString> pair_str_t;
    pair_uint_t strCoordToUint(QString);
    pair_str_t intToStrCoord(unsigned int, unsigned int);
};

class NetCmdParser {
  private:
    cmd_t last_cmd;
    QString s;
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

    const char[] TOK_INVITE        = "INVITE ";
    const char[] TOK_INVITE_ACCEPT = "INVITE_ACCEPT ";
    const char[] TOK_INVITE_REJECT = "INVITE_REJECT";
    const char[] TOK_GAME          = "GAME ";
    const char[] TOK_WHITE         = "WHITE ";
    const char[] TOK_BLACK         = "BLACK ";
    const char[] TOK_NEW           = "NEW ";
    const char[] TOK_LOAD          = "LOAD ";
    const char[] TOK_GAME_ACCEPT   = "GAME_ACCEPT";
    const char[] TOK_GAME_REJECT   = "GAME_REJECT";
    const char[] TOK_MOVE          = "MOVE ";
    const char[] TOK_EXIT          = "EXIT";

    CmdParser(QString);
    cmd_t getNextCmd();
    QString getRest();
};

class Player {
  private:
    QObject *parent;
    int kicked;
  public:
    typedef enum {
      COLOR_DONT_KNOW,
      COLOR_WHITE,
      COLOR_BLACK,
    } color_t;

    QString name;
    bool local;

    Player(QObject *);
    bool incKicked(void);
    void decKicked(void);
    int getKicked(void);
};

/**
 * class for one game counting 2 players
 */
class Game : public QObject {
  Q_OBJECT

  private:
    typedef enum {
      move_xml_backwards,
      move_xml_forwards,
    } move_xml_dir_t;

    /** the whole game XML tree including history */
    QDomDocument *doc;
    QTcpSocket *socket;
    Player *player_white;
    Player *player_black;
    QString filepath;
    /** queue for errors which can not be output immediately */
    QList<QString> err_queue;
    game_state_t game_state;
    /** needed for game loading from XML, because the connection port
       is temporary */
    QString remote_server_port;
    /** we are loading game from file and the remote side will load our game */
    bool remote_will_load;

    /** used for checking of possible moves before a user a move is processed */
    IcpSyntaxParser::pair_uint_t last_move_dst;
    IcpSyntaxParser::pair_uint_t possible_jump;
    bool possible_move_present;
    /** for replay mode: pointer to (index of) item with the current move */
    int current_move_pointer;

    QTimer *replay_timer;
    int replay_delay;
    int replay_stop_index;

    /** create a new document template if none is existing yet */
    void initXml(void);
    void appendMoveToXml(unsigned int, unsigned int, unsigned int, unsigned int);
    /** returns true if the move was successful */
    bool moveFromXml(move_xml_dir_t);
    void syncXml(void);
    bool loadFromIcpSyntax(QString);
    void prepareNewSocket(QHostAddress, int);
    void prepareNewTimer(void);
    Player *getPlayerFromCoord(unsigned int, unsigned int);

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
      STATE_WAIT_FOR_CONNECTION,
      STATE_INVITE_DISPATCH,  /**< question */
      STATE_INVITE_RECEIVE,  /**< answer */
      STATE_GAME_DISPATCH,
      STATE_GAME_RECEIVE,
      STATE_MOVE_DISPATCH,
      STATE_MOVE_RECEIVE,
      STATE_EXIT_DISPATCH,  /**< FIXME redundant??? */
      STATE_EXIT_RECEIVE,
      STATE_MOVE_WHITE,
      STATE_MOVE_BLACK,
      //FIXME
      STATE_REPLAY_STEP,  /**< accepting user steps */
      STATE_REPLAY_TIMED,
      //STATE_REPLAY_PAUSE,
      STATE_REPLAY_STOP,
      STATE_END,  /**< disconnected, exit, error, etc. */
    } state_t;

    const char[] XML_STR_DRAUGHTS = "draughts";
    const char[] XML_STR_GAME     = "game";
    const char[] XML_STR_TYPE     = "type";
    const char[] XML_STR_LOCAL    = "local";
    const char[] XML_STR_NETWORK  = "network";
    const char[] XML_STR_HOST     = "host";
    const char[] XML_STR_PORT     = "port";
    const char[] XML_STR_PLAYERS  = "players";
    const char[] XML_STR_WHITE    = "white";
    const char[] XML_STR_BLACK    = "black";

    const int DEFAULT_TIMEOUT = 300;  /**< miliseconds */

    /**
     * the top left corner is always white
     * convention: board(ROWS, QVector<Player>(COLUMNS))
     */
    QVector< QVector<men_t> > board(8, QVector<men_t>(8, MEN_NONE));

    Game(void);
    ~Game(void);
    /** @return true if OK */
    bool gameRemote(QHostAddress, int, bool);  /**< locally initiated */
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

    int move(unsigned int, unsigned int, unsigned int, unsigned int, bool);
    void showPossibleMoves(unsigned int, unsigned int, bool);
    void hidePossibleMoves(bool);
    void adviceMove(void);

    void setReplayTimeout(int);
    bool replayMoveForward(int);
    bool replayMoveBackward(int);
    bool replayMoveToggle(void);  /**< toggle pause/play */
    bool replayMoveStop(void);
    /** used e.g. for user input locking */
    bool isReplaying(void);

    Player *getPlayerWhite(void);
    Player *getPlayerBlack(void);

    //FIXME update variables inside (player names etc.)
    QString getXmlStr(void);
    QString getIcpSyntaxStr(bool);
    QString getError(void);
    QHostAddress getRemoteAddr(void);
    int getRemotePort(void);
    QString getFilePath(void);

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
