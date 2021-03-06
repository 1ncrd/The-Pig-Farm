#ifndef PIGSTYMANAGER_HPP
#define PIGSTYMANAGER_HPP

#include <QObject>
#include "PigSty.hpp"
#include "QuarantinePigSty.hpp"

class PigStyManager : public QObject
{

    Q_OBJECT
public:
    static const int PigStyAmount = 100;
    static const int PigStyPerRow;
    static const int PigStyPerColumn;
    static const QString QuarantineStyID;
    static const int QuarantineStyID_int;
    static QString ArchiveName;
private:
    PigSty * pig_sty[PigStyAmount];
    QuarantinePigSty * quarantine_sty;
    float InfectionPosibility;
public:
    PigStyManager(QObject * parent = nullptr);
    ~PigStyManager();
    QString GetID(const int &sty_num);
    void GetStyData(const int &sty_num);
    void GetQuarantineStyData();
    void StartTheFarm();
    void StartTheFarm(const FileManager::GameData &game_data);
    void ConfigueFunc();
    QVector<Pig::PigInfo> GetAllPigData();
    void InfectionSpreadAcrossSty();
    void SetInfectionPosibility(const float &posibility);
    void DisposeQuarantineSty();
    void SendInfectionInfo();
    void SetArchiveName(const QString &name);
    void CountPigAmount();
    int CalculateTimeToDieout();
    int * GetStyAround(int i, int sty_to_infect[]);
signals:
    InfectionOccur();
    SellPigFinished();
    SendPigAmount(PigAmount amount);
    SendSaleMoney(int sale_money);
    SendTimeToDieOut(int days);
    SendInfectedAmount(int amount);
    SendStyData(QVector<Pig::PigInfo> sty_data);
    SendQuarantineStyData(QVector<Pig::PigInfo> sty_data);
    StyIsInfected(int sty_num, bool is_infected);
    InfectionExists(bool infecion_exists);
    QuarantineAllInfectedPig();
};
extern PigStyManager * pig_sty_manager;
#endif // PIGSTYMANAGER_HPP
