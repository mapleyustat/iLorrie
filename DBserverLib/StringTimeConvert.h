/************************************************************************
*						 StringTimeConver.h
*						�����ַ�����ʱ�����͵����ת��
*						weigaofeng  2012-1-13
************************************************************************/

#pragma once

#include <string>

using namespace std;

/************************************************************************
*						CStringTimeConvert	ʱ��ת����
*					         �ṩʱ��ת����д�ӿ�
*                                         
************************************************************************/
class CStringTimeConvert
{
public:
	static bool		_StringToTime( const char *szTime, time_t &tTime);
	static string	_TimeToString( const time_t &tTime);
};
