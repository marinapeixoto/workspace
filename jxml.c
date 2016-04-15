#include "jxml.h"
#include "mem_pool.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    XML_RET code;
    char    info[64];
} CodeInfo;

CodeInfo gCodeInfo[XML_CODE_NUM] = {
    {XML_SUCCESS,"success"},
    {XML_FAILED,"failed"},
    {XML_PARAM_ERR,"invalid paramenter"},
    {XML_MALLOC_ERR,"failed to alloc memory"},
    {XML_FILE_ERR,"failed to handle with file"},
    
    {XML_CODE_NUM,"bad code number"}
    
};

static const char* XML_GetCodeInfo(XML_RET code) {
    int i;
    CodeInfo* ci = gCodeInfo;
    if(code < XML_CODE_START || code >= XML_CODE_NUM){
        return (const char*)ci[XML_CODE_NUM].info;
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

