#include "bitmap.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace jx {

bitmap::bitmap(string filename) {
    isvalid = false;

    fstream fs;
    fs.open(filename, ios::in|ios::binary);
    if(!fs.is_open()) {
        cout<<"openf"<<endl;
        return ;
    }

    fs.read((char*)&bmf, sizeof(bitmap_file_hdr));
    if( bmf.type != 0x4d42 || bmf.reserved1!=0 || bmf.reserved2!=0 
        || bmf.offbits<(sizeof(bitmap_file_hdr) + sizeof(bitmap_info_hdr)) ) {
        return ;
    }

    fs.read((char*)&bminfo, sizeof(bitmap_info_hdr));
    if(bminfo.infosize != sizeof(bitmap_info_hdr) || bminfo.planes!=1) {
        return ;
    }

    if(bmf.offbits == (sizeof(bitmap_file_hdr)+sizeof(bitmap_info_hdr))) {
        uint32_t datasize = bmf.size - sizeof(bitmap_file_hdr) - sizeof(bitmap_info_hdr);
        imgdata = new uint8_t[datasize];
        fs.read((char*)imgdata, datasize);

        isvalid = true;
    }else {
        return ;
    }
    fs.close();
}

bool bitmap::read(string filename) {

    return true;
}

bool bitmap::save(string filename) {
    return true;
}

bool bitmap::copyimg(uint8_t* imgdata, uint32_t* x, uint32_t* y, uint8_t* type) {
    return true;
}

void bitmap::dumpinfo() {
    if(!isvalid) {
        cout<<"bitmap is invalid or open file failed"<<endl;
        return ;
    }

    cout<<"filesize:"<<bmf.size<<endl;
    cout<<"width:"<<bminfo.width<<endl;
    cout<<"height:"<<bminfo.height<<endl;
    cout<<"bitcount:"<<bminfo.bitcount<<endl;
}

};
