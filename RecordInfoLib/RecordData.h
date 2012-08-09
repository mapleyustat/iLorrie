/************************************************************************
*						RecordData.h
*					�����¼�����ݽṹ
*					weigaofeng  2012-1-1
************************************************************************/
#pragma once

#include <vector>
#include "time.h"

using namespace std;

/************************************************************************
*						CEvnet	�¼���
*					  ��¼�������¼���Ϣ
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
	time_t	m_tEventTime;	// ����ʱ��
	string	m_strType;		// ����
	string	m_strTitle;		// ����
	string	m_strDetail;	// ����
};

/************************************************************************
*						CActBase	��Ϊ����
*					  �¼�����֮�����Ϊ����
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
	string	m_strPerson;	// ��Ϊ������
};

/************************************************************************
*						CFeedback	����һ����Ӧ��
*					  ���¼�������������һ���ķ�Ӧ��Ϣ
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
	string	m_strAttitude;	// ����¼���̬��
	string	m_strType;		// ��������
	string	m_strDetail;	// ��������
};

/************************************************************************
*						CEffect	��һ����ִ��Ч����
*					  �ڵõ��Է���������һ����ִ��Ч����
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
	string	m_strLevel;		// ִ��Ч������
	string	m_strDetail;	// ִ������
};

/************************************************************************
*						CRecord	��¼��
*					  һ�������ļ�¼������even, feedback, effect
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

	bool	IsRecordIntact();	// ��¼�Ƿ����������г�Ա����Ч
	bool	IsRecordValid();	// ��¼�Ƿ���Ч����һ����Ա��Ч����

protected:
private:	
	time_t	m_tCreateTime;	// ϵͳ����ʱ��
	time_t	m_tSaveTime;	// ����޸�ʱ��
	string	m_strModifier;	// ����޸���

	CEvent		m_objEvent;		// �������¼�
	CFeedback	m_objFeedBack;	// һ���ķ����ͽ���
	CEffect		m_objEffect;	// ��һ����ִ��Ч��

	unsigned int	m_unValidFlag;		// ��ÿһλ����ʾEvent,Feedback,Effect��Ա�Ƿ���Ч
};

/************************************************************************
*						CBriefInfo	ժҪ��Ϣ��
*					 �洢��¼�����еĴ���ʱ��ͱ�����Ϣ
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
*						CRecordMgr	��¼������
*					  ��Զ�����¼���й���
*
*Remarks:	1.m_vecRecords���������ݿ��в��ּ�¼�ļ���
*			2.m_vecAllBriefs�����ݿ������м�¼��ժҪ��Ϣ�ļ��ϣ��ڶ����ݿ��м�¼�����޸�ʱ����Ҫͬ��ά�������ݵ�����
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
	CRecord*	GetRecordByTime( time_t tCreateTime);			// ���ݴ���ʱ���ȡ������¼
	void	ClearRecords()	{ m_vecRecords.clear(); }

	int GetBriefCnt()	{ return (int)m_vecBriefs.size(); }
	CBriefInfo* GetBrief( int index);
	void	AddBriefAsc( CBriefInfo& brief);					// ���մ���ʱ���Ⱥ�˳�����ժҪ
	void	ClearBriefs()	{ m_vecBriefs.clear(); }

protected:
private:
	CRecordMgr();			// ����ģʽ�µĹ��캯��Ϊ˽��

	vector<CRecord>	m_vecRecords;			// ��¼����
	vector<CBriefInfo> m_vecBriefs;		// ���м�¼��ժҪ��Ϣ
};


