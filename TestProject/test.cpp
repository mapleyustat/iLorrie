#include "RecordData.h"
#include "ConfigData.h"
#include "DBInterface.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void main()
{
	string strError = "";
	
	string strDBName = ".\\Data\\RecordDB.db3";

	CDBInterface::Instance().InitDBName(strDBName);
	vector<time_t>	vecTime;
	CDBInterface::Instance().ReadAllRecordTime( vecTime, strError);
	cout<<"创建日期个数: "<<vecTime.size()<<endl;

}