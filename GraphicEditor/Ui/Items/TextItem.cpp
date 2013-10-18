#include "TextItem.h"

#include <QPainter>
#include <QFont>

////////////////////////////////////////////////////////////////
TextItem::TextItem(const QString& text)
    : BaseItem()
    , _displayText(text)
{
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
void TextItem::setDisplayText(const QString& text)
{
    if (text == _displayText)
        return;

    _displayText = text;

    // TODO: call requestRedraw/Layout
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
    const QSizeF& size = fm.size(Qt::TextExpandTabs, _displayText);
    // NOTE: flag Qt::TextSingleLine ignores newline characters.
    return size;
}

////////////////////////////////////////////////////////////////
void TextItem::draw(QPainter *painter, const QRectF& bounds)
{
    painter->setFont(_font);

    // TODO: mozno bude treba specialne handlovat novy riadok
    painter->drawText(bounds, _displayText);
}
