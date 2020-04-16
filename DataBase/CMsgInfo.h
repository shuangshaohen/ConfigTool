#ifndef CMSGINFO_H
#define CMSGINFO_H


#include <QString>
#include "DataBase_global.h"

class DATABASE_EXPORT CMsgInfo
{
public:
    enum Enum_Msg_Level
    {
        CN_HINT_MSG = 0,
        CN_WARNNING_MSG,
        CN_ERROR_MSG
    };
    enum Enum_Verify_Mode
    {
        Enum_Comm_Mode = 0,
        Enum_Application_Parse_Mode,
        Enum_Application_Save_Mode,
        Enum_Application_Verify_Mode,
    };
public:
    Enum_Verify_Mode	m_Enum_Verify_Mode;
    Enum_Msg_Level		m_Enum_Msg_Level;
    QString				m_strInfo;
    QString				m_strInfoToken;
public:
    CMsgInfo(){};
    CMsgInfo( Enum_Verify_Mode mode, Enum_Msg_Level nLevel, QString strInfo, QString strInfoToken=QString("") );
    CMsgInfo( const CMsgInfo & );
    CMsgInfo & operator= (const CMsgInfo &);
    bool IsHintMsg(){ return CN_HINT_MSG==m_Enum_Msg_Level; };
    bool IsWarnMsg(){ return CN_WARNNING_MSG==m_Enum_Msg_Level; };
    bool IsErrorMsg(){ return CN_ERROR_MSG==m_Enum_Msg_Level; };
public:
    ~CMsgInfo(void);
};

#endif // CMSGINFO_H
