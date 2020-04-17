#include "crc32.h"
#include <stdio.h>
#define BUFSIZE     4096
unsigned int crc_table[256];  
/* 
**初始化crc表,生成32位大小的crc表 
**也可以直接定义出crc表,直接查表, 
**但总共有256个,看着眼花,用生成的比较方便. 
*/  
void init_crc_table(void)  
{  
    unsigned int c = 0;        
    for (unsigned int i = 0; i < 256; i++) {  
        c = (unsigned int)i;  
        for (unsigned int j = 0; j < 8; j++) {  
            if (c & 1)  
                c = 0xedb88320L ^ (c >> 1);  
            else  
                c = c >> 1;  
        }  
        crc_table[i] = c;  
    }  
}  
  
/*计算buffer的crc校验码*/  
unsigned int crc32(unsigned int nCrcNum, unsigned char *pBuffer, unsigned int nSize)
{  
    for (unsigned int  i = 0; i < nSize; i++) {  
        nCrcNum = crc_table[(nCrcNum ^ pBuffer[i]) & 0xff] ^ (nCrcNum >> 8);  
    }  
    return nCrcNum ;  
} 

/*计算buffer的crc校验码*/  
unsigned int crc32(unsigned char *pBuffer, unsigned int nSize)
{
	unsigned int crc = 0xffffffff;
	crc = crc32( crc, pBuffer, nSize );
	return ~crc;
}

/* 
**计算大文件的CRC校验码:crc32函数,是对一个buffer进行处理, 
**但如果一个文件相对较大,显然不能直接读取到内存当中 
**所以只能将文件分段读取出来进行crc校验, 
**然后循环将上一次的crc校验码再传递给新的buffer校验函数, 
**到最后，生成的crc校验码就是该文件的crc校验码.(经过测试) 
*/  
int calcFileCrc32(const char *pFilePath, unsigned int *FileCrc)  
{  
    int nReadCount;  
    unsigned char buf[BUFSIZE];  
    /*第一次传入的值需要固定,如果发送端使用该值计算crc校验码, 
    **那么接收端也同样需要使用该值进行计算*/  
    unsigned int crc = 0xffffffff;   
  
    FILE* pFile = fopen(pFilePath, "rb");  
    if (NULL == pFile) {  
        return -1;  
    }  
          
    while ((nReadCount = fread(buf, BUFSIZE, 1, pFile)) > 0) {  
        crc = crc32(crc, buf, nReadCount);  
    }  

	fclose(pFile);
	if (nReadCount < 0) {
		return -1;
	}

    *FileCrc = crc;  
    return 0;  
}  

class CCrc32Info
{
public:
	CCrc32Info()
	{
		init_crc_table();
	}
};

// 初始化crc32表
CCrc32Info mCrc32Info;