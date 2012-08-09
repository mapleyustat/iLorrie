/************************************************************************
*						ConfigData.cpp
*					����������Ϣ�����ݽṹ����ض�ȡ
*					weigaofeng  2012-1-6
************************************************************************/
#include "ConfigData.h"
#include "tinyxml.h"


/************************************************************************
*						CConfigData
*					����������Ϣ�����ݽṹ
*					weigaofeng  2012-1-6
************************************************************************/

bool CConfigData::m_bInitialized = false;		// ���岢��ʼ����̬��Ա����

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
*					����������Ϣ�����ݽṹ
*					weigaofeng  2012-1-6
************************************************************************/
bool CConfigReader::ReadRecordSetting( const string & strFile, string &strError)
{
	strError = "��ȡ�ļ���";
	strError += strFile;
	strError += "\n";

	TiXmlDocument doc(strFile.c_str());
	if ( !doc.LoadFile())
	{
		strError += "�޷��򿪣�\n";
		return false;
	}

	TiXmlElement *pRoot = doc.RootElement();
	if ( pRoot == NULL)
	{
		strError += "��ȡ���ڵ�ʧ��! \n";
		return false;
	}

	// ��ȡRecordSetting�������Ϣ
	TiXmlElement *pRecordElem = pRoot->FirstChildElement("RecordSetting");
	if ( pRecordElem == NULL)
	{
		strError += "�Ҳ���RecordSetting��¼! \n";
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
	strError += "��ȡEvent��\n";

	if ( pRecordElem == NULL)
	{
		strError += "����ڵ�Ϊ�գ�\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Event");
	if ( pRecordElem == NULL)
	{
		strError += "�Ҳ���Event��Ϣ��\n";
		return false;
	}

	// ��ȡType
	TiXmlElement *pElem = pRecordElem->FirstChildElement("Type");
	if ( pElem == NULL)
	{
		strError += "�Ҳ���Type��¼��\n";
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
	strError += "��ȡFeedback��\n";

	if ( pRecordElem == NULL)
	{
		strError += "����ڵ�Ϊ�գ�\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Feedback");
	if ( pRecordElem == NULL)
	{
		strError += "�Ҳ���Feedback��Ϣ��\n";
		return false;
	}

	// ��ȡAttitude
	TiXmlElement *pElemAtt = pRecordElem->FirstChildElement("Attitude");
	if ( pElemAtt == NULL)
	{
		strError += "�Ҳ���Attitude��¼��\n";
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

	// ��ȡType
	TiXmlElement *pElemType = pRecordElem->FirstChildElement("Type");
	if ( pElemType == NULL)
	{
		strError += "�Ҳ���Type��¼��\n";
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
	strError += "��ȡEvent��\n";

	if ( pRecordElem == NULL)
	{
		strError += "����ڵ�Ϊ�գ�\n";
		return false;
	}

	pRecordElem = pRecordElem->FirstChildElement("Effect");
	if ( pRecordElem == NULL)
	{
		strError += "�Ҳ���Effect��Ϣ��\n";
		return false;
	}

	// ��ȡLevel
	TiXmlElement *pElem = pRecordElem->FirstChildElement("Level");
	if ( pElem == NULL)
	{
		strError += "�Ҳ���Level��¼��\n";
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