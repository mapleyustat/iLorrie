/************************************************************************
*						ConfigData.cpp
*					定义配置信息的数据结构和相关读取
*					weigaofeng  2012-1-6
************************************************************************/
#include "ConfigData.h"
#include "tinyxml.h"


/************************************************************************
*						CConfigData
*					定义配置信息的数据结构
*					weigaofeng  2012-1-6
************************************************************************/

bool CConfigData::m_bInitialized = false;		// 定义并初始化静态成员变量

CConfigData::~CConfigData()
{
	ClearAll();
}

CConfigData & CConfigData::Instance()
{
	assert(m_bInitialized);
	static CConfigData objMgr;
	return objMgr;
}

void CConfigData::ClearAll()
{
	ClearEventType();
	ClearFeedbackAttitude();
	ClearFeedbackType();
	ClearEffectLevel();
}

bool CConfigData::Initialize( const string & strFile, string &strError )
{
	m_bInitialized = true;
	return CConfigReader::ReadRecordSetting( strFile, strError);
}

std::string CConfigData::GetEventType( int iIndex )
{
	if ( iIndex >= 0 && iIndex < GetEventTypeCnt())
	{
		return m_vecEventType[iIndex];
	}else
	{
		return UNKONWN;
	}
}

/************************************************************************
*						CConfigReader
*					定义配置信息的数据结构
*					weigaofeng  2012-1-6
************************************************************************/
bool CConfigReader::ReadRecordSetting( const string & strFile, string &strError)
{
	strError = "读取文件：";
	strError += strFile;
	strError += "\n";

	TiXmlDocument doc(strFile.c_str());
	if ( !doc.LoadFile())
	{
		strError += "无法打开！\n";
		return false;
	}

	TiXmlElement *pRoot = doc.RootElement();
	if ( pRoot == NULL)
	{
		strError += "获取根节点失败! \n";
		return false;
	}

	// 读取RecordSetting的相关信息
	TiXmlElement *pRecordElem = pRoot->FirstChildElement("RecordSetting");
	if ( pRecordElem == NULL)
	{
		strError += "找不到RecordSetting记录! \n";
		return false;
	}	
	CConfigData::Instance().ClearAll();
	bool bResult = true;
	bResult &= ReadEvent( pRecordElem, strError);
	bResult &= ReadFeedback( pRecordElem, strError);
	bResult &= ReadEffect( pRecordElem, strError);
	if ( !bResult)
	{
		return false;
	}

	return true;
}

bool CConfigReader::ReadEvent( TiXmlElement *pRecordElem, string &strError )
{
	strError += "读取Event：\n";

	if ( pRecordElem == NULL)
	{
		strError += "传入节点为空！\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Event");
	if ( pRecordElem == NULL)
	{
		strError += "找不到Event信息！\n";
		return false;
	}

	// 读取Type
	TiXmlElement *pElem = pRecordElem->FirstChildElement("Type");
	if ( pElem == NULL)
	{
		strError += "找不到Type记录！\n";
		return false;
	}

	while ( pElem)
	{
		const char * pSz = pElem->GetText();
		if ( pSz )
		{
			string strValue(pSz);
			CConfigData::Instance().AddEventType(strValue);
		}

		pElem = pElem->NextSiblingElement("Type");
	}

	return true;
}

bool CConfigReader::ReadFeedback( TiXmlElement *pRecordElem, string &strError )
{
	strError += "读取Feedback：\n";

	if ( pRecordElem == NULL)
	{
		strError += "传入节点为空！\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Feedback");
	if ( pRecordElem == NULL)
	{
		strError += "找不到Feedback信息！\n";
		return false;
	}

	// 读取Attitude
	TiXmlElement *pElemAtt = pRecordElem->FirstChildElement("Attitude");
	if ( pElemAtt == NULL)
	{
		strError += "找不到Attitude记录！\n";
		return false;
	}

	while ( pElemAtt)
	{
		const char * pSz = pElemAtt->GetText();
		if ( pSz )
		{
			string strValue(pSz);
			CConfigData::Instance().AddFeedbackAttitude(strValue);
		}

		pElemAtt = pElemAtt->NextSiblingElement("Attitude");
	}

	// 读取Type
	TiXmlElement *pElemType = pRecordElem->FirstChildElement("Type");
	if ( pElemType == NULL)
	{
		strError += "找不到Type记录！\n";
		return false;
	}

	while ( pElemType)
	{
		const char * pSz = pElemType->GetText();
		if ( pSz )
		{
			string strValue(pSz);
			CConfigData::Instance().AddFeedbackType(strValue);
		}

		pElemType = pElemType->NextSiblingElement("Type");
	}

	return true;
}

bool CConfigReader::ReadEffect( TiXmlElement *pRecordElem, string &strError )
{
	strError += "读取Event：\n";

	if ( pRecordElem == NULL)
	{
		strError += "传入节点为空！\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Effect");
	if ( pRecordElem == NULL)
	{
		strError += "找不到Effect信息！\n";
		return false;
	}

	// 读取Level
	TiXmlElement *pElem = pRecordElem->FirstChildElement("Level");
	if ( pElem == NULL)
	{
		strError += "找不到Level记录！\n";
		return false;
	}

	while ( pElem)
	{
		const char * pSz = pElem->GetText();
		if ( pSz )
		{
			string strValue(pSz);
			CConfigData::Instance().AddEffectLevel(strValue);
		}

		pElem = pElem->NextSiblingElement("Level");
	}

	return true;
}