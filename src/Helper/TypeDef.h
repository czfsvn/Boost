#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <boost/lexical_cast.hpp>
//#include <string>



typedef __int64   int64;
typedef __int32   int32;
typedef __int16   int16;
typedef __int8    int8;
typedef unsigned __int64  uint64;
typedef unsigned __int32  uint32;
typedef unsigned __int16  uint16;
typedef unsigned __int8   uint8;

typedef const char* CCharPtr;



template<typename T1, typename T2> inline T1 Boost_Lexical(T1& des, T2& res) 
{
	return boost::lexical_cast<T1>(res);
}

std::string IntBStr(uint16 num)
{
	std::string res;
	return Boost_Lexical(res, num);
}

std::string IntBStr(uint32 num)
{
	std::string res;
	return Boost_Lexical(res, num);
}

std::string IntBStr(uint64 num)
{
	std::string res;
	return  Boost_Lexical(res, num);
}

std::string IntBStr(int16 num)
{
	std::string res;
	return Boost_Lexical(res, num);
}

std::string IntBStr(int32 num)
{
	std::string res;
	return Boost_Lexical(res, num);
}

std::string IntBStr(int64 num)
{
	std::string res;
	return Boost_Lexical(res, num);
}

uint32 StrBUInt(std::string res)
{
	uint32 des = 0;
	return Boost_Lexical(des, res);
}

uint16 StrBUInt16(std::string res)
{
	uint16 des = 0;
	return Boost_Lexical(des, res);
}

uint64 StrBUInt64(std::string res)
{
	uint64 des = 0;
	return Boost_Lexical(des, res);
}

int32 StrBInt(std::string res)
{
	int32 des = 0;
	return Boost_Lexical(des, res);
}

int16 StrBInt16(std::string res)
{
	int16 des = 0;
	return Boost_Lexical(des, res);
}

int64 StrBInt64(std::string res)
{
	int64 des = 0;
	return Boost_Lexical(des, res);
}

int32 StrBInt(CCharPtr res)
{
	return res ? StrBInt((std::string)res) : 0;
}

int16 StrBInt16(CCharPtr res)
{
	return res ? StrBInt16((std::string)res) : 0;
}

int64 StrBInt64(CCharPtr res)
{
	return res ? StrBInt64((std::string)res) : 0;
}

uint32 StrBUInt(CCharPtr res)
{
	return res ? StrBUInt((std::string)res) : 0;
}

uint16 StrBUInt16(CCharPtr res)
{
	return res ? StrBUInt16((std::string)res) : 0;
}

uint64 StrBUInt64(CCharPtr res)
{
	return res ? StrBUInt64((std::string)res) : 0;
}

#endif