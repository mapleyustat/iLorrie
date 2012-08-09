/************************************************************************
*							DBInterface.cpp
*						�������ݿ����ز���
*						weigaofeng  2012-1-13
************************************************************************/

#include "DBInterface.h"
#include "RecordData.h"
#include "StringTimeConvert.h"

#include <iostream>

using namespace std;

extern "C"
{
#include "sqlite3.h"
};

#define SQL_LENGTH	1024

/************************************************************************
*						CDBInterface	���ݿ�ӿ���
*					         �ṩ���ݿ��д�ӿ�
*                                         
************************************************************************/

CDBInterface::CDBInterface()
{
}

CDBInterface::~CDBInterface()
{
}

// ��ʼ�����ݿ�����
void CDBInterface::InitDBName( const string strDBName )
{
	m_strDBName = strDBName;
}

// ��ȡ��¼���е����������Ĵ���ʱ��
bool CDBInterface::ReadAllRecordTime( vector<time_t> &vecTime, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "�޷���" + m_strDBName + "\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;

	char szSql[SQL_LENGTH] = "select create_time from RECORD";
	//��3������dbResult�ǲ�ѯ���������һά���顣���ڴ沼���ǣ���һ�����ֶ����ƣ������ǽ�������ÿ���ֶε�ֵ�����д洢
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{		
		for ( int i = 0; i < nRow; i++)
		{
			int nSeq = ( i + 1) * nColumn;
			time_t tCreateTime;
			if ( CStringTimeConvert::_StringToTime( dbResult[nSeq++],tCreateTime))
			{
				vecTime.push_back(tCreateTime);
			}else			// ����ʱ�䲻��Ϊ��
			{
				strErr += "��ȡ����ʱ��ʧ�ܣ�\n";
				bResult = false;
				break;
			}
		}		
	}else
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);
	sqlite3_close(pDB);

	return bResult;
}

// ���ݴ���ʱ���ȡ������¼����
bool CDBInterface::ReadRecord( const time_t tCreateTime, CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "�޷���" + m_strDBName + "\n";
		bResult = false;
	}

	if ( bResult && !ReadRecInfoByDateTime( pDB, tCreateTime, objRecord, strErr))
	{
		strErr += "���ݴ���ʱ���ȡ��¼������Ϣʧ�ܣ�\n";
		bResult = false;
	}

	string strIndex("");
	if ( bResult && !ReadIndexByDateTime( pDB, tCreateTime, strIndex, strErr))
	{
		strErr += "���ݴ���ʱ���ȡ������Ϣʧ�ܣ�\n";
		bResult = false;
	}
	
	CEvent objEvent;
	if ( bResult && !ReadEventByIndex( pDB, strIndex, objEvent, strErr))
	{
		strErr += "����������Ϣ��ȡ�¼���Ϣʧ�ܣ�\n";
		bResult = false;
	}

	CFeedback objFeedback;
	if ( bResult && !ReadFeedbackByIndex( pDB, strIndex, objFeedback, strErr))
	{
		strErr += "����������Ϣ��ȡ������Ϣʧ�ܣ�\n";
		bResult = false;
	}

	CEffect objEffect;
	if ( bResult && !ReadEffectByIndex( pDB, strIndex, objEffect, strErr))
	{
		strErr += "����������Ϣ��ȡִ��Ч����Ϣʧ�ܣ�\n";
		bResult = false;
	}

	if ( bResult)
	{
		objRecord.SetEvent( objEvent);
		objRecord.SetFeedback( objFeedback);
		objRecord.SetEffect( objEffect);
	}
	
	// close db
	sqlite3_close(pDB);

	return bResult;

}

// ��������ʱ���ȡ������
bool CDBInterface::ReadIndexByDateTime( sqlite3 *pDB, const time_t &tCreateTime, string &strIndex, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);
	sprintf_s( szSql,"select record_index from RECORD where create_time = \'%s\'",strCreateTime.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// ��Ϊcreate_time��UNIQUE�ģ����Բ����ܳ��ֶ���һ����¼
		if ( nRow > 1)
		{
			strErr += "���ֶ����¼��create_timeֵΪ" + strCreateTime + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			strIndex = 	dbResult[ nColumn];
		}		
	}else
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);

	return bResult;
}

