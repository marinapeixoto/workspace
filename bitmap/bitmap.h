#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <cstdint>
#include <string>
using namespace std;

namespace jx {

#pragma pack(1)

typedef struct {
    uint16_t    type;
    uint32_t    size;
    uint16_t    reserved1;
    uint16_t    reserved2;
    uint32_t    offbits; 
} bitmap_file_hdr;

typedef struct {
    uint32_t    infosize;
    uint32_t    width;
    uint32_t    height;
    uint16_t    planes;
    uint16_t    bitcount;
    uint32_t    compression;
    uint32_t    imgsize;
    uint32_t    xpels;
    uint32_t    ypels;
    uint32_t    clrused;
    uint32_t    clrimportant;
} bitmap_info_hdr;

typedef struct {
    uint8_t     rgb_blue;
    uint8_t     rgb_green;
    uint8_t     rgb_red;
    uint8_t     reserved;
} rgbquad;

#pragma pack(4)

class bitmap {
public:
    bitmap(){}
    bitmap(string filename);
    bool read(string filename);
    bool save(string filename);
    bool copyimg(uint8_t* imgdata, uint32_t* x, uint32_t* y, uint8_t* type); 
    void dumpinfo();
private:
    bitmap_info_hdr bminfo; 
    bitmap_file_hdr bmf;
    uint8_t*  imgdata;
    bool isvalid;

};

};
#endif // __BITMAP_H__

