#include "parseclass.h"
int main()
{
	CParseClass tParseClass;
	tParseClass.parseClass("template.h", "CTemplateHorseSkillPropertyConfig");
	tParseClass.writeTempHeadFile();
	tParseClass.writeTempSourceFile();
	tParseClass.writeTempPBFile();
}