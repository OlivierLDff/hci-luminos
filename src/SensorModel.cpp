#include <SensorModel.hpp>
#include <sstream>

SensorModel::SensorModel(const bool bBackend, QObject* parent) : QObject(parent),
WeatherImage("Sun"), 
Temperature(20),
Lux(10000), 
bBackend(bBackend),
UDPServer(this, ISocketFactory::CreateUDPSocket(BACKEND_PORT, BACKEND_PORT))
{
	UDPServer.Start();
}

SensorModel::~SensorModel()
{
	UDPServer.Stop();
}

QString SensorModel::GetWeatherImage() const
{
	return WeatherImage;
}

void SensorModel::SetWeatherImage(const QString& weatherImage)
{
	WeatherImage = weatherImage;
	std::stringstream ss;

	//qInfo << weatherImage;
	//qInfo("image changed to : %s", weatherImage.data());

	emit WeatherChanged(weatherImage);
}

ESensorWeather SensorModel::GetWeatherEnum() const
{
	if (WeatherImage == "Sun")
		return ESensorWeather_Sun;
	if (WeatherImage == "Cloudy")
		return ESensorWeather_Cloudy;
	if (WeatherImage == "Night")
		return ESensorWeather_Night;
	if (WeatherImage == "Rain")
		return ESensorWeather_Rain;
	if (WeatherImage == "Snowing")
		return ESensorWeather_Snowing;
	if (WeatherImage == "Storm")
		return ESensorWeather_Storm;
	return ESensorWeather_Sun;
}

void SensorModel::SetWeatherImage(const ESensorWeather e)
{
	SetWeatherImage(WeatherToString(e));
}

qreal SensorModel::GetTemperature() const
{
	return Temperature;
}

void SensorModel::SetTemperature(const qreal temperature)
{
	Temperature = temperature;
	emit TemperatureChanged(temperature);
}

qreal SensorModel::GetLux() const
{
	return Lux;
}

void SensorModel::SetLux(const qreal lux)
{
	Lux = lux;
	emit LuxChanged(lux);
}

QString SensorModel::WeatherToString(const ESensorWeather e)
{
	switch (e)
	{
	case ESensorWeather_Sun: return "Sun";
	case ESensorWeather_Night: return "Night";
	case ESensorWeather_Rain: return "Rain";
	case ESensorWeather_Snowing: return "Snowing";
	case ESensorWeather_Storm: return "Storm";
	case ESensorWeather_Cloudy: return "Cloudy";
	default: ;
	}
	return "";
}

UDPSocketReceiver::UDPSocketReceiver(SensorModel* parent, IUDPSocket* s): s(s), t(IThreadFactory::CreateThread()),
                                                                          Parent(parent)
{
	s->AllowBroadcast(true);
}

UDPSocketReceiver::~UDPSocketReceiver()
{
	if (t)
		IThreadFactory::DestroyThread(t);
	if (s)
		ISocketFactory::DestroyUDPSocket(s);
}

void UDPSocketReceiver::Start()
{
	NetworkAdapterV4 a = NetworkHelper::GetWildcardNetworkAdapterV4();
	//NetworkAdapterV4 a = NetworkHelper::NetworkAdapterFromIpString("192.168.0.100");
	ENetworkError e = s->Bind(&a);
	//qInfo("Bind ret : %d", e);
	t->Start(this);
}

void UDPSocketReceiver::Stop()
{
	IThreadFactory::DestroyThread(t);
	t = nullptr;
}

void UDPSocketReceiver::Run(IThreadArg* threadArg)
{
	//qInfo("Start run");
	while (bIsRunning)
	{
		SensorBackendPacket msg;
		if (Parent->bBackend)
		{
			Sleep(40);
			msg.Lux = Parent->GetLux();
			msg.Temperature = Parent->GetTemperature();
			msg.Weather = Parent->GetWeatherEnum();
			s->Send((uint8_t*)&msg, sizeof(msg));			
		}
		else
		{
			size_t read_cnt = sizeof(msg);
			int32_t read;
			uint32_t ip;
			//qInfo("Start read");
			if (s->Receive((uint8_t*)&msg, read_cnt, read, ip) == ENetworkError_Ok)
			{
				Parent->SetWeatherImage(msg.Weather);
				Parent->SetLux(msg.Lux);
				Parent->SetTemperature(msg.Temperature);
				//qInfo("receive %d", Parent->GetWeatherEnum());
			}
			//else
			//	qInfo("failed to receive");
		}
	}
}
