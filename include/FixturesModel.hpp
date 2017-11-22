#ifndef FIXTURES_MODEL_HPP
#define FIXTURES_MODEL_HPP

enum EConsumptionMode
{
	EConsumptionMode_Eco,
	EConsumptionMode_Weather,
	EConsumptionMode_Full
};

#include <QObject>

class FixturesModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(EConsumptionMode ComsumptionMode READ GetComsumptionMode WRITE SetComsumptionMode NOTIFY ComsumptionModeChanged)
public:
	FixturesModel(class SensorModel * sensor, QObject* parent = nullptr) : QObject(parent), Sensor(sensor),
	                                                                       ComsumptionMode(EConsumptionMode_Eco)
	{
		qRegisterMetaType<EConsumptionMode>("EConsumptionMode");
	}

	~FixturesModel()
	{
		
	}

	EConsumptionMode GetComsumptionMode() const
	{
		return ComsumptionMode;
	}

	void SetComsumptionMode(const EConsumptionMode comsumptionMode)
	{
		ComsumptionMode = comsumptionMode;
	}

signals:
	void ComsumptionModeChanged(EConsumptionMode comsumptionMode);

private:
	class SensorModel * Sensor;
	EConsumptionMode ComsumptionMode;
};


#endif