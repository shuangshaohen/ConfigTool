#include "CBasePara.h"

CBasePara::CBasePara()
{
    m_TopoPara = nullptr;
}

void CBasePara::CreatePara(tdsTopoPara *   pTopoPara,int bDeviceOnly, int SNum, int MNum, int LnNum, int TNum, int LdNum, int RNum, int SWNum)
{
    m_TopoPara = pTopoPara;
    if(m_TopoPara == NULL)
        return;

    m_bDeviceOnly = bDeviceOnly;

    m_SoureMaxCount = SNum;
    m_BusbarMaxCount = MNum;
    m_LineMaxCount = LnNum;
    m_TransformerMaxCount = TNum;
    m_LoadMaxCount = LdNum;
    m_RemoteMaxCount = RNum;
    m_SwitchMaxCount = SWNum;

    m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexTripTime","跳闸时间定值","EN_SET_TRIPTIME"));

    m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexRly","保护投退软压板","EN_SOFTYB_RLY"));
    m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexTrip","故障跳闸投退软压板","EN_SOFTYB_TRIP"));

    if(m_bDeviceOnly)
    {
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanTripTime","分段开关跳闸时间定值","EN_SET_FENDUANTRIPTIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanCloseTime","分段开关合闸时间定值","EN_SET_FENDUANCLOSETIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanCloseGDTime","分段开关合闸固定时间定值","EN_SET_FENDUANCLOSEGDTIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexLianLuoCloseTime","联络开关合闸时间定值","EN_SET_LIANLUOCLOSETIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexOFFLine","离线馈线自动化投退软压板","EN_SOFTYB_OFFLINE"));

        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexIsolateTime","隔离等待时间定值","EN_SET_ISOLATETIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexIsolate","隔离投退软压板","EN_SOFTYB_ISOLATE"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexRecovery","自愈投退软压板","EN_SOFTYB_RECOVERY"));
    }
    else
    {
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanTripTime","分段开关跳闸时间定值",""));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanCloseTime","分段开关合闸时间定值",""));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexFenduanCloseGDTime","分段开关合闸固定时间定值",""));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexLianLuoCloseTime","联络开关合闸时间定值",""));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexOFFLine","离线馈线自动化投退软压板",""));

        m_TopoPara->ptPara->items.push_back(new tdsItem("wSetIndexIsolateTime","隔离等待时间定值","EN_SET_ISOLATETIME"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexIsolate","隔离投退软压板","EN_SOFTYB_ISOLATE"));
        m_TopoPara->ptPara->items.push_back(new tdsItem("wSoftYBIndexRecovery","自愈投退软压板","EN_SOFTYB_RECOVERY"));
    }


    tdsPara * ptRlyStart = new tdsPara(m_TopoPara->ptPara,"RlyStart", "公共启动模块");
    ptRlyStart->items.push_back(new tdsItem("wActEvtTabIn","启动动作事件","EN_ACT_RLYSTART"));
    ptRlyStart->items.push_back(new tdsItem("wFlgOutIndexAct","启动动作标志","EN_SIGNAL_RLYSTART"));
    m_TopoPara->ptPara->sons.push_back(ptRlyStart);

    for(int i = 0 ;i < m_SoureMaxCount; i++)
    {
        tdsPara * ptSource = CreateSource(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptSource);
    }

    for(int i = 0 ;i < m_BusbarMaxCount; i++)
    {
        tdsPara * ptBusbar = CreateBusbar(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptBusbar);
    }

    for(int i = 0 ;i < m_LineMaxCount; i++)
    {
        tdsPara * ptLine = CreateLine(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptLine);
    }

    for(int i = 0 ;i < m_TransformerMaxCount; i++)
    {
        tdsPara * ptTransformer = CreateTransformer(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptTransformer);
    }

    for(int i = 0 ;i < m_LoadMaxCount; i++)
    {
        tdsPara * ptLoad = CreateLoad(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptLoad);
    }

    for(int i = 0 ;i < m_RemoteMaxCount; i++)
    {
        tdsPara * ptRemote = CreateRemote(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptRemote);
    }

    for(int i = 0 ;i < m_SwitchMaxCount; i++)
    {
        tdsPara * ptSwitch = CreateSwitch(m_TopoPara->ptPara, i+1);
        m_TopoPara->ptPara->sons.push_back(ptSwitch);
    }

    //return m_TopoPara;
}

