#include <FixturesModel.hpp>

#include <QColor>

uint8_t Fixture::GetR() const
{
	return R;
}

void Fixture::SetR(const uint8_t r)
{
	R = r;
}

uint8_t Fixture::GetG() const
{
	return G;
}

void Fixture::SetG(const uint8_t g)
{
	G = g;
}

uint8_t Fixture::GetB() const
{
	return B;
}

void Fixture::SetB(const uint8_t b)
{
	B = b;
}

uint8_t Fixture::GetD() const
{
	return D;
}

void Fixture::SetD(const uint8_t d)
{
	D = d;
}

uint16_t Fixture::GetAddr() const
{
	return Addr;
}

void Fixture::SetAddr(const uint16_t addr)
{
	Addr = addr;
}

FixturesModel::FixturesModel(SensorModel* sensor, QObject* parent): QAbstractListModel(parent),
#ifdef DMX_MANAGER_CORE
IDmxTickCallback(EDmxTickCallback_App),
#endif
Sensor(sensor),
ConsumptionMode(ModeClass::EConsumptionMode_Eco)
#ifdef DMX_MANAGER_CORE
, t(CreateThread()),
s(CreateSemaphore(0, 1))
#endif
{
	ModeClass::declareQML();
#ifdef DMX_MANAGER_CORE
	t->Start(this);
#endif
}

FixturesModel::~FixturesModel()
{

#ifdef DMX_MANAGER_CORE
	DestroyThread(t);
	DestroySemaphore(s);
#endif
}

#ifdef DMX_MANAGER_CORE
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
#endif

ModeClass::EConsumptionMode FixturesModel::GetComsumptionMode() const
{
	return ConsumptionMode;
}

void FixturesModel::SetComsumptionMode(const ModeClass::EConsumptionMode comsumptionMode)
{
	ConsumptionMode = comsumptionMode;
}

void FixturesModel::SetColorFromPicker(double angle, double white)
{
}

void FixturesModel::AddFixture(const Fixture& fixture)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	Fixtures.push_back(fixture);
	endInsertRows();
}

int FixturesModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return Fixtures.size();
}

QVariant FixturesModel::data(const QModelIndex& index, const int role) const
{
	if (index.row() < 0 || index.row() >= Fixtures.size())
		return QVariant();

	const Fixture & fixture = Fixtures[index.row()];
	if (role == AddressRole)
		return fixture.GetAddr();
	if (role == ColorRole)
		return QColor(fixture.GetR(), fixture.GetG(), fixture.GetB(), fixture.GetD());
	if (role == DimmerRole)
		return fixture.GetD();
	return QVariant();
}

QHash<int, QByteArray> FixturesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[AddressRole] = "address";
	roles[ColorRole] = "color";
	roles[DimmerRole] = "dimmer";
	return roles;
}
