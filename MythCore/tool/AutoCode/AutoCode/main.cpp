#include "parseheader.h"
#include "test_back.h"
int main()
{
	CParseHeader tParseHeader;
	tParseHeader.parseHeaderFile("test.h");
	tParseHeader.writeHeaderFile();
	tParseHeader.parseSourceFile("test.cpp");
	tParseHeader.writeSourceFile();
}