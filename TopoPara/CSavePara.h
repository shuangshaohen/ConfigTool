#ifndef CSAVEPARA_H
#define CSAVEPARA_H

#include "ParaStruct.h"
#include <QDomDocument>

class CSavePara
{
public:
    CSavePara();

    void Save(const QString &fileName, tdsTopoPara * ptTopoPara);

private:
    QDomDocument    m_doc;  //QDomDocument类

    void SavePara(QDomElement * parentDom, tdsPara * ptPara);
    void SaveItem(QDomElement * parentDom, tdsItem * ptItem);
};

#endif // CSAVEPARA_H
