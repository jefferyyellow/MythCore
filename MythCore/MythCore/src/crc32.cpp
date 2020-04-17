#include "crc32.h"
#include <stdio.h>
#define BUFSIZE     4096
unsigned int crc_table[256];  
/* 
**��ʼ��crc��,����32λ��С��crc�� 
**Ҳ����ֱ�Ӷ����crc��,ֱ�Ӳ��, 
**���ܹ���256��,�����ۻ�,�����ɵıȽϷ���. 
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
  
/*����buffer��crcУ����*/  
unsigned int crc32(unsigned int nCrcNum, unsigned char *pBuffer, unsigned int nSize)
{  
    for (unsigned int  i = 0; i < nSize; i++) {  
        nCrcNum = crc_table[(nCrcNum ^ pBuffer[i]) & 0xff] ^ (nCrcNum >> 8);  
    }  
    return nCrcNum ;  
} 

/*����buffer��crcУ����*/  
unsigned int crc32(unsigned char *pBuffer, unsigned int nSize)
{
	unsigned int crc = 0xffffffff;
	crc = crc32( crc, pBuffer, nSize );
	return ~crc;
}

/* 
**������ļ���CRCУ����:crc32����,�Ƕ�һ��buffer���д���, 
**�����һ���ļ���Խϴ�,��Ȼ����ֱ�Ӷ�ȡ���ڴ浱�� 
**����ֻ�ܽ��ļ��ֶζ�ȡ��������crcУ��, 
**Ȼ��ѭ������һ�ε�crcУ�����ٴ��ݸ��µ�bufferУ�麯��, 
**��������ɵ�crcУ������Ǹ��ļ���crcУ����.(��������) 
*/  
int calcFileCrc32(const char *pFilePath, unsigned int *FileCrc)  
{  
    int nReadCount;  
    unsigned char buf[BUFSIZE];  
    /*��һ�δ����ֵ��Ҫ�̶�,������Ͷ�ʹ�ø�ֵ����crcУ����, 
    **��ô���ն�Ҳͬ����Ҫʹ�ø�ֵ���м���*/  
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

// ��ʼ��crc32��
CCrc32Info mCrc32Info;