// ���ݴ���ʱ���ȡRecord��������Ϣ
bool CDBInterface::ReadRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);

//	�˴�sprintf_s֮���Գ��ֹ��쳣������Ϊ�����������%������ţ�������������%s�����%Y�����ζ�����
//	sprintf_s( szSql,"select strftime(\'%Y-%m-%d %H:%M:%S\',save_time),modifier from RECORD where strftime(\'%Y-%m-%d %H:%M:%S\',create_time) = \'%s\'",strCreateTime.c_str());
	
	sprintf_s( szSql,"select save_time,valid_flag,modifier from RECORD where create_time = \'%s\'",strCreateTime.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// ��Ϊcreate_time��UNIQUE�ģ����Բ����ܳ��ֶ���һ����¼
		if ( nRow > 1)
		{
			strErr += "���ֶ����¼��create_timeֵΪ" + strCreateTime + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			objRecord.SetCreateTime( tCreateTime);

			int nSeq = nColumn;	
			
			time_t tSaveTime;
			if ( CStringTimeConvert::_StringToTime( dbResult[nSeq++], tSaveTime))
			{
				objRecord.SetSaveTime( tSaveTime);
			}
						
			int nValidFlag = atoi(dbResult[nSeq++]);
			objRecord.SetValidFlag( nValidFlag);

			objRecord.SetModifier( dbResult[nSeq++]);
		}
	}else
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);

	return bResult;
}

// ��������ʱ���ȡEvent��Ϣ
bool CDBInterface::ReadEventByIndex( sqlite3 *pDB, const string &strIndex, CEvent &objEvent, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "select event_time,type,title,detail from EVENT where record_index = \'%s\'", strIndex.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// ��Ϊrecord_index��UNIQUE�ģ����Բ����ܳ��ֶ���һ����¼
		if ( nRow > 1)
		{
			strErr += "���ֶ����¼��record_indexֵΪ" + strIndex + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			time_t tEventTime;
			int nSeq = nColumn;
			if ( CStringTimeConvert::_StringToTime( dbResult[ nSeq++], tEventTime))	// ǰnColumn�����ֶ���
			{
				objEvent.SetEventTime( tEventTime);
			}
			objEvent.SetType( dbResult[ nSeq++]);
			objEvent.SetTitle( dbResult[ nSeq++]);
			objEvent.SetDetail( dbResult[ nSeq++]);			
		}
	}else 
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);

	return bResult;
}

// ��������ʱ���ȡFeedback��Ϣ
bool CDBInterface::ReadFeedbackByIndex( sqlite3 *pDB, const string &strIndex, CFeedback &objFeedback, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "select actor,attitude,adv_type,adv_detail from FEEDBACK where record_index = \'%s\'", strIndex.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// ��Ϊrecord_index��UNIQUE�ģ����Բ����ܳ��ֶ���һ����¼
		if ( nRow > 1)
		{
			strErr += "���ֶ����¼��record_indexֵΪ" + strIndex + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			int nSeq = nColumn;
			objFeedback.SetPerson( dbResult[ nSeq++]);	
			objFeedback.SetAttitude( dbResult[ nSeq++]);
			objFeedback.SetType( dbResult[ nSeq++]);
			objFeedback.SetDetail( dbResult[ nSeq++]);
		}
	}else 
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);

	return bResult;
}

