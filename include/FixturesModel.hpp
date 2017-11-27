#ifndef FIXTURES_MODEL_HPP
#define FIXTURES_MODEL_HPP

#include <QObject>
#include <QtQml>

#ifdef DMX_MANAGER_CORE
#include <DmxManagerCore/IDmxTickCallback.hpp>
#include <IMutex.hpp>
#include <IThread.hpp>
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

class Fixture : public QObject
{
	Q_OBJECT
	//Q_PROPERTY(QColor Color READ GetColor WRITE SetColor NOTIFY ColorChanged)
	Q_PROPERTY(quint8 Red READ GetRed WRITE SetRed NOTIFY RedChanged)
	Q_PROPERTY(quint8 Green READ GetGreen WRITE SetGreen NOTIFY GreenChanged)
	Q_PROPERTY(quint8 Blue READ GetBlue WRITE SetBlue NOTIFY BlueChanged)
	Q_PROPERTY(quint8 Dimmer READ GetDimmer WRITE SetDimmer NOTIFY DimmerChanged)
	Q_PROPERTY(quint16 Address READ GetAddress WRITE SetAddress NOTIFY AddressChanged)
	Q_PROPERTY(bool Selected READ GetIsSelected WRITE SetSelected NOTIFY SelectedChanged)

public:
	Fixture(const uint16_t addr, const double x, const double y, QObject * parent = nullptr) : QObject(parent), Red(0), Green(0), Blue(0), Dimmer(0), Address(addr), bSelected(false), x(x), y(y) {}
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

signals:
	void RedChanged(uint8_t value);
	void GreenChanged(uint8_t value);
	void BlueChanged(uint8_t value);
	void DimmerChanged(uint8_t value);
	void AddressChanged(uint16_t value);
	void SelectedChanged(bool value);

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
	double x;
	double y;
};

//#define NUMBER_OF_FIXTURE

class FixturesModel : public QAbstractListModel
#ifdef DMX_MANAGER_CORE
, public IDmxTickCallback, public IThreadFunction
#endif
{
Q_OBJECT
	Q_PROPERTY(ModeClass::EConsumptionMode ComsumptionMode READ GetComsumptionMode WRITE SetComsumptionMode NOTIFY ComsumptionModeChanged)
	Q_PROPERTY(qreal Master READ GetMaster WRITE SetMaster NOTIFY MasterChanged)
	Q_PROPERTY(qint32 SelectionSize READ GetSelectionSize WRITE SetSelectionSize NOTIFY SelectionSizeChanged)
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
#endif

	// ─────────────────────────────────────────────────────────────
	// 						COMSUMTION MODE		 					 
	// ─────────────────────────────────────────────────────────────
public:
	/** \brief current consumption mode, used in the run frunction to multiply the dimmer of every fixtures */
	ModeClass::EConsumptionMode GetComsumptionMode() const;
	void SetComsumptionMode(const ModeClass::EConsumptionMode comsumptionMode);
signals:
	void ComsumptionModeChanged(ModeClass::EConsumptionMode comsumptionMode);
private:
	class SensorModel * Sensor;
	ModeClass::EConsumptionMode ConsumptionMode;

	// ─────────────────────────────────────────────────────────────
	// 						FIXTURE MODEL	 					 
	// ─────────────────────────────────────────────────────────────

public:
	/** \brief apply color from color picker to the current selection */
	Q_INVOKABLE void SetColorFromPicker(double angle, double white);
	//Q_INVOKABLE void SetMaster(const double value);
	Q_INVOKABLE void SelectOrDeselectFixture(const int idx);
	Q_INVOKABLE void SelectAll();
	Q_INVOKABLE void ClearAll();

	

	qint32 GetSelectionSize() const;
	void SetSelectionSize(const qint32 selectionSize);

	qreal GetMaster() const { return Master; }
	void SetMaster(qreal comsumptionMode);
signals:
	void MasterChanged(qreal master);
	void SelectionSizeChanged(qint32 n);
private:
	qint32 SelectionSize;
	qreal Master;
public:
	enum EFixtureRoles 
	{
		AddressRole = Qt::UserRole + 1,
		ColorRole,
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
private:
	/** \brief Array that contain every fixture to be displayed inside the map */
	std::vector<Fixture *> Fixtures;
};


#endif