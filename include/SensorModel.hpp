#ifndef SENSOR_MODEL_HPP
#define SENSOR_MODEL_HPP

#include <QObject>
#include <OsDef.hpp>
#include <IThread.hpp>
#include <INetwork.hpp>

#define BACKEND_PORT 5123

typedef enum ESensorWeather
{
	ESensorWeather_Sun,
	ESensorWeather_Night,
	ESensorWeather_Rain,
	ESensorWeather_Snowing,
	ESensorWeather_Storm,
	ESensorWeather_Cloudy,
}ESensorWeather;

PRE_PACKED_ENUM_DEF
struct SensorBackendPacket //No header very ugly but BALEC
{
	ESensorWeather Weather;
	qreal Temperature;
	qreal Lux;
}
POST_PACKED_ENUM_DEF

class SensorModel;

class SensorServer : public IThreadFunction
{
public:
	explicit SensorServer(SensorModel* parent, IUDPSocket* s);

	~SensorServer();
private:
	IUDPSocket * s;
	IThread * t;
	SensorModel * Parent;

public:
	void Start();
	void Stop();

	void Run(IThreadArg* threadArg) override;
};

class SensorModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString WeatherImage READ GetWeatherImage WRITE SetWeatherImage NOTIFY WeatherChanged)
	Q_PROPERTY(qreal Temperature READ GetTemperature WRITE SetTemperature NOTIFY TemperatureChanged)
	Q_PROPERTY(qreal Lux READ GetLux WRITE SetLux NOTIFY LuxChanged)
public:
	SensorModel(const bool bBackend, QObject* parent = nullptr);
	~SensorModel();

	//Q_INVOKABLE int doSomething() { setSomeProperty(5); return m_someProperty; }

	static QString WeatherToString(const ESensorWeather e);

	QString GetWeatherImage() const;
	void SetWeatherImage(const QString& weatherImage);

	ESensorWeather GetWeatherEnum()const;
	void SetWeatherImage(const ESensorWeather e);

	qreal GetTemperature() const;
	void SetTemperature(const qreal temperature);

	qreal GetLux() const;
	void SetLux(const qreal lux);

signals:
	void WeatherChanged(QString newValue);
	void TemperatureChanged(qreal newValue);
	void LuxChanged(qreal newValue);


private:
	QString WeatherImage;
	qreal Temperature;
	qreal Lux;
	bool bBackend;	
	SensorServer UDPServer;
	friend class SensorServer;
};


#endif