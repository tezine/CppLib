#include <QtCore>
#include <QtQuick>
#include "cqml.h"
#include "blogger.h"

QPointer<CQML> CQML::o=0;
CQML::CQML(QObject *parent) :QObject(parent){

}

/*
 *Executes the javascript function 'functionBaseName'
 *If the function could not be executed due an error inside the function, we can check the error and trown an exception
 */
qint32 CQML::runFunction( QObject *obj, const QString &functionBaseName, const QVariantList &vList){
	QString completeFunctionName=QString(obj->objectName())+"." +functionBaseName;
    if(vList.size()>10){BLogger::error(0,__FUNCTION__, "Unable to invoke qml function:"+completeFunctionName+".Maximum allowed arguments:10.Passed:"+QString::number(vList.size()));return -1;}
	QByteArray method=QMetaObject::normalizedSignature(functionBaseName.toUtf8().constData());
	QGenericArgument arg[10];
	for(int n = 0; n < vList.count(); ++n) {
		QString barbosa("QVariant");
		QByteArray text = barbosa.toLocal8Bit();
		char *data = new char[text.size() + 1];//todo apagar esses dados depois
		strcpy(data, text.data());//nao pode ser memcpy
		arg[n] = QGenericArgument(data,vList[n].constData());
	}
	QVariant returnedValue;
    if(!QMetaObject::invokeMethod(obj, method.data(), Qt::AutoConnection, Q_RETURN_ARG(QVariant, returnedValue),  arg[0], arg[1], arg[2], arg[3], arg[4],arg[5], arg[6], arg[7], arg[8], arg[9])) {
        BLogger::error(0,__FUNCTION__, "Unable to invoke qml function:"+completeFunctionName);
        return -1;
    }
	qApp->processEvents();
    return 0;
}

qint32 CQML::runFunctionWithoutArgs(QObject *obj, const QString &functionBaseName){
    QString completeFunctionName=QString(obj->objectName())+"." +functionBaseName;
    QByteArray method=QMetaObject::normalizedSignature(functionBaseName.toUtf8().constData());
    qDebug()<<"(CQML)runFunctionWithoutArgs:"<<QString(obj->metaObject()->className())+"."+ functionBaseName;
    if(!QMetaObject::invokeMethod(obj, method.data(),Qt::AutoConnection)) {
        BLogger::error(0,__FUNCTION__, "Unable to invoke qml function:"+completeFunctionName);
        return -1;
    }
    qApp->processEvents();
    return 0;
}
