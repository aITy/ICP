/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

Coord::Coord(int _x, int _y) : x(_x), y(_y) {}

Player::Player(QString &_name) : name(_name) {}

Game::Game(QString &filename) : x(8), y(8) id(id_last++) {
  //QVector< QVector<Box> > board(ROWS, QVector<Box>(COLUMNS));
  //for (int i = 0; i < board.size(); ++i) {
  //  for(int j = 0;j < board[i].size();++j) {
  //  }
  //}

  // assign 16bit ID to this game and increment the global counter
};

err_t Game::move(Player &p, Coord &src, Coord &dst) {
  /** check
   *   the presence of some draughtsmen
   *   the ownership of the draughtsmen (must conform to the player)
   *   the distance in conjunction with jumping over
   *   the placement (can be only black square)
   */
  //FIXME CheckCoord;
  return E_BAD_COORD;

  //FIXME XML tree update
  // http://www.qtcentre.org/threads/12313-remove-node-in-xml-file

  board[dst.x][dst.y] = board[src.x][src.y];

  //FIXME do update of the output/GUI, but not here,
  //  because this is internal function only to change
  //  internal states and check for inconsistencies
  //update();

  if (DroughtsmenCount(p) == 0) {
    return E_PLAYER_FAILED;
  }

  return E_OK;
}
