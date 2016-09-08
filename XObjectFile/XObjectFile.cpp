#include "XObjectFile.h"

using namespace std;

XObjectFile::XObjectFile(string filename, OpenMode m) {
    if(load == m) {
        fs.open(filename, ios::in|ios::binary);
        mode = load;
    } else {
        fs.open(filename, ios::out|ios::binary);
        mode = store;
    }
    if(!fs.is_open()) {
        mode = invalid_mode;
    }
}

XObjectFile& XObjectFile::operator<<(XObject& o) {
    o.Dump(*this);
    return *this;
}

XObjectFile& XObjectFile::operator>>(XObject& o) {
    o.Dump(*this);
    return *this;
}

void XObjectFile::Write(void* buf, size_t size) {
    if(mode != store) return ;
    fs.write((char*)buf,size);
}

void XObjectFile::Read(void* buf, size_t size) {
    if(mode != load) return ;
    fs.read((char*)buf,size);
}

void XObjectFile::Close() {
    if(fs.is_open()) fs.close();
    mode = invalid_mode;
}



