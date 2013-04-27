/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef __BOARD_H__
#define __BOARD_H__

//FIXME
//#include "../share/player.h"

typedef enum {
  E_OK = 0,
  E_PLAYER_LOST,
  E_BAD_COORD,
} err_t;

typedef enum {
  PL_NONE,  // white boxes on the board
  PL_BLACK,
  PL_WHITE,
} player_side_t;

class Coord {
public:
  Coord(int, int);
  int x;
  int y;
}

class Player {
private:
  QString name;
public:
  Player(QString &);
}

// FIXME maybe QObject not needed
class Box : public QObject {
private:
  player_side_t side;
}

class Game {
private:
  /** the whole game XML tree with history */
  QDomDocument tree;
  /** first unused localhost ID */
  static uint16_t id_last
  /** a localhost unique ID */
  uint16_t id;
  /** a remote ID (can't be used directly
     mapping to local ID needed first) */
  uint16_t remote_id;

public:
  // FIXME size could be easily found out
  std::vector< std::vector<Box> > board;

  Game(void);
  int move(Player&, const Coord& src, const Coord& dst);
  unsigned int DroughtsmenCount(const Player&);
  int CheckCoord(const Coord&, const Coord&);
  //FIXME place it somewhere else?
  QString getWholeXML();
  QString getDiffXML();
}

#endif
