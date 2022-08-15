#pragma once

#include <stdint.h>
#include <string>
#include <cstring>


// Working Testers
int32_t clib_add_internal(int32_t left, int32_t right);
int32_t clib_sub_internal(int32_t left, int32_t right);
char* clib_str_internal();
//////////////////////////////////////////////////////////

class WebFB {
	int num;
	std::string str;
public:
	WebFB();
	WebFB(int n, std::string s);

	int getNum();
	char* getStr();

	void setNum(int n);
	void setStr(std::string s);
};