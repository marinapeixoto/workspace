# JXML
JXML is just a tiny XML parser for C language. 
It has a simple interface for parse xml and read xml data.
JXML does not support CDATA and DTD, because it is just a toy. Aha!

# Usage

```
XML_Doc_t   doc;
XML_RET     ret;
XML_Node_t* node;
char*       attrVal;
ret = XML_ParseFile("demo.xml", &doc);
if(ret != XML_SUCCESS ) {
    printf("ErrInfo: %s\n",XML_GetCodeInfo(ret));
    return 0;
}

// get first child node
node = doc.firstChild;
XML_GetAttr(node,"attrname",attrVal);
printf("Node attr value: %s",attrVal);

XML_Destroy(&doc);
```

Most of JXML functions return error code. You can use `XML_GetCodeInfo` to get error info.

# Details

## How to manage memory
We store node in a simple memory pool, while attribute name and value is directly read from raw data.
Xml raw data is a heap memory too, but not in memory pool. JXML will allocate memory (using malloc) and 
release (using free) automatically.

DO NOT change data. Any change may cause error. 

# About 
Author: tkorays
Email: tkorays@hotmail.com
