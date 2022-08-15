#include "CLib.h"

int32_t clib_add_internal(int32_t left, int32_t right)
{
    return left + right;
}

int32_t clib_sub_internal(int32_t left, int32_t right)
{
    return left - right;
}

char* clib_str_internal()
{
    return const_cast<char*>("Howdy");
}
//////////////////////////////////////////////////////////

// Internal class funcs
WebFB::WebFB() : num(69), str("Howdy") {}
WebFB::WebFB(int n, std::string s) : num(n), str(s) {}

int WebFB::getNum() { return this->num;  }
char* WebFB::getStr() { return const_cast<char*>((this->str).c_str()); }

void WebFB::setNum(int n) { this->num = n; }
void WebFB::setStr(std::string s) { this->str = s;  }
//////////////////////////////////////////////////////////