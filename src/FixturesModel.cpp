#include <FixturesModel.hpp>

FixturesModel::FixturesModel(SensorModel* sensor, QObject* parent): QObject(parent),
IDmxTickCallback(EDmxTickCallback_App),
t(CreateThread()),
s(CreateSemaphore(0, 1)), 
Sensor(sensor),
ConsumptionMode(ModeClass::EConsumptionMode_Eco)
{
	ModeClass::declareQML();
	t->Start(this);
}

FixturesModel::~FixturesModel()
{
	DestroyThread(t);
	DestroySemaphore(s);
}

void FixturesModel::notify(DmxManagerCore* parent)
{
	s->Post();
}

bool FixturesModel::StopSignal()
{
	s->Post();
	return IThreadFunction::StopSignal();
}

void FixturesModel::Run(IThreadArg* threadArg)
{
	s->Wait();
	while (bIsRunning)
	{
		//dmx tick here
		s->Wait();
	}
}

ModeClass::EConsumptionMode FixturesModel::GetComsumptionMode() const
{
	return ConsumptionMode;
}

void FixturesModel::SetComsumptionMode(const ModeClass::EConsumptionMode comsumptionMode)
{
	ConsumptionMode = comsumptionMode;
}
