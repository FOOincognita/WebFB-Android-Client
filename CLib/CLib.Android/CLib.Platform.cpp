#include "CLib.h"

extern "C" {
    void* create() { return (void*) new WebFB(); } 
    void* createParams(const char* _IP, const char* _Port) { return (void*) new WebFB(std::string(_IP), std::string(_Port)); }
    void destruct(WebFB* wfb) { delete wfb; }
    std::uint32_t getWFBErr(WebFB* wfb) { return wfb->getErr(); }
    double getWFBLatData(WebFB* wfb) { 
        try_and_catch_abort(GetLatData__, wfb);
        if (!wfb->getLatDat()) { return wfb->getLatErr(); }
        return wfb->getLatDat();  
    }
}