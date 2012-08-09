/************************************************************************
*							DBInterface.cpp
*						定义数据库的相关操作
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
*						CDBInterface	数据库接口类
*					         提供数据库读写接口
*                                         
************************************************************************/

CDBInterface::CDBInterface()
{
}

CDBInterface::~CDBInterface()
{
}

// 初始化数据库名称
void CDBInterface::InitDBName( const string strDBName )
{
	m_strDBName = strDBName;
}

// 读取记录表中的所有索引的创建时间
bool CDBInterface::ReadAllRecordTime( vector<time_t> &vecTime, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "无法打开" + m_strDBName + "\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;

	char szSql[SQL_LENGTH] = "select create_time from RECORD";
	//第3个参数dbResult是查询结果，它是一维数组。它内存布局是：第一行是字段名称，后面是紧接着是每个字段的值。按行存储
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{		
		for ( int i = 0; i < nRow; i++)
		{
			int nSeq = ( i + 1) * nColumn;
			time_t tCreateTime;
			if ( CStringTimeConvert::_StringToTime( dbResult[nSeq++],tCreateTime))
			{
				vecTime.push_back(tCreateTime);
			}else			// 创建时间不能为空
			{
				strErr += "获取创建时间失败！\n";
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

// 根据创建时间读取整个记录内容
bool CDBInterface::ReadRecord( const time_t tCreateTime, CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "无法打开" + m_strDBName + "\n";
		bResult = false;
	}

	if ( bResult && !ReadRecInfoByDateTime( pDB, tCreateTime, objRecord, strErr))
	{
		strErr += "根据创建时间获取记录自身信息失败！\n";
		bResult = false;
	}

	string strIndex("");
	if ( bResult && !ReadIndexByDateTime( pDB, tCreateTime, strIndex, strErr))
	{
		strErr += "根据创建时间获取索引信息失败！\n";
		bResult = false;
	}
	
	CEvent objEvent;
	if ( bResult && !ReadEventByIndex( pDB, strIndex, objEvent, strErr))
	{
		strErr += "根据索引信息获取事件信息失败！\n";
		bResult = false;
	}

	CFeedback objFeedback;
	if ( bResult && !ReadFeedbackByIndex( pDB, strIndex, objFeedback, strErr))
	{
		strErr += "根据索引信息获取反馈信息失败！\n";
		bResult = false;
	}

	CEffect objEffect;
	if ( bResult && !ReadEffectByIndex( pDB, strIndex, objEffect, strErr))
	{
		strErr += "根据索引信息获取执行效果信息失败！\n";
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

// 根据索引时间读取索引数
bool CDBInterface::ReadIndexByDateTime( sqlite3 *pDB, const time_t &tCreateTime, string &strIndex, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
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
		// 因为create_time是UNIQUE的，所以不可能出现多于一个记录
		if ( nRow > 1)
		{
			strErr += "出现多个记录的create_time值为" + strCreateTime + "\n";
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

// 根据创建时间读取Record的自身信息
bool CDBInterface::ReadRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;
	char szSql[SQL_LENGTH];
	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);

//	此处sprintf_s之所以出现过异常，是因为里面存在诸多的%输出符号，以至于正常的%s输出被%Y等屏蔽而出错
//	sprintf_s( szSql,"select strftime(\'%Y-%m-%d %H:%M:%S\',save_time),modifier from RECORD where strftime(\'%Y-%m-%d %H:%M:%S\',create_time) = \'%s\'",strCreateTime.c_str());
	
	sprintf_s( szSql,"select save_time,valid_flag,modifier from RECORD where create_time = \'%s\'",strCreateTime.c_str());
	if ( sqlite3_get_table( pDB, szSql, &dbResult, &nRow, &nColumn, &errmsg) == SQLITE_OK)
	{	
		// 因为create_time是UNIQUE的，所以不可能出现多于一个记录
		if ( nRow > 1)
		{
			strErr += "出现多个记录的create_time值为" + strCreateTime + "\n";
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

// 根据索引时间读取Event信息
bool CDBInterface::ReadEventByIndex( sqlite3 *pDB, const string &strIndex, CEvent &objEvent, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
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
		// 因为record_index是UNIQUE的，所以不可能出现多于一个记录
		if ( nRow > 1)
		{
			strErr += "出现多个记录的record_index值为" + strIndex + "\n";
			bResult = false;
		}else if ( nRow == 1)
		{
			time_t tEventTime;
			int nSeq = nColumn;
			if ( CStringTimeConvert::_StringToTime( dbResult[ nSeq++], tEventTime))	// 前nColumn个是字段名
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

// 根据索引时间读取Feedback信息
bool CDBInterface::ReadFeedbackByIndex( sqlite3 *pDB, const string &strIndex, CFeedback &objFeedback, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
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
		// 因为record_index是UNIQUE的，所以不可能出现多于一个记录
		if ( nRow > 1)
		{
			strErr += "出现多个记录的record_index值为" + strIndex + "\n";
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

// 根据索引时间读取Effect信息
bool CDBInterface::ReadEffectByIndex( sqlite3 *pDB, const string &strIndex, CEffect &objEffect, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
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
		// 因为record_index是UNIQUE的，所以不可能出现多于一个记录
		if ( nRow > 1)
		{
			strErr += "出现多个记录的record_index值为" + strIndex + "\n";
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

// 向数据库中写记录，若创建时间存在则更新，不存在则新插入记录
bool CDBInterface::WriteRecord( /*const*/ CRecord &objRecord, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		bResult = false;
		strErr += "无法打开" + m_strDBName + "\n";
	}

	// 开始事务
	if ( sqlite3_exec( pDB, "begin transaction", NULL, NULL, NULL) == SQLITE_OK)
	{
		string strIndex;
		bResult &= ( bResult &WriteRecInfoByDateTime( pDB, objRecord, objRecord.GetCreateTime(), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetEventPt() && WriteEventByIndex( pDB, *(objRecord.GetEventPt()), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetFeedbackPt() && WriteFeedbackByIndex( pDB, *(objRecord.GetFeedbackPt()), strIndex, strErr));
		bResult &= ( bResult && objRecord.GetEffectPt() && WriteEffectByIndex( pDB, *(objRecord.GetEffectPt()), strIndex, strErr));

		if ( bResult)		// 提交事务
		{
			if ( sqlite3_exec( pDB, "commit transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "提交事务失败！\n";
			}
		}else		// 回滚事务
		{
			if ( sqlite3_exec( pDB, "rollback transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "回滚事务失败！\n";
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

// 判断数据库中已存在具有相同创建时间的记录
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
		// 判断记录个数
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

// 判断数据库中是否已存在相同索引
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

// 根据创建时间更新/添加Record自身信息，并获取索引值
bool CDBInterface::WriteRecInfoByDateTime( sqlite3 *pDB, CRecord &objRecord, const time_t tCreateTime, string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		strErr += "传入的数据库指针为空！\n";
		return false;
	}

	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);
	string strSaveTime = CStringTimeConvert::_TimeToString( objRecord.GetSaveTime());
	char szValidFlag[10];
	itoa( objRecord.GetValidFlag(), szValidFlag, 10);

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasRecordTimeExisted( pDB, strCreateTime))		//  更新相应条目
	{
		sprintf_s( szSql, "update RECORD set save_time=\'%s\',valid_flag=\'%s\',modifier=\'%s\' where create_time = \'%s\'", 
			strSaveTime.c_str(), szValidFlag, objRecord.GetModifier().c_str(), strCreateTime.c_str());
	}else		// 插入新条目
	{
		sprintf_s( szSql, "insert into RECORD(create_time,save_time,valid_flag,modifier) values(\'%s\',\'%s\',\'%s\',\'%s\')", 
			strCreateTime.c_str(), strSaveTime.c_str(), szValidFlag, objRecord.GetModifier().c_str());
	}
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) == SQLITE_OK)
	{
		// 获取索引值
		string strNewIndex;
		if ( !ReadIndexByDateTime( pDB, tCreateTime, strNewIndex, strErr))
		{
			bResult = false;
			strErr += "更新记录索引信息后，获取索引值失败！\n";
		}else
		{
			strIndex = strNewIndex;
		}
	}else
	{
		bResult = false;
		strErr += "更新/添加记录索引自身信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值更新/添加event信息
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
	if ( HasIndexExisted( pDB, "EVENT", strIndex))		//  更新相应条目
	{
		sprintf_s( szSql, "update EVENT set event_time=\'%s\',type=\'%s\,title=\'%s\',detail=\'%s\' where record_index = \'%s\'", 
			strEventTime.c_str(), objEvent.GetType().c_str(), objEvent.GetTitle().c_str(), objEvent.GetDetail().c_str(), strIndex.c_str());
	}else		// 插入新条目
	{
		sprintf_s( szSql, "insert into EVENT(record_index,event_time,type,title,detail) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), strEventTime.c_str(), objEvent.GetType().c_str(), objEvent.GetTitle().c_str(), objEvent.GetDetail().c_str());
	}
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "更新/添加事件信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值更新/添加Feedback信息
bool CDBInterface::WriteFeedbackByIndex( sqlite3 *pDB, CFeedback &objFeedback, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasIndexExisted( pDB, "FEEDBACK", strIndex))		//  更新相应条目
	{
		sprintf_s( szSql, "update FEEDBACK set actor=\'%s\',attitude=\'%s\,adv_type=\'%s\',adv_detail=\'%s\' where record_index = \'%s\'", 
			objFeedback.GetPerson().c_str(), objFeedback.GetAttitude().c_str(), objFeedback.GetType().c_str(), objFeedback.GetDetail().c_str(), strIndex.c_str());
	}else		// 插入新条目
	{
		sprintf_s( szSql, "insert into FEEDBACK(record_index,actor,attitude,adv_type,adv_detail) values(\'%s\',\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), objFeedback.GetPerson().c_str(), objFeedback.GetAttitude().c_str(), objFeedback.GetType().c_str(), objFeedback.GetDetail().c_str());
	}
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "更新/添加反馈信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值更新/添加Effect信息
bool CDBInterface::WriteEffectByIndex( sqlite3 *pDB, CEffect &objEffect, const string &strIndex, string &strErr )
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];

	if ( HasIndexExisted( pDB, "EFFECT", strIndex))		//  更新相应条目
	{
		sprintf_s( szSql, "update EFFECT set actor=\'%s\',level=\'%s\',detail=\'%s\' where record_index = \'%s\'", 
			objEffect.GetPerson().c_str(), objEffect.GetLevel().c_str(), objEffect.GetDetail().c_str(), strIndex.c_str());
	}else		// 插入新条目
	{
		sprintf_s( szSql, "insert into EFFECT(record_index,actor,level,detail) values(\'%s\',\'%s\',\'%s\',\'%s\')", 
			strIndex.c_str(), objEffect.GetPerson().c_str(), objEffect.GetLevel().c_str(), objEffect.GetDetail().c_str());
	}
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "更新/添加执行信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 删除记录
bool CDBInterface::DelRecord( const time_t tCreateTime, string &strErr )
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	// open db
	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		bResult = false;
		strErr += "无法打开" + m_strDBName + "\n";
	}

	// 开始事务
	if ( sqlite3_exec( pDB, "begin transaction", NULL, NULL, NULL) == SQLITE_OK)
	{		
		string strIndex;
		bResult &= ( bResult && DelRecInfoByDateTime( pDB, tCreateTime, strIndex, strErr));
		bResult &= ( bResult && DelEventByIndex( pDB, strIndex, strErr));
		bResult &= ( bResult && DelFeedbackByIndex( pDB, strIndex, strErr));
		bResult &= ( bResult && DelEffectByIndex( pDB, strIndex, strErr));

		if ( bResult)		// 提交事务
		{
			if ( sqlite3_exec( pDB, "commit transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "提交事务失败！\n";
			}
		}else		// 回滚事务
		{
			if ( sqlite3_exec( pDB, "rollback transaction", NULL, NULL, NULL) != SQLITE_OK)
			{
				bResult = false;
				strErr += "回滚事务失败！\n";
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

// 根据索引值删除Record自身信息
bool CDBInterface::DelRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, string &strIndex, string &strErr)
{
	bool bResult = true;
	if ( !pDB)
	{
		return false;
	}

	// 获取索引值
	string strNewIndex;
	if ( !ReadIndexByDateTime( pDB, tCreateTime, strNewIndex, strErr))
	{
		bResult = false;
		strErr += "更新记录索引信息后，获取索引值失败！\n";
	}else
	{
		strIndex = strNewIndex;
	}

	char * errmsg = NULL;
	char szSql[SQL_LENGTH];
	string strCreateTime = CStringTimeConvert::_TimeToString( tCreateTime);
	sprintf_s( szSql, "delete from RECORD where create_time = \'%s\'", strCreateTime.c_str());
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "删除记录索引信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值删除Event信息
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
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "删除事件信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值删除Feedback信息
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
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "删除反馈信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 根据索引值删除Effect信息
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
	// 执行sql语句
	if ( sqlite3_exec( pDB, szSql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		bResult = false;
		strErr += "删除执行信息失败！\n";
		if ( errmsg)
		{
			strErr += string(errmsg) + "\n";
			sqlite3_free(errmsg);
		}
	}

	return bResult;
}

// 读取所有数据库中所有记录的摘要信息
bool CDBInterface::ReadAllRecordAbstract( vector<CBriefInfo> &vecAbstracts, string &strErr)
{
	bool bResult = true;
	sqlite3 * pDB = NULL;

	if( sqlite3_open( m_strDBName.c_str(), &pDB ) != SQLITE_OK )
	{
		strErr += "无法打开" + m_strDBName + "\n";
		return false;
	}

	char * errmsg = NULL;
	char **dbResult;
	int nRow = 0;
	int nColumn = 0;

	// 读取所有记录的创建时间和时间标题
	char szSql[200] = "select RECORD.create_time,EVENT.title from RECORD,EVENT where RECORD.record_index=EVENT.record_index";
	//第3个参数dbResult是查询结果，它是一维数组。它内存布局是：第一行是字段名称，后面是紧接着是每个字段的值。按行存储
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
			}else			// 创建时间和事件标题不能为空
			{
				strErr += "获取创建时间或事件标题失败！\n";
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