tdsPara *CBasePara::CreateSource(tdsPara *parent , int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Source" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wSetIndexPn","功率定值","EN_SET_PN"));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexCompU","外部复压开放标志索引","EN_GSIN_COMPU"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引","EN_GSIN_HAVEU"));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wSetIndexPn","功率定值",""));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexCompU","外部复压开放标志索引",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引",""));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->sons.push_back(CreateNodeStatus(ptPara));
    ptPara->sons.push_back(CreateUCalc(ptPara));
    ptPara->sons.push_back(CreateCompU(ptPara));
    ptPara->sons.push_back(CreatePTDX(ptPara));
    ptPara->sons.push_back(CreateLowU(ptPara));

    tdsPara * ptLimitU = CreateLimitHL(ptPara);
    ptLimitU->key += "_U";
    ptLimitU->plus("_U","电压");
    ptPara->sons.push_back(ptLimitU);

    tdsPara * ptLimit3U0 = CreateLimitH(ptPara);
    ptLimit3U0->key += "_3U0";
    ptLimit3U0->plus("_3U0","零序电压");
    ptPara->sons.push_back(ptLimit3U0);

    ptPara->sons.push_back(CreateFrCalc(ptPara));

    tdsPara * ptLimitFr = CreateLimitHL(ptPara);
    ptLimitFr->key += "_Fr";
    ptLimitFr->plus("_Fr","频率");
    ptPara->sons.push_back(ptLimitFr);

    ptPara->sons.push_back(CreateOverI(ptPara,1));
    ptPara->sons.push_back(CreateOverI(ptPara,2));
    ptPara->sons.push_back(CreateOverI(ptPara,3));
    ptPara->sons.push_back(CreateOverIPTDX(ptPara));
    ptPara->sons.push_back(CreateOverZ1(ptPara));
    ptPara->sons.push_back(CreateOverZ2(ptPara));
    ptPara->sons.push_back(CreateOverLoad(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_S" + strIndex, "电源" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateBusbar(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Busbar" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引","EN_GSIN_HAVEU"));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引",""));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->sons.push_back(CreateNodeStatus(ptPara));
    ptPara->sons.push_back(CreateUCalc(ptPara));
    ptPara->sons.push_back(CreateCompU(ptPara));
    ptPara->sons.push_back(CreatePTDX(ptPara));
    ptPara->sons.push_back(CreateLowU(ptPara));

    tdsPara * ptLimitU = CreateLimitHL(ptPara);
    ptLimitU->key += "_U";
    ptLimitU->plus("_U","电压");
    ptPara->sons.push_back(ptLimitU);

    tdsPara * ptLimit3U0 = CreateLimitH(ptPara);
    ptLimit3U0->key += "_3U0";
    ptLimit3U0->plus("_3U0","零序电压");
    ptPara->sons.push_back(ptLimit3U0);

    tdsPara * ptDIFFICalcS = CreateDIFFICalc(ptPara);
    ptDIFFICalcS->key += "_S";
    ptDIFFICalcS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFICalcS);

    tdsPara * ptDIFFICalcB = CreateDIFFICalc(ptPara);
    ptDIFFICalcB->key += "_B";
    ptDIFFICalcB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFICalcB);

    tdsPara * ptDIFFRlyS = CreateDIFFRly(ptPara,false);
    ptDIFFRlyS->key += "_S";
    ptDIFFRlyS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFRlyS);

    tdsPara * ptDIFFRlyB = CreateDIFFRly(ptPara,false);
    ptDIFFRlyB->key += "_B";
    ptDIFFRlyB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFRlyB);

    tdsPara * ptDIFFRlyCTDX = CreateDIFFRly(ptPara,false);
    ptDIFFRlyCTDX->key += "_CTDX";
    ptDIFFRlyCTDX->plus("_CTDX","CT断线");
    ptPara->sons.push_back(ptDIFFRlyCTDX);

    ptPara->sons.push_back(CreateCTDX(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_M" + strIndex, "母线" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateLine(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Line" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->sons.push_back(CreateNodeStatus(ptPara));

    tdsPara * ptDIFFICalcS = CreateDIFFICalc(ptPara);
    ptDIFFICalcS->key += "_S";
    ptDIFFICalcS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFICalcS);

    tdsPara * ptDIFFICalcB = CreateDIFFICalc(ptPara);
    ptDIFFICalcB->key += "_B";
    ptDIFFICalcB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFICalcB);

    tdsPara * ptDIFFRlyS = CreateDIFFRly(ptPara,false);
    ptDIFFRlyS->key += "_S";
    ptDIFFRlyS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFRlyS);

    tdsPara * ptDIFFRlyB = CreateDIFFRly(ptPara,false);
    ptDIFFRlyB->key += "_B";
    ptDIFFRlyB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFRlyB);

    tdsPara * ptDIFFRlyCTDX = CreateDIFFRly(ptPara,false);
    ptDIFFRlyCTDX->key += "_CTDX";
    ptDIFFRlyCTDX->plus("_CTDX","CT断线");
    ptPara->sons.push_back(ptDIFFRlyCTDX);

    ptPara->sons.push_back(CreateCTDX(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_LN" + strIndex, "线路" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateTransformer(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Transformer" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->items.push_back(new tdsItem("wSetIndexHPTPrimary","高压侧PT一次额定值","EN_SET_HPTPRIMARY"));
    ptPara->items.push_back(new tdsItem("wSetIndexLPTPrimary","高压侧PT一次额定值","EN_SET_LPTPRIMARY"));
    ptPara->items.push_back(new tdsItem("wSetIndexCnnMode","接线方式","EN_SET_CNNMODE"));

    ptPara->sons.push_back(CreateNodeStatus(ptPara));

    tdsPara * ptDIFFICalcS = CreateDIFFICalc(ptPara);
    ptDIFFICalcS->key += "_S";
    ptDIFFICalcS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFICalcS);

    tdsPara * ptDIFFICalcB = CreateDIFFICalc(ptPara);
    ptDIFFICalcB->key += "_B";
    ptDIFFICalcB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFICalcB);

    tdsPara * ptDIFFRlyS = CreateDIFFRly(ptPara,false);
    ptDIFFRlyS->key += "_S";
    ptDIFFRlyS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFRlyS);

    tdsPara * ptDIFFRlyB = CreateDIFFRly(ptPara,false);
    ptDIFFRlyB->key += "_B";
    ptDIFFRlyB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFRlyB);

    tdsPara * ptDIFFRlyCTDX = CreateDIFFRly(ptPara,false);
    ptDIFFRlyCTDX->key += "_CTDX";
    ptDIFFRlyCTDX->plus("_CTDX","CT断线");
    ptPara->sons.push_back(ptDIFFRlyCTDX);

    ptPara->sons.push_back(CreateCTDX(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_T" + strIndex, "变压器" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateLoad(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Load" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->sons.push_back(CreateNodeStatus(ptPara));

    ptPara->sons.push_back(CreateOverI(ptPara,1));
    ptPara->sons.push_back(CreateOverI(ptPara,2));
    ptPara->sons.push_back(CreateOverI(ptPara,3));
    ptPara->sons.push_back(CreateOverIPTDX(ptPara));
    ptPara->sons.push_back(CreateOverZ1(ptPara));
    ptPara->sons.push_back(CreateOverZ2(ptPara));
    ptPara->sons.push_back(CreateOverLoad(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_LD" + strIndex, "负荷" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateRemote(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Remote" ,"节点");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    if(0 == m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexCompU","外部复压开放标志索引","EN_GSIN_COMPU"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引","EN_GSIN_HAVEU"));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志","EN_GSIN_ACT1"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志","EN_GSIN_ACT2"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志","EN_GSIN_ACT3"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志","EN_GSIN_ACT4"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志","EN_GSIN_ACT5"));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志","EN_GSIN_ACT6"));
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexCompU","外部复压开放标志索引",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveVol","外部有压标志索引",""));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct1","外部保护动作1标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct2","外部保护动作2标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct3","外部保护动作3标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct4","外部保护动作4标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct5","外部保护动作5标志",""));
        ptPara->items.push_back(new tdsItem("wFlgInBIIndexRlyAct6","外部保护动作6标志",""));
    }

    ptPara->items.push_back(new tdsItem("strFlgOutDiffPermit","差动允许标志","EN_SIGNAL_DIFFPERMIT"));

    ptPara->sons.push_back(CreateNodeStatus(ptPara));
    ptPara->sons.push_back(CreateRemoteICalc(ptPara));
    ptPara->sons.push_back(CreateUCalc(ptPara));
    ptPara->sons.push_back(CreateCompU(ptPara));
    ptPara->sons.push_back(CreatePTDX(ptPara));
    ptPara->sons.push_back(CreateLowU(ptPara));

    tdsPara * ptLimitU = CreateLimitHL(ptPara);
    ptLimitU->key += "_U";
    ptLimitU->plus("_U","电压");
    ptPara->sons.push_back(ptLimitU);

    tdsPara * ptLimit3U0 = CreateLimitH(ptPara);
    ptLimit3U0->key += "_3U0";
    ptLimit3U0->plus("_3U0","零序电压");
    ptPara->sons.push_back(ptLimit3U0);

    tdsPara * ptDIFFICalcS = CreateDIFFICalc(ptPara);
    ptDIFFICalcS->key += "_S";
    ptDIFFICalcS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFICalcS);

    tdsPara * ptDIFFICalcB = CreateDIFFICalc(ptPara);
    ptDIFFICalcB->key += "_B";
    ptDIFFICalcB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFICalcB);

    tdsPara * ptDIFFRlyS = CreateDIFFRly(ptPara,true);
    ptDIFFRlyS->key += "_S";
    ptDIFFRlyS->plus("_S","小差");
    ptPara->sons.push_back(ptDIFFRlyS);

    tdsPara * ptDIFFRlyB = CreateDIFFRly(ptPara,true);
    ptDIFFRlyB->key += "_B";
    ptDIFFRlyB->plus("_B","大差");
    ptPara->sons.push_back(ptDIFFRlyB);

    tdsPara * ptDIFFRlyCTDX = CreateDIFFRly(ptPara,true);
    ptDIFFRlyCTDX->key += "_CTDX";
    ptDIFFRlyCTDX->plus("_CTDX","CT断线");
    ptPara->sons.push_back(ptDIFFRlyCTDX);

    ptPara->sons.push_back(CreateCTDX(ptPara));

    ptPara->sons.push_back(CreateCDPermit(ptPara));

    ptPara->sons.push_back(CreateOverI(ptPara,1));
    ptPara->sons.push_back(CreateOverI(ptPara,2));
    ptPara->sons.push_back(CreateOverI(ptPara,3));
    ptPara->sons.push_back(CreateOverIPTDX(ptPara));
    ptPara->sons.push_back(CreateOverZ1(ptPara));
    ptPara->sons.push_back(CreateOverZ2(ptPara));
    ptPara->sons.push_back(CreateOverLoad(ptPara));

    string strIndex = to_string(index);
    ptPara->plus("_R" + strIndex, "远方" + strIndex);
    return ptPara;
}

