#include <iostream>
#include "XObjectFile.h"

using namespace std;

class A : public XObject {
public:
    A():a(123),b(67),c(23.45){}
    void Dump(XObjectFile& of) {
        if(of.IsStoring()) { 
            of.Write(&a, sizeof(int));
            of.Write(&b, sizeof(char));
            of.Write(&c, sizeof(double));
        } else if(of.IsLoading()) {
            of.Read(&a, sizeof(int));
            of.Read(&b, sizeof(char));
            of.Read(&c, sizeof(double));
        }
    }
public:
    int a;
    char b;
    double c;
};

int main() {
    A a;
    a.a = 456;
    a.b = 97;
    a.c = 999.999;
    XObjectFile of("out.bin", XObjectFile::store);
    of<<a;
    of.Close();


    XObjectFile ofr("out.bin", XObjectFile::load);
    A ar;
    ofr>>ar;
    ofr.Close();


    cout<<ar.a<<","<<ar.b<<","<<ar.c<<endl;

    return 0;
}
