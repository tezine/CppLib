#ifndef CTEZINEBASEPROPERTIES_H
#define CTEZINEBASEPROPERTIES_H
#include <QObject>
#include <QUrl>

/*
 *Classe base de CTezine e CTezineBase
 * Ficam só propriedades
 */
class CTezineBaseProperties: public QObject{
    Q_OBJECT
    Q_PROPERTY(qint32 screenHeight READ getScreenHeight CONSTANT) //we have to write CONSTANT to get rid of "depends on non-NOTIFYable properties"
    Q_PROPERTY(qint32 screenWidth READ getScreenWidth CONSTANT)
    Q_PROPERTY(qint32 height READ getScreenHeight CONSTANT)
    Q_PROPERTY(qint32 width READ getScreenWidth CONSTANT)
    Q_PROPERTY(qint32 platform READ getPlatform CONSTANT)
    Q_PROPERTY(QString storagePath READ getStoragePath CONSTANT)
    Q_PROPERTY(QString sectionSeparatorColor READ getSectionSeparatorColor CONSTANT)
    Q_PROPERTY(qreal layoutSpacing READ getLayoutSpacing CONSTANT)
    Q_PROPERTY(QString tabUnselectedColor READ getTabBackColor CONSTANT)
    Q_PROPERTY(QString tabSelectedColor READ getTabSelected CONSTANT)
    Q_PROPERTY(QString toolbarColor READ getToolbarBackColor CONSTANT)
    Q_PROPERTY(QString buttonNormalColor READ getButtonNormalColor CONSTANT)
    Q_PROPERTY(QString buttonFocusedColor READ getButtonFocusedColor CONSTANT)
    Q_PROPERTY(QString buttonPressedColor READ getButtonPressedColor CONSTANT)
    Q_PROPERTY(QString buttonFocusedDisabledColor READ getButtonFocusedDisabledColor CONSTANT)
    Q_PROPERTY(QString buttonDisabledColor READ getButtonDisabledColor CONSTANT)
    Q_PROPERTY(QString switchBackColor READ getSwitchBackColor CONSTANT)
    Q_PROPERTY(QString switchButtonOnColor READ getSwitchButtonOnColor CONSTANT)
    Q_PROPERTY(QString switchButtonOffColor READ getSwitchButtonOffColor CONSTANT)
    Q_PROPERTY(QString sectionNormalColor READ getSectionNormalColor CONSTANT)
    Q_PROPERTY(QString sectionPressedColor READ getSectionPressedColor CONSTANT)
    Q_PROPERTY(QString sectionActivatedColor READ getSectionActivatedColor CONSTANT)
    Q_PROPERTY(QString sectionFocusedColor READ getSectionFocusedColor CONSTANT)
    Q_PROPERTY(QString sectionDisabledFocusedColor READ getSectionDisabledFocusedColor CONSTANT)
    Q_PROPERTY(QString sectionDisabledColor READ getSectionDisabledColor CONSTANT)
    Q_PROPERTY(QString backColor READ getBackColor CONSTANT)
    Q_PROPERTY(QString fontColor READ getFontColor CONSTANT)
    Q_PROPERTY(QString secondLineFontColor READ getSecondLineFontColor CONSTANT)
    Q_PROPERTY(QString tabFontColor READ getTabFontColor CONSTANT)
    Q_PROPERTY(QString buttonFontColor READ getButtonFontColor CONSTANT)
    Q_PROPERTY(QString dlgTitleBarColor READ getDlgTitleBarColor CONSTANT)
    Q_PROPERTY(QString textFieldBackColor READ getTextFieldBackColor CONSTANT)
    Q_PROPERTY(QString menuItemBackColor READ getMenuItemBackColor CONSTANT)
    Q_PROPERTY(QString comboNormalColor READ getComboNormalColor CONSTANT)
    Q_PROPERTY(QString comboFocusedColor READ getComboFocusedColor CONSTANT)
    Q_PROPERTY(QString comboPressedColor READ getComboPressedColor CONSTANT)
    Q_PROPERTY(QString comboDisabledFocusedColor READ getComboDisabledFocusedColor CONSTANT)
    Q_PROPERTY(QString comboDisabledColor READ getComboDisabledColor CONSTANT)
    Q_PROPERTY(QString progressBackColor READ getProgressBackColor CONSTANT)
    Q_PROPERTY(QString progressForeColor READ getProgressForeColor CONSTANT)
    Q_PROPERTY(QString scrollBarIcon READ getScrollBarIcon CONSTANT)
    Q_PROPERTY(qint32 listCount READ getListCount CONSTANT)
    Q_PROPERTY(qreal scrollWidth READ getScrollWidth CONSTANT)
    Q_PROPERTY(QString sectionRectColor READ getSectionRectColor CONSTANT)
    //font pointsize below
    Q_PROPERTY(qreal tinyFontPointSize READ getTinyFontPointSize CONSTANT)
    Q_PROPERTY(qreal smallFontPointSize READ getSmallFontPointSize CONSTANT)
    Q_PROPERTY(qreal mediumFontPointSize READ getMediumFontPointSize CONSTANT)
    Q_PROPERTY(qreal largeFontPointSize READ getLargeFontPointSize CONSTANT)
    Q_PROPERTY(qreal hugeFontPointSize READ getHugeFontPointSize CONSTANT)
    Q_PROPERTY(qreal imenseFontPointSize READ getImenseFontPointSize CONSTANT)
    Q_PROPERTY(qreal extraImenseFontPointSize READ getExtraImenseFontPointSize CONSTANT)
    Q_PROPERTY(qreal pointSize1 READ getPointSize1 CONSTANT)
    Q_PROPERTY(qreal pointSize2 READ getPointSize2 CONSTANT)
    Q_PROPERTY(qreal pointSize3 READ getPointSize3 CONSTANT)
    Q_PROPERTY(qreal pointSize4 READ getPointSize4 CONSTANT)
    Q_PROPERTY(qreal pointSize5 READ getPointSize5 CONSTANT)
    Q_PROPERTY(qreal pointSize6 READ getPointSize6 CONSTANT)
    Q_PROPERTY(qreal pointSize7 READ getPointSize7 CONSTANT)
    Q_PROPERTY(qreal pointSize8 READ getPointSize8 CONSTANT)
    Q_PROPERTY(qreal pointSize9 READ getPointSize9 CONSTANT)
    Q_PROPERTY(qreal pointSize10 READ getPointSize10 CONSTANT)
    Q_PROPERTY(qreal pointSize11 READ getPointSize11 CONSTANT)
    Q_PROPERTY(qreal pointSize12 READ getPointSize12 CONSTANT)
    Q_PROPERTY(qreal pointSize13 READ getPointSize13 CONSTANT)
    Q_PROPERTY(qreal pointSize14 READ getPointSize14 CONSTANT)
    //margins below
    Q_PROPERTY(qreal tinyMargin READ getTinyMargin CONSTANT)
    Q_PROPERTY(qreal smallMargin READ getSmallMargin CONSTANT)
    Q_PROPERTY(qreal mediumMargin READ getMediumMargin CONSTANT)
    Q_PROPERTY(qreal largeMargin READ getLargeMargin CONSTANT)
    Q_PROPERTY(qreal hugeMargin READ getHugeMargin CONSTANT)
    Q_PROPERTY(qreal imenseMargin READ getImenseMargin CONSTANT)
    Q_PROPERTY(qreal hm1 READ getHorizontalMargin1 CONSTANT)
    Q_PROPERTY(qreal hm2 READ getHorizontalMargin2 CONSTANT)
    Q_PROPERTY(qreal hm3 READ getHorizontalMargin3 CONSTANT)
    Q_PROPERTY(qreal hm4 READ getHorizontalMargin4 CONSTANT)
    Q_PROPERTY(qreal hm5 READ getHorizontalMargin5 CONSTANT)
    Q_PROPERTY(qreal vm1 READ getVerticalMargin1 CONSTANT)
    Q_PROPERTY(qreal vm2 READ getVerticalMargin2 CONSTANT)
    Q_PROPERTY(qreal vm3 READ getVerticalMargin3 CONSTANT)
    Q_PROPERTY(qreal vm4 READ getVerticalMargin4 CONSTANT)
    Q_PROPERTY(qreal vm5 READ getVerticalMargin5 CONSTANT)
    //others below
    Q_PROPERTY(QString baseURL READ getBaseURL WRITE setBaseURL NOTIFY sigBaseURLChanged)
    Q_PROPERTY(QUrl qmlBaseURL READ getQmlBaseURL WRITE setQmlBaseUrl)
    Q_PROPERTY(QString token READ getToken WRITE setToken NOTIFY sigTokenChanged)
    Q_PROPERTY(bool hotReloadEnabled READ getHotReload WRITE setHotReload NOTIFY sigHotReloadChanged)
    Q_PROPERTY(QString appVersion READ getAppVersion WRITE setAppVersion CONSTANT)

public:
    QString getToken() const{return m_token;}
    void setToken(const QString &token){this->m_token=token;}
    bool getHotReload() const{return hotReload;}
    void setHotReload(bool hotReload){this->hotReload=hotReload; emit sigHotReloadChanged(this->hotReload);}
     void setScreenSize(qint32 width, qint32 height){screenWidth=width; screenHeight=height;}
     qint32 getScreenHeight(){return screenHeight; }
     qint32 getScreenWidth(){return screenWidth; }
     Q_INVOKABLE void setScreenHeight(qint32 height);//precisa do invokable nesses dois
     Q_INVOKABLE void setScreenWidth(qint32 width);
     qint32 getPlatform();
     QString getSectionSeparatorColor(){return sectionSeparatorColor; }
     qreal getLayoutSpacing(){return layoutSpacing; }
     //fonts below
     qreal getPointSize1(){return pointSize1;}
     qreal getPointSize2(){return pointSize2;}
     qreal getPointSize3(){return pointSize3;}
     qreal getPointSize4(){return pointSize4;}
     qreal getPointSize5(){return pointSize5;}
     qreal getPointSize6(){return pointSize6;}
     qreal getPointSize7(){return pointSize7;}
     qreal getPointSize8(){return pointSize8;}
     qreal getPointSize9(){return pointSize9;}
     qreal getPointSize10(){return pointSize10;}
     qreal getPointSize11(){return pointSize11;}
     qreal getPointSize12(){return pointSize12;}
     qreal getPointSize13(){return pointSize13;}
     qreal getPointSize14(){return pointSize14;}
     qreal getTinyFontPointSize(){return tinyFontPointSize; }
     qreal getSmallFontPointSize(){return smallFontPointSize; }
     qreal getMediumFontPointSize(){return mediumFontPointSize; }
     qreal getLargeFontPointSize(){return largeFontPointSize; }
     qreal getHugeFontPointSize(){return hugeFontPointSize; }
     qreal getImenseFontPointSize(){return imenseFontPointSize; }
     qreal getExtraImenseFontPointSize(){return extraImenseFontPointSize; }
     //margins below
     qreal getTinyMargin(){return tinyMargin; }
     qreal getSmallMargin(){return smallMargin; }
     qreal getMediumMargin(){return mediumMargin; }
     qreal getLargeMargin(){return largeMargin; }
     qreal getHugeMargin(){return hugeMargin; }
     qreal getImenseMargin(){return imenseMargin; }
     qreal getHorizontalMargin1(){return horizontalMargin1;}
     qreal getHorizontalMargin2(){return horizontalMargin2;}
     qreal getHorizontalMargin3(){return horizontalMargin3;}
     qreal getHorizontalMargin4(){return horizontalMargin4;}
     qreal getHorizontalMargin5(){return horizontalMargin5;}
     qreal getVerticalMargin1(){return verticalMargin1;}
     qreal getVerticalMargin2(){return verticalMargin2;}
     qreal getVerticalMargin3(){return verticalMargin3;}
     qreal getVerticalMargin4(){return verticalMargin4;}
     qreal getVerticalMargin5(){return verticalMargin5;}

