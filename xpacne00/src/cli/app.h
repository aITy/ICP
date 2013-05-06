/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __APP_H__
#define __APP_H__

#include <QtCore>
#include <QtNetwork>
#include "../share/game.h"

/** FIXME no idea if this works on Windows without ANSI.SYS file */
#define _TERM_C_START "\x01B["  /**< `escape' and `left bracket' characters */
#define _TERM_C_END   "m"
#define _TERM_CLR     "2J"

#define _TERM_C_RESET     "0"
#define _TERM_C_BRIGHT    "1"
#define _TERM_C_DIM       "2"
#define _TERM_C_UNDERLINE "3"
#define _TERM_C_BLINK     "4"
#define _TERM_C_REVERSE   "7"
#define _TERM_C_HIDDEN    "8"

#define _TERM_C_BLACK   "0"
#define _TERM_C_RED     "1"
#define _TERM_C_GREEN   "2"
#define _TERM_C_YELLOW  "3"
#define _TERM_C_BLUE    "4"
#define _TERM_C_MAGENTA "5"
#define _TERM_C_CYAN    "6"
#define _TERM_C_WHITE   "7"

// yellow_bg 1;33;43 white_bg 0;30;47
#define TERM_C_RESET       _TERM_C_START _TERM_C_RESET  _TERM_C_END
#define TERM_BG_BLACK      _TERM_C_START _TERM_C_RESET ";4" _TERM_C_BLACK _TERM_C_END
#define TERM_BG_WHITE      _TERM_C_START _TERM_C_RESET ";4" _TERM_C_WHITE _TERM_C_END
#define TERM_BG_TIP        _TERM_C_START _TERM_C_RESET ";4" _TERM_C_GREEN _TERM_C_END
#define TERM_FG_MEN_BLACK  _TERM_C_START _TERM_C_DIM   ";3" _TERM_C_BLUE  _TERM_C_END
#define TERM_FG_MEN_WHITE  _TERM_C_START _TERM_C_DIM   ";3" _TERM_C_RED   _TERM_C_END
#define TERM_FG_KING_BLACK _TERM_C_START _TERM_C_DIM   ";3" _TERM_C_BLUE  _TERM_C_END
#define TERM_FG_KING_WHITE _TERM_C_START _TERM_C_DIM   ";3" _TERM_C_RED   _TERM_C_END

/**
 * main task which comprises the whole console program
 */
class App : public QObject {
  Q_OBJECT;

private:
  QTextStream qtout;
  QTextStream qterr;
  QTextStream qtin;

  QTcpServer *server;
  QSocketNotifier *notifier;
  bool new_conn_handled;  /**< user must handle it by hand */
  QString line;
  QStringList cmd_l;
  Game *g;

  void prepareNewGame(void);

public:
  App(QCoreApplication *);
  ~App();

public Q_SLOTS:
  void refresh(void);
  void handleInput(void);
  void gotConnection(void);
  void schedule_refresh(void);

Q_SIGNALS:
  // emit blocks unless queued connections are used
  void finished(void);
};

#endif
