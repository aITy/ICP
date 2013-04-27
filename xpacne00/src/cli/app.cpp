/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "app.h"
#include "../share/board.h"

App::App(QCoreApplication &_parent) :
  QObject(_parent), par(_parent) {}

void App::run(void) {
  //QStringList sl = par->arguments();

  ////QList<QString>::iterator
  //for (QStringList::iterator i = sl.begin(); i != sl.end(); ++i) {
  //  //std::cout << i.toLocal8Bit().constData() << std::endl;
  //  std::cout << (*i).toLocal8Bit().constData() << std::endl;
  //}

  // FIXME pripoj se na nahodny lokalni port (a vypis ho uzivateli) a
  //   cekej na prichozi pozadavky (tzn. spust serverovou cast)
  //   TCP cekani na prijem/odeslani zpravy
  //   => bude reseno pres Qt a bude to asi nejake makro z hlavickoveho
  //     souboru implementujiciho .o modul sitove komunikace

  Player p1("player 1 (white)");
  Player p2("player 2 (black)");

  err_t ret = Game::move(p1, Coord(5, 5), Coord(6, 6))) {
  switch (ret) {
    default:
      std::cout << "ret: " << ret << std::endl;
  }

  // header
  //   [color_start] player 1 [color_end]
  //   [color_start] player 2 [color_end]
  // content
  //   <none>
  //   or
  //   <the whole board>
  // footer
  //   local port: <port>
  //   currently playing: (IP/hostname:port|localhost|none)
  //   for help type h
  //   >>>

  /**
  h               this help
  q               quit
  a               alias of player1
  aa              alias of player2
  n               new game
  nn              new network game (master)
  nnc <IP> <port> new network connect game (slave)
  <CR>            refresh
  */

  /*
  std::cout << TERM_BG_WHITE;

  std::cout << TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_KING_BLACK <<
               TERM_FG_KING_WHITE <<
               TERM_C_RESET       <<
               std::endl;

  std::cout << TERM_BG_BLACK;

  std::cout << TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_MEN_BLACK  <<
               TERM_FG_MEN_WHITE  <<
               TERM_FG_KING_BLACK <<
               TERM_FG_KING_WHITE <<
               TERM_C_RESET       <<
               std::endl;
               */

  Q_EMIT finished();
}