tdsPara *CBasePara::CreateSwitch(tdsPara *parent, int index)
{
    tdsPara * ptPara = new tdsPara(parent,"Switch" ,"节点");

    ptPara->sons.push_back(CreateICalc(ptPara));
    ptPara->sons.push_back(CreateLowI(ptPara));
    ptPara->sons.push_back(CreatePhaseDir(ptPara));
    ptPara->sons.push_back(CreateDir3I0(ptPara));
    ptPara->sons.push_back(CreatePower(ptPara));
    ptPara->sons.push_back(CreatePositionAlm(ptPara));
    ptPara->sons.push_back(CreateTWJAlm(ptPara));

    ptPara->items.push_back(new tdsItem("wFlgOutIndexNodeValid","节点有效标志索引","EN_SIGNAL_NODEVALID"));

    tdsPara * ptTrip = CreateSWOper(ptPara);
    ptTrip->key += "_TRIP";
    ptTrip->plus("_TRIP", "故障跳闸");
    ptPara->sons.push_back(ptTrip);

    if(m_bDeviceOnly)
    {
        ptPara->items.push_back(new tdsItem("wBITWJ","跳位","EN_BI_TWJ"));
        ptPara->items.push_back(new tdsItem("wBIHWJ","合位","EN_BI_HWJ"));
        ptPara->items.push_back(new tdsItem("wGroundSWIndex","地刀","EN_BI_GROUND"));
        ptPara->items.push_back(new tdsItem("wNoEnergyIndex","未储能","EN_BI_NOENERGY"));

        ptPara->items.push_back(new tdsItem("wFlgOutRunModeFenDuan","分段开关标志","EN_SIGNAL_FENDUAN"));
        ptPara->items.push_back(new tdsItem("wFlgOutRunModeLianLuo","联络开关标志","EN_SIGNAL_LIANLUO"));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockLianLuoClose","闭锁联络开关合闸标志","EN_SIGNAL_BLOCKLIANLUOCLOSE"));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockFenDuanTrip","闭锁分段开关分闸标志","EN_SIGNAL_BLOCKFENDUANTRIP"));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockFenDuanClose","闭锁分段开关合闸标志","EN_SIGNAL_BLOCKFENDUANCLOSE"));
        ptPara->items.push_back(new tdsItem("wFlgOutFenDuanCloseGD","分段开关合闸固定标志","EN_SIGNAL_FENDUANCLOSEGD"));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveCurrent","外部有流标志",""));
        ptPara->items.push_back(new tdsItem("wAnaInPower","外部采集功率",""));
        ptPara->items.push_back(new tdsItem("wSetIndexPowerPositive","外部采集功率极性定值",""));

        tdsPara * ptFenDuanTrip = CreateSWOper(ptPara);
        ptFenDuanTrip->key += "_FenDuanTrip";
        ptFenDuanTrip->plus("_FENDUANTRIP", "分段开关跳闸");
        ptPara->sons.push_back(ptFenDuanTrip);

        tdsPara * ptFenDuanClose = CreateSWOper(ptPara);
        ptFenDuanClose->key += "_FenDuanClose";
        ptFenDuanClose->plus("_FENDUANCLOSE", "分段开关合闸");
        ptPara->sons.push_back(ptFenDuanClose);

        tdsPara * ptLianLuoClose = CreateSWOper(ptPara);
        ptLianLuoClose->key += "_LianLuoClose";
        ptLianLuoClose->plus("_LIANLUOCLOSE", "联络开关合闸");
        ptPara->sons.push_back(ptLianLuoClose);

        tdsPara * ptIsolate = CreateSWOper(ptPara);
        ptLianLuoClose->key += "_LianLuoClose";
        ptLianLuoClose->plus("_LIANLUOCLOSE", "联络开关合闸");
        ptIsolate->clearName();
        ptPara->sons.push_back(ptIsolate);

        tdsPara * ptDiscnn = CreateSWOper(ptPara);
        ptDiscnn->key += "_Discnn";
        ptDiscnn->plus("_DISCNN", "自愈分闸");
        ptDiscnn->clearName();
        ptPara->sons.push_back(ptDiscnn);

        tdsPara * ptClose = CreateSWOper(ptPara);
        ptClose->key += "_Close";
        ptClose->plus("_CLOSE", "自愈合闸");
        ptClose->clearName();
        ptPara->sons.push_back(ptClose);

        string strIndex = to_string(index);
        ptPara->plus("_SW" + strIndex, "开关" + strIndex);

        ptIsolate->clearName();
        ptDiscnn->clearName();
        ptClose->clearName();
    }
    else
    {
        ptPara->items.push_back(new tdsItem("wBITWJ","跳位","EN_GSIN_TWJ"));
        ptPara->items.push_back(new tdsItem("wBIHWJ","合位","EN_GSIN_HWJ"));
        ptPara->items.push_back(new tdsItem("wGroundSWIndex","地刀","EN_GSIN_GROUND"));
        ptPara->items.push_back(new tdsItem("wNoEnergyIndex","未储能","EN_GSIN_NOENERGY"));

        ptPara->items.push_back(new tdsItem("wFlgOutRunModeFenDuan","分段开关标志",""));
        ptPara->items.push_back(new tdsItem("wFlgOutRunModeLianLuo","联络开关标志",""));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockLianLuoClose","闭锁联络开关合闸标志",""));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockFenDuanTrip","闭锁分段开关分闸标志",""));
        ptPara->items.push_back(new tdsItem("wFlgOutBlockFenDuanClose","闭锁分段开关合闸标志",""));
        ptPara->items.push_back(new tdsItem("wFlgOutFenDuanCloseGD","分段开关合闸固定标志",""));

        ptPara->items.push_back(new tdsItem("wFlgInBIIndexHaveCurrent","外部有流标志","EN_GSIN_HAVEI"));
        ptPara->items.push_back(new tdsItem("wAnaInPower","外部采集功率","EN_GSANA_POWER"));
        ptPara->items.push_back(new tdsItem("wSetIndexPowerPositive","外部采集功率极性定值","EN_SET_POWERPOSITIVE"));

        tdsPara * ptFenDuanTrip = CreateSWOper(ptPara);
        ptFenDuanTrip->key += "_FenDuanTrip";
        ptFenDuanTrip->plus("_FENDUANTRIP", "分段开关跳闸");
        ptPara->sons.push_back(ptFenDuanTrip);

        tdsPara * ptFenDuanClose = CreateSWOper(ptPara);
        ptFenDuanClose->key += "_FenDuanClose";
        ptFenDuanClose->plus("_FENDUANCLOSE", "分段开关合闸");
        ptPara->sons.push_back(ptFenDuanClose);

        tdsPara * ptLianLuoClose = CreateSWOper(ptPara);
        ptLianLuoClose->key += "_LianLuoClose";
        ptLianLuoClose->plus("_LIANLUOCLOSE", "联络开关合闸");
        ptPara->sons.push_back(ptLianLuoClose);

        tdsPara * ptIsolate = CreateSWOper(ptPara);
        ptIsolate->key += "_Isolate";
        ptIsolate->plus("_ISOLATE", "隔离跳闸");
        ptPara->sons.push_back(ptIsolate);

        tdsPara * ptDiscnn = CreateSWOper(ptPara);
        ptDiscnn->key += "_Discnn";
        ptDiscnn->plus("_DISCNN", "自愈分闸");
        ptPara->sons.push_back(ptDiscnn);

        tdsPara * ptClose = CreateSWOper(ptPara);
        ptClose->key += "_Close";
        ptClose->plus("_CLOSE", "自愈合闸");
        ptPara->sons.push_back(ptClose);

        string strIndex = to_string(index);
        ptPara->plus("_SW" + strIndex, "开关" + strIndex);

        ptFenDuanTrip->clearName();
        ptFenDuanClose->clearName();
        ptLianLuoClose->clearName();
    }

    return ptPara;
}

