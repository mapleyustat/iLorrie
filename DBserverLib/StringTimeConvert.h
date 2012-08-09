/************************************************************************
*						 StringTimeConver.h
*						定义字符串到时间类型的相关转换
*						weigaofeng  2012-1-13
************************************************************************/

#pragma once

#include <string>

using namespace std;

/************************************************************************
*						CStringTimeConvert	时间转换类
*					         提供时间转换读写接口
*                                         
************************************************************************/
class CStringTimeConvert
{
public:
	static bool		_StringToTime( const char *szTime, time_t &tTime);
	static string	_TimeToString( const time_t &tTime);
};
