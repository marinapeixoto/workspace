#include "XObjectFile.h"

using namespace std;

XObjectFile::XObjectFile(string filename, OpenMode m) {
    Open(filename, m);
    bufsize = 0;
}

XObjectFile& XObjectFile::operator<<(XObject& o) {
    o.Dump(*this);
    return *this;
}

XObjectFile& XObjectFile::operator>>(XObject& o) {
    o.Dump(*this);
    return *this;
}

void XObjectFile::Fetch(XObject& o) {
    o.Dump(*this);
}

void XObjectFile::Push(XObject& o) {
    o.Dump(*this);
}

void XObjectFile::Write(void* buf, size_t size) {
    if(mode != store) return ;
    if(size + bufsize > XOBJECT_BUF_SIZE) Flush();    
    else {
        memcpy(buffer+bufsize, buf, size);
        bufsize += size;
    }
}

void XObjectFile::Read(void* buf, size_t size) {
    if(mode != load) return ;
    fs.read((char*)buf,size);
}

bool XObjectFile::Open(string filename, OpenMode m) {
    mode = invalid_mode;
    if(load == m) { 
        fs.open(filename,ios::in|ios::binary);
        mode = load;
    } else if(store == m) {
        fs.open(filename, ios::out|ios::binary);
        mode = store;
    }

    if(!fs.is_open()) mode = invalid_mode;
    return mode !=invalid_mode;
}

void XObjectFile::Close() {
    Flush();
    if(fs.is_open()) fs.close();
    mode = invalid_mode;
}

void XObjectFile::Flush() {
    if(bufsize == 0 || mode!=store) return ;
    fs.write((char*)buffer, bufsize);
    bufsize = 0;
}


