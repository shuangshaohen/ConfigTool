#include "CMsgInfo.h"

CMsgInfo::CMsgInfo( Enum_Verify_Mode nMode, Enum_Msg_Level nLevel, QString strInfo, QString strInfoToken )
{
    this->m_Enum_Msg_Level = nLevel;
    this->m_strInfo = strInfo;
    this->m_strInfoToken = strInfoToken;
    this->m_Enum_Verify_Mode = nMode;
}

CMsgInfo::CMsgInfo( const CMsgInfo & msgInfo )
{
    this->m_Enum_Msg_Level = msgInfo.m_Enum_Msg_Level;
    this->m_strInfo = msgInfo.m_strInfo;
    this->m_strInfoToken = msgInfo.m_strInfoToken;
    this->m_Enum_Verify_Mode = msgInfo.m_Enum_Verify_Mode;
}
CMsgInfo & CMsgInfo::operator= (const CMsgInfo & msgInfo )
{
    if( this == &msgInfo )
        return *this;
    this->m_Enum_Msg_Level = msgInfo.m_Enum_Msg_Level;
    this->m_strInfo = msgInfo.m_strInfo;
    this->m_strInfoToken = msgInfo.m_strInfoToken;
    this->m_Enum_Verify_Mode = msgInfo.m_Enum_Verify_Mode;
    return *this;
}

CMsgInfo::~CMsgInfo(void)
{
}