tdsPara *CBasePara::CreateSWOper(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"SwitchOper","操作");

    ptPara->items.push_back(new tdsItem("wEvtTabIndex","动作事件","EN_ACT_OPER"));
    ptPara->items.push_back(new tdsItem("wAlmTabIndex","失败告警事件","EN_ALM_FAIL"));
    ptPara->items.push_back(new tdsItem("wFlgOutOper","动作标志","EN_SIGNAL_OPER"));
    ptPara->items.push_back(new tdsItem("wFlgOutOperFail","失败标志","EN_SIGNAL_FAIL"));
    return ptPara;
}

tdsPara *CBasePara::CreateNodeStatus(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"Status","节点状态");

    ptPara->items.push_back(new tdsItem("WFlgOutIndexRunON","运行态","EN_SIGNAL_RUNON"));
    ptPara->items.push_back(new tdsItem("WFlgOutIndexRunOFF","待自愈态","EN_SIGNAL_RUNOFF"));
    ptPara->items.push_back(new tdsItem("WFlgOutIndexTrip","跳闸态","EN_SIGNAL_TRIP"));
    ptPara->items.push_back(new tdsItem("WFlgOutIndexFault","故障态","EN_SIGNAL_FAULT"));
    ptPara->items.push_back(new tdsItem("wFaultEvtTabIndex","节点故障事件索引","EN_ACT_NODEFAULT"));
    return ptPara;
}

