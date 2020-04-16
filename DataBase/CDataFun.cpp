#include "CDataFun.h"

CDataFun::CDataFun(QObject *parent) : QObject(parent)
{

}

CDataFun::~CDataFun()
{

}

QString CDataFun::ChangeDECToHEX(unsigned int num)
{
    if(num == 0)
        return "0";

    QString s = "0x" + QString::number(num,16).toUpper();
    return s;
}

QString CDataFun::ChangeDecToDec(unsigned int num)
{
    if(num == 0xFFFF)
        return "0xFFFF";
    else
        return QString::number(num);
}

QString CDataFun::ChangeShortToDec(unsigned int num)
{
    if(num != 255)
        return QString::number(num);
    else
        return "0xFF";
}

unsigned int CDataFun::ChangeQStringToUInt(QString str)
{
    unsigned int num;
    bool ok = false;
    if(str.startsWith("0x"))
        num = str.toUInt(&ok,16);
    else
        num = str.toUInt(&ok,10);

    if(ok)
        return num;
    else
        return 65535;
}

QString CDataFun::GetQStringfromAnaType(int type)
{
    QStringList strList;
    strList << "未定义" << "电流" << "电压" << "站用变电压" << "有功功率" << "无功功率" << "视在功率" << "功率因素"
            << "直流电压" << "直流电流" << "档位" << "频率" << "温度" << "直流" << "脉冲" << "振幅" << "设备态"
            << "阻抗" << "故障电压" << "故障电流" << "零序电流" << "差动电流" << "故障测距" << "零序差压";

    if(type >= strList.size())
        return "ERROR";
    else
        return strList[type];
}

QString CDataFun::GetQStringfromAnaPhase(int phs)
{
    QStringList strList;
    strList << "N" << "A" << "B" << "C";

    if(phs >= strList.size())
        return "N";
    else
        return strList[phs];
}

QString CDataFun::GetQStringfromBIType(int type)
{
    QStringList strList;
    strList << "A类遥信" << "B类遥信" << "C类遥信" << "未定义" ;

    if(type >= strList.size())
        return "未定义";
    else
        return strList[type];
}

QString CDataFun::GetQStringfromGSBIAttr(unsigned int attr)
{
    if(attr == 0x1)
        return "单点";
    else if(attr == 0x2)
        return "双点";
    else if(attr == 0x4)
        return "浮点";
    else if(attr == 0x8)
        return "整型";
    else
        return  "ERROR";
}

QString CDataFun::GetQStringfromSetType(int type)
{
    QStringList strList;
    strList << "无类型" << "控制字" << "电流型" << "电压型" << "阻抗型" << "角度型" << "时间型" << "计算型"
            << "PTCT型" << "PT型" << "CT型" << "CT1次额定值" << "CT2次额定值" << "比率系数" << "电压滑差" << "字符串型" << "定值区号"
            << "系统参数" << "长度型" << "频率型" << "频率滑差型" << "出口定值" << "距离保护用角度" << "不受CT二次值影响的电流型";

    if(type >= strList.size())
        return "ERROR";
    else
        return strList[type];
}

QString CDataFun::GetQStringfromSetGroup(int group)
{
    QStringList strList;
    strList << "备用侧" << "基本信息" << "装置参数" << "内部定值" << "出口矩阵" << "全侧" << "高压侧" << "中压侧1"
            << "中压侧2" << "低压侧1" << "低压侧2" << "电抗器" << "Z变" << "自定义";

    if(group >= strList.size())
        return "ERROR";
    else
        return strList[group];
}

bool CDataFun::CheckBINameExist(QString name, GseConfig *p)
{
    for(int i = 0 ; i  < p->biConfig.size(); i++)
    {
        if(name == p->biConfig[i]->sName)
        {
            return true;
        }
    }
    return false;
}

bool CDataFun::CheckAnaNameExist(QString name, GseConfig *p)
{
    for(int i = 0 ; i  < p->anaConfig.size(); i++)
    {
        if(name == p->anaConfig[i]->sName)
        {
            return true;
        }
    }
    return false;
}

bool CDataFun::CheckTZJZNameExist(QString name, GseConfig *p)
{
    for(int i = 0 ; i  < p->tripMatrix.size(); i++)
    {
        if(name == p->tripMatrix[i]->sName)
        {
            return true;
        }
    }
    return false;
}

void CDataFun::moveRow(QTableWidget *pTable, int fromRow, int toRow)
{
    if( pTable == NULL )
        return;

    pTable->setFocus();

    if( fromRow == toRow )
        return;

    if( fromRow < 0 || toRow < 0 )
        return;

    int nRowCount = pTable->rowCount();

    if( fromRow >= nRowCount  || toRow > nRowCount )
        return;

    if( toRow < fromRow )
        fromRow++;

    pTable->insertRow( toRow );
    int nCol = pTable->columnCount();

    for( int i=0; i<nCol; i++ )
    {
        pTable->setItem( toRow, i, pTable->takeItem( fromRow , i ) );
    }

    if( fromRow < toRow  )
        toRow--;

    pTable->removeRow( fromRow );
    pTable->selectRow( toRow );
}

CDataFun *CDataFun::GetInstance()
{
    static  CDataFun *_pInstance = NULL;
    if( NULL==_pInstance)
    {
        _pInstance = new CDataFun();

    }
    return _pInstance;
}
