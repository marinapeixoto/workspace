#include "bitmap.h"
#include <string>
using namespace std;

int main(int argc, char** argv) {
    string s("a.bmp");
    jx::bitmap bm(s);
    bm.dumpinfo();

    return 0;
}

