#pragma once

#include "./BaseItem.h"

#include <QFont>

class TextItem
    : public BaseItem
{
public:
    TextItem(const QString& text);
    ~TextItem();

    void setDisplayText(const QString& text);
    void setFont(const QFont& font);

    QSizeF measureSize() const override;
    void draw(QPainter *painter, const QRectF& bounds) override;

private:
    QString _displayText;
    QFont _font;
};
