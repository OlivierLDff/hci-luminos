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
	SelectionSize(0),
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

void FixturesModel::SelectOrDeselectFixture(const int idx)
{
	if(idx >= 0 && idx < Fixtures.size())
		Fixtures[idx]->SetSelected(!Fixtures[idx]->GetIsSelected());

	if (Fixtures[idx]->GetIsSelected()) ++SelectionSize;
	else --SelectionSize;
	emit SelectionSizeChanged(SelectionSize);

	const QModelIndex top = createIndex(idx, 0);
	emit dataChanged(top, top);
}

void FixturesModel::SelectAll()
{
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
	{
		(*it)->SetSelected(true);
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex((int)Fixtures.size() - 1, 0);

	SelectionSize = (qint32)Fixtures.size();
	emit SelectionSizeChanged(SelectionSize);

	emit dataChanged(top, bottom);
}

void FixturesModel::ClearAll()
{
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
	{
		(*it)->SetSelected(false);
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex((int)Fixtures.size() - 1, 0);
	SelectionSize = 0;
	emit SelectionSizeChanged(SelectionSize);
	emit dataChanged(top, bottom);
}

qint32 FixturesModel::GetSelectionSize() const
{
	return SelectionSize;
}

void FixturesModel::SetSelectionSize(const qint32 selectionSize)
{
	SelectionSize = selectionSize;
	emit SelectionSizeChanged(selectionSize);
}

void FixturesModel::SetMaster(const qreal value)
{
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
	{
		(*it)->SetDimmer(value * 255);	
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex((int)Fixtures.size() - 1, 0);

	emit dataChanged(top, bottom);
	//emit layoutChanged(); //way to heavy
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
	if (role == AddressRole)
		return fixture->GetAddress();
	if (role == ColorRole)
		return QColor(fixture->GetRed(), fixture->GetGreen(), fixture->GetBlue(), 255);
	if (role == ColorBackground1Role)
		return QColor(fixture->GetRed(), fixture->GetGreen(), fixture->GetBlue(), 127);
	if (role == ColorBackground2Role)
		return QColor(fixture->GetRed(), fixture->GetGreen(), fixture->GetBlue(), 64);
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
	roles[ColorBackground1Role] = "colorBack1";
	roles[ColorBackground2Role] = "colorBack2";
	roles[DimmerRole] = "dimmer";
	roles[IsSelectedRole] = "selected";
	roles[XRole] = "x";
	roles[YRole] = "y";
	return roles;
}
