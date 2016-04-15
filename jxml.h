#ifndef __JXML_H__
#define __JXML_H__

#include <stdint.h>

#define JXML_MAJOR_VER 0
#define JXML_MINOR_VER 1

typedef char* JXMLStr;
typedef struct XML_Stm_s  XML_Stm_t;
typedef struct XML_Attr_s XML_Attr_t;
typedef struct XML_Node_s XML_Node_t;
typedef struct XML_Doc_s  XML_Doc_t;
typedef uint32_t XML_RET;

struct XML_Stm_s {
   uint8_t ver_major;
   uint8_t ver_minor;
   uint8_t encoding;
};

struct XML_Attr_s {
    char*       name;
    char*       value;
    XML_Attr_t* next;
};

struct XML_Node_s {
    char*       name;
    char*       text;
    XML_Node_t* parent;
    XML_Node_t* fistChild;
    XML_Node_t* lastChild;
    XML_Node_t* prev;
    XML_Node_t* next;
};

struct XML_Doc_s {
    XML_Stm_t   stm;
    XML_Node_t* root;
    mem_pool_t* mpoo;
    XML_RET     lastCode;
};

typedef enum {
    XML_CODE_START = 0,
    XML_SUCCESS = 0,
    XML_FAILED,
    XML_PARAM_ERR,
    XML_MALLOC_ERR,
    XML_FOPEN_ERR,

    
    XML_CODE_NUM
};

void XML_ParseFile(const char* filename, XML_Doc_t* doc);
void XML_ParseStr(const char* str, XML_Doc_t* doc);
void XML_Destory(XML_Doc_t* doc);
const char* XML_GetAttr(XML_Node_t* node, char* name);
int XML_GetIntAttr(XML_Node_t* node, char* name);
const char* XML_GetLastErrInfo(XML_Doc_t* doc); 

#endif // __JXML_H__
