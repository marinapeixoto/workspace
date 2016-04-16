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

void XML_StrPrint(XML_Str_t* str) {
    int i = 0;
    while(i<str->size){
        printf("%c",str->addr[i++]);
    }
    printf("\n");
}

extern XML_RET XML_GetName(const char* addr, int size, XML_Str_t* str);
extern XML_RET XML_GetStr(const char* addr, int size, XML_Str_t* str);
extern XML_RET XML_GetAttrPair(const char* addr, int size, int* offset, XML_Str_t* name, XML_Str_t* value);

int main(){
    XML_Str_t str;
    XML_Str_t name;
    XML_Str_t value;
    XML_RET ret;
    int offset = 0;
    ret = XML_GetName("abc= 3eff",9,&str);
    printf("XML_GetName Ret:%s\n",XML_GetCodeInfo(ret));
    XML_StrPrint(&str);

    ret = XML_GetStr("\"abcfuck\\\"heheh\"345",19,&str);
    printf("XML_GetStr Ret:%s\n",XML_GetCodeInfo(ret));
    XML_StrPrint(&str);

    ret = XML_GetAttrPair(" fuck = \"wocao\\\"hehe\"",22,&offset,&name,&value);
    printf("XML_GetAttrPair Ret:%s\n",XML_GetCodeInfo(ret));
    XML_StrPrint(&name);
    XML_StrPrint(&value);


    return 0;
}