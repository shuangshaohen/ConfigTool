#ifndef CBASEPARA_H
#define CBASEPARA_H

#include "ParaStruct.h"


class CBasePara
{
public:
    CBasePara();

    void   CreatePara(tdsTopoPara *   pTopoPara, int bDeviceOnly, int SNum, int MNum, int LnNum,int TNum,int LdNum,int RNum,int SWNum);

protected:

private:
    tdsTopoPara *   m_TopoPara;

    int m_bDeviceOnly;  // (0)

    int m_SoureMaxCount;  // (4)
    int m_BusbarMaxCount;  // (12)
    int m_LineMaxCount;  // (12)
    int m_TransformerMaxCount;  // (6)
    int m_LoadMaxCount;  // (36)
    int m_RemoteMaxCount;  // (4)
    int m_SwitchMaxCount;  // (48)

    tdsPara     *   CreateSource(tdsPara * parent, int index);
    tdsPara     *   CreateBusbar(tdsPara * parent, int index);
    tdsPara     *   CreateLine(tdsPara * parent, int index);
    tdsPara     *   CreateTransformer(tdsPara * parent, int index);
    tdsPara     *   CreateLoad(tdsPara * parent, int index);
    tdsPara     *   CreateRemote(tdsPara * parent, int index);

    tdsPara     *   CreateSwitch(tdsPara * parent, int index);
    tdsPara     *   CreateSWOper(tdsPara * parent);

    tdsPara     *   CreateNodeStatus(tdsPara * parent);

    tdsPara     *   CreateICalc(tdsPara * parent);
    tdsPara     *   CreateRemoteICalc(tdsPara * parent);
    tdsPara     *   CreateUCalc(tdsPara * parent);
    tdsPara     *   CreateFrCalc(tdsPara *parent);
    tdsPara     *   CreateDIFFICalc(tdsPara *parent);
    tdsPara     *   CreatePower(tdsPara * parent);

    tdsPara     *   CreateCompU(tdsPara * parent);
    tdsPara     *   CreatePTDX(tdsPara * parent);
    tdsPara     *   CreateLowU(tdsPara * parent);

    tdsPara     *   CreatePhaseDir(tdsPara * parent);
    tdsPara     *   CreateDir3I0(tdsPara * parent);

    tdsPara     *   CreatePositionAlm(tdsPara * parent);
    tdsPara     *   CreateTWJAlm(tdsPara * parent);

    tdsPara     *   CreateOverI(tdsPara * parent, int index);
    tdsPara     *   CreateOverIPTDX(tdsPara * parent);
    tdsPara     *   CreateOverZ1(tdsPara * parent);
    tdsPara     *   CreateOverZ2(tdsPara * parent);
    tdsPara     *   CreateOverLoad(tdsPara * parent);

    tdsPara     *   CreateDIFFRly(tdsPara * parent, bool bWithPermit);
    tdsPara     *   CreateCTDX(tdsPara * parent);
    tdsPara     *   CreateCDPermit(tdsPara * parent);

    tdsPara     *   CreateLimitHL(tdsPara * parent);
    tdsPara     *   CreateLimitH(tdsPara * parent);

    tdsPara     *   CreateLowI(tdsPara * parent);
};

#endif // CBASEPARA_H
