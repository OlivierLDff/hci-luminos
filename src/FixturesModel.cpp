#include <FixturesModel.hpp>
#include <SensorModel.hpp>

uint8_t Fixture::GetRed() const
{
	return Red;
}

void Fixture::SetRed(const uint8_t red)
{
	Red = red;
}

uint8_t Fixture::GetGreen() const
{
	return Green;
}

void Fixture::SetGreen(const uint8_t green)
{
	Green = green;
}

uint8_t Fixture::GetBlue() const
{
	return Blue;
}

void Fixture::SetBlue(const uint8_t blue)
{
	Blue = blue;
}

uint8_t Fixture::GetDimmer() const
{
	return Dimmer;
}

void Fixture::SetDimmer(const uint8_t dimmer)
{
	Dimmer = dimmer;
}

uint16_t Fixture::GetAddress() const
{
	return Address;
}

void Fixture::SetAddress(const uint16_t address)
{
	Address = address;
}

bool Fixture::GetIsSelected() const
{
	return bSelected;
}

void Fixture::SetSelected(const bool selected)
{
	bSelected = selected;
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
	DmxManager(1),
	Node(&DmxManager, GetWildcardNetworkAdapterV4(), false, false, EArtnodeMode_Controller),
	Universe(DmxManager.GetDmxUniverse(1)),
#endif
	Sensor(sensor), //Link to sensor class
	ModelMode(ModeClass::EConsumptionMode_Eco),
	bProgrammerChanged(false),
	SelectionSize(0),
	Master(1.f)
{
	ModeClass::declareQML();
#ifdef DMX_MANAGER_CORE

	DmxManager.AddDmxCallback(this); //Add a listener to be updated every 25ms
	Node.SetBroadcastInNetwork(false); //broadcast to 255.255.255.255 to be sure information arrive to computer
	Node.AddPort(Universe, 0, EArtnetPortMode_OutputBroadcast); // universe 0 artnet is internal universe 1
	Node.ActivateOutput(); // activate the output

#ifdef WIN32 //oliv : only for debug, this will be replace by a proper menu, on phone wildcard work well
	NetworkAdapterV4 a = NetworkAdapterFromIpString("2.0.0.2");
	Node.SetNetworkAdapter(&a);
#endif

	Node.Start(); //Start artnet
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
	DmxManager.RemoveDmxCallback(this);
	s->Post();
	return IThreadFunction::StopSignal();
}

void FixturesModel::Run(IThreadArg* threadArg)
{
	s->Wait();
	while (bIsRunning)
	{
//_________________PROTECT THE UNIVERSE IN WRITE MODE_________________________
		Universe->ProtectUniverseWrite(); //Take MUTEX
		//dmx tick here
		for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if (*it)
		{
			const Fixture * f = (*it);
			Universe->SetChannel(f->GetAddress(), f->GetDimmer()*GetDimmerMultiplier());
			Universe->SetChannel(f->GetAddress() + 2, 14);
			Universe->SetChannel(f->GetAddress() + 3, f->GetRed());
			Universe->SetChannel(f->GetAddress() + 4, f->GetGreen());
			Universe->SetChannel(f->GetAddress() + 5, f->GetBlue());
			Universe->SetChannel(f->GetAddress() + 8, f->GetZoom());
		}
		Universe->ReleaseUniverseWrite();
//_________________RELEASE THE UNIVERSE IN WRITE MODE_________________________

		s->Wait();
	}
}
#endif

double FixturesModel::GetDimmerMultiplier() const
{
	switch(ModelMode)
	{
	case ModeClass::EConsumptionMode_Eco: return 0.5f;
	case ModeClass::EConsumptionMode_Weather: return 1.f-Sensor->GetLux()/20000;
	case ModeClass::EConsumptionMode_Full: return 1.f;
	default: ;
	}
	return 1.f;
}

qint32 FixturesModel::GetModelMode() const
{
	return ModelMode;
}

void FixturesModel::SetModelMode(qint32 comsumptionMode)
{
	ModelMode = comsumptionMode;
	emit ModelModeChanged(comsumptionMode);
}

void FixturesModel::SetColorFromPicker(double angle, double white)
{
	int modulto = (int)(angle)/360;
	angle -= modulto * 360;
	white *= 255;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if((*it) && (*it)->GetIsSelected())
	{
		//(*it)->SetSelected(true);
		if(angle >= 0 && angle <= 120)
		{
			if (angle <= 60)
			{
				(*it)->SetRed(255);
				(*it)->SetGreen(std::max(angle/60*255, white));
				(*it)->SetBlue(white);
			}			
			else
			{
				(*it)->SetRed(std::max(255 - (angle - 60) / 60 * 255, white));
				(*it)->SetGreen(255);
				(*it)->SetBlue(white);
			}			
		}
		else if( angle > 120 && angle <= 240)
		{
			if (angle <= 180)
			{
				(*it)->SetRed(white);
				(*it)->SetGreen(255);
				(*it)->SetBlue(std::max((angle - 120) / 60 * 255, white));
			}
			else
			{
				(*it)->SetRed(white);
				(*it)->SetGreen(std::max(255 - (angle - 180) / 60 * 255, white));/*std::max(255 - (angle - 120) / 60 * 255*/
				qInfo("%d)", (int)(255 - (angle - 180) / 60 * 255));
				(*it)->SetBlue(255);
			}
		}
		else
		{
			if (angle <= 300)
			{
				(*it)->SetRed(std::max((angle - 240) / 60 * 255, white));
				(*it)->SetGreen(white);
				(*it)->SetBlue(255);
			}
			else
			{
				(*it)->SetRed(255);
				(*it)->SetGreen(white);
				(*it)->SetBlue(std::max(255 - (angle - 300) / 60 * 255, white));
			}
		}
		const QModelIndex top = createIndex(it - Fixtures.begin(), 0); //not efficiant
		emit dataChanged(top, top);
	}
	bProgrammerChanged = true;
}

void FixturesModel::SetColor(QColor color)
{
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (*it)->GetIsSelected())
	{
		(*it)->SetRed(color.red());
		(*it)->SetGreen(color.green());
		(*it)->SetBlue(color.blue());

		const QModelIndex top = createIndex(it - Fixtures.begin(), 0); //not efficiant
		emit dataChanged(top, top);
	}
	bProgrammerChanged = true;
}

void FixturesModel::SelectOrDeselectFixture(const int idx)
{
	if (bProgrammerChanged) ClearSelection();
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
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it))
	{
		(*it)->SetSelected(true);
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex((int)Fixtures.size() - 1, 0);

	SelectionSize = (qint32)Fixtures.size();
	emit SelectionSizeChanged(SelectionSize);

	emit dataChanged(top, bottom);
}

void FixturesModel::ClearSelection()
{
	bProgrammerChanged = false;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it))
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
	const bool bAll = SelectionSize == 0;
	bProgrammerChanged = true;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (bAll || (*it)->GetIsSelected()))
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
