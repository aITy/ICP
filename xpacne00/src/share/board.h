/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __BOARD_H__
#define __BOARD_H__

//FIXME
//#include <QtCore>
//#include "../share/player.h"

class IcpSyntaxParser {
  public:
    typedef QPair<unsigned int, unsigned int> pair_uint_t;
    typedef QPair<QString, QString> pair_str_t;
    pair_uint_t strCoordToUint(QString);
    pair_str_t intToStrCoord(unsigned int, unsigned int);
};

#define NET_TOK_INVITE        "INVITE "
#define NET_TOK_INVITE_ACCEPT "INVITE_ACCEPT "
#define NET_TOK_INVITE_REJECT "INVITE_REJECT "
#define NET_TOK_GAME          "GAME "
//#define NET_TOK_WHITE         "WHITE "
//#define NET_TOK_BLACK         "BLACK "
#define NET_TOK_NEW           "NEW "
#define NET_TOK_LOAD          "LOAD "
#define NET_TOK_GAME_ACCEPT   "GAME_ACCEPT"
#define NET_TOK_GAME_REJECT   "GAME_REJECT"
#define NET_TOK_MOVE          "MOVE "
#define NET_TOK_EXIT          "EXIT"

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

    CmdParser(QString);
    cmd_t getNextCmd();
    QString getRest();
};

class Player {
  private:
    bool is_black;
    int kicked;
  public:
    QString name;
    int getKickedCount();
};

/**
 * one game counting 2 players
 */
class Game : public QObject {
  Q_OBJECT

  private:
    /** the whole game XML tree including history */
    QDomDocument *doc;
    QTcpSocket *socket;
    Player *player_white;
    Player *player_black;
    QString filepath;
    /** queue for errors which can not be handled immediately */
    QList<err_t> err_queue;
    game_state_t game_state;
    //FIXME initialize
    /** used for checking of possible moves before a user move is processed */
    IcpSyntaxParser::pair_uint_t last_move_dst;
    IcpSyntaxParser::pair_uint_t possible_jump;
    bool possible_move_present;

    void initXml();
    void appendMoveToXml(unsigned int, unsigned int, unsigned int, unsigned int);

  public:
    /**
     * the top left corner is always white
     * convention: board(ROWS, QVector<Player>(COLUMNS))
     */
    QVector< QVector<men_t> > board(8, QVector<men_t>(8, MEN_NONE));

    typedef enum {
      ERR_OK = 0,
      ERR_INVALID_MOVE,
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
      REPLAY_TIMED,
      REPLAY_STEP,
    } replay_t;

    typedef enum {
      STATE_PRE_INIT,
      STATE_CAN_START,
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
      STATE_REPLAYING,
      STATE_END,  /**< disconnected, exit, error, etc. */
    } state_t;

    Game();
    ~Game();
    void gameLocal();
    void gameRemote(QHostAddress, int);
    void gameRemote(QTcpSocket *);
    void gameFromFile(QString);
    //FIXME some flag, that the game can not be user-modified while replaying
    void gameFromFile(QString, replay_t);
    //FIXME adjust to state_t!!!
    bool isRunning();
    bool isLocal();

    int move(unsigned int, unsigned int, unsigned int, unsigned int);
    void showPossibleMoves(unsigned int, unsigned int);
    void hidePossibleMoves();
    void adviceMove();

    Player *getPlayerWhite();
    Player *getPlayerBlack();

    //FIXME update variables inside (player names etc.)
    QString getXmlStr();
    QString getError();
    QHostAddress getRemoteAddr();
    int getRemotePort();

  public Q_SLOTS:
    // FIXME GUI must react on the following => reimplement them
    //QMessageBox::critical(this, tr("Network error"), socket->errorString());
    void gotConnected();
    void gotDisconnected();
    void readReply();
};

#endif
