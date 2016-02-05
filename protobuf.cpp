// protobuf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdint>

using namespace std;

char protbuf1[] = {0x08, 0x96, 0x01};
char protbuf[] = {0x12, 0x07, 0x74, 0x65, 0x73, 0x74, 0x69, 0x6e, 0x67, 0x08, 0x96, 0x01, 0x1a, 0x03, 0x08, 0x96, 0x01, 0x09, 0x99, 0xa3, 0xb1, 0xc3, 0x99, 0xa3, 0xb1, 0x03};
typedef enum {
	TYPE_VARINT = 0,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_START_GROUP,
	TYPE_END_GROUP,
	TYPE_FLOAT,
	TYPE_UNKNOWN
} ProtoBufType;





// TLV of the form
//expect int a = 150;
void decoder(char *pbuf, size_t len)
{
	uint32_t field_no = 0;
	uint8_t idx = 0;
	uint32_t value = 0;
	char *ValString = NULL;
	char *pend = (pbuf+len);
	while (pbuf<pend)
	{
		//check the type of the key - val pair
		//3 bits wire type
		uint8_t buf  = *(pbuf++);
		uint8_t type = buf & 0x07;
		field_no     = buf >> 3;
		switch (type)
		{
			case TYPE_VARINT:
				//varint type parsing
				cout << "Field no  : " << field_no << endl;
				cout << "TYPE      : VARINT" <<endl;
				//now get the value
				buf  = *(pbuf++);
				
				while (buf & 0x80)
				{
					//there are more bytes ?
					value |= ((buf & 0x7F) << 7*(idx++));
					buf  = *(pbuf++);
				}
				//add the last byte also 
				value |= (buf << 7*idx);
				cout << "VALUE      : " << value << endl;
				break;
			case TYPE_DOUBLE:
				cout << "Field no  : " << field_no << endl;
				cout << "TYPE      : DOUBLE" <<endl;
				double val;
				memcpy(&val, pbuf, sizeof(double));
				cout << "VALUE      : " << val << endl;

				break;
			case TYPE_STRING:
				//string type parsing
				cout << "Field no  : " << field_no << endl;
				cout << "TYPE      : STRING" <<endl;
				//next is the length of chars
				buf  = *(pbuf++);
				uint8_t sLen = buf;
				cout << "Length      : "<< (uint32_t)sLen <<endl;
				ValString = new char[sLen+1];
				memset(ValString, 0, sLen+1);
				memcpy(ValString, pbuf, sLen);
				cout << "VALUE      : " << ValString <<endl;
				delete [] ValString;
				pbuf+=sLen;
				break;
		}


	}



}

int _tmain(int argc, _TCHAR* argv[])
{

	cout << "Testing protocol buffer decoding...." << endl;
	decoder(protbuf,26);
	
	return 0;
}

