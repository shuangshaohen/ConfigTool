#ifndef PARASTRUCT_H
#define PARASTRUCT_H

#include <string>
#include <vector>

using namespace std;

struct tdsItem
{
    string key;
    string desc;
    string name;

    tdsItem(string k, string d, string n)
    {
        key = k;
        desc = d;
        name = n;
    }

    void plus(string n, string d)
    {
        desc = d + desc;
        if(name != "")
            name += n;
    }
};

struct tdsPara
{
    tdsPara * parent;

    string key;
    string desc;

    vector<tdsPara *> sons;
    vector<tdsItem *> items;

    tdsPara(tdsPara * p, string k, string d)
    {
        parent = p;
        key = k;
        desc = d;
        sons.clear();
        items.clear();
    }

    void plus(string n, string d)
    {
        desc = d + desc;
        for(unsigned int i = 0; i < sons.size(); i++)
        {
            sons[i]->plus(n,d);
        }

        for(unsigned int i = 0; i < items.size(); i++)
        {
            items[i]->plus(n,d);
        }
    }

    int itemCounts()
    {
        int counts = items.size();
        for(unsigned int i = 0; i < sons.size(); i++)
        {
            counts += sons[i]->itemCounts();
        }
        return counts;
    }

    void clearName()
    {
        for(unsigned int i = 0; i < sons.size(); i++)
        {
            sons[i]->clearName();
        }

        for(unsigned int i = 0; i < items.size(); i++)
        {
            items[i]->name = "";
        }
    }
};

struct tdsTopoPara
{
    string name;
    string desc;
    string version;
    string date;

    tdsPara * ptPara;

//    vector<tdsItem *> items;

//    tdsPara * ptRlyStart;

//    tdsPara * ptSourcePara;
//    tdsPara * ptBusbarPara;
//    tdsPara * ptLinePara;
//    tdsPara * ptThransformerPara;
//    tdsPara * ptLoadPara;
//    tdsPara * ptRemotePara;

//    tdsPara * ptSwitchPara;

    tdsTopoPara()
    {
        name = "Default";
        desc = "缺省拓扑参数配置";
        version = "1.00";
        date = "0000-00-00 00:00:00:000";

        ptPara = new tdsPara(nullptr,"TopoPara","拓扑参数");

//        items.clear();

//        ptSourcePara = new tdsPara(nullptr,"SourcePara","电源拓扑参数");
//        ptBusbarPara = new tdsPara(nullptr,"BusbarPara","母线拓扑参数");
//        ptLinePara = new tdsPara(nullptr,"LinePara","线路拓扑参数");
//        ptThransformerPara = new tdsPara(nullptr,"ThransPara","变压器拓扑参数");
//        ptLoadPara = new tdsPara(nullptr,"LoadPara","负荷拓扑参数");
//        ptRemotePara = new tdsPara(nullptr,"RemotePara","远方拓扑参数");

//        ptSwitchPara = new tdsPara(nullptr,"SwitchPara","开关拓扑参数");
    }
};

#endif // PARASTRUCT_H
