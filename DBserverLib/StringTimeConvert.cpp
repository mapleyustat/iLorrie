/************************************************************************
*							StringTimeConver.cpp
*						定义字符串到时间类型的相关转换
*						weigaofeng  2012-1-13
************************************************************************/
#include <time.h>
#include "StringTimeConvert.h"

#include <vector>

using namespace std;


/************************************************************************
*						CStringTimeConvert	时间转换类
*					         提供时间转换读写接口
*                                         
************************************************************************/
// 读取时间类型：2012-01-13 17:44:00( 事实上，该代码可兼容2012-01-13、2012-01-13 17:44和2012-01-13 17:44:00)
bool CStringTimeConvert::_StringToTime( const char *szTime, time_t &tTime)
{
	if ( szTime == NULL)
	{
		return false;
	}

	string strTime(szTime);
	int nBegin = 0;
	vector<int> vecNumber;

	while (1)
	{
		// 找到"-", " ", ":"中的任何一个都作为分隔符
		int nPos = strTime.find_first_of( "-", nBegin);
		if ( nPos == -1)
		{
			nPos = strTime.find_first_of( " ", nBegin);
			if ( nPos == -1)
			{
				nPos = strTime.find_first_of( ":", nBegin);
			}
		}

		if ( nPos == -1)	// 是否搜索到了最后一个可识别的标识符
		{
			string strEnd = strTime.substr( nBegin, strTime.size() - nBegin);
			vecNumber.push_back( atoi( strEnd.c_str()));
			break;
		}else
		{
			string strValue = strTime.substr( nBegin, nPos - nBegin);
			vecNumber.push_back( atoi( strValue.c_str()));
			nBegin = nPos + 1;
		}		
		
	}

// 	struct tm {
// 		int tm_sec;     /* seconds after the minute - [0,59] */
// 		int tm_min;     /* minutes after the hour - [0,59] */
// 		int tm_hour;    /* hours since midnight - [0,23] */
// 		int tm_mday;    /* day of the month - [1,31] */
// 		int tm_mon;     /* months since January - [0,11] */
// 		int tm_year;    /* years since 1900 */
// 		int tm_wday;    /* days since Sunday - [0,6] */
// 		int tm_yday;    /* days since January 1 - [0,365] */
// 		int tm_isdst;   /* daylight savings time flag */
// 	};
	struct tm sourcedate;
	memset( (void *)&sourcedate, 0, sizeof(sourcedate));

	if ( vecNumber.size() < 3 || vecNumber.size() > 6)
	{
		return false;
	}else			// 年月日
	{
		sourcedate.tm_year = vecNumber[0] - 1900;
		sourcedate.tm_mon = vecNumber[1] - 1;
		sourcedate.tm_mday = vecNumber[2];
	}

	// 时分秒
	if ( vecNumber.size() >= 4)
	{
		sourcedate.tm_hour = vecNumber[3];
	}
	if ( vecNumber.size() >= 5)
	{
		sourcedate.tm_min = vecNumber[4];
	}
	if ( vecNumber.size() == 6)
	{
		sourcedate.tm_sec = vecNumber[5];
	}

	tTime = mktime(&sourcedate);

	return true;
}

// 写入时间类型：2012-1-13 17:44:03
string CStringTimeConvert::_TimeToString( const time_t &tTime)
{
	struct tm *pm;
	pm = ::localtime(&tTime);

	char szBuff[50];
	sprintf_s(szBuff,"%04d-%02d-%02d %02d:%02d:%02d",
		pm->tm_year + 1900,
		pm->tm_mon + 1,
		pm->tm_mday,
		pm->tm_hour,
		pm->tm_min,
		pm->tm_sec);

	return string(szBuff);
}