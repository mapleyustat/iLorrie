/************************************************************************
*						RecordData.cpp
*					定义记录的数据结构
*					weigaofeng  2012-1-1
************************************************************************/
#include "RecordData.h"

#include <string>
using namespace std;

/************************************************************************
*						CRecord	记录类
*					  一条完整的记录：包含even, feedback, effect
*                                         
************************************************************************/
CRecord::CRecord()
:m_tCreateTime(0), m_tSaveTime(0), m_strModifier(""),m_unValidFlag(0)
{

}

CRecord::~CRecord()
{
	ResetRecord();
}

void CRecord::ResetRecord()
{
	m_unValidFlag = 0;
}

string CRecord::GetEventTitle()
{
	return m_objEvent.GetTitle();
}

void CRecord::SetEventValid( bool bValid /*= true*/ )
{
	if ( bValid)
	{
		m_unValidFlag |= VALID_FLAG_BIT_EVENT;
	}
	else
	{
		m_unValidFlag &= ~VALID_FLAG_BIT_EVENT;
	}
}

void CRecord::SetFeedbackValid( bool bValid /*= true*/ )
{
	if ( bValid)
	{
		m_unValidFlag |= VALID_FLAG_BIT_FEEDBACK;
	}
	else
	{
		m_unValidFlag &= ~VALID_FLAG_BIT_FEEDBACK;
	}
}

void CRecord::SetEffectValid( bool bValid /*= true*/ )
{
	if ( bValid)
	{
		m_unValidFlag |= VALID_FLAG_BIT_EFFECT;
	}
	else
	{
		m_unValidFlag &= ~VALID_FLAG_BIT_EFFECT;
	}
}

bool CRecord::IsEventValid()
{
	return m_unValidFlag & VALID_FLAG_BIT_EVENT;
}

bool CRecord::IsFeedbackValid()
{
	return m_unValidFlag & VALID_FLAG_BIT_FEEDBACK;
}

bool CRecord::IsEffectValid()
{
	return m_unValidFlag & VALID_FLAG_BIT_EFFECT;
}

bool CRecord::IsRecordIntact()
{
	return IsEventValid() && IsFeedbackValid() && IsEffectValid();
}

bool CRecord::IsRecordValid()
{
	return IsEventValid() || IsFeedbackValid() || IsEffectValid();
}

/************************************************************************
*						CBriefInfo	摘要信息类
*					 存储记录集合中的创建时间和标题信息
*                                         
************************************************************************/
CBriefInfo::CBriefInfo()
{
	m_strAbstract = "";
}

CBriefInfo::CBriefInfo( CRecord * pRecord )
{
	if ( pRecord)
	{
		m_strAbstract = pRecord->GetEventTitle();
		m_tCreateTime = pRecord->GetCreateTime();
	}else
	{
		CBriefInfo();
	}

}

/************************************************************************
*						CRecordMgr	记录管理类
*					  针对多条记录进行管理
*                                         
************************************************************************/
CRecordMgr::CRecordMgr()
{
	ClearRecords();
	ClearBriefs();
}

CRecordMgr::~CRecordMgr()
{
	ClearRecords();
	ClearBriefs();
}

CRecordMgr & CRecordMgr::Instance()
{
	static CRecordMgr objInstance;
	return objInstance;
}


CRecord* CRecordMgr::AddRecordToEnd( const CRecord &record )
{
	m_vecRecords.push_back( record);
	int nCnt = m_vecRecords.size();
	return &m_vecRecords[ nCnt - 1];
}

CRecord* CRecordMgr::GetRecord( int index )
{
	if ( index >= 0 && index < GetRecordCnt())
	{
		return &m_vecRecords[index];
	}else
	{
		return NULL;
	}
}

CRecord* CRecordMgr::GetRecordByTime( time_t tCreateTime)
{
	for ( int i = 0; i < m_vecRecords.size(); i++)
	{
		if ( m_vecRecords[i].GetCreateTime() == tCreateTime)
		{
			return &m_vecRecords[i];
		}
	}

	return NULL;
}

CBriefInfo* CRecordMgr::GetBrief( int index )
{
	if ( index >= 0 && index < GetBriefCnt())
	{
		return &m_vecBriefs[index];
	}else
	{
		return NULL;
	}
}

void CRecordMgr::AddBriefAsc( CBriefInfo& brief )
{
	vector<CBriefInfo>::iterator iter = m_vecBriefs.begin();
	for ( ; iter != m_vecBriefs.end(); iter++)
	{
		if ( brief.GetCreateTime() < iter->GetCreateTime())
		{
			break;
		}
	}

	m_vecBriefs.insert( iter, brief);

}
