#include <stddef.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <btypes.h>
#include <bvcstr.h>
#include <bvio.h>
#include <bvio.win32.h>
#include <memtaf.h>
#include <w32ex.h>

#include "unragp.h"

bool __WDECL ForEachRagpFile(char const* const lpszPath, void const* const lpData, size_t const uDataLength, void* lpContext)
{
    bool bSuccess = false;
    char szPath[512];
    char* lpszSlash;
    size_t uPushed = 0;
    BVIO* lpbvioFile = NULL;

    puts(lpszPath);
    BvStrNCpyA(szPath, lpszPath, __ARRAYSIZE(szPath));
    BvChrReplaceA(szPath, '/', '\\');

    lpszSlash = strrchr(szPath, '\\');

    if(lpszSlash!=NULL)
    {
        lpszSlash[0] = '\0';

        if(!CreateDirectoryRecursive(szPath, NULL) && GetLastError()!=ERROR_ALREADY_EXISTS)
        {
            return false;
        }

        lpszSlash[0] = '\\';
    }

    if(!BvIOWin32FileCreateDesperatelyA(szPath, 0, NULL, &lpbvioFile))
    {
        return false;
    }

    bSuccess = BvIOSeqPush(lpbvioFile, lpData, uDataLength, &uPushed) && uDataLength==uPushed;
    BvIORelease(&lpbvioFile);

    return bSuccess;
}

int main(int nArgc, char** lppszArgv)
{
    int nIdx;

    for(nIdx = 1; nIdx<nArgc; nIdx++)
    {
        BVIO* lpbvioFile = NULL;

        puts(lppszArgv[nIdx]);

        if(BvIOWin32FileOpenDesperatelyA(lppszArgv[nIdx], FILE_FLAG_SEQUENTIAL_SCAN, NULL, &lpbvioFile))
        {
            if(UnRagp(lpbvioFile, &ForEachRagpFile, NULL))
            {
                puts("OK");
            }
            else
            {
                puts("NG");
            }

            BvIORelease(&lpbvioFile);
        }
    }

    return 0;
}
