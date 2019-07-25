#include <QtCore>
#include "csqlimageprovider.h"
#include "dstore.h"
#include <QQuickImageResponse>
#include <QNetworkReply>


CSqlImageProvider::CSqlImageProvider(const QString &basePath, QQmlImageProviderBase::ImageType type, QQmlImageProviderBase::Flags flags): QQuickImageProvider(type,flags){
    this->basePath=basePath;
}

QImage CSqlImageProvider::requestImage(const QString &image, QSize *size, const QSize &requestedSize){    
    QString completePath;
    completePath=basePath+image;
    qDebug()<<"(CSqlImageProvider)requestImage:"<<completePath<<"============================";
    QUrl url= QUrl(completePath);
    QNetworkRequest request(url);
    QNetworkReply *reply= manager.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),&loop,SLOT(quit()));
    loop.exec();
    QByteArray im = QByteArray::fromBase64(reply->readAll());
    if(im.isEmpty())return QImage();
    QImage img=QImage::fromData(im);
    qDebug()<<"(CSqlImageProvider)requestImage. Recebeu imagem. Tamanho:"<<im.size()<<".IsNull:"<<img.isNull();
    return img;
}
