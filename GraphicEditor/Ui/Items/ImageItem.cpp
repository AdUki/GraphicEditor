#include "ImageItem.h"

#include <QPainter>
#include <QImage>

////////////////////////////////////////////////////////////////
ImageItem::ImageItem(const QString& imageName)
    : BaseItem()
{
    setImage(imageName);
}

////////////////////////////////////////////////////////////////
ImageItem::~ImageItem()
{
    delete _image;
}

////////////////////////////////////////////////////////////////
void ImageItem::setImage(const QString& imageName)
{
    if (_imageName == imageName)
        return;

    if (_imageName != nullptr)
        delete _image;

    _image = new QImage(imageName);
}

////////////////////////////////////////////////////////////////
QSizeF ImageItem::measureSize() const
{
    return _image->size();
}

////////////////////////////////////////////////////////////////
void ImageItem::draw(QPainter *painter, const QRectF& bounds)
{
    painter->drawImage(bounds, *_image);
}
