#include <QGuiApplication>
#include <QQuickStyle>
#include <QtQml>

#include <SensorModel.hpp>
#include <FixturesModel.hpp>
#include <sstream>

SensorModel * Sensor;
FixturesModel * FixturesManager;

static QObject *SensorModelProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)

	//SensorModel *singleton = new SensorModel(false);
	return Sensor;
}

static QObject *FixturesModelProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)

	//FixturesModel *singleton = new FixturesModel();
	return FixturesManager;
}

int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	Sensor = new SensorModel(false); //Sensor model that leave in his own thread
	FixturesManager = new FixturesModel(Sensor); //Fixture model also leave in his own thread

	qmlRegisterSingletonType<SensorModel>("LuminosModel", 1, 0, "SensorModel", SensorModelProvider); //Register Sensor model as a singleton for the model
	qmlRegisterSingletonType<FixturesModel>("LuminosModel", 1, 0, "FixturesModel", FixturesModelProvider); //Register fixture model as a singleton for the model
	
	//Create all fixture in the model

	//TABLE MAIN
	//for(int i = 0; i < 1000; ++i)
	//FixturesManager->AddFixture(1, (double)i/10, 0.3535f);
	FixturesManager->AddFixture(1, 0.371f, 0.3535f);
	FixturesManager->AddFixture(10, 0.579f, 0.348f);
	FixturesManager->AddFixture(19, 0.371f, 0.505f);
	FixturesManager->AddFixture(28, 0.585f, 0.507f);

	//TABLE TOP
	FixturesManager->AddFixture(37, 0.478f, 0.181f);
	FixturesManager->AddFixture(46, 0.703f, 0.177f);

	//TABLE LEFT
	FixturesManager->AddFixture(55, 0.898f, 0.4f);
	FixturesManager->AddFixture(64, 0.9f, 0.59f);

	//HALL
	FixturesManager->AddFixture(73, 0.775f, 0.65f);
	FixturesManager->AddFixture(82, 0.775f, 0.8f);

	//KITCHEN
	FixturesManager->AddFixture(91, 0.1605f, 0.7f);
	FixturesManager->AddFixture(100, 0.466f, 0.71f);

	//DJ
	FixturesManager->AddFixture(109, 0.2f, 0.25);

	QQuickStyle::setStyle("Material"); //todo for later, set our own style
	QQuickStyle::setFallbackStyle("Material"); //fallback style is material if an object doesn't exist in our style

	QQmlApplicationEngine engine; //Create the qml engine
	engine.load(QUrl(QStringLiteral("qrc:/main.qml"))); //launch the application views
	if (engine.rootObjects().isEmpty()) //check that it has been found
		return -1;

#ifdef DMX_MANAGER_CORE
	//MyApp ap2p;		//Create the app
	//ap2p.Launch();	//And launch it
#endif
	return app.exec(); //Launch qt core thread
}