// ��������ʱ���ȡEffect��Ϣ
bool CDBInterface::ReadEffectByIndex( sqlite3 *pDB, const string &strIndex, CEffect &objEffect, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "select actor,level,detail from EFFECT where record_index = \'%s\'", strIndex.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// ��Ϊrecord_index��UNIQUE�ģ����Բ����ܳ��ֶ���һ����¼
		if ( nRow > 1)
		{
			strErr += "���ֶ����¼��record_indexֵΪ" + strIndex + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			int nSeq = nColumn;
			objEffect.SetPerson( dbResult[ nSeq++]);	
			objEffect.SetLevel( dbResult[ nSeq++]);
			objEffect.SetDetail( dbResult[ nSeq++]);
		}
	}else 
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);

	return bResult;
}

// �����ݿ���д��¼��������ʱ���������£����������²����¼
bool CDBInterface::WriteRecord( /*const*/ CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		bResult = false;
		strErr += "�޷���" + m_strDBName + "\n";
	}

	// ��ʼ����
	if ( sqlite3_exec( pDB, "begin transaction", NULL, NULL, NULL) == SQLITE_OK)
	{
		string strIndex;
		bResult &= ( bResult &WriteRecInfoByDateTime( pDB, objRecord, objRecord.GetCreateTime(), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetEventPt() && WriteEventByIndex( pDB, *(objRecord.GetEventPt()), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetFeedbackPt() && WriteFeedbackByIndex( pDB, *(objRecord.GetFeedbackPt()), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetEffectPt() && WriteEffectByIndex( pDB, *(objRecord.GetEffectPt()), strIndex, strErr));

		if ( bResult)		// �ύ����
		{
			if ( sqlite3_exec( pDB, "commit transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "�ύ����ʧ�ܣ�\n";
			}
		}else		// �ع�����
		{
			if ( sqlite3_exec( pDB, "rollback transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "�ع�����ʧ�ܣ�\n";
			}
		}

	}else
	{
		bResult = false;
	}

	// close db
	sqlite3_close(pDB);

	return bResult;
}

// �ж����ݿ����Ѵ��ھ�����ͬ����ʱ��ļ�¼
bool CDBInterface::HasRecordTimeExisted( sqlite3 *pDB, const string &strCreateTime)
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "select count(*) from RECORD where create_time = \'%s\'", strCreateTime.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, NULL) == SQLITE_OK)
	{	
		// �жϼ�¼����
		bResult = false;
		if ( nRow > 0 && dbResult[nColumn])
		{
			if( atoi( dbResult[nColumn]) > 0)
			{
				bResult = true;
			}
		}
	}else
	{
		bResult = false;
	}

	sqlite3_free_table(dbResult);
	return bResult;
}

// �ж����ݿ����Ƿ��Ѵ�����ͬ����
bool CDBInterface::HasIndexExisted( sqlite3 *pDB,  const string &strTablename, const string &strIndex)
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "select count(*) from \'%s\' where record_index = \'%s\'", strTablename.c_str(), strIndex.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, NULL) == SQLITE_OK)
	{	
		bResult = false;
		if ( nRow > 0 && dbResult[nColumn])
		{
			if( atoi( dbResult[nColumn]) > 0)
			{
				bResult = true;
			}
		}
	}else
	{
		bResult = false;
	}

	sqlite3_free_table(dbResult);
	return bResult;
}

