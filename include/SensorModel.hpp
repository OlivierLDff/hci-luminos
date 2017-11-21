#ifndef SENSOR_MODEL_HPP
#define SENSOR_MODEL_HPP
#include <QObject>

class SensorModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString WeatherImage READ GetWeatherImage WRITE SetWeatherImage NOTIFY WeatherChanged)
	Q_PROPERTY(qreal Temperature READ GetTemperature WRITE SetTemperature NOTIFY TemperatureChanged)
	Q_PROPERTY(qreal Lux READ GetLux WRITE SetLux NOTIFY LuxChanged)
public:
	SensorModel(QObject* parent = nullptr) : QObject(parent), WeatherImage("Sun"), Temperature(20), Lux(10000)
	{
	}

	//Q_INVOKABLE int doSomething() { setSomeProperty(5); return m_someProperty; }

	QString GetWeatherImage() const
	{
		return WeatherImage;
	}

	void SetWeatherImage(const QString& weatherImage)
	{
		WeatherImage = weatherImage;
		emit WeatherChanged(weatherImage);
	}


	qreal GetTemperature() const
	{
		return Temperature;
	}

	void SetTemperature(const qreal temperature)
	{
		Temperature = temperature;
		emit TemperatureChanged(temperature);
	}

	qreal GetLux() const
	{
		return Lux;
	}

	void SetLux(const qreal lux)
	{
		Lux = lux;
		emit LuxChanged(lux);
	}

signals:
	void WeatherChanged(QString newValue);
	void TemperatureChanged(qreal newValue);
	void LuxChanged(qreal newValue);

private:
	QString WeatherImage;
	qreal Temperature;
	qreal Lux;
};


#endif