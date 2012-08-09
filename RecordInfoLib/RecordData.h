/************************************************************************
*						RecordData.h
*					定义记录的数据结构
*					weigaofeng  2012-1-1
************************************************************************/
#pragma once

#include <vector>
#include "time.h"

using namespace std;

/************************************************************************
*						CEvnet	事件类
*					  记录发生的事件信息
*                                         
************************************************************************/
class  CEvent
{
public:
	time_t	GetEventTime()	{ return m_tEventTime; }
	void	SetEventTime( const time_t &val )	{ m_tEventTime = val; }
	string	GetType()	{ return m_strType; }
	void	SetType( const string & val ) { m_strType = val; }
	string	GetTitle()	{ return m_strTitle; }
	void	SetTitle( const string & val ) { m_strTitle = val; }
	string	GetDetail()	{ return m_strDetail; }
	void	SetDetail( const string & val )	{ m_strDetail = val; }
protected:
private:
	time_t	m_tEventTime;	// 发生时间
	string	m_strType;		// 类型
	string	m_strTitle;		// 标题
	string	m_strDetail;	// 详情
};

/************************************************************************
*						CActBase	行为基类
*					  事件发生之后的行为基类
*                                         
************************************************************************/
class  CActBase
{
public:
	CActBase():m_strPerson("")	{}

	string	GetPerson()	{ return m_strPerson; }
	void	SetPerson( const string & val )	{ m_strPerson = val; }
protected:
private:
	string	m_strPerson;	// 行为的主角
};

/************************************************************************
*						CFeedback	其中一方反应类
*					  因事件产生导致其中一方的反应信息
*                                        
************************************************************************/
class  CFeedback: public CActBase
{
public:
	CFeedback():m_strAttitude(""), m_strType(""), m_strDetail("")	{}

	string	GetAttitude()	{ return m_strAttitude; }
	void	SetAttitude( const string & val)	{ m_strAttitude = val; }
	string	GetType()	{ return m_strType;	}
	void	SetType( const string & val )	{ m_strType = val; }
	string	GetDetail()	{ return m_strDetail; }
	void	SetDetail( const string & val )	{ m_strDetail = val; }
protected:
private:
	string	m_strAttitude;	// 面对事件的态度
	string	m_strType;		// 建议类型
	string	m_strDetail;	// 建议详情
};

/************************************************************************
*						CEffect	另一方的执行效果类
*					  在得到对方反馈后，另一方的执行效果类
*                                         
************************************************************************/
class  CEffect: public CActBase
{
public:
	CEffect(): m_strLevel(""), m_strDetail("")	{}

	string	GetLevel()	{ return m_strLevel; }
	void	SetLevel( const string & val )	{ m_strLevel = val; }
	string	GetDetail()	{ return m_strDetail; }
	void	SetDetail( const string & val )	{ m_strDetail = val; }
protected:
private:
	string	m_strLevel;		// 执行效果级别
	string	m_strDetail;	// 执行详情
};

/************************************************************************
*						CRecord	记录类
*					  一条完整的记录：包含even, feedback, effect
*                                         
************************************************************************/
#define VALID_FLAG_BIT_EVENT	( 1)
#define VALID_FLAG_BIT_FEEDBACK	( 1<<1)
#define VALID_FLAG_BIT_EFFECT	( 1<<2)

class  CRecord
{
public:
	CRecord();
	~CRecord();

	void	ResetRecord();

	time_t	GetCreateTime()	{ return m_tCreateTime; }
	void	SetCreateTime( const time_t &val )	{ m_tCreateTime = val; }
	time_t	GetSaveTime()	{ return m_tSaveTime; }
	void	SetSaveTime( const time_t &val )	{ m_tSaveTime = val; }
	string	GetModifier()	{ return m_strModifier; }
	void	SetModifier( const string & val )	{ m_strModifier = val; }

	CEvent*	GetEventPt() { return &m_objEvent;}
	void	SetEvent( const CEvent &val )	{ m_objEvent = val; }
	string  GetEventTitle();
	CFeedback*	GetFeedbackPt()	{ return &m_objFeedBack;}
	void	SetFeedback( const CFeedback &val )	{ m_objFeedBack = val; }
	CEffect*	GetEffectPt()	{ return &m_objEffect; }
	void	SetEffect( const CEffect &val )	{ m_objEffect = val; }

	unsigned int GetValidFlag()	{ return m_unValidFlag; }
	void	SetValidFlag( unsigned int val)	{ m_unValidFlag = val; }

	void	SetEventValid( bool bValid = true);;
	void	SetFeedbackValid( bool bValid = true);
	void	SetEffectValid( bool bValid = true);
	bool	IsEventValid();
	bool	IsFeedbackValid();
	bool	IsEffectValid();

	bool	IsRecordIntact();	// 记录是否完整，所有成员都有效
	bool	IsRecordValid();	// 记录是否有效，有一个成员有效即可

protected:
private:	
	time_t	m_tCreateTime;	// 系统创建时间
	time_t	m_tSaveTime;	// 最后修改时间
	string	m_strModifier;	// 最后修改人

	CEvent		m_objEvent;		// 发生的事件
	CFeedback	m_objFeedBack;	// 一方的反馈和建议
	CEffect		m_objEffect;	// 另一方的执行效果

	unsigned int	m_unValidFlag;		// 用每一位来表示Event,Feedback,Effect成员是否有效
};

/************************************************************************
*						CBriefInfo	摘要信息类
*					 存储记录集合中的创建时间和标题信息
*                                         
************************************************************************/
class  CBriefInfo
{
public:
	CBriefInfo();
	CBriefInfo( CRecord * pRecord);

	void	SetCreateTime( const time_t val)	{ m_tCreateTime = val; }
	time_t	GetCreateTime()	{ return m_tCreateTime; }
	void	SetAbstract( const string val)	{ m_strAbstract = val; }
	string	GetAbstract()	{ return m_strAbstract; }
private:
	time_t m_tCreateTime;
	string m_strAbstract;
};

/************************************************************************
*						CRecordMgr	记录管理类
*					  针对多条记录进行管理
*
*Remarks:	1.m_vecRecords可以是数据库中部分记录的集合
*			2.m_vecAllBriefs是数据库中所有记录的摘要信息的集合，在对数据库中记录进行修改时，需要同步维护该数据的内容
*                                         
************************************************************************/
class  CRecordMgr
{
public:
	static CRecordMgr & Instance();
	~CRecordMgr();

	int	GetRecordCnt()	{ return (int)m_vecRecords.size(); }
	CRecord*	AddRecordToEnd( const CRecord &record);
	CRecord*	GetRecord( int index);
	CRecord*	GetRecordByTime( time_t tCreateTime);			// 根据创建时间获取完整记录
	void	ClearRecords()	{ m_vecRecords.clear(); }

	int GetBriefCnt()	{ return (int)m_vecBriefs.size(); }
	CBriefInfo* GetBrief( int index);
	void	AddBriefAsc( CBriefInfo& brief);					// 按照创建时间先后顺序插入摘要
	void	ClearBriefs()	{ m_vecBriefs.clear(); }

protected:
private:
	CRecordMgr();			// 单例模式下的构造函数为私有

	vector<CRecord>	m_vecRecords;			// 记录集合
	vector<CBriefInfo> m_vecBriefs;		// 所有记录的摘要信息
};


