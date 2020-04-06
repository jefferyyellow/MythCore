#include "dir.h"
#include <tchar.h>
int main()
{
	CFileList tFileList;
	//tFileList.ListFileDirectory(_T("."), _T(""));
	//tFileList.SaveFileList();
	tFileList.LoadFileList();
}