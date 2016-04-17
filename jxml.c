#include "jxml.h"
#include "mem_pool.h"

#include <stdio.h>
#include <stdlib.h>

#define XML_NODE_STACK_SIZE 100

typedef struct {
    uint32_t    rc;
    XML_Node_t* node[XML_NODE_STACK_SIZE];
} XML_NodeStack_t;

typedef struct {
    XML_RET code;
    char    info[64];
} CodeInfo;

XML_RET XML_SkipBlanks(const char* addr, int size, int* offset);
XML_RET XML_GetName(const char* addr, int size,int* offset,XML_Str_t* str);
XML_RET XML_GetStr(const char* addr,int size,int* offset, XML_Str_t* str);
XML_RET XML_GetAttrPair(const char* addr,int size,int *offset,XML_Str_t* name, XML_Str_t* value);
XML_RET XML_GetNode(const char* addr,int size,int* offset, XML_Node_t* node, XML_Str_t* closeTag);
XML_RET XML_GetStm(const char* addr, XML_Stm_t* stm);

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

XML_RET XML_SkipBlanks(const char* addr, int size,int* offset) {
    while((addr[*offset] == ' ' || addr[*offset]=='\t' || addr[*offset]=='\r' || addr[*offset]=='\n') && (*offset+1)<size) {
        *offset += 1;
        continue;
    }
    if((*offset+1)>size) {
        return XML_PARSE_ERR;
    } else {
        return XML_SUCCESS;
    }
}

XML_RET XML_GetName(const char* addr, int size, int* offset, XML_Str_t* str) {
    int i = 0;
    int valid;
    if((addr[*offset]>='A' && addr[*offset]<='Z') || (addr[*offset]>='a' && addr[*offset]<='z')) {
        while(1){
            if((*offset+1)>size) {
                return XML_PARSE_ERR;
            }
            valid = (addr[*offset]>='A' && addr[*offset]<='Z') || (addr[*offset]>='a' && addr[*offset]<='z') \
                || (addr[*offset]>='0' && addr[*offset]<='9') || addr[*offset]=='_';
            if(!valid){
                break;
            }
            *offset += 1;
            i++;
        }
        if(addr[*offset]!=' ' && addr[*offset]!='\r' && addr[*offset]!='\n' && addr[*offset]!='\t' && addr[*offset]!='=') {
            return XML_INVALID_NAME;
        }
        str->addr = (char*)(addr+*offset-i);
        str->size = i;
        return XML_SUCCESS;
    } else {
        return XML_INVALID_NAME;
    }
}


XML_RET XML_GetStr(const char* addr, int size,int* offset, XML_Str_t* str) {
    int i = 0;
    if(addr[*offset]!='\"') {
        return XML_INVALID_STR;
    }
    i++;
    *offset += 1;
    while(1) {
        if((*offset+1)>size) {
            return XML_PARSE_ERR;
        }
        if(addr[*offset]=='\"' && addr[*offset-1]!='\\') {
            break;
        }
        i++;
        *offset += 1;
    }
    str->addr = (char*)(addr+*offset-i+1);
    str->size = i-1;

    return XML_SUCCESS;
}


XML_RET XML_GetAttrPair(const char* addr, int size, int* offset, XML_Str_t* name, XML_Str_t* value) {
    XML_RET ret;

    // skip blanks
    ret = XML_SkipBlanks(addr, size, offset);
    if(ret != XML_SUCCESS){
        return ret;
    }

    // get name
    ret = XML_GetName(addr,size,offset,name);
    if(ret != XML_SUCCESS) {
        return ret;
    }

    ret = XML_SkipBlanks(addr,size,offset);
    if(ret != XML_SUCCESS) {
        return ret;
    }

    if(addr[*offset]!='='){
        return XML_PARSE_ERR;
    }
    *offset += 1;
    XML_SkipBlanks(addr,size,offset);

    // get value
    ret = XML_GetStr(addr,size,offset,value);
    if(ret != XML_SUCCESS) {
        return ret;
    }

    return XML_SUCCESS;
}

// start with <xxx, not <!-- and </
XML_RET XML_GetNode(const char* addr, int size,int* offset, XML_Node_t* node, XML_Str_t* closeTag) {
    XML_RET  ret;
    XML_Str_t tagName;
    XML_Str_t name,value;

    if(addr[*offset]!='<') {
        return XML_PARSE_ERR;
    }
    *offset += 1;

    ret = XML_GetName(addr,size,offset,&tagName);
    if(ret != XML_SUCCESS) {
        return ret;
    }

    // skip blanks to '/' or '>'
    ret = XML_SkipBlanks(addr,size,offset);
    while(1) {
        if (addr[*offset] == '>') {
            // TODO
            break;
        } else if (addr[*offset] == '/') {
            ret = XML_SkipBlanks(addr, size, offset);
            if (addr[*offset] == '>') {
                // TODO
                break;
            } else {
                return XML_PARSE_ERR;
            }
        } else {
            ret = XML_GetAttrPair(addr,offset,offset,&name,&value);
            if(ret!=XML_SUCCESS) {
                return ret;
            }

        }
    }

    return XML_SUCCESS;
}