// ���ݴ���ʱ�����/���Record������Ϣ������ȡ����ֵ
bool CDBInterface::WriteRecInfoByDateTime( sqlite3 *pDB, CRecord &objRecord, const time_t tCreateTime, string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "��������ݿ�ָ��Ϊ�գ�\n";
		return false;
	}

	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);
	string strSaveTime = CStringTimeConvert::_TimeToString( objRecord.GetSaveTime());
	char szValidFlag[10];
	itoa( objRecord.GetValidFlag(), szValidFlag, 10);

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasRecordTimeExisted( pDB, strCreateTime))		//  ������Ӧ��Ŀ
	{
		sprintf_s( szSql, "update RECORD set save_time=\'%s\',valid_flag=\'%s\',modifier=\'%s\' where create_time = \'%s\'", 
			strSaveTime.c_str(), szValidFlag, objRecord.GetModifier().c_str(), strCreateTime.c_str());
	}else		// ��������Ŀ
	{
		sprintf_s( szSql, "insert into RECORD(create_time,save_time,valid_flag,modifier) values(\'%s\',\'%s\',\'%s\',\'%s\')", 
			strCreateTime.c_str(), strSaveTime.c_str(), szValidFlag, objRecord.GetModifier().c_str());
	}
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) == SQLITE_OK)
	{
		// ��ȡ����ֵ
		string strNewIndex;
		if ( !ReadIndexByDateTime( pDB, tCreateTime, strNewIndex, strErr))
		{
			bResult = false;
			strErr += "���¼�¼������Ϣ�󣬻�ȡ����ֵʧ�ܣ�\n";
		}else
		{
			strIndex = strNewIndex;
		}
	}else
	{
		bResult = false;
		strErr += "����/��Ӽ�¼����������Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵ����/���event��Ϣ
bool CDBInterface::WriteEventByIndex( sqlite3 *pDB, CEvent &objEvent, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	string strEventTime = CStringTimeConvert::_TimeToString( objEvent.GetEventTime());
	if ( HasIndexExisted( pDB, "EVENT", strIndex))		//  ������Ӧ��Ŀ
	{
		sprintf_s( szSql, "update EVENT set event_time=\'%s\',type=\'%s\,title=\'%s\',detail=\'%s\' where record_index = \'%s\'", 
			strEventTime.c_str(), objEvent.GetType().c_str(), objEvent.GetTitle().c_str(), objEvent.GetDetail().c_str(), strIndex.c_str());
	}else		// ��������Ŀ
	{
		sprintf_s( szSql, "insert into EVENT(record_index,event_time,type,title,detail) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), strEventTime.c_str(), objEvent.GetType().c_str(), objEvent.GetTitle().c_str(), objEvent.GetDetail().c_str());
	}
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "����/����¼���Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵ����/���Feedback��Ϣ
bool CDBInterface::WriteFeedbackByIndex( sqlite3 *pDB, CFeedback &objFeedback, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasIndexExisted( pDB, "FEEDBACK", strIndex))		//  ������Ӧ��Ŀ
	{
		sprintf_s( szSql, "update FEEDBACK set actor=\'%s\',attitude=\'%s\,adv_type=\'%s\',adv_detail=\'%s\' where record_index = \'%s\'", 
			objFeedback.GetPerson().c_str(), objFeedback.GetAttitude().c_str(), objFeedback.GetType().c_str(), objFeedback.GetDetail().c_str(), strIndex.c_str());
	}else		// ��������Ŀ
	{
		sprintf_s( szSql, "insert into FEEDBACK(record_index,actor,attitude,adv_type,adv_detail) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), objFeedback.GetPerson().c_str(), objFeedback.GetAttitude().c_str(), objFeedback.GetType().c_str(), objFeedback.GetDetail().c_str());
	}
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "����/��ӷ�����Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵ����/���Effect��Ϣ
bool CDBInterface::WriteEffectByIndex( sqlite3 *pDB, CEffect &objEffect, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasIndexExisted( pDB, "EFFECT", strIndex))		//  ������Ӧ��Ŀ
	{
		sprintf_s( szSql, "update EFFECT set actor=\'%s\',level=\'%s\',detail=\'%s\' where record_index = \'%s\'", 
			objEffect.GetPerson().c_str(), objEffect.GetLevel().c_str(), objEffect.GetDetail().c_str(), strIndex.c_str());
	}else		// ��������Ŀ
	{
		sprintf_s( szSql, "insert into EFFECT(record_index,actor,level,detail) values(\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), objEffect.GetPerson().c_str(), objEffect.GetLevel().c_str(), objEffect.GetDetail().c_str());
	}
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "����/���ִ����Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ɾ����¼
bool CDBInterface::DelRecord( const time_t tCreateTime, string &strErr )
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		bResult = false;
		strErr += "�޷���" + m_strDBName + "\n";
	}

	// ��ʼ����
	if ( sqlite3_exec( pDB, "begin transaction", NULL, NULL, NULL) == SQLITE_OK)
	{		
		string strIndex;
		bResult &= ( bResult && DelRecInfoByDateTime( pDB, tCreateTime, strIndex, strErr));
		bResult &= ( bResult && DelEventByIndex( pDB, strIndex, strErr));
		bResult &= ( bResult && DelFeedbackByIndex( pDB, strIndex, strErr));
		bResult &= ( bResult && DelEffectByIndex( pDB, strIndex, strErr));

		if ( bResult)		// �ύ����
		{
			if ( sqlite3_exec( pDB, "commit transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "�ύ����ʧ�ܣ�\n";
			}
		}else		// �ع�����
		{
			if ( sqlite3_exec( pDB, "rollback transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "�ع�����ʧ�ܣ�\n";
			}
		}

	}else
	{
		bResult = false;
	}

	// close db
	sqlite3_close(pDB);

	return bResult;
}

// ��������ֵɾ��Record������Ϣ
bool CDBInterface::DelRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, string &strIndex, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	// ��ȡ����ֵ
	string strNewIndex;
	if ( !ReadIndexByDateTime( pDB, tCreateTime, strNewIndex, strErr))
	{
		bResult = false;
		strErr += "���¼�¼������Ϣ�󣬻�ȡ����ֵʧ�ܣ�\n";
	}else
	{
		strIndex = strNewIndex;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];
	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);
	sprintf_s( szSql, "delete from RECORD where create_time = \'%s\'", strCreateTime.c_str());
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "ɾ����¼������Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵɾ��Event��Ϣ
bool CDBInterface::DelEventByIndex( sqlite3 *pDB, const string &strIndex, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "delete from EVENT where record_index = \'%s\'", strIndex.c_str());
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "ɾ���¼���Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵɾ��Feedback��Ϣ
bool CDBInterface::DelFeedbackByIndex( sqlite3 *pDB, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "delete from FEEDBACK where record_index = \'%s\'", strIndex.c_str());
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "ɾ��������Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��������ֵɾ��Effect��Ϣ
bool CDBInterface::DelEffectByIndex( sqlite3 *pDB, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];
	sprintf_s( szSql, "delete from EFFECT where record_index = \'%s\'", strIndex.c_str());
	// ִ��sql���
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "ɾ��ִ����Ϣʧ�ܣ�\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// ��ȡ�������ݿ������м�¼��ժҪ��Ϣ
bool CDBInterface::ReadAllRecordAbstract( vector<CBriefInfo> &vecAbstracts, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "�޷���" + m_strDBName + "\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;

	// ��ȡ���м�¼�Ĵ���ʱ���ʱ�����
	char szSql[200] = "select RECORD.create_time,EVENT.title from RECORD,EVENT where RECORD.record_index=EVENT.record_index";
	//��3������dbResult�ǲ�ѯ���������һά���顣���ڴ沼���ǣ���һ�����ֶ����ƣ������ǽ�������ÿ���ֶε�ֵ�����д洢
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{		
		for ( int i = 0; i < nRow; i++)
		{
			int nSeq = ( i + 1) * nColumn;
			time_t tCreateTime;
			char * pTime = dbResult[ nSeq++];
			char * pTitle = dbResult[ nSeq++];
			if ( CStringTimeConvert::_StringToTime( pTime,tCreateTime)
				&& pTitle)
			{
				CBriefInfo objInfo;
				objInfo.SetCreateTime(tCreateTime);
				objInfo.SetAbstract(pTitle);

				vecAbstracts.push_back(objInfo);
			}else			// ����ʱ����¼����ⲻ��Ϊ��
			{
				strErr += "��ȡ����ʱ����¼�����ʧ�ܣ�\n";
				bResult = false;
				break;
			}
		}		
	}else
	{
		bResult = false;
		if (errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	sqlite3_free_table(dbResult);
	sqlite3_close(pDB);

	return bResult;
}

CDBInterface & CDBInterface::Instance()
{
	static CDBInterface objInstance;
	return objInstance;
}