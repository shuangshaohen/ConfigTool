#include "CParsePara.h"

const char* STR_TOPOINFO 	= "TopoInfo";   //参数信息;
const char* STR_PARA 		= "Para";       //参数块;
const char*	STR_ITEM        = "Item";       //条目;

const char* STR_KEY         = "key";        //关键字;
const char* STR_NAME 		= "name";       //名称;
const char*	STR_DESC        = "desc";       //描述;
const char* STR_VERSION     = "version";    //版本;
const char*	STR_DATE        = "date";       //日期;

CParsePara::~CParsePara()
{

}

bool CParsePara::ParsePara(const string &paraPath, tdsTopoPara *para)
{
    if (paraPath.empty())
        return false;

    m_Para = para;
    //释放Para;
    ReleasePara();

    char topoPath[256] = {0};
    std::strcpy(topoPath, paraPath.c_str());
    m_xmlret = m_doc.load_file(paraPath.c_str());
    if (m_xmlret.status != 0 )
    {
        printf("load para file error[%s][%d],!\n", topoPath, m_xmlret.offset);
        return false;
    }

    m_root = m_doc.first_child();
    if(m_root == NULL)
    {
        printf("Para File[%s] is Empty!\n", topoPath);
        return false;
    }

    //解析个节点配置;
    for(xml_node pNode = m_root.first_child();
        pNode; pNode = pNode.next_sibling())
    {
        if(NULL == pNode.name())
            continue;

        if (0 == strcmp(pNode.name(), STR_TOPOINFO))
        {
            ParseTopoInfo(pNode);
        }
        else if (0 == strcmp(pNode.name(), STR_PARA))
        {
            ParseParaNode(pNode, nullptr);
        }
    }

    return true;
}

CParsePara::CParsePara()
{

}

void CParsePara::ReleasePara()
{
    m_Para->date.clear();
    m_Para->desc.clear();
    m_Para->version.clear();
    m_Para->date.clear();

    m_Para->ptPara->key.clear();
    m_Para->ptPara->desc.clear();
    m_Para->ptPara->parent = nullptr;
    m_Para->ptPara->items.clear();
    m_Para->ptPara->sons.clear();
}

void CParsePara::ParseTopoInfo(xml_node topoInfo)
{
    if (!topoInfo)
        return;

    m_Para->name = topoInfo.attribute(STR_NAME).as_string();
    m_Para->name = topoInfo.attribute(STR_DESC).as_string();
    m_Para->name = topoInfo.attribute(STR_VERSION).as_string();
    m_Para->name = topoInfo.attribute(STR_DATE).as_string();

}

void CParsePara::ParseParaNode(xml_node para, tdsPara *parent)
{
    if (!para)
        return;

    tdsPara * ptPara;
    if(parent == nullptr)
    {
        ptPara = m_Para->ptPara;
    }
    else
    {
        ptPara = new tdsPara(parent,"","");
        parent->sons.push_back(ptPara);
    }
    ptPara->key = para.attribute(STR_NAME).as_string();
    ptPara->desc = para.attribute(STR_DESC).as_string();

    for(xml_node pNode = para.first_child();
        pNode; pNode = pNode.next_sibling())
    {
        if(NULL == pNode.name())
            continue;

        if (0 == strcmp(pNode.name(), STR_ITEM))
        {
            ParseItemNode(pNode,ptPara);
        }
        else if (0 == strcmp(pNode.name(), STR_PARA))
        {
            ParseParaNode(pNode, ptPara);
        }
    }
}

void CParsePara::ParseItemNode(xml_node node, tdsPara *parent)
{
    if (!node)
        return;

    if(parent == nullptr)
        return;

    string name = node.attribute(STR_NAME).as_string();
    string key  = node.attribute(STR_KEY).as_string();
    string desc = node.attribute(STR_DESC).as_string();

    parent->items.push_back(new tdsItem(key,desc,name));
}
