#ifndef DSTORE_H
#define DSTORE_H
#include <QPointer>
#include <QJSValue>
#include "cpplib_global.h"

/**
 *@brief    The DStore saves/reads data used by the app
 *@group data
 *@level model
 */
class CPPLIB_EXPORT  DStore : public QObject{
    Q_OBJECT
    Q_PROPERTY(bool isProfileComplete READ getIsProfileComplete WRITE setIsProfileComplete USER true)
    Q_PROPERTY(int placeID READ getCurrentPlaceID WRITE setCurrentPlaceID USER true)
    Q_PROPERTY(int userID READ getCurrentUserID WRITE setCurrentUserID NOTIFY sigUserIDChanged USER true)
    Q_PROPERTY(qint64 revendaID READ getRevendaID WRITE setRevendaID USER true)
    Q_PROPERTY(qint64 caminhaoID READ getCaminhaoID WRITE setCaminhaoID USER true)
    Q_PROPERTY(qint64 driverID READ getDriverID WRITE setDriverID USER true)
    Q_PROPERTY(int languageCode READ getLanguageCode WRITE setLanguageCode USER true)
    Q_PROPERTY(QString userName READ getCurrentUserName WRITE setCurrentUserName USER true)
    Q_PROPERTY(QString userEmail READ getUserEmail WRITE setUserEmail USER true)
    Q_PROPERTY(QString userPassword READ getUserPassword WRITE setUserPassword USER true)
    Q_PROPERTY(qint64 otherUserID READ getOtherUserID WRITE setOtherUserID NOTIFY sigOtherUserIDChanged USER true)
    Q_PROPERTY(QString userNumber READ getCurrentUserNumber WRITE setCurrentUserNumber NOTIFY sigUserNumberChanged USER true)
    Q_PROPERTY(bool membroConselho READ getMembroConselho WRITE setMembroConselho NOTIFY sigMembroConselhoChanged)
    Q_PROPERTY(QString baseURL READ getBaseURL WRITE setBaseURL NOTIFY sigBaseURLChanged)
    Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY sigTokenChanged)
    Q_PROPERTY(int companyType READ getCompanyType WRITE setCompanyType NOTIFY companyTypeChanged)
    Q_PROPERTY(bool adicionaisHabilitados READ getAdicionaisHabilitados WRITE setAdicionaisHabilitados NOTIFY sigAdicionaisHabilitadosChanged)

public:
    explicit DStore();
    static DStore* obj(){if(!staticObj) staticObj=new DStore(); return staticObj; }
    int getCurrentPlaceID(){return currentPlaceID;}
    void setCurrentPlaceID(int placeID){currentPlaceID=placeID;}
    void setCurrentUserID(int userID){this->currentUserID=userID;}
    int getCurrentUserID(){return currentUserID;}
    void setCurrentUserNumber(const QString & userNumber){this->currentUserNumber=userNumber;}
    QString getCurrentUserNumber(){return currentUserNumber;}
    void setUserPassword(const QString & userPassword){this->userPassword=userPassword;}
    QString getUserPassword(){return userPassword;}
    bool getIsProfileComplete(){return isProfileComplete;}
    void setIsProfileComplete(bool b){isProfileComplete=b;}
    QString getCurrentUserName(){return currentUserName;}
    void setCurrentUserName(const QString &userName){currentUserName=userName;}
    QString getUserEmail(){return currentUserEmail;}
    int getLanguageCode(){return languageCode;}
    qint64 getDriverID(){return driverID;}
    void setDriverID(qint64 driverID){this->driverID=driverID;}
    void setLanguageCode(int languageCode){this->languageCode=languageCode;}
    bool getMembroConselho(){return membroConselho;}
    void setMembroConselho(bool b){membroConselho=b;}
    void setUserEmail(const QString &userEmail){this->currentUserEmail=userEmail;}
    Q_INVOKABLE void setOtherUserID(qint64 otherUserID){this->otherUserID=otherUserID;}
    Q_INVOKABLE qint64 getOtherUserID(){return otherUserID;}
    Q_INVOKABLE void saveValue(const QString &name, QJSValue jsValue);
    Q_INVOKABLE QJSValue getValue(const QString &key);
    QMetaObject getMeta();
    qint64 getRevendaID() const{return revendaID;}
    void setRevendaID(const qint64 &value){revendaID = value;}
    qint64 getCaminhaoID() const{return caminhaoID;}
    void setCaminhaoID(const qint64 &value){caminhaoID = value;}
    QString getBaseURL() const{return baseURL;}
    void setBaseURL(const QString &value){baseURL = value;}
    QString getToken() const{return m_token;}
    void setToken(const QString &token){this->m_token=token;}
    bool getAdicionaisHabilitados() const{return m_adicionaisHabilitados;}
    void setAdicionaisHabilitados(bool adicionaisHabilitados);    
    int getCompanyType() const{return m_companyType;}

public slots:
    void setCompanyType(int companyType){  m_companyType = companyType;}

signals:
    void sigOtherUserIDChanged(qint64 otherUserID);
    void sigUserIDChanged(qint64 userID);
    void sigUserNumberChanged(QString userNumber);
    void sigMembroConselhoChanged(bool b);
    void sigBaseURLChanged(const QString &value);
    void sigTokenChanged(QString token);
    void sigAdicionaisHabilitadosChanged(bool adicionaisHabilitados);

    void companyTypeChanged(int companyType);

private:
    static QPointer<DStore> staticObj;
    bool isProfileComplete;
    qint64 currentPlaceID;
    qint64 currentUserID;
    QString currentUserEmail;
    QString currentUserNumber;
    QString currentUserName;
    QString userPassword;
    qint64 otherUserID;
    qint64 revendaID;
    qint64 caminhaoID;
    qint64 driverID;
    bool membroConselho;
    int languageCode;
    QString baseURL;
    QHash<QString,QJSValue> jsValueHash;
    QString m_token;
    bool m_adicionaisHabilitados;
    int m_companyType;
};
#endif // DSTORE_H
