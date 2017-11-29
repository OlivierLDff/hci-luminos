#ifndef FIXTURES_MODEL_HPP
#define FIXTURES_MODEL_HPP

#include <QObject>
#include <QtQml>
#include <QColor>

#ifdef DMX_MANAGER_CORE
#include <IMutex.hpp>
#include <IThread.hpp>

#include <DmxManagerCore/DmxManagerCore.hpp>			//Create the manager that manage universes
#include <DmxManagerCore/DmxUniverse.hpp>				//Array of data
#include <DmxManagerCore/IDmxChangedCallback.hpp>		//Listen to change, mainly for debug and to see when data arrive
#include <Artnode/Artnode.hpp>							//Create artnet node

#endif

/** \brief call that store the enumeration */
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
	Q_ENUMS(EConsumptionMode) //Declare the enum

	// Must be called only ONCE!!!
	static void declareQML() 
	{
		qmlRegisterType<ModeClass>("LuminosModel", 1, 0, "Mode");
	}
};

class FxType : public QObject
{
	Q_OBJECT
public:
	// Default constructor, required for classes you expose to QML.
	FxType() : QObject() {}

		enum EFxType
	{
		None,
		Sin,
		Lin,
		Random,
		Rainbow,
	};
	Q_ENUMS(EFxType) //Declare the enum

					 // Must be called only ONCE!!!
		static void declareQML()
	{
		qmlRegisterType<FxType>("LuminosModel", 1, 0, "FxType");
	}
};

class Fixture : public QObject
{
public:
	Fixture(const uint16_t addr, const double x, const double y, QObject * parent = nullptr) : QObject(parent), cooldown(0), fxtime(0), Red(255), Green(0), Blue(0), Dimmer(0), Address(addr), bSelected(false), x(x), y(y), fx(FxType::None) {}
	~Fixture() {}

	uint8_t GetRed() const;
	void SetRed(const uint8_t red);

	uint8_t GetGreen() const;
	void SetGreen(const uint8_t green);

	uint8_t GetBlue() const;
	void SetBlue(const uint8_t blue);

	uint8_t GetDimmer() const;
	void SetDimmer(const uint8_t dimmer);

	uint16_t GetAddress() const;
	void SetAddress(const uint16_t address);

	bool GetIsSelected() const;
	void SetSelected(const bool selected);

	double GetX() const;
	void SetX(const double x);

	double GetY() const;
	void SetY(const double y);

	void ApplyFx();
	int cooldown;
	double fxtime;

	FxType::EFxType GetFx() const;
	void SetFx(const FxType::EFxType fx);

	static uint8_t GetZoom() { return 0; } //yolo yolo

private:
	/** \brief value for red led */
	uint8_t Red;
	/** \brief value for green led */
	uint8_t Green;
	/** \brief value for blue led */
	uint8_t Blue;
	/** \brief value for dimmer led */
	uint8_t Dimmer;
	/** \brief dmx address (1 t 512) */
	uint16_t Address;
	/** \brief is the fixture currently selected */
	bool bSelected;
	/** \brief x position on the map */
	double x;
	/** \brief y position on the map */
	double y;
	FxType::EFxType fx;
};

//#define NUMBER_OF_FIXTURE

