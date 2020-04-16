#ifndef CDATAFUN_H
#define CDATAFUN_H

#include <QObject>
#include <QTableWidget>
#include "DataBase_global.h"
#include "GSEConfig.h"

class DATABASE_EXPORT CDataFun : public QObject
{
    Q_OBJECT
public:
    static CDataFun * GetInstance();
    ~CDataFun();

    QString ChangeDECToHEX(unsigned int num);

    QString ChangeDecToDec(unsigned int num);

    QString ChangeShortToDec(unsigned int num);

    unsigned int ChangeQStringToUInt(QString str);

    QString GetQStringfromAnaType(int type);

    QString GetQStringfromAnaPhase(int phs);

    QString GetQStringfromBIType(int type);

    QString GetQStringfromGSBIAttr(unsigned int attr);

    QString GetQStringfromSetType(int type);
    QString GetQStringfromSetGroup(int group);

    bool CheckBINameExist(QString name, GseConfig * p);

    bool CheckAnaNameExist(QString name, GseConfig * p);

    bool CheckTZJZNameExist(QString name, GseConfig * p);

    void moveRow(QTableWidget *pTable, int fromRow, int toRow);
private:
    CDataFun(QObject *parent = nullptr);


};

#endif // CDATAFUN_H
