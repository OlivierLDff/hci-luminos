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

void Fixture::ApplyFx()
{
	//fx_time + PI / 64.0;
	switch(fx)
	{
	case FxType::None: break;
	case FxType::Sin:
		fxtime += 3.14 / 64;
		//SetDimmer(GetDimmer());
		//if (!(cooldown % 5))
		{
			Dimmer = int(127.5 + 127.5 * sin(fxtime) + 0.5); //int(127.5 + 127.5 * sin(fxtime + (Dimmer * 3.14f / 2.0)) + 0.5); // round to nearest
		}
		break;
	case FxType::Lin: Dimmer -= 1; break;
	case FxType::Random:
		if(!(cooldown%10))
		{
			SetDimmer(rand() % 255);
			if (Dimmer < 50)
				Dimmer = 50;
		}
		break;
	case FxType::Rainbow:
		if (Blue == 0 && Red == 255 && Green < 255)
			++Green;
		else if (Blue == 0 && Green == 255 && Red > 0)
			--Red;
		else if (Red == 0 && Green == 255 && Blue < 255)
			++Blue; 
		else if (Red == 0 && Blue == 255 && Green > 0)
			--Green;
		if (Green == 0 && Blue == 255 && Red < 255)
			++Red;
		else if (Green == 0 && Red == 255 && Blue > 0)
			--Blue;
		break;
	default: ;
	}
	++cooldown;
}

FxType::EFxType Fixture::GetFx() const
{
	return fx;
}

void Fixture::SetFx(const FxType::EFxType fx)
{
	this->fx = fx;
	switch(fx)
	{
	case FxType::None: break;
	case FxType::Sin:
	case FxType::Lin:
	case FxType::Random: Dimmer = rand() % 255; break;
	case FxType::Rainbow:
		switch (rand() % 3)
		{
		case 0:
			Red = 255;
			Green = rand() % 255;
			Blue = 0;
			break;
		case 1:
			Green = 255;
			Blue = rand() % 255;
			Red = 0;
			break;
		case 2:
			Blue = 255;
			Red = rand() % 255;
			Green = 0;
			break;
		default:;
		}	
		
		break;
	default: ;
	}
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
	//, AdapterList(new QStringList())
{
	ModeClass::declareQML();
	FxType::declareQML();
#ifdef DMX_MANAGER_CORE

	DmxManager.AddDmxCallback(this); //Add a listener to be updated every 25ms
	Node.SetBroadcastInNetwork(false); //broadcast to 255.255.255.255 to be sure information arrive to computer
	Node.AddPort(Universe, 0, EArtnetPortMode_OutputBroadcast); // universe 0 artnet is internal universe 1
	Node.ActivateOutput(); // activate the output

#ifdef WIN32 //oliv : only for debug, this will be replace by a proper menu, on phone wildcard work well
	NetworkAdapterV4 a = NetworkAdapterFromIpString("2.0.0.2");
	Node.SetNetworkAdapter(&a);
#endif

	std::vector<NetworkAdapterV4> list = GetAllNetworkAdaptersV4();
	for (size_t i = 0; i < list.size(); ++i)
		AdapterList.append(QString::fromStdString(list[i].Ipv4ToString()));
	AdapterList.append("0.0.0.0");
	emit AdapterListChanged(AdapterList);

	Node.Start(); //Start artnet
	t->Start(this); //Start application thread that handle dmx data and update fx
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
		if(AdapterIp != Node.GetNetworkAdapter().Ipv4ToString())
			emit ArtnetAdapterIndexChanged(GetArtnetAdapterIndex()); //check network deconnection
			
//_________________PROTECT THE UNIVERSE IN WRITE MODE_________________________
		Universe->ProtectUniverseWrite(); //Take MUTEX
		//dmx tick here
		for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if (*it)
		{
			Fixture * f = (*it);
			f->ApplyFx();
			if(f->GetFx() != FxType::None)
			{
				const QModelIndex top = createIndex(it - Fixtures.begin(), 0); //not efficiant
				emit dataChanged(top, top);		
			}
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
	case ModeClass::EConsumptionMode_Eco: return ((double)EcoMultiplier)/100.f;
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

qint32 FixturesModel::GetEcoMultiplier() const
{
	return EcoMultiplier;
}

void FixturesModel::SetEcoMultiplier(const qint32 ecoMultiplier)
{
	EcoMultiplier = ecoMultiplier;
}

void FixturesModel::SetColorFromPicker(double angle, double white)
{
	int modulto = (int)(angle)/360;
	angle -= modulto * 360;
	white *= 255;
	const bool bAll = SelectionSize == 0;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (bAll || (*it)->GetIsSelected()))
	{
		if ((*it)->GetFx() == FxType::Rainbow)
			(*it)->SetFx(FxType::None);
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
	const bool bAll = SelectionSize == 0;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (bAll || (*it)->GetIsSelected()))
	{
		if ((*it)->GetFx() == FxType::Rainbow)
			(*it)->SetFx(FxType::None);
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

void FixturesModel::SetFx(const int fx)
{
	const bool bAll = SelectionSize == 0;
	bProgrammerChanged = true;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (bAll || (*it)->GetIsSelected()))
	{
		(*it)->SetFx((FxType::EFxType)fx);
		const QModelIndex top = createIndex(it - Fixtures.begin(), 0); //not efficiant
		emit dataChanged(top, top);
	}
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
	bProgrammerChanged = true;
	const bool bAll = SelectionSize == 0;
	for (std::vector<Fixture *>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it) if ((*it) && (bAll || (*it)->GetIsSelected()))
	{
		if ((*it)->GetFx() != FxType::Rainbow || (*it)->GetFx() != FxType::None)
			(*it)->SetFx(FxType::None);
		(*it)->SetDimmer(value * 255);	
	}
	const QModelIndex top = createIndex(0, 0);
	const QModelIndex bottom = createIndex((int)Fixtures.size() - 1, 0);

	emit dataChanged(top, bottom);
	//emit layoutChanged(); //way to heavy
	emit MasterChanged(value);
}

void FixturesModel::SetArtnetOutput(const bool value)
{
	if (value) Node.ActivateOutput();
	else Node.DeactivateOutput();
	emit ArtnetOutputChanged(value);
}

void FixturesModel::SetAdapterList(const QStringList& adapterList)
{
	AdapterList = adapterList;
	emit AdapterListChanged(adapterList);
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
