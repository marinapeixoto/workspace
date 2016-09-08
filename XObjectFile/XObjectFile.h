#ifndef __XOBJECT_FILE__
#define __XOBJECT_FILE__

#include <fstream>
#include <string>

class XObject;
class XObjectFile {
public:
    enum OpenMode {
        load,
        store,
        invalid_mode
    };
    XObjectFile(std::string filename, OpenMode m);
    XObjectFile& operator<<(XObject&);
    XObjectFile& operator>>(XObject&);
    void Write(void* buf, size_t size);
    void Read(void* buf, size_t size);
    void Close();
    bool IsStoring() const { return store == mode; }
    bool IsLoading() const { return load == mode; }
private:
    std::fstream fs;
    OpenMode mode;
};

class XObject {
public:
    virtual void Dump(XObjectFile&) = 0;
};

#endif // __XOBJECT_FILE__
