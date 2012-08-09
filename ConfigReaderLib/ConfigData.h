/************************************************************************
*						ConfigData.h
*					定义配置信息的数据结构和相关读取
*					weigaofeng  2012-1-6
************************************************************************/
#pragma once

#include <vector>

using namespace std;

#define UNKONWN	"未知"

/************************************************************************
*						CConfigData
*					定义配置信息的数据结构
*					weigaofeng  2012-1-6
************************************************************************/
class  CConfigData
{
public:
	static CConfigData &Instance();
	static bool Initialize( const string & strFile, string &strError);		// 在程序启动的时候读取本地xml文件
	~CConfigData();

	void	ClearAll();

	void	AddEventType( const string & val)	{ m_vecEventType.push_back(val); }
	void	ClearEventType()	{ m_vecEventType.clear(); }
	vector<string> & GetEventTypes()	{ return m_vecEventType;}
	string	GetEventType( int iIndex);
	int		GetEventTypeCnt()	{ return (int)m_vecEventType.size();}

	void	AddFeedbackAttitude(  string & val)	{ m_vecFeedbackAttitude.push_back(val); }
	void	ClearFeedbackAttitude()	{ m_vecFeedbackAttitude.clear(); }
	vector<string> & GetFeedbackAttitudes()	{ return m_vecFeedbackAttitude;}
	void	AddFeedbackType(  string & val)	{ m_vecFeedbackType.push_back(val); }
	void	ClearFeedbackType()	{ m_vecFeedbackType.clear(); }
	vector<string> & GetFeedbackTypes()	{ return m_vecFeedbackType;}
	void	AddEffectLevel(  string & val)		{ m_vecEffectLevel.push_back(val); }
	void	ClearEffectLevel()	{ m_vecEffectLevel.clear(); }
	vector<string> & GetEffectLevels()	{ return m_vecEffectLevel;}

private:

	CConfigData()	{};

	vector<string>	m_vecEventType;
	vector<string>	m_vecFeedbackAttitude;
	vector<string>	m_vecFeedbackType;
	vector<string>	m_vecEffectLevel;

	static bool	m_bInitialized;						// 用来防止未读取本地文件就调用Instance()

};


/************************************************************************
*						CConfigReader
*					定义配置信息的数据结构
*					weigaofeng  2012-1-6
************************************************************************/
class TiXmlElement;
class   CConfigReader
{
public:
	static bool ReadRecordSetting( const string & strFile, string &strError);
private:
	static bool ReadEvent( TiXmlElement *pRecordElem, string &strError);
	static bool ReadFeedback( TiXmlElement *pRecordElem, string &strError);
	static bool ReadEffect( TiXmlElement *pRecordElem, string &strError);
};

