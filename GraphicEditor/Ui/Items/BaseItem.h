#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsLayoutItem>

#include "../BaseElement.h"

class BaseItem:
    public BaseElement,
    public QGraphicsLayoutItem,
    public QGraphicsRectItem
{
public:
    explicit BaseItem(QGraphicsLayoutItem *parent = 0);
    ~BaseItem() override;

    void setText(const QString& text);

    virtual QSizeF measureSize() const = 0;
    virtual void draw(QPainter *painter, const QRectF& bounds) = 0;

    QRectF boundingRect() const override final;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

    // From QGraphicsLayoutItem
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override;
    void setGeometry(const QRectF &rect) override;
    /*
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const override;
    void updateGeometry() override;
    */

protected:
    QString _text;

private:
    friend class BaseGrid;
};