tdsPara *CBasePara::CreateICalc(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"ICalc","电流计算通道");

    ptPara->items.push_back(new tdsItem("wDstIndexIa","电流计算通道Ia","EN_DRV_IA"));
    ptPara->items.push_back(new tdsItem("wDstIndexIb","电流计算通道Ib","EN_DRV_IB"));
    ptPara->items.push_back(new tdsItem("wDstIndexIc","电流计算通道Ic","EN_DRV_IC"));
    ptPara->items.push_back(new tdsItem("wDstIndex3I0","电流计算通道3I0","EN_DRV_3I0"));
    return ptPara;
}

tdsPara *CBasePara::CreateRemoteICalc(tdsPara *parent)
{
    tdsPara * ptRICalc = new tdsPara(parent,"RICalc","远方节点电流计算通道");

    tdsPara * ptParaM = new tdsPara(ptRICalc,"IMCalc","电流计算通道");
    ptParaM->items.push_back(new tdsItem("wDstIndexIa","M侧电流计算通道Ia","EN_DRV_IMA"));
    ptParaM->items.push_back(new tdsItem("wDstIndexIb","M侧电流计算通道Ib","EN_DRV_IMB"));
    ptParaM->items.push_back(new tdsItem("wDstIndexIc","M侧电流计算通道Ic","EN_DRV_IMC"));
    ptParaM->items.push_back(new tdsItem("wDstIndex3I0","M侧电流计算通道3I0","EN_DRV_3IM0"));
    ptRICalc->sons.push_back(ptParaM);

    tdsPara * ptParaN = new tdsPara(ptRICalc,"INCalc","电流计算通道");
    ptParaN->items.push_back(new tdsItem("wDstIndexIa","N侧电流计算通道Ia","EN_DRV_INA"));
    ptParaN->items.push_back(new tdsItem("wDstIndexIb","N侧电流计算通道Ib","EN_DRV_INB"));
    ptParaN->items.push_back(new tdsItem("wDstIndexIc","N侧电流计算通道Ic","EN_DRV_INC"));
    ptParaN->items.push_back(new tdsItem("wDstIndex3I0","N侧电流计算通道3I0","EN_DRV_3IN0"));
    ptRICalc->sons.push_back(ptParaN);

    return ptRICalc;
}

tdsPara *CBasePara::CreateUCalc(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"UCalc","电压计算通道");

    ptPara->items.push_back(new tdsItem("wDstIndexUa","电压计算通道Ua","EN_DRV_UA"));
    ptPara->items.push_back(new tdsItem("wDstIndexUb","电压计算通道Ub","EN_DRV_UB"));
    ptPara->items.push_back(new tdsItem("wDstIndexUc","电压计算通道Uc","EN_DRV_UC"));
    ptPara->items.push_back(new tdsItem("wDstIndexUab","电压计算通道Uab","EN_DRV_UAB"));
    ptPara->items.push_back(new tdsItem("wDstIndexUbc","电压计算通道Ubc","EN_DRV_UBC"));
    ptPara->items.push_back(new tdsItem("wDstIndexUca","电压计算通道Uca","EN_DRV_UCA"));
    ptPara->items.push_back(new tdsItem("wDstIndexU1","电压计算通道U1","EN_DRV_U1"));
    ptPara->items.push_back(new tdsItem("wDstIndexU2","电压计算通道U2","EN_DRV_U2"));
    ptPara->items.push_back(new tdsItem("wDstIndex3I0","电压计算通道3U0","EN_DRV_3U0"));
    return ptPara;
}

