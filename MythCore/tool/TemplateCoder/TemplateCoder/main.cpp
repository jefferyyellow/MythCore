#include "parseclass.h"
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("param 1 is target xml file name");
		return -1;
	}

	char acClassName[MAX_PATH] = { 0 };
	strncpy(acClassName, argv[1], sizeof(acClassName)-1);
	
	CParseClass tParseClass;
	tParseClass.parseClass("template.h", acClassName);
	tParseClass.writeTempHeadFile();
	tParseClass.writeTempSourceFile();
	tParseClass.writeTempPBFile();
}