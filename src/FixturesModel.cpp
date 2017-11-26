#include <FixturesModel.hpp>

#include <QColor>

uint8_t Fixture::GetRed() const
{
	return Red;
}

void Fixture::SetRed(const uint8_t red)
{
	Red = red;
	emit RedChanged(red);
}

uint8_t Fixture::GetGreen() const
{
	return Green;
}

void Fixture::SetGreen(const uint8_t green)
{
	Green = green;
	emit GreenChanged(green);
}

uint8_t Fixture::GetBlue() const
{
	return Blue;
}

void Fixture::SetBlue(const uint8_t blue)
{
	Blue = blue;
	emit BlueChanged(blue);
}

uint8_t Fixture::GetDimmer() const
{
	return Dimmer;
}

void Fixture::SetDimmer(const uint8_t dimmer)
{
	Dimmer = dimmer;
	emit DimmerChanged(dimmer);
}

uint16_t Fixture::GetAddress() const
{
	return Address;
}

void Fixture::SetAddress(const uint16_t address)
{
	Address = address;
	emit AddressChanged(address);
}

bool Fixture::GetIsSelected() const
{
	return bSelected;
}

void Fixture::SetSelected(const bool selected)
{
	bSelected = selected;
	emit SelectedChanged(selected);
}

double Fixture::GetX() const
{
	return x;
}

void Fixture::SetX(const double x)
{
	this->x = x;
}

double Fixture::GetY() const
{
	return y;
}

void Fixture::SetY(const double y)
{
	this->y = y;
}

FixturesModel::FixturesModel(SensorModel* sensor, QObject* parent) : 
	QAbstractListModel(parent), //Qt object
	#ifdef DMX_MANAGER_CORE
	IDmxTickCallback(EDmxTickCallback_App),
	t(CreateThread()),
	s(CreateSemaphore(0, 1)),
	#endif
	Sensor(sensor), //Link to sensor class
	ConsumptionMode(ModeClass::EConsumptionMode_Eco),
	Master(1.f)
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
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it !=Fixtures.end(); ++it)
		delete *it;
	Fixtures.clear();
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

void FixturesModel::SetMaster(const qreal value)
{
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
	{
		(*it)->SetDimmer(value * 255);	
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex(Fixtures.size() - 1, 0);

	emit dataChanged(top, bottom);
	emit MasterChanged(value);
}

void FixturesModel::AddFixture(const uint16_t address, const double x, const double y)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	Fixtures.push_back(new Fixture(address, x, y));
	endInsertRows();
}

int FixturesModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return static_cast<int>(Fixtures.size());
}

QVariant FixturesModel::data(const QModelIndex& index, const int role) const
{
	if (index.row() < 0 || index.row() >= Fixtures.size())
		return QVariant();

	const Fixture * fixture = Fixtures[index.row()];
	//if (role == FixtureRole)
	//	return fixture;
	if (role == AddressRole)
		return fixture->GetAddress();
	if (role == ColorRole)
		return QColor(fixture->GetRed(), fixture->GetGreen(), fixture->GetBlue(), fixture->GetDimmer());
	if (role == DimmerRole)
		return fixture->GetDimmer();
	if (role == IsSelectedRole)
		return fixture->GetIsSelected();
	if (role == XRole)
		return fixture->GetX();
	if (role == YRole)
		return fixture->GetY();
	return QVariant();
}

QHash<int, QByteArray> FixturesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[AddressRole] = "address";
	roles[ColorRole] = "color";
	roles[DimmerRole] = "dimmer";
	roles[IsSelectedRole] = "selected";
	roles[XRole] = "x";
	roles[YRole] = "y";
	return roles;
}
