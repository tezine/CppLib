#ifndef BASE_H
#define BASE_H
#include <QObject>
#include "cpplib_global.h"

/**
 *
 */
class CPPLIB_EXPORT CBase : public QObject {
	Q_OBJECT
	Q_FLAGS(DebugModes)
    Q_ENUMS(DebugMode BasicType  DatabaseType  ViewPropertyType Platform)

	public:
		enum BasicType {
			BasicTypeString=1,
			BasicTypeChar=2,
			BasicTypeInt32=3,
			BasicTypeInt64=4,
			BasicTypeDouble=5,
			BasicTypeFloat=6,
			BasicTypeBoolean=7,
			BasicTypeDate=8,
			BasicTypeDateTime=9,
			BasicTypeTime=10,
			BasicTypeArray=11,
			BasicTypeStringList=65
		};
        enum ViewPropertyType{
            ViewPropertyTypeString=1,
            ViewPropertyTypeInt,
            ViewPropertyTypeDouble,
            ViewPropertyTypeDate,
            ViewPropertyTypeBool,
            ViewPropertyTypeAlias,
            ViewPropertyTypeReal,
            ViewPropertyTypeUrl,
            ViewPropertyTypeColor,
            ViewPropertyTypeFont,
            ViewPropertyTypePoint,
            ViewPropertyTypeSize,
            ViewPropertyTypeRect,
            ViewPropertyTypeVariant,
            ViewPropertyTypeUnknown
        };
		enum DebugMode {
			DebugModeGC=0x1,
			DebugModeFiles=0x2,
			DebugModeFunctions=0x4,
			DebugModeLines=0x8,
			DebugModeLineBlocks=0x10,
			DebugModeLinePieces=0x20,
			DebugModeReleaseOrNone=0x40,
            DebugModeArguments=0x80,
			DebugModeLineControls=0x100,
			DebugModeInternalGC=0x200,
			DebugModeIncrementalCompiler=0x400,
			DebugModeExtra=0x800,
			DebugModeOperations=0x1000,
            DebugModeSignalSlots=0x2000,
            DebugModeCodeComplete=0x4000,
            DebugModeSemanticHighlight=0x8000// nexts 0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000
		};
		Q_DECLARE_FLAGS(DebugModes, DebugMode)	
        enum Platform {
            PlatformWindows=1,
            PlatformLinux=2,
            PlatformMac=3,
            PlatformAndroid=4,
            PlatformIOS=5
        };
		enum DatabaseType{
			DatabaseTypeMySQL=1,
			DatabaseTypePostgreSQL
		};	
        CBase();
		static QString getWikiosStoragePath();
		static void setWikiosStoragePath(const QString &storagePath);
		static void internalDebug(const QString& msg);
		static void checkAndDebug(bool check, qint32 debugTabCount, const QString& msg);
		static void checkAndDebug(bool check, const QString& msg){checkAndDebug(check,debugTabCount,msg); }
		static void resetDebug(){debugLine=0; }
		static bool copyFileAndMakeWritable(const QString &sourcePath, const QString &destPath, bool replaceIfExists);
		static bool copyDirRecursively(const QString &sourceDirPath, const QString &destDirPath, const QStringList &excludeList);
		static bool removeDirRecursively(const QString & dirName);
		static QString getMD5InBase64(const QString &txt);		
		static QString getTimeFromMsecs(double msecs);
        static QStringList convertToStringList(const QList<qint64>& list);
        static QList<qint64> removeDuplication(const QList<qint64>& list);
        static QVariant deserializeArray(const QByteArray& array);
        static QByteArray serializeVariant(QVariant variant);
        static void setStudioMode(){studioMode=true;}
        static bool isStudioMode(){return studioMode;}
        static bool writeArrayToFile(const QByteArray &array, const QString &completeFilePath);
        static bool readFileToArray(const QString &completeFilePath, bool textOnly,  QByteArray &outArray, QString &outError);

private:
		static qint64 debugLine;
		static qint32 debugTabCount;
		static QString storagePath;
        static bool studioMode;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(CBase::DebugModes)
#endif
