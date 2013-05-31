/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QPainter>
#include <QLayoutItem>
#include <QPaintEvent>
#include "ui_board.h"
#include "../share/game.h"

class DarkRock;
class LightRock;
class Canvas;
class MainWindow;

class GameBoard : public QWidget, Ui::Form_Board
{
    Q_OBJECT
public:

    GameBoard(Game *parent = 0);
    ~GameBoard();
    /** function that return the scene */
    Canvas * getScene(){ return canvas; }

    /** functions that initialize or return the game */
    void initGame();
    Game * getGame() { return game; }

    // TO DO DELETE OR NOT

    void storeMove(uint, uint, uint, uint);
    bool compareWithLastState();

    /** player´s color and player´s alias */
    Player::color_t player_color;
    QString player_alias;

    /** functions, that convert coordinates from scene to board coordinates */
    QList<QRectF> convertCoords(QList<QPair<int, int> > );
    QList<QPair<uint, uint> > convertCoords(QList<QPointF> );
    QPointF convertCoords(uint, uint );
    QList<QPointF> convertCoords(QList<QPair<uint, uint> > );
    QList<QPointF> convertCoords(QPair<uint, uint>, QPair<uint, uint>);

    /** functions that ligthen up or darken the background of one rectengular area */
    void lightenPossibleMoves(QList<QPair<int, int> >);
    void hidePossibleMoves();

    /** functions that remove rocks by given scene position, order in light/rock list or delete all rocks */
    void removeRock(QPointF );
    void removeBlackRock(int);
    void removeLightRock(int);
    void clearBoard();

    /** function that retuns instance of canvas */
    Canvas * getCanvas() { return canvas;  }

    /** variable holding all moves showed in notification area */
    QString lineedit_moves;







	void checkForEmptyBoard();
	





private:
    /** rock */
    QList <DarkRock *> dark_rocks;
    QList <LightRock *> light_rocks;
    /** area that hold currently showed possible moves */
    QList <QPair<QString, int> > possible_moves;

    /** pointers to scene and current game "wrapping" this gameboard */
    Canvas * canvas;
    Game * game;
    /** fucntion that compares current and stored board state */
    QVector< QVector<Game::men_t> > board;
    QVector< QVector<Game::men_t> > stored_board_state;

    /** stored source and destination coordinates */
    QPair<uint,uint> stored_src;
    QPair<uint,uint> stored_dest;
public slots:
    /** refresh slot that refresh whole scene */
    void refresh();

    void showMoves(uint, uint);
    /** slot that is received by the scene when the moves is performed by the user */
    void sceneUpdate(uint srcx, uint srcy, uint destx, uint desty);
    /** timeout expired */
    void toutExpired();
};

#endif // GAMEBOARD_H
