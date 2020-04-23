#ifndef CPARSEPARA_H
#define CPARSEPARA_H

#include "ParaStruct.h"
#include "pugixml.hpp"
#include "codecutil.h"
#include <cstring>

using namespace pugi;

class CParsePara
{
public:
    CParsePara();
    //析构函数;
    ~CParsePara(void);

    bool ParsePara(const std::string& paraPath,tdsTopoPara  * para);
private:


    void			ReleasePara();
    void            ParseTopoInfo(xml_node topoInfo);
    void            ParseParaNode(xml_node para , tdsPara *parent);
    void            ParseItemNode(xml_node node , tdsPara *parent);

    //XML配置文件根节点
    xml_node 			m_root;
    //XML配置文件DOC对象
    xml_document 		m_doc;
    //XML配置文件解析结果
    xml_parse_result 	m_xmlret;
    //ssd解析内容;
    tdsTopoPara   *     m_Para;
};

#endif // CPARSEPARA_H
