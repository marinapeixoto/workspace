#include "jxml.h"
#include "mem_pool.h"

#include <stdio.h>
#include <stdlib.h>

#define XML_NODE_STACK_SIZE 100

typedef struct {
    uint32_t    ptr;
    XML_Node_t* node[XML_NODE_STACK_SIZE];
} XML_NodeStack_t;

typedef struct {
    XML_RET code;
    char    info[64];
} CodeInfo;

static XML_RET XML_GetName(const char* addr, int size, XML_Str_t* str);
static XML_RET XML_GetStr(const char* addr,int size,  XML_Str_t* str);
static XML_RET XML_GetAttrPair(const char* addr,XML_Str_t* name, XML_Str_t* value);
static XML_RET XML_GetNode(const char* addr,XML_NodeStack_t* ns, XML_Node_t* node);
static XML_RET XML_GetStm(const char* addr, XML_Stm_t* stm);

CodeInfo gCodeInfo[XML_CODE_NUM] = {
    {XML_SUCCESS,"success"},
    {XML_FAILED,"failed"},
    {XML_PARAM_ERR,"invalid paramenter"},
    {XML_MALLOC_ERR,"failed to alloc memory"},
    {XML_FILE_ERR,"failed to handle with file"},
    {XML_PARSE_ERR,"failed to parse xml"},
    {XML_INVALID_NAME,"invalid name"},
    {XML_INVALID_STR,"invalid string"},
    
};

const char* XML_GetCodeInfo(XML_RET code) {
    int i;
    CodeInfo* ci = gCodeInfo;
    if(code < XML_CODE_START || code >= XML_CODE_NUM){
        return (const char*)ci[XML_FAILED].info;
    }
    if(ci[code].code == code) {
        return (const char*)ci[code].info;
    }
    for(i=XML_CODE_START; i<XML_CODE_NUM; i++) {
        if(ci[i].code == code) {
            return (const char*)ci[i].info; 
        }
    }
    return (const char*)ci[XML_CODE_NUM].info;
}



XML_RET XML_ParseFile(const char* filename, XML_Doc_t* doc) {
    char* content = 0;
    FILE* f = 0;
    long fsize = 0;
    XML_RET ret = XML_SUCCESS;
    if(!filename || !doc){
        return XML_PARAM_ERR;
    }
    if((f = fopen(filename,"r"))==0) {
        return XML_FILE_ERR;
    }
    fseek(f,0,SEEK_END);
    fsize = ftell(f);

    if((content = malloc(fsize))==0) {
        fclose(f);
        return XML_MALLOC_ERR;
    }
    fread(content,fsize,1,f);
    fclose(f);

    ret = XML_ParseStr(content,doc);

    free(content);
    return ret;
}


XML_RET XML_ParseStr(char* str, XML_Doc_t* doc) {
    if(!str || !doc) {
        return XML_PARAM_ERR;
    }


    return XML_SUCCESS;
}

void XML_Destory(XML_Doc_t* doc) {
    if(doc) {
        mem_pool_destroy(doc->mpool);
        doc->mpool = 0;
        doc->root = 0;
    }
}

static XML_RET XML_GetName(const char* addr, int size, XML_Str_t* str) {
    int i = 0;
    int valid;
    if((addr[i]>='A' && addr[i]<='Z') || (addr[i]>='a' && addr[i]<='z')) {
        valid = 1;
        while(valid){
            if((i+1)>size) {
                return XML_PARSE_ERR;
            }
            valid = (addr[i]>='A' && addr[i]<='Z') || (addr[i]>='a' && addr[i]<='z') \
                || (addr[i]>='0' && addr[i]<='9') || addr[i]=='_';
            if(!valid){
                break;
            }
            i++;
        }
        if(addr[i]!=' ' || addr[i]!='\r' || addr[i]!='\n' || addr[i]!='\t' || addr[i]!='=') {
            return XML_INVALID_NAME;
        }
        str->addr = (char*)addr;
        str->size = i;
        return XML_SUCCESS;
    } else {
        return XML_INVALID_NAME;
    }
}


static XML_RET XML_GetStr(const char* addr, int size, XML_Str_t* str) {
    int i = 0;
    if(addr[i]!='\"') {
        return XML_INVALID_STR;
    }
    i++;
    while(1) {
        if((i+1)>size) {
            return XML_PARSE_ERR;
        }
        if(addr[i]=='\"' && addr[i-1]!='\\') {
            break;
        }
        i++;
    }
    str->addr = (char*)(addr+1);
    str->size = i-1;

    return XML_SUCCESS;
}


