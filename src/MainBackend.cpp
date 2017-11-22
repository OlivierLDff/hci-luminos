#include <QGuiApplication>
#include <QQuickStyle>
#include <QtQml>
#include <SensorModel.hpp>

static QObject *SensorModelProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)

		SensorModel *singleton = new SensorModel(true);
	return singleton;
}

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	//qmlRegisterType<SqlContactModel>("io.qt.examples.chattutorial", 1, 0, "SqlContactModel");
	//qmlRegisterType<SqlConversationModel>("io.qt.examples.chattutorial", 1, 0, "SqlConversationModel");
	qmlRegisterSingletonType<SensorModel>("LuminosModel", 1, 0, "SensorModel", SensorModelProvider);
	//qmlRegisterUncreatableType<MyStyle>("MyStyle", 1, 0, "MyStyle", "MyStyle is an attached property");

	//QQuickStyle::setStyle(":/MyStyle");
	QQuickStyle::setStyle("Material");
	QQuickStyle::setFallbackStyle("Material");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/MainBackend.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
