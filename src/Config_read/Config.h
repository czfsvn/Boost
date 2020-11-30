#ifndef __CONFIG_READ_20110813_H__
#define __CONFIG_READ_20110813_H__

#include "TypeDef.h"

#include <vector>

#include <boost/bimap.hpp>

using namespace std;
using namespace boost;

class Config
{
public:
	Config(const char* Name);
	~Config();


private:
	void _ReadContent(const char* Name);

	const char* _GetParam(const char* firstParam, const char* secParam);
protected:
private:	
	bimap<const char*, std::vector<pair<const char*, const char*>>*> m_allbufMap;
};

#endif