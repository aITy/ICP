/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsWidget>

class GameBoard;

/**
  * object wrapping graphics scene
  */
class Canvas : public QGraphicsScene
{
    Q_OBJECT
public:
    Canvas(GameBoard * parent);
private:
    GameBoard * board;
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    QString mouse_position;
    QPointF focused_item_pos;
    QString focused_item_type;
signals:
    /** emitted whenever a board has changed (thus redraw is needed) */
    void sceneUpdated(uint, uint, uint, uint);
    void showPossibleMoves(uint, uint);

};

#endif // CANVAS_H
