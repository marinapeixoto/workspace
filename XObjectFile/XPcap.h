#ifndef __XPCAP_H__
#define __XPCAP_H__

#include "XObjectFile.h"

class XPcapFileHeader : public XObject {
public:
    void Dump(XObjectFile&);
private:
       
};

#endif // __XPCAP_H__