class FixturesModel : public QAbstractListModel
#ifdef DMX_MANAGER_CORE
, public IDmxTickCallback, public IThreadFunction
#endif
{
	Q_OBJECT

	// ─────────────────────────────────────────────────────────────
	// 						Q PROPERTY			 
	// ─────────────────────────────────────────────────────────────
	
	Q_PROPERTY(qint32 ModelMode READ GetModelMode WRITE SetModelMode NOTIFY ModelModeChanged)
	Q_PROPERTY(qreal Master READ GetMaster WRITE SetMaster NOTIFY MasterChanged)
	Q_PROPERTY(qint32 SelectionSize READ GetSelectionSize WRITE SetSelectionSize NOTIFY SelectionSizeChanged)
	Q_PROPERTY(bool ArtnetOutput READ GetArtnetOutput WRITE SetArtnetOutput NOTIFY ArtnetOutputChanged)
	Q_PROPERTY(QStringList AdapterList READ GetAdapterList WRITE SetAdapterList NOTIFY AdapterListChanged)
	Q_PROPERTY(qint32 ArtnetAdapterIndex READ GetArtnetAdapterIndex WRITE SetArtnetAdapterIndex NOTIFY ArtnetAdapterIndexChanged)

	// ─────────────────────────────────────────────────────────────
	// 					CONSTRUCTOR / DESTRUCTOR					 
	// ─────────────────────────────────────────────────────────────
public:
	explicit FixturesModel(class SensorModel* sensor, QObject* parent = nullptr);
	~FixturesModel();

	// ─────────────────────────────────────────────────────────────
	// 						DMX APP THREAD		 					 
	// ─────────────────────────────────────────────────────────────

#ifdef DMX_MANAGER_CORE
	/** \brief notify of dmx tick, post sem s */
	void notify(DmxManagerCore* parent) override;
	/** \brief received when the thread must be destroyed */
	bool StopSignal() override;
	/** \brief run the main thread that update dmx data, threadARg not used. This is start inside the constructor and destroy in destructor */
	void Run(IThreadArg* threadArg) override;
private:
	class IThread * t; //Handle dmx refreshment app
	class ISemaphore * s; //tick semaphore with dmx

	DmxManagerCore DmxManager;  //Create the dmx manager, we set at the initialisation the number of universes, this number can't be changed later
	Artnode Node; 
	DmxUniverse * Universe;
#endif

	// ─────────────────────────────────────────────────────────────
	// 						COMSUMTION MODE		 					 
	// ─────────────────────────────────────────────────────────────
public:
	double GetDimmerMultiplier() const;
	/** \brief current consumption mode, used in the run frunction to multiply the dimmer of every fixtures */
	qint32 GetModelMode() const;
	void SetModelMode(qint32 comsumptionMode);
signals:
	void ModelModeChanged(qint32 comsumptionMode);
private:
	class SensorModel * Sensor;
	qint32 ModelMode;
	bool bProgrammerChanged;

	// ─────────────────────────────────────────────────────────────
	// 						FIXTURE MODEL	 					 
	// ─────────────────────────────────────────────────────────────

public:
	/** 
	 * \brief apply color from color picker to the current selection
	 * \param angle 0 t 360 
	 * \param white 0 t 1
	 */
	Q_INVOKABLE void SetColorFromPicker(double angle, double white);
	Q_INVOKABLE void SetColor(QColor color);
	/** \brief select one fixture */
	Q_INVOKABLE void SelectOrDeselectFixture(const int idx);
	/** \brief select all fixtures */
	Q_INVOKABLE void SelectAll();
	/** \brief clear selection */
	Q_INVOKABLE void ClearSelection();
	Q_INVOKABLE void SetFx(const int fx);

	qint32 GetSelectionSize() const;
	void SetSelectionSize(const qint32 selectionSize);
	qreal GetMaster() const { return Master; }
	void SetMaster(qreal comsumptionMode);
	bool GetArtnetOutput() const { return Node.IsOutputActive(); }
	void SetArtnetOutput(const bool value);
signals:
	void MasterChanged(qreal master);
	void SelectionSizeChanged(qint32 n);
	void ArtnetOutputChanged(bool value);
private:
	qint32 SelectionSize;
	qreal Master;

private:
	/** \brief Array that contain every fixture to be displayed inside the map */
	std::vector<Fixture *> Fixtures;

	// ─────────────────────────────────────────────────────────────
	// 					 NETWORK ADAPTER MODEL		 
	// ─────────────────────────────────────────────────────────────


public:
	QStringList GetAdapterList() const
	{
		return AdapterList;
	}

	void SetAdapterList(const QStringList& adapterList)
	{
		AdapterList = adapterList;
		emit AdapterListChanged(adapterList);
	}	
signals:
	void AdapterListChanged(QStringList value);
private:
	QStringList AdapterList;

public:
	qint32 GetArtnetAdapterIndex()
	{
		adapterip = Node.GetNetworkAdapter().Ipv4ToString();
		std::vector<NetworkAdapterV4> list = GetAllNetworkAdaptersV4();
		for (size_t i = 0; i < list.size(); ++i)
			if (adapterip == list[i].Ipv4ToString())
				return (qint32)i;
		return (qint32)list.size();
	}

	void SetArtnetAdapterIndex(const qint32 artnetAdapterIndex)
	{
		std::vector<NetworkAdapterV4> list = GetAllNetworkAdaptersV4();
		if(artnetAdapterIndex >= 0 && artnetAdapterIndex < list.size())
		{
			if (adapterip != list[artnetAdapterIndex].Ipv4ToString())
				Node.SetNetworkAdapter(&list[artnetAdapterIndex]);
		}
		else
		{
			NetworkAdapterV4 a;
			Node.SetNetworkAdapter(&a);
		}
		emit ArtnetAdapterIndexChanged(artnetAdapterIndex);
	}
	std::string adapterip;
signals:
	void ArtnetAdapterIndexChanged(qint32 value);


	// ─────────────────────────────────────────────────────────────
	// 					    MODEL OVERRIDE 					 
	// ─────────────────────────────────────────────────────────────

public:
	enum EFixtureRoles 
	{
		AddressRole = Qt::UserRole + 1,
		ColorRole,
		ColorBackground1Role,
		ColorBackground2Role,
		DimmerRole,
		IsSelectedRole,
		XRole,
		YRole
	};

	void AddFixture(const uint16_t address, const double x, const double y);
	int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
protected:
	QHash<int, QByteArray> roleNames() const override;
};


#endif