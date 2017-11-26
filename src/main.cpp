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
#ifdef DMX_MANAGER_CORE
#include <DmxManagerCore/DmxManagerCore.hpp>			//Create the manager that manage universes
#include <DmxManagerCore/DmxUniverse.hpp>				//Array of data
#include <DmxManagerCore/IDmxChangedCallback.hpp>		//Listen to change, mainly for debug and to see when data arrive
#include <Artnode/Artnode.hpp>

#define UNIVERSE_NBR 1 //Number of universe handle by this application

DmxManagerCore DmxManager(UNIVERSE_NBR); //Create the dmx manager, we set at the initialisation the number of universes, this number can't be changed later
Artnode node(&DmxManager, NetworkHelper::GetWildcardNetworkAdapterV4(), false, false, EArtnodeMode_Controller);

class DmxChangedOnUniv1 : public IDmxChangedCallback //Create an object that listen to any change on dmx universes
{
public:
	void notify(DmxUniverse* callbackUniverse) override //Display the changed that have been notified
	{
		std::cout << "data changed on universe : " << callbackUniverse->GetUniverse() << "   CH 1 : " << static_cast<int>(callbackUniverse->GetChannel(1)) << " \tCH 5 : " << static_cast<int>(callbackUniverse->GetChannel(5)) <<
			". \tSource is : " << EDmxTransportTypeToString[callbackUniverse->GetLastDmxInTransportType()] << std::endl;
	}
};
DmxChangedOnUniv1 ReceivedListener; //The dmx changed listener

class MyApp : public IDmxTickCallback, public IThreadFunction
{
public:
	MyApp();
	~MyApp() override;
protected:
	ISemaphore * sem;							//The semaphore that handle the wait
	bool bIsRunning;							//If the thread is running
	DmxUniverse * tUniv;						//Pointer to the first universe
	IThread* t;									//Handler for the main thread
	bool bStrob, bLin;								//To switch between lin and strob function
	void Lin() const;							//linear effect on universe
	void Strob() const;							//Strob effect on universe
	void RampUpDown() const;
	void Run(IThreadArg* threadArg) override;	//The main thread function
	bool StopSignal() override;					//Used to stop the run function
public:
	void Launch();								//Launch the application

	void notify(DmxManagerCore* parent) override;
};

MyApp::MyApp(): IDmxTickCallback(EDmxTickCallback_App), sem(CreateSemaphore(0, 1)),
                bIsRunning(true), tUniv(nullptr), t(CreateThread()), bStrob(false), bLin(false)
{
	DmxManager.AddDmxCallback(this); //Add a listener to be updated every 25ms
}

MyApp::~MyApp()
{
	DmxManager.RemoveDmxCallback(this);
	DestroyThread(t);
	DestroySemaphore(sem);
}

void MyApp::Lin() const
{
	//std::cout << "j = " << (int)j << std::endl;
	for (uint16_t i = 1; i <= DMX_UNIVERSE_MAX_SIZE; ++i) //Linear up effect on all channel
		tUniv->SetChannel(i, tUniv->GetChannel(i) + 1);	
}

void MyApp::RampUpDown() const
{
	static uint8_t j = 0;
	static bool bRampUp = true;
	//std::cout << "j = " << (int)j << std::endl;
	for (uint16_t i = 1; i <= DMX_UNIVERSE_MAX_SIZE; ++i) //Linear up effect on all channel
		tUniv->SetChannel(i, j);

	if (bRampUp) 
		++j; 
	else 
		--j;
	if (j == 0) bRampUp = true;
	if (j == 255) bRampUp = false;
}

void MyApp::Strob() const
{
	tUniv->SetUnivAt(tUniv->GetChannel(1) == 0 ? 255 : 0);	//strob efect
}

void MyApp::Run(IThreadArg* threadArg)
{
	for (uint16_t i = 1; i <= DMX_UNIVERSE_MAX_SIZE; ++i)
		tUniv->SetChannel(i, static_cast<uint8_t>(i));
	sem->Wait();
	while (bIsRunning)
	{
		if (bStrob)
			Strob();
		else if (bLin)
			Lin();
		//else
		//	RampUpDown();
		//qInfo("update");
		sem->Wait();
	}
}

