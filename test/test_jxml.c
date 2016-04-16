#include "../jxml.h"
#include <stdio.h>

void test_xml_code(){
    int i = XML_CODE_START;
    int ret = 0;
    while(i<=XML_CODE_NUM) {
        printf("CODE(%d) getInfo: %s\n", i, XML_GetCodeInfo(i));
        i++;
    }
}


int main(){
    test_xml_code();
    return 0;
}