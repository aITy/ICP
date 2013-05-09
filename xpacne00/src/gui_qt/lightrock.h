/**
 * Authors: Jiri Navratil (xnavra36)
 *          Jan Pacner (xpacne00)
 */

#ifndef LIGHTROCK_H
#define LIGHTROCK_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsItem>

class LightRock : public QGraphicsItem
{
public:
    LightRock();
    ~LightRock();
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

#endif // LIGHTROCK_H
