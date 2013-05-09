/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */


#ifndef DARKROCK_H
#define DARKROCK_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsItem>

class DarkRock : public QGraphicsItem
{
public:
    DarkRock();
    ~DarkRock();
    QPointF getPosition();
    void setPosition(QPointF p);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget);
    void becomeKing();
    bool isKing();
private:
    QPointF pos;
    bool king;
};

#endif // DARKROCK_H
