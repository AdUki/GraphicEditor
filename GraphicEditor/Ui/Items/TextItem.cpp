#include "TextItem.h"

#include <QPainter>
#include <QFont>
#include <QDebug>

////////////////////////////////////////////////////////////////
TextItem::TextItem(const QString& text, QGraphicsLayoutItem *parent)
    : BaseItem(parent)
{
    _text = text;

    QFont font;
    font.setPointSize(11);
    font.setBold(false);

    setFont(font);
}

////////////////////////////////////////////////////////////////
TextItem::~TextItem()
{
}

////////////////////////////////////////////////////////////////
void TextItem::setFont(const QFont &font)
{
    _font = font;
    QFontMetrics fm(_font);
}

////////////////////////////////////////////////////////////////
QSizeF TextItem::measureSize() const
{
    QFontMetrics fm(_font);

    const QSizeF& size = fm.size(Qt::TextExpandTabs, _text);
    // NOTE: flag Qt::TextSingleLine ignores newline characters.
    return size;
}

////////////////////////////////////////////////////////////////
void TextItem::draw(QPainter *painter, const QRectF& bounds)
{
    painter->setFont(_font);

    // TODO: mozno bude treba specialne handlovat novy riadok
    painter->drawText(bounds, _text);
}
