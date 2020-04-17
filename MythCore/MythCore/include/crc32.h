#ifndef __CRC32_H__
#define __CRC32_H__

/*����buffer��crcУ����*/
unsigned int crc32(unsigned char *pBuffer, unsigned int nSize);

/*
**������ļ���CRCУ����:crc32����,�Ƕ�һ��buffer���д���,
**�����һ���ļ���Խϴ�,��Ȼ����ֱ�Ӷ�ȡ���ڴ浱��
**����ֻ�ܽ��ļ��ֶζ�ȡ��������crcУ��,
**Ȼ��ѭ������һ�ε�crcУ�����ٴ��ݸ��µ�bufferУ�麯��,
**��������ɵ�crcУ������Ǹ��ļ���crcУ����.(��������)
*/
int calcFileCrc32(const char *pFile, unsigned int *FileCrc);
#endif

