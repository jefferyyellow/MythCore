#include "parseheader.h"
int main()
{
	CParseHeader tParseHeader;
	tParseHeader.parseHeaderFile("test.h");
	tParseHeader.writeHeaderFile();
	tParseHeader.parseSourceFile("test.cpp");
}