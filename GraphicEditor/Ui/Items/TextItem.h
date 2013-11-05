#pragma once

#include "./BaseItem.h"

#include <QFont>

class TextItem
    : public BaseItem
{
public:
    TextItem(const QString& text, QGraphicsLayoutItem* parent = nullptr);
    ~TextItem();

    void setFont(const QFont& font);

    QSizeF measureSize() const override;
    void draw(QPainter *painter, const QRectF& bounds) override;

private:
    QFont _font;
};
