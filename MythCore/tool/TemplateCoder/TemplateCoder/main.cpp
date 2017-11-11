#include "parseclass.h"
int main()
{
	CParseClass tParseClass;
	tParseClass.parseClass("template.h", "CTemplateGashaponDropTable");
	tParseClass.writeTempHeadFile();
	tParseClass.writeTempSourceFile();
	tParseClass.writeTempPBFile();
}