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

class Fixture
{
public:
	explicit Fixture(const uint16_t addr) : R(0), G(0), B(0), D(0), Addr(addr), bSelected(false) {}

	uint8_t GetR() const;
	void SetR(const uint8_t r);

	uint8_t GetG() const;
	void SetG(const uint8_t g);

	uint8_t GetB() const;
	void SetB(const uint8_t b);

	uint8_t GetD() const;
	void SetD(const uint8_t d);

	uint16_t GetAddr() const;
	void SetAddr(const uint16_t addr);

private:
	/** \brief value for red led */
	uint8_t R;
	/** \brief value for green led */
	uint8_t G;
	/** \brief value for blue led */
	uint8_t B;
	/** \brief value for dimmer led */
	uint8_t D;
	/** \brief dmx address (1 t 512) */
	uint16_t Addr;
	/** \brief is the fixture currently selected */
	bool bSelected;
};

#define NUMBER_OF_FIXTURE

class FixturesModel : public QAbstractListModel
#ifdef DMX_MANAGER_CORE
, public IDmxTickCallback, public IThreadFunction
#endif
{
	Q_OBJECT
	Q_PROPERTY(ModeClass::EConsumptionMode ComsumptionMode READ GetComsumptionMode WRITE SetComsumptionMode NOTIFY ComsumptionModeChanged)
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

	enum EFixtureRoles 
	{
		ColorRole = Qt::UserRole + 1,
		AddressRole,
		DimmerRole,
	};

	void AddFixture(const Fixture & fixture);
	int rowCount(const QModelIndex & parent = QModelIndex()) const override;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
protected:
	QHash<int, QByteArray> roleNames() const override;
private:
	/** \brief Array that contain every fixture to be displayed inside the map */
	std::vector<Fixture> Fixtures;
};


#endif