tdsPara *CBasePara::CreateFrCalc(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"FrCalc","频率计算通道");

    ptPara->items.push_back(new tdsItem("wIndexFr","频率计算通道Fr","EN_DRV_FR"));
    return ptPara;
}

tdsPara *CBasePara::CreateDIFFICalc(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"DIFFICalc","差流计算通道");

    tdsPara * ptPhaseA = new tdsPara(ptPara,"PhaseA","A相差流计算通道");
    ptPhaseA->items.push_back(new tdsItem("wIndexId","A相差流计算通道Ida","EN_DRV_IDA"));
    ptPhaseA->items.push_back(new tdsItem("wIndexIr","A相制动电流计算通道Ira","EN_DRV_IRA"));
    ptPhaseA->items.push_back(new tdsItem("wIndexId_2D","A相差流二次谐波计算通道Ida2D","EN_DRV_IDA2D"));
    ptPhaseA->items.push_back(new tdsItem("wIndexId_3D","A相差流三次谐波计算通道Ida3D","EN_DRV_IDA3D"));
    ptPara->sons.push_back(ptPhaseA);

    tdsPara * ptPhaseB = new tdsPara(ptPara,"PhaseB","B相差流计算通道");
    ptPhaseB->items.push_back(new tdsItem("wIndexId","B相差流计算通道Idb","EN_DRV_IDB"));
    ptPhaseB->items.push_back(new tdsItem("wIndexIr","B相制动电流计算通道Irb","EN_DRV_IRB"));
    ptPhaseB->items.push_back(new tdsItem("wIndexId_2D","B相差流二次谐波计算通道Idb2D","EN_DRV_IDB2D"));
    ptPhaseB->items.push_back(new tdsItem("wIndexId_3D","B相差流三次谐波计算通道Idb3D","EN_DRV_IDB3D"));
    ptPara->sons.push_back(ptPhaseB);

    tdsPara * ptPhaseC = new tdsPara(ptPara,"PhaseC","C相差流计算通道");
    ptPhaseC->items.push_back(new tdsItem("wIndexId","C相差流计算通道Idc","EN_DRV_IDC"));
    ptPhaseC->items.push_back(new tdsItem("wIndexIr","C相制动电流计算通道Irc","EN_DRV_IRC"));
    ptPhaseC->items.push_back(new tdsItem("wIndexId_2D","C相差流二次谐波计算通道Idc2D","EN_DRV_IDC2D"));
    ptPhaseC->items.push_back(new tdsItem("wIndexId_3D","C相差流三次谐波计算通道Idc3D","EN_DRV_IDC3D"));
    ptPara->sons.push_back(ptPhaseC);

    tdsPara * ptPhaseZ = new tdsPara(ptPara,"PhaseZ","零序差流计算通道");
    ptPhaseZ->items.push_back(new tdsItem("wIndexId","零序差流计算通道Ida","EN_DRV_3ID0"));
    ptPhaseZ->items.push_back(new tdsItem("wIndexIr","零序制动电流计算通道Ira","EN_DRV_3IR0"));
    ptPhaseZ->items.push_back(new tdsItem("wIndexId_2D","零序差流二次谐波计算通道Ida2D","EN_DRV_3ID0_2D"));
    ptPhaseZ->items.push_back(new tdsItem("wIndexId_3D","零序差流三次谐波计算通道Ida3D","EN_DRV_3ID0_3D"));
    ptPara->sons.push_back(ptPhaseZ);

    return ptPara;
}

tdsPara *CBasePara::CreatePower(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"Power","功率计算");

    ptPara->items.push_back(new tdsItem("wIndexP","功率计算通道","EN_DRV_P"));

    return ptPara;
}

tdsPara *CBasePara::CreateCompU(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"CompU","复压开放逻辑");

    ptPara->items.push_back(new tdsItem("wSetIndexUl","复压开放低压定值","EN_SET_UL"));
    ptPara->items.push_back(new tdsItem("wSetIndexU2","复压开放负序定值","EN_SET_U2"));
    ptPara->items.push_back(new tdsItem("wSetIndex3U0","复压开放零序定值","EN_SET_3U0"));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexCompU","复压开放标标志","EN_SIGNAL_COMPU"));
    return ptPara;
}

tdsPara *CBasePara::CreatePTDX(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"PTDX","PT断线逻辑");

    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex","PT断线告警事件","EN_ALM_PTDX"));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexPTDX","PT断线标志","EN_SIGNAL_PTDX"));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexPTDXBS","PT断线闭锁保护标志","EN_SIGNAL_PTDXBS"));

    return ptPara;
}

tdsPara *CBasePara::CreateLowU(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"LowU","有压无压判断逻辑");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexHaveU","有压标志","EN_SIGNAL_HAVEU"));

    return ptPara;
}

tdsPara *CBasePara::CreatePhaseDir(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"PhaseDir","功率方向计算逻辑");

    ptPara->items.push_back(new tdsItem("wFlgOutPhaseADirP","A相正方向标志","EN_SIGNAL_PHASEADIRP"));
    ptPara->items.push_back(new tdsItem("wFlgOutPhaseADirN","A相反方向标志","EN_SIGNAL_PHASEADIRN"));

    ptPara->items.push_back(new tdsItem("wFlgOutPhaseBDirP","B相正方向标志","EN_SIGNAL_PHASEBDIRP"));
    ptPara->items.push_back(new tdsItem("wFlgOutPhaseBDirN","B相反方向标志","EN_SIGNAL_PHASEBDIRN"));

    ptPara->items.push_back(new tdsItem("wFlgOutPhaseCDirP","C相正方向标志","EN_SIGNAL_PHASECDIRP"));
    ptPara->items.push_back(new tdsItem("wFlgOutPhaseCDirN","C相反方向标志","EN_SIGNAL_PHASECDIRN"));

    return ptPara;
}

