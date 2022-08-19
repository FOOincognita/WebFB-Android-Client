#include "CLib.h"

/* 
 * These functions are wrappers for the WebFB class methods 
 * The name of the function will be your P/Invoke Entry Point in C# in CLib.cs
 */

extern "C" 
{
    void* create() { return (void*) new WebFB(); } 
    void* createParams(const char* _IP, const char* _Port) { return (void*) new WebFB(std::string(_IP), std::string(_Port)); }
    void destruct(WebFB* wfb) { delete wfb; }
    std::uint32_t getWFBErr(WebFB* wfb) { return wfb->getErr(); }
    double getWFBLatData(WebFB* wfb) { return wfb->GetLatData(); }
}