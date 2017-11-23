#ifndef FIXTURES_MODEL_HPP
#define FIXTURES_MODEL_HPP

#include <QObject>
#include <QtQml>

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

class FixturesModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(ModeClass::EConsumptionMode ComsumptionMode READ GetComsumptionMode WRITE SetComsumptionMode NOTIFY ComsumptionModeChanged)
public:
	FixturesModel(class SensorModel * sensor, QObject* parent = nullptr) : QObject(parent), Sensor(sensor),
		ConsumptionMode(ModeClass::EConsumptionMode_Eco)
	{
		ModeClass::declareQML();
		//qmlRegisterType<EConsumptionMode>("SensorModel", 1, 0, "EConsumptionMode");
	}

	~FixturesModel()
	{
		
	}

	
	//Q_ENUMS(EConsumptionMode)

	// Do not forget to declare your class to the QML system.
	/*static void declareQML() 
	{
		
	}*/

	ModeClass::EConsumptionMode GetComsumptionMode() const
	{
		return ConsumptionMode;
	}

	void SetComsumptionMode(const ModeClass::EConsumptionMode comsumptionMode)
	{
		ConsumptionMode = comsumptionMode;
	}

signals:
	void ComsumptionModeChanged(ModeClass::EConsumptionMode comsumptionMode);

private:
	class SensorModel * Sensor;
	ModeClass::EConsumptionMode ConsumptionMode;
};


#endif