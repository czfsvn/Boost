#include "Config.h"


#include <boost/algorithm/string.hpp>
#include <boost/typeof/typeof.hpp>


Config::Config(const char* Name)
{
	_ReadContent(Name);
}

Config::~Config()
{

}

void Config::_ReadContent(const char* file)
{
	m_allbufMap.clear();
	if (!file)
		return;

	FILE * f = fopen(file, "r");
	fseek(f, 0, SEEK_END);
	uint32 leng = ftell(f);
	char* buf = new char[leng + 1];
	fseek(f, 0, SEEK_SET);

	fread(buf, leng, 1, f);
	string buffer = string(buf);
	delete []buf;

	fclose(f);

	std::vector<string> strVec;
	split(strVec, buffer, is_any_of("<>"));
	for (uint16 idx = 0; idx < strVec.size(); idx++)
	{
		std::vector<string> strVec1;
		split(strVec1, strVec[idx], is_any_of(" "));
		if (strVec1.size() == 0)
			continue;
		//m_allbufMap.insert()
		std::string header = strVec1[0];
		std::vector<pair<const char*, const char*>>* vec = new std::vector<pair<const char*, const char*>>;
		m_allbufMap.left.insert(make_pair(header.c_str(), vec));

		for (uint16 i = 1; i < strVec1.size(); i++)
		{
			std::vector<string> strVec2;
			split(strVec2, strVec1[i], is_any_of("="));
			vec->push_back(std::make_pair(strVec2[0].c_str(), strVec2[1].c_str()));
		}
	}
}

const char* Config::_GetParam(const char* firstParam, const char* secParam)
{
	bimap<const char*, std::vector<pair<const char*, const char*>>*>::left_iterator iter = m_allbufMap.left.find(firstParam);
	if (iter != m_allbufMap.left.end())
	{
		std::vector<pair<const char*, const char*>>* vec = iter->second;
		if (vec)
		{
			for (BOOST_AUTO(pos, vec->begin()); pos != vec->end(); pos++)
			{
				if ((*(pos->first)) == secParam)
				{
					return &(*(pos->second));
				}
			}
			return NULL;
		}
		return NULL;
	}
	return NULL;
}