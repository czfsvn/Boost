#include <iostream>

#include <boost/crc.hpp>


#pragma region CRC_1

void TestCrc_1()
{
	boost::crc_32_type crc32;
	std::cout<<std::hex;
	std::cout<<crc32.checksum()<<std::endl;
	crc32.process_byte('a');
	std::cout<<crc32.checksum()<<std::endl;
	crc32.process_bytes("1234567890", 10);
	std::cout<<crc32.checksum()<<std::endl;

	char szCh[] = "1234567890";
	crc32.reset();
	crc32.process_block(szCh, szCh + 10);
	std::cout<<crc32.checksum()<<std::endl;
}

#pragma endregion CRC_1

#pragma region CRC_2

void TestCrc_2()
{
	boost::crc_32_type crc32;
	std::cout<<std::hex;

	crc32.process_bytes("1234567890", 10);
	std::cout<<crc32()<<std::endl;		//<==>std::cout<<crc32.checksum()<<std::endl;	

	std::string str = "1234567890";
	crc32.reset();
	std::cout<<std::for_each(str.begin(), str.end(), crc32)()<<std::endl;
	std::cout<<crc32()<<std::endl;
}

#pragma endregion CRC_2


void main()
{
	TestCrc_1();
}