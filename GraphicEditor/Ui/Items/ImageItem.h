#pragma once

#include "./BaseItem.h"

class QImage;

class ImageItem
        : public BaseItem
{
public:
    ImageItem(const QString &imageName);
    ~ImageItem();

    void setImage(const QString& imageName);

    QSizeF measureSize() const override;
    void draw(QPainter *painter, const QRectF& bounds) override;

private:
    QImage* _image;
    QString _imageName;
};
