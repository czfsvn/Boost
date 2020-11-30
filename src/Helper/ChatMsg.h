#ifndef __CHATMSG_20120313_H__
#define __CHATMSG_20120313_H__

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ChatMsg
{
public:
	enum
	{
		Header_Length		= 4
	};

	enum 
	{
		Max_Body_Length		= 512
	};

	ChatMsg() : m_bodyLength(0)
	{

	}

	const char* Data() const
	{
		return m_data;
	}

	char* Data()
	{
		return m_data;
	}

	size_t Length() const
	{
		return Header_Length + m_bodyLength;
	}

	const char* Body() const
	{
		return m_data + Header_Length;
	}

	char* Body()
	{
		return m_data + Header_Length;
	}

	size_t BodyLength() const
	{
		return m_bodyLength;
	}

	void BodyLength(size_t length)
	{
		m_bodyLength = length;
		if (m_bodyLength > Max_Body_Length)
		{
			m_bodyLength = Max_Body_Length;
		}
	}

	bool DecodeHeader()
	{
		using namespace std;
		char header[Header_Length + 1] = "";
		strncat(header, m_data, Header_Length);
		m_bodyLength = atoi(header);
		if (m_bodyLength > Max_Body_Length)
		{
			m_bodyLength = 0;
			return false;
		}
		return true;
	}

	void EncodeHeader()
	{
		using namespace std;
		char header[Header_Length + 1] = "";
		sprintf(header, "%4d", m_bodyLength);
		memcpy(m_data, header, Header_Length);
	}



protected:
private:
	char m_data[Header_Length + Max_Body_Length];
	size_t m_bodyLength;
};

#endif