tdsPara *CBasePara::CreateDir3I0(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"Dir3I0","零序功率方向计算逻辑");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexDirP","零序正方向标志","EN_SIGNAL_DIR3I0P"));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexDirN","零序反方向标志","EN_SIGNAL_DIR3I0N"));

    return ptPara;
}

tdsPara *CBasePara::CreatePositionAlm(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"Position","位置采集判断逻辑");

    ptPara->items.push_back(new tdsItem("wBITWJ","跳位","EN_BI_TWJ"));
    ptPara->items.push_back(new tdsItem("wBIHWJ","合位","EN_BI_HWJ"));

    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex" ,"位置异常告警事件"     ,   "EN_ALM_POSITION" ));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexAlm","位置异常标志"     ,   "EN_SIGNAL_POSITIONALM"));

    return ptPara;
}

tdsPara *CBasePara::CreateTWJAlm(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"TWJAlm","TWJ异常判断逻辑");

    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex" ,"TWJ异常告警事件"     ,   "EN_ALM_TWJABNORMAL" ));
    ptPara->items.push_back(new tdsItem("wFlgOutIndexAlm","TWJ异常标志"     ,   "EN_SIGNAL_TWJABNORMAL"));

    return ptPara;
}

tdsPara *CBasePara::CreateOverI(tdsPara *parent, int index)
{
    string strName = to_string(index);
    string strDesc = "";
    if(1 == index)
        strDesc = "I段";
    else if(2 == index)
        strDesc = "II段";
    else if(3 == index)
        strDesc = "III段";

    tdsPara * ptPara = new tdsPara(parent,"OverI"+strName,"过流" + strDesc + "保护");

    ptPara->items.push_back(new tdsItem("wSetIndexCtl"      ,"过流保护"   + strDesc + "控制字"     ,"EN_SET_CTL_OVERCURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wSetIndexI"        ,"过流保护"   + strDesc + "电流定值"     ,   "EN_SET_I_OVERCURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wSetIndexTime"     ,"过流保护"   + strDesc + "时间定值"     ,   "EN_SET_T_OVERCURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wSetIndexCtlCompU" ,"过流保护"   + strDesc + "复压投退定值"   ,   "EN_SET_CTL_COMPU_OVERCURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wSetDirCtl"        ,"过流保护"   + strDesc + "方向投退定值"   ,   "EN_SET_CTL_DIR_OVERCURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wRlyEventTabIndex" ,"过流保护"   + strDesc + "动作事件"     ,   "EN_ACT_CURRENT" + strName));
    ptPara->items.push_back(new tdsItem("wFlgOutOverCurrent","过流保护"   + strDesc + "动作标志"     ,   "EN_SIGNAL_CURRENT" + strName));

    return ptPara;
}

tdsPara *CBasePara::CreateOverIPTDX(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"OverIPTDX","PT断线过流保护");

    ptPara->items.push_back(new tdsItem("wSetIndexI"        ,"PT断线过流保护电流定值"     ,   "EN_SET_I_OVERCURRENT_PTDX" ));
    ptPara->items.push_back(new tdsItem("wSetIndexTime"     ,"PT断线过流保护时间定值"     ,   "EN_SET_T_OVERCURRENT_PTDX" ));
    ptPara->items.push_back(new tdsItem("wRlyEventTabIndex" ,"PT断线过流保护动作事件"     ,   "EN_ACT_CURRENT_PTDX" ));
    ptPara->items.push_back(new tdsItem("wFlgOutOverCurrent","PT断线过流保护动作标志"     ,   "EN_SIGNAL_CURRENT_PTDX"));

    return ptPara;
}

tdsPara *CBasePara::CreateOverZ1(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"OverZ1","零序过流保护I段");

    ptPara->items.push_back(new tdsItem("wSetIndexCtl"        ,"零序过流保护I段控制字"     ,   "EN_SET_CTL_OVERZ1" ));
    ptPara->items.push_back(new tdsItem("wSetIndex3I0"        ,"零序过流保护I段电流定值"     ,   "EN_SET_I_OVERZ1" ));
    ptPara->items.push_back(new tdsItem("wSetIndexTime"     ,"零序过流保护I段时间定值"     ,   "EN_SET_T_OVERZ1" ));
    ptPara->items.push_back(new tdsItem("wSetDirCtl"        ,"零序过流保护I段方向投退定值"     ,   "EN_SET_CTL_DIR_OVERZ1" ));
    ptPara->items.push_back(new tdsItem("wRlyEventTabIndex" ,"零序过流保护I段动作事件"     ,   "EN_ACT_OVERZ1" ));
    ptPara->items.push_back(new tdsItem("wFlgOutOverCurrent","零序过流保护I段动作标志"     ,   "EN_SIGNAL_OVERZ1"));

    return ptPara;
}

tdsPara *CBasePara::CreateOverZ2(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"OverZ2","零序过流保护II段");

    ptPara->items.push_back(new tdsItem("wSetIndexCtl"        ,"零序过流保护II段控制字"     ,   "EN_SET_CTL_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wSetIndexCtlAlarm"   ,"零序过流保护II段告警控制字"     ,   "EN_SET_CTL_OVERZ2ALM" ));
    ptPara->items.push_back(new tdsItem("wSetIndex3I0"        ,"零序过流保护II段电流定值"     ,   "EN_SET_I_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wSetIndexTime"     ,"零序过流保护II段时间定值"     ,   "EN_SET_T_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wSetDirCtl"        ,"零序过流保护II段方向投退定值"     ,   "EN_SET_CTL_DIR_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wRlyEventTabIndex" ,"零序过流保护II段动作事件"     ,   "EN_ACT_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wAlarmEventTabIndex" ,"零序过流保护II段告警事件"     ,   "EN_ALM_OVERZ2" ));
    ptPara->items.push_back(new tdsItem("wFlgOutOverCurrent","零序过流保护II段动作标志"     ,   "EN_SIGNAL_OVERZ2"));

    return ptPara;
}

