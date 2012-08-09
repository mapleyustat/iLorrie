/************************************************************************
*							DBInterface.h
*						�������ݿ����ز���
*						weigaofeng  2012-1-13
************************************************************************/

#pragma once

#include <vector>
#include <string>

using namespace std;

/************************************************************************
*						CDBInterface	���ݿ�ӿ���
*					         �ṩ���ݿ��д�ӿ�
*	Remark:	1.�����ݿ���datetime�ֶ�Ŀǰֻ��ȷ�����Ӽ������������������һ��Ϊ00���ݶ�
*			2.����ʱ�䣨������sql���ֹ��ġ�����ִ�У������� "YYYY-mm-dd HH:MM:SS")�ķ�ʽ��ȫ��д
*
************************************************************************/
class sqlite3;
class CRecord;
class CEvent;
class CFeedback;
class CEffect;
class CBriefInfo;
class CDBInterface
{
public:
	static CDBInterface & Instance();

	~CDBInterface();

	void	InitDBName( const string strDBName);

	bool	ReadAllRecordTime( vector<time_t> &vecTime, string &strErr);
	bool	ReadAllRecordAbstract( vector<CBriefInfo> &vecAbstracts, string &strErr);

	bool	ReadRecord( const time_t tCreateTime, CRecord &objRecord, string &strErr);
	bool	WriteRecord( /*const*/ CRecord &objRecord, string &strErr);
	bool	DelRecord( const time_t tCreateTime, string &strErr);
	
protected:
	bool	ReadRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, CRecord &objRecord, string &strErr);
	bool	ReadIndexByDateTime( sqlite3 *pDB, const time_t &tCreateTime, string &strIndex, string &strErr);
	bool	ReadEventByIndex( sqlite3 *pDB, const string &strIndex, CEvent &objEvent, string &strErr);
	bool	ReadFeedbackByIndex( sqlite3 *pDB, const string &strIndex, CFeedback &objFeedback, string &strErr);
	bool	ReadEffectByIndex( sqlite3 *pDB, const string &strIndex, CEffect &objEffect, string &strErr);

	bool	WriteRecInfoByDateTime( sqlite3 *pDB, CRecord &objRecord, const time_t tCreateTime, string &strIndex, string &strErr);
	bool	WriteEventByIndex( sqlite3 *pDB, CEvent &objEvent, const string &strIndex, string &strErr);
	bool	WriteFeedbackByIndex( sqlite3 *pDB, CFeedback &objFeedback, const string &strIndex, string &strErr);
	bool	WriteEffectByIndex( sqlite3 *pDB, CEffect &objEffect, const string &strIndex, string &strErr);
	bool	HasRecordTimeExisted( sqlite3 *pDB, const string &strCreateTime);
	bool	HasIndexExisted( sqlite3 *pDB, const string &strTablename, const string &strIndex);

	bool	DelRecInfoByDateTime( sqlite3 *pDB, const time_t tCreateTime, string &strIndex, string &strErr);
	bool	DelEventByIndex( sqlite3 *pDB, const string &strIndex, string &strErr);
	bool	DelFeedbackByIndex( sqlite3 *pDB, const string &strIndex, string &strErr);
	bool	DelEffectByIndex( sqlite3 *pDB, const string &strIndex, string &strErr);

private:
	CDBInterface();

	string	m_strDBName;
}; 