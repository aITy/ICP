/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "canvas.h"
#include "darkrock.h"
#include "lightrock.h"
#include "gameboard.h"
#include "mainwindow.h"
#include <QDebug>

Canvas::Canvas(GameBoard *parent)
    :board(parent)
{
    focused_item_pos = QPointF(0.0, 0.0);
}

/**
 * function that handle mouse click on scene and emit signals
 * the first signal send coordinates of one move - source and destination
 * the sencond signal send coordinates of one item that is focused to show possible moves
 */
void Canvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qreal pos_x = event->scenePos().x();
    qreal pos_y = event->scenePos().y();
    QGraphicsItem * item = itemAt(pos_x, pos_y);
    /** if there is an item, store item´s coordinates and emit it */
    if (item != 0) {
        QList<QPair<uint, uint> >focused_item_coords;
        QList<QPointF> list;
        list.append(QPointF(pos_x, pos_y));
        focused_item_coords = board->convertCoords(list);

        // TODO
        focused_item_pos = QPointF(item->scenePos());
        emit(showPossibleMoves(focused_item_coords.at(0).first, focused_item_coords.at(0).second));
    }
    /** click is not the empty area
      * if some item is already focused - move is detected and signal is emited with coordinates of move
      */
    else {
        if (focused_item_pos != QPointF(0.0, 0.0)) {

            QList<QPointF> move_positions;
            QList<QPair<uint, uint> > move_coords;
            move_positions.append(focused_item_pos);
            move_positions.append(QPointF(pos_x, pos_y));
            move_coords = board->convertCoords(move_positions);
            board->storeMove(move_coords.at(0).first - 1, move_coords.at(0).second -1, move_coords.at(1).first - 1, move_coords.at(1).second - 1);
            //qDebug() << move_coords;
            focused_item_pos = QPointF(0.0, 0.0);
            emit(sceneUpdated(move_coords.at(0).first - 1, move_coords.at(0).second -1, move_coords.at(1).first - 1, move_coords.at(1).second - 1));
        }
    }
    this->update();
}