     QString getTabBackColor(){return tabUnselectedColor; }
     QString getTabSelected(){return tabSelectedColor; }
     QString getToolbarBackColor(){return toolbarColor; }
     QString getButtonNormalColor(){return buttonNormalColor; }
     QString getButtonFocusedColor(){return buttonFocusedColor; }
     QString getButtonPressedColor(){return buttonPressedColor; }
     QString getButtonFocusedDisabledColor(){return buttonFocusedDisabledColor; }
     QString getButtonDisabledColor(){return buttonDisabledColor; }
     QString getSwitchBackColor(){return switchBackColor; }
     QString getSwitchButtonOnColor(){return switchButtonOnColor; }
     QString getSwitchButtonOffColor(){return switchButtonOffColor; }
     QString getSectionNormalColor(){return sectionNormalColor; }
     QString getSectionPressedColor(){return sectionPressedColor; }
     QString getSectionActivatedColor(){return sectionActivatedColor; }
     QString getSectionFocusedColor(){return sectionFocusedColor; }
     QString getSectionDisabledFocusedColor(){return sectionDisabledFocusedColor; }
     QString getSectionDisabledColor(){return sectionDisabledColor; }
     QString getBackColor(){return backColor; }
     QString getFontColor(){return fontColor; }
     QString getSecondLineFontColor(){return secondLineFontColor; }
     QString getTabFontColor(){return tabFontColor; }
     QString getButtonFontColor(){return buttonFontColor; }
     QString getDlgTitleBarColor(){return dlgTitleBarColor; }
     QString getTextFieldBackColor(){return textFieldBackColor; }
     QString getMenuItemBackColor(){return menuItemBackColor; }
     QString getComboNormalColor(){return comboNormalColor; }
     QString getComboFocusedColor(){return comboFocusedColor; }
     QString getComboPressedColor(){return comboPressedColor; }
     QString getComboDisabledFocusedColor(){return comboDisabledFocusedColor; }
     QString getComboDisabledColor(){return comboDisabledColor; }
     QString getProgressBackColor(){return progressBackColor; }
     QString getProgressForeColor(){return progressForeColor; }
     QString getScrollBarIcon(){return scrollBarIcon; }
     qreal getScrollWidth(){return scrollWidth;}
     qint32 getListCount(){return 50;}
     QString getSectionRectColor(){return "#EAEAEA";}
     QString getStoragePath();
    CTezineBaseProperties(QObject *parent);

