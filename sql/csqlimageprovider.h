#ifndef CSQLIMAGEPROVIDER_H
#define CSQLIMAGEPROVIDER_H
#include <QPointer>
#include "cpplib_global.h"
#include <QNetworkAccessManager>
#include <QQuickImageProvider>


class CPPLIB_EXPORT CSqlImageProvider : public QQuickImageProvider{

public:
       CSqlImageProvider(const QString &basePath, ImageType type, Flags flags = 0);
       QImage requestImage(const QString& image, QSize* size, const QSize& requestedSize);     

private:
      QNetworkAccessManager manager;
      QString basePath;
};
#endif
