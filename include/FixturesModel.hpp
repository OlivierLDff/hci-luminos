#ifndef FIXTURES_MODEL_HPP
#define FIXTURES_MODEL_HPP

#include <QObject>
#include <QtQml>

#ifdef DMX_MANAGER_CORE
#include <DmxManagerCore/IDmxTickCallback.hpp>
#include <IMutex.hpp>
#include <IThread.hpp>
#endif

class ModeClass : public QObject
{
	Q_OBJECT
public:
	// Default constructor, required for classes you expose to QML.
	ModeClass() : QObject() {}

	enum EConsumptionMode
	{
		EConsumptionMode_Eco,
		EConsumptionMode_Weather,
		EConsumptionMode_Full
	};
	Q_ENUMS(EConsumptionMode)

	// Do not forget to declare your class to the QML system.
	static void declareQML() 
	{
		qmlRegisterType<ModeClass>("LuminosModel", 1, 0, "Mode");
	}
};

class Fixture
{
public:
	Fixture(uint16_t addr) : addr(addr), r(0), g(0), b(0), d(0) {}
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t d;
	uint16_t addr;
};

#define NUMBER_OF_FIXTURE

class FixturesModel : public QObject, public IDmxTickCallback, public IThreadFunction
{
	Q_OBJECT
	Q_PROPERTY(ModeClass::EConsumptionMode ComsumptionMode READ GetComsumptionMode WRITE SetComsumptionMode NOTIFY ComsumptionModeChanged)
public:
	FixturesModel(class SensorModel* sensor, QObject* parent = nullptr);

	~FixturesModel();

	Q_INVOKABLE void SetColorFromPicker(double angle, double white)
	{
	}

	class IThread * t;
	class ISemaphore * s;

	/** THREAD */
	virtual void notify(DmxManagerCore* parent) override;
	bool StopSignal() override;
	void Run(IThreadArg* threadArg) override;

	ModeClass::EConsumptionMode GetComsumptionMode() const;
	void SetComsumptionMode(const ModeClass::EConsumptionMode comsumptionMode);

signals:
	void ComsumptionModeChanged(ModeClass::EConsumptionMode comsumptionMode);

private:
	class SensorModel * Sensor;
	ModeClass::EConsumptionMode ConsumptionMode;
};


#endif