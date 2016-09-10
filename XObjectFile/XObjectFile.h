#ifndef __XOBJECT_FILE__
#define __XOBJECT_FILE__

#include <fstream>
#include <string>

#define XOBJECT_BUF_SIZE 1024

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
    void Fetch(XObject&);
    void Push(XObject&);
    void Write(void* buf, size_t size);
    void Read(void* buf, size_t size);
    bool Open(std::string filename, OpenMode m);
    void Close();
    bool IsStoring() const { return store == mode; }
    bool IsLoading() const { return load == mode; }
    bool IsOpen() const { return store==mode || load==mode; }
private:
    std::fstream fs;
    OpenMode mode;
    uint8_t buffer[XOBJECT_BUF_SIZE];
    uint32_t bufsize;
    void Flush();
};

class XObject {
public:
    virtual void Dump(XObjectFile&) = 0;
};

#endif // __XOBJECT_FILE__

