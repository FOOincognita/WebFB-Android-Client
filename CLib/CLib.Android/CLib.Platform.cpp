#include "CLib.h"

extern "C" {
    void* create() { return (void*) new WebFB(); } 
    void* createParams(const char* _IP, const char* _Port) { return (void*) new WebFB(std::string(_IP), std::string(_Port)); }
    void destruct(WebFB* wfb) { delete wfb; }
    std::uint32_t getWFBErr(WebFB* wfb) { return wfb->getErr(); }
    latitude_t getWFBLatData(WebFB* wfb) { return wfb->GetLatData(); }
}