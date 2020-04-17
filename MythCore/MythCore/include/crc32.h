#ifndef __CRC32_H__
#define __CRC32_H__

/*计算buffer的crc校验码*/
unsigned int crc32(unsigned char *pBuffer, unsigned int nSize);

/*
**计算大文件的CRC校验码:crc32函数,是对一个buffer进行处理,
**但如果一个文件相对较大,显然不能直接读取到内存当中
**所以只能将文件分段读取出来进行crc校验,
**然后循环将上一次的crc校验码再传递给新的buffer校验函数,
**到最后，生成的crc校验码就是该文件的crc校验码.(经过测试)
*/
int calcFileCrc32(const char *pFile, unsigned int *FileCrc);
#endif

