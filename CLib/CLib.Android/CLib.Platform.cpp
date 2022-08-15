#include <stdint.h>
#include "CLib.h"

/*
extern "C" {
    // [WORKING]: Tester Funcs
    int32_t clib_add(int32_t left, int32_t right) { return clib_add_internal(left, right); }
    int32_t clib_sub(int32_t left, int32_t right) { return clib_sub_internal(left, right); }
    char* clib_str() { return clib_str_internal(); }
    //////////////////////////////////////////////////////////

    // [WORKING] Tester Class wrapper funcs
    void* create() { return (void*) new WebFB(); } 
    void* createParams(int n, const char* s) { return (void*) new WebFB(n, std::string(s)); } 

    int getWFBNum(WebFB* wfb) { return wfb->getNum(); }
    char* getWFBStr(WebFB* wfb) { return wfb->getStr(); }

    void setWFBNum(WebFB* wfb, int n) { wfb->setNum(n); }
    void setWFBStr(WebFB* wfb, const char* s) { wfb->setStr(s); } 
}
*/


extern "C" {
    void* create() { return (void*) new WebFB(); }
    void* createParams(const char* _IP, const char* _Port) { return (void*) new WebFB(std::string(_IP), std::string(_Port)); }
    void destruct(WebFB* wfb) { delete wfb; }
    std::uint32_t getWFBErr(WebFB* wfb) { return wfb->getErr(); }
}