#include "filelist.h"
#include "version.h"
#include <tchar.h>
int main()
{
	CVersion tVersion;
	tVersion.loadVersionXml("version.xml");

	CFileList tFileList;
	tFileList.loadFileList();
	tFileList.listFileDirectory(_T("D:\\MythCoreGit\\MythCore\\MythCore\\tool\\VersionUpdate\\Debug\\Resource"), _T(""));
	tFileList.saveFileList(tVersion.getCurVersion());
	tFileList.generateDiffList(tVersion.getStartVersion(), tVersion.getCurVersion());
	tFileList.modifyAllDiffXml(tVersion.getStartVersion(), tVersion.getCurVersion());
	

}