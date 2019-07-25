QT += qml quick core sql network  xml  positioning

windows{
	message(Compilacao para Windows)
        ProjetosFolder=C:/Projetos
	OutputFolder=$$ProjetosFolder/Builds/Output
}
android{
	message(Compilacao para Android)
        ProjetosFolder=C:/Projetos
	QT+=androidextras
	OutputFolder=$$ProjetosFolder/Builds/Output
}
ios{
        ProjetosFolder=/Users/tezine/Documents/Projetos
	QTPLUGIN= qavfcamera
        OutputFolder=/Users/tezine/Documents/Projetos/Builds/Output
}
macx{
	ProjetosFolder=/Users/tezine/Projetos
      OutputFolder=$$ProjetosFolder/Builds/Output
}
CppLibFolder=$$ProjetosFolder/Libs/CppLib
DESTDIR=$$OutputFolder

INCLUDEPATH+=$$CppLibFolder \
                        $$CppLibFolder/data \
                        $$CppLibFolder/entities \
                        $$CppLibFolder/json \
                        $$CppLibFolder/jsonrpc \
                        $$CppLibFolder/net \
                        $$CppLibFolder/platform \
                        $$CppLibFolder/sql \
                        $$CppLibFolder/qml