    QString getBaseURL() const{return baseURL;}
    void setBaseURL(const QString &value){baseURL = value;}
    QUrl getQmlBaseURL(){return this->qmlBaseUrl;}
    void setQmlBaseUrl(const QUrl &url){this->qmlBaseUrl=url;}    
    QString getAppVersion(){return this->appVersion;}
    void setAppVersion(const QString &version){this->appVersion = version;}

signals:
    void sigBaseURLChanged(const QString &value);
    void sigTokenChanged(QString token);
    void sigHotReloadChanged(bool hotReload);

protected:
    QString baseURL;
    QString m_token;
    QString appVersion;
    bool previewMode;
    bool hotReload;
    qint32 screenWidth;
    qint32 screenHeight;
    //font point size below
    qreal tinyFontPointSize;
    qreal smallFontPointSize;
    qreal mediumFontPointSize;
    qreal largeFontPointSize;
    qreal hugeFontPointSize;
    qreal imenseFontPointSize;
    qreal pointSize1, pointSize2, pointSize3, pointSize4, pointSize5, pointSize6, pointSize7, pointSize8, pointSize9,pointSize10, pointSize11, pointSize12, pointSize13, pointSize14;
    //margins below
    qreal horizontalMargin1, horizontalMargin2, horizontalMargin3, horizontalMargin4, horizontalMargin5;
    qreal verticalMargin1, verticalMargin2, verticalMargin3, verticalMargin4, verticalMargin5;
    qreal tinyMargin;
    qreal smallMargin;
    qreal mediumMargin;
    qreal largeMargin;
    qreal hugeMargin;
    qreal imenseMargin;
    qreal extraImenseFontPointSize;
    qreal layoutSpacing;
    QString tabUnselectedColor;
    QString tabSelectedColor;
    QString toolbarColor;
    QString buttonNormalColor;
    QString buttonFocusedColor;
    QString buttonPressedColor;
    QString buttonFocusedDisabledColor;
    QString buttonDisabledColor;
    QString switchBackColor;
    QString switchButtonOnColor;
    QString switchButtonOffColor;
    QString sectionNormalColor;
    QString sectionPressedColor;
    QString sectionActivatedColor;
    QString sectionFocusedColor;
    QString sectionDisabledFocusedColor;
    QString sectionDisabledColor;
    QString backColor;
    QString fontColor;
    QString secondLineFontColor;
    QString tabFontColor;
    QString buttonFontColor;
    QString dlgTitleBarColor;
    QString textFieldBackColor;
    QString menuItemBackColor;
    QString comboNormalColor;
    QString comboFocusedColor;
    QString comboPressedColor;
    QString comboDisabledFocusedColor;
    QString comboDisabledColor;
    QString progressBackColor;
    QString progressForeColor;
    QString sectionSeparatorColor;
    QString scrollBarIcon;
    QUrl qmlBaseUrl;
    qreal scrollWidth;
};
#endif
