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

	Sensor = new SensorModel(false); //Sensor model that leave in his own thread
	FixturesManager = new FixturesModel(Sensor); //Fixture model also leave in his own thread

	qmlRegisterSingletonType<SensorModel>("LuminosModel", 1, 0, "SensorModel", SensorModelProvider); //Register Sensor model as a singleton for the model
	qmlRegisterSingletonType<FixturesModel>("LuminosModel", 1, 0, "FixturesModel", FixturesModelProvider); //Register fixture model as a singleton for the model
	
	//Create all fixture in the model

	//TABLE MAIN
	//for(int i = 0; i < 1000; ++i)
	//	FixturesManager->AddFixture(1, (double)i/10, 0.3535f);
	FixturesManager->AddFixture(1, 0.371f, 0.3535f);
	FixturesManager->AddFixture(6, 0.579f, 0.348f);
	FixturesManager->AddFixture(11, 0.371f, 0.505f);
	FixturesManager->AddFixture(16, 0.585f, 0.507f);

	//TABLE TOP
	FixturesManager->AddFixture(21, 0.478f, 0.181f);
	FixturesManager->AddFixture(26, 0.703f, 0.177f);

	//TABLE LEFT
	FixturesManager->AddFixture(31, 0.898f, 0.4f);
	FixturesManager->AddFixture(36, 0.9f, 0.59f);

	//HALL
	FixturesManager->AddFixture(41, 0.775f, 0.8f);

	//KITCHEN
	FixturesManager->AddFixture(46, 0.1605f, 0.7f);
	FixturesManager->AddFixture(51, 0.466f, 0.71f);

	QQuickStyle::setStyle("Material"); //todo for later, set our own style
	QQuickStyle::setFallbackStyle("Material"); //fallback style is material if an object doesn't exist in our style

	QQmlApplicationEngine engine; //Create the qml engine
	engine.load(QUrl(QStringLiteral("qrc:/main.qml"))); //launch the application views
	if (engine.rootObjects().isEmpty()) //check that it has been found
		return -1;

#ifdef DMX_MANAGER_CORE
	MyApp ap2p;		//Create the app
	ap2p.Launch();	//And launch it
#endif
	return app.exec(); //Launch qt core thread
}
