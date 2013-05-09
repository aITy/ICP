/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#include "lightrock.h"
#include "ui_board.h"

#include <QDebug>

/**
 * light men object
 * similar to dark men with same methods
 */

LightRock::LightRock() {

    setFlag(ItemIsMovable);
    king = false;
}

LightRock::~LightRock() {

}

/** reimplemented bounding rect fuction
  * @return bouding rectangular that is occupied by rock
  */
QRectF LightRock::boundingRect() const {
    return QRectF(pos.x(), pos.y(), 50, 50);
}

void LightRock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    QRectF rect = boundingRect();
    QRectF src(0.0, 0.0, 50.0, 50.0);
    QImage img;
    if (!king)
        img = QImage(":/light_piece.png");
    else
        img = QImage(":/light_king.png");

    painter->drawImage(rect, img, src);
}

QPointF LightRock::getPosition() {
    return pos;
}

void LightRock::setPosition(QPointF p) {
    pos = p;
}

void LightRock::becomeKing() {
    king = true;
    this->update();
}

bool LightRock::isKing() {
    return king;
}