tdsPara *CBasePara::CreateOverLoad(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"OverLoad","过负荷告警");

    ptPara->items.push_back(new tdsItem("wSetIndexCtl"      ,"过负荷告警控制字"     ,   "EN_SET_CTL_OVERLOAD" ));
    ptPara->items.push_back(new tdsItem("wSetIndexI"        ,"过负荷告警电流定值"     ,   "EN_SET_I_OVERLOAD" ));
    ptPara->items.push_back(new tdsItem("wSetIndexTime"     ,"过负荷告警时间定值"     ,   "EN_SET_T_OVERLOAD" ));
    ptPara->items.push_back(new tdsItem("wRlyEventTabIndex" ,"过负荷告警事件"     ,   "EN_ALM_OVERLOAD" ));
    ptPara->items.push_back(new tdsItem("wFlgOutOverCurrent","过负荷告警标志"     ,   "EN_SIGNAL_OVERLOAD"));

    return ptPara;
}

tdsPara *CBasePara::CreateDIFFRly(tdsPara *parent , bool bWithPermit)
{
    tdsPara * ptPara = new tdsPara(parent,"DIFFRly","差动保护");

    if(bWithPermit)
    {
        ptPara->items.push_back(new tdsItem("wIndexMPermit"       ,"差动保护M侧外部允许信号"     ,   "EN_GSIN_M_PERMINT" ));
        ptPara->items.push_back(new tdsItem("wIndexNPermit"       ,"差动保护N侧外部允许信号"     ,   "EN_GSIN_N_PERMINT" ));
    }

    ptPara->items.push_back(new tdsItem("wSetIndexId"       ,"差动保护电流定值"     ,   "EN_SET_ID" ));
    ptPara->items.push_back(new tdsItem("wSetIndex3Id0"     ,"零序差动保护电流定值"     ,   "EN_SET_I3D0" ));
    ptPara->items.push_back(new tdsItem("wSetIndexCtl"      ,"差动保护控制字"     ,   "EN_SET_CTL_ID" ));
    ptPara->items.push_back(new tdsItem("wSetIndexCtlZ"     ,"零序差动保护控制值"     ,   "EN_SET_CTL_I3D0" ));
    ptPara->items.push_back(new tdsItem("wRlyIndexDiff"     ,"差动保护动作事件"     ,   "EN_ACT_DIFF"));
    ptPara->items.push_back(new tdsItem("wRlyIndexDiffZ"     ,"零序差动保护动作事件"     ,   "EN_ACT_DIFFZ"));

    ptPara->items.push_back(new tdsItem("wFlgOutDiffAct"     ,"差动保护标志"     ,   "EN_SIGNAL_DIFF"));
    ptPara->items.push_back(new tdsItem("wFlgOutDiffZAct"    ,"零序差动保护标志"     ,   "EN_SIGNAL_DIFFZ"));
    ptPara->items.push_back(new tdsItem("wFlgOutTASatury"    ,"CT饱和标志"     ,   "EN_SIGNAL_TASATURY"));
    ptPara->items.push_back(new tdsItem("wFlgOutTASaturyZ"   ,"零序CT饱和标志"     ,   "EN_SIGNAL_TASATURYZ"));

    return ptPara;
}

tdsPara *CBasePara::CreateCTDX(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"CTDX","CT断线告警逻辑");

    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex","CT断线告警事件","EN_ALM_CTDX"));
    ptPara->items.push_back(new tdsItem("wFlgOutCTBrokenAlm","CT断线告警信号","EN_SIGNAL_CTDX"));

    return ptPara;
}

tdsPara *CBasePara::CreateCDPermit(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"CDPermit","差动保护允许信号");

    ptPara->items.push_back(new tdsItem("wFlgOutDiffPermit","差动保护允许信号","EN_SIGNAL_PERMIT"));

    return ptPara;
}

tdsPara *CBasePara::CreateLimitHL(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"LimitHL","越上下限告警逻辑");

    ptPara->items.push_back(new tdsItem("wSetIndexL","越下限定值","EN_SET_MIN"));
    ptPara->items.push_back(new tdsItem("wSetIndexH","越上限定值","EN_SET_MAX"));
    ptPara->items.push_back(new tdsItem("wSetIndexCtl","越限控制字","EN_SET_CTL_LIMIT"));
    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex","越限告警标志","EN_ALM_OVERLIMIT"));

    return ptPara;
}

tdsPara *CBasePara::CreateLimitH(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"LimitH","越上限告警逻辑");

    ptPara->items.push_back(new tdsItem("wSetIndexH","越上限定值","EN_SET_MAX"));
    ptPara->items.push_back(new tdsItem("wSetIndexCtl","越限控制字","EN_SET_CTL_LIMIT"));
    ptPara->items.push_back(new tdsItem("wAlmEvtTabIndex","越限告警标志","EN_ALM_OVERLIMIT"));

    return ptPara;
}

tdsPara *CBasePara::CreateLowI(tdsPara *parent)
{
    tdsPara * ptPara = new tdsPara(parent,"LowI","有流无流判断逻辑");

    ptPara->items.push_back(new tdsItem("wFlgOutIndexHaveI","有流标志","EN_SIGNAL_HAVEI"));

    return ptPara;
}
