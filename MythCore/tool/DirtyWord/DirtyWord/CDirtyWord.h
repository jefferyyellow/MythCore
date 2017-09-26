#ifndef DIRTYWORD_DIRTYWORD_CDIRTYWORD_H
#define DIRTYWORD_DIRTYWORD_CDIRTYWORD_H

#include <iostream>
#include <string>

namespace std
{
    class CDirtyWord
    {
    public:
        CDirtyWord();

        int IsValid(char* cpStr);

        int Getpath(const char *ccpSrcPath, char cDestPath[]);

        int DoIt(const char * ccpPath);

        ~CDirtyWord();
    private:
        FILE * m_FpSour;
        FILE * m_FpDirty;
        FILE * m_FpCf;
        FILE * m_FpEf;

        char m_PathDirth[MAX_PATH];
        char m_PathCf[MAX_PATH];
        char m_PathEf[MAX_PATH];
    };

}


#endif