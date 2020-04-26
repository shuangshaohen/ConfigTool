#ifndef CDATASQL_H
#define CDATASQL_H

#include <QObject>

class CDataBase;

class CDataSQL : public QObject
{
    Q_OBJECT
public:
    explicit CDataSQL(QObject *parent = nullptr);
    ~CDataSQL();

    void save(QString dbName, CDataBase * data);

signals:


private:
    CDataBase * m_data;

    void deletDB();
    void createDeviceInfo();
    void createDeviceParas();
    void creatMasterIndex();
    void createAnaConfig(QString tableType);
    void createBiConfig(QString tableType);
    void creatSoftYB();
    void createGooseBoConfig();
    void createTripMatrixConfig();
    void createSetConfig(QString tableType);
    void createEvtConfig(QString tableType);
};

#endif // CDATASQL_H
