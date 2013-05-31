/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */


#include "darkrock.h"
//#include "rock.h"
#include "ui_board.h"

#include <QDebug>

/**
 * the black men object
 */

DarkRock::DarkRock() {
    king = false;
}

DarkRock::~DarkRock() {

}
/** reimplemented bounding rect fuction
  * @return bouding rectangular that is occupied by rock
  */
QRectF DarkRock::boundingRect() const {
    return QRectF(pos.x(), pos.y(), 50, 50);
}

/**
 * reimplemented paint function to provide object repaint whenever the scene is updated
 */
void DarkRock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *){
    QRectF rect = boundingRect();
    QRectF src(0.0, 0.0, 50.0, 50.0);
    QImage img;
    if (!king)
        img = QImage(":/dark_piece.png");
    else
        img = QImage(":/dark_king.png");

    painter->drawImage(rect, img, src);
}

/**
 * function that return position of men
 * @return returns men´s position
 */

QPointF DarkRock::getPosition() {
    return pos;
}

/**
 * set the men´s position
 */

void DarkRock::setPosition(QPointF p) {
    pos = p;
}

/**
 * function that set king flag and the rock is repainted after the scene is updated
 */

void DarkRock::becomeKing() {
    king = true;
    this->update();
}

/**
 * function that return boolean reflecting whether the men is king or not
 * @return return if the rock is king or not
 */

bool DarkRock::isKing() {
    return king;
}