bool MyApp::StopSignal()
{
	bIsRunning = false;
	sem->Post();
	return true;
}

void MyApp::Launch()
{
	std::cout << "Application launch" << std::endl;

	//tUniv = DmxManager.GetDmxUniverse(2);					//Manipulate the first universe
	//tUniv->AddOnDmxChangedCallback(&ReceivedListener);		//Update log

	//tUniv->SetMergingType(EMergeArbitration_HTP);			//Merge ltp
	DmxManager.SetDmxRefreshRateInHz(32);

	std::vector<NetworkAdapterV4> AdapterList = NetworkHelper::GetAllNetworkAdaptersV4();

	for (size_t i = 0; i < AdapterList.size(); ++i)
	{
		std::stringstream ss;
		ss << "Network Adapter " << (i + 1) << " : " << AdapterList[i].Description << std::endl;
		ss << "> Friendly Name : " << AdapterList[i].FriendlyName << std::endl;
		ss << "> Ipv4 : " << AdapterList[i].Ipv4ToString() << ", Mask : " << AdapterList[i].Ipv4MaskToString() << ", Broadcast : " << AdapterList[i].Ipv4BroadcastToString() << std::endl;
		ss << "> Mac Address : " << AdapterList[i].PhysicalAddressToString() << std::endl << std::endl;
		qInfo(ss.str().c_str()); //<< (std::string)();
	}

	tUniv = DmxManager.GetDmxUniverse(1);					//Manipulate the first universe
	tUniv->AddOnDmxChangedCallback(&ReceivedListener);		//Update log
	tUniv->SetMergingType(EMergeArbitration_HTP);			//Merge ltp
	tUniv->SetUnivAt(255);	//strob efect
	//node.AddPort(tUniv, 0, EArtnetPortMode_Input);
	//NetworkAdapterV4 a = NetworkAdapterFromIpString("2.0.0.2");
	//node.SetNetworkAdapter(&a);
	//node.SetNetworkAdapter(&AdapterList[0]);
	node.SetBroadcastInNetwork(false);
	//node.AddPort(tUniv, 0, EArtnetPortMode_OutputAuto);
	//node.AddPort(tUniv, 1, EArtnetPortMode_OutputAuto);
	node.AddPort(tUniv, 0, EArtnetPortMode_OutputBroadcast);
	/*node.AddPort(tUniv, 2, EArtnetPortMode_OutputBroadcast);*/
	node.ActivateOutput();
	node.SetStreamConstant(false);
	node.Start();
	//node.ActivateInput();
	t->Start(this);											//Start the run function
	bStrob = false;
	bLin = true;
}

void MyApp::notify(DmxManagerCore* parent)
{
	sem->Post();
}
#endif


int main(int argc, char *argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);

	Sensor = new SensorModel(false);
	FixturesManager = new FixturesModel(Sensor);

	//qmlRegisterType<SqlContactModel>("io.qt.examples.chattutorial", 1, 0, "SqlContactModel");
	//qmlRegisterType<SqlConversationModel>("io.qt.examples.chattutorial", 1, 0, "SqlConversationModel");
	qmlRegisterSingletonType<SensorModel>("LuminosModel", 1, 0, "SensorModel", SensorModelProvider);
	qmlRegisterSingletonType<FixturesModel>("LuminosModel", 1, 0, "FixturesModel", FixturesModelProvider);
	//qmlRegisterUncreatableType<MyStyle>("MyStyle", 1, 0, "MyStyle", "MyStyle is an attached property");

	//QQuickStyle::setStyle(":/MyStyle");
	QQuickStyle::setStyle("Material");
	QQuickStyle::setFallbackStyle("Material");

	QQmlApplicationEngine engine;
	QQmlContext * context = new QQmlContext(engine.rootContext());
	//context->setContextProperty("myModel", &modelData);
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;
#ifdef DMX_MANAGER_CORE
	MyApp ap2p;		//Create the app
	ap2p.Launch();	//And launch it
#endif
	return app.exec();
}
