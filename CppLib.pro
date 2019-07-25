include ( base.pri )
include ( jsonrpc/jsonrpc.pri )
include ( data/data.pri )
include ( entities/entities.pri )
include ( sql/sql.pri )
include ( platform/platform.pri )
include ( json/json.pri )
include ( net/net.pri )
include ( qml/qml.pri )

TARGET = CppLib
TEMPLATE = lib
DEFINES+=CPPLIB
CONFIG += c++17
android {
    CONFIG += staticlib
    DEFINES+=CPPLIBSTATIC
    QT += androidextras
    DEFINES+=FIREBASEANDROID
    FIREBASE_SDK = C:/firebase_cpp_sdk
    INCLUDEPATH += $${FIREBASE_SDK}/include/
    LIBS += -LC:/firebase_cpp_sdk/libs/android/armeabi-v7a/c++ -lfirebase_messaging -lfirebase_app
    HEADERS += fblistener.h
    SOURCES += fblistener.cpp
}
ios{
    CONFIG += staticlib
    DEFINES+=CPPLIBSTATIC
#    HEADERS += fblistener.h
#    SOURCES += fblistener.cpp
#    INCLUDEPATH+=/Volumes/500Mac2/firebase_cpp_sdk/include
}
macx{
      CONFIG += staticlib # para funcionar o script compileAndRun
      DEFINES+=CPPLIBSTATIC
#nao tem uikit para macos
#    QMAKE_CXXFLAGS += -F$$(HOME)/Library/Frameworks
#    QMAKE_LFLAGS += -F$$(HOME)/Library/Frameworks
#    LIBS += framework UIKit
}
CONFIG(debug) {
        # recolocar se for msvc PRECOMPILED_HEADER =precompileddebug.pch
}else{
	message(release compilation)
	PRECOMPILED_HEADER = precompiledrelease.pch
}


OTHER_FILES +=entities/cpplibentities.xml \
    ../AndroidLib/src/tezine/com/androidlib/JAndroidLib.java \
    ../AndroidLib/src/tezine/com/androidlib/JConstants.java \
    ../AndroidLib/src/tezine/com/androidlib/JInterJava.java \
    ../AndroidLib/src/tezine/com/androidlib/JString.java \
    ../AndroidLib/src/tezine/com/androidlib/MyGcmListenerService.java \
    ../AndroidLib/src/tezine/com/androidlib/RegistrationIntentService.java

SOURCES += \
	cbase.cpp \
	ccpp.cpp \
	cenums.cpp \
	blogger.cpp \
	cmeta.cpp \
	csingletonperthread.cpp \
	cqml.cpp \
        cinter.cpp \
        ctezine.cpp \
        ctezinebase.cpp \
        ctezinebaseproperties.cpp \
        enhancedqmlapplicationengine.cpp \
        firebase.cpp

HEADERS += \
	cbase.h \
	ccpp.h \
	cenums.h \
	blogger.h \
	cmeta.h \
	csingletonperthread.h \
	cdefines.h \
	cqml.h \
        cinter.h \
        ctezine.h \
        ctezinebase.h \
        ctezinebaseproperties.h \
        cpplib_global.h \
        enhancedqmlapplicationengine.h \
        firebase.h
