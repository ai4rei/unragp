#include <stddef.h>

#include <btypes.h>
#include <bvio.h>
#include <memtaf.h>

#include "unragp.h"

bool __WDECL UnRagp(BVIO* const lpbvioRagp, bool (__WDECL* lpfnForEach)(char const* const lpszPath, void const* const lpData, size_t const uDataLength, void* lpContext), void* lpContext)
{
#pragma pack(push,1)
    struct RAGP_HEADER
    {
        ulong_t ulSignature;
        ulong_t ulCount;
        ulong_t ulIndex;
    }
    Head;
    struct RAGP_FILE
    {
        char szName[512];
        ulong_t ulLength;
        ulong_t ulOffset;
    }
    File;
#pragma pack(pop)
    bool bSuccess = false;
    size_t uPulled = 0;

    if(BvIORndPull(lpbvioRagp, 0, &Head, sizeof(Head), &uPulled) && sizeof(Head)==uPulled)
    {
        if(Head.ulSignature=='PGAR' /* RAGP */)
        {
            ulong_t ulIdx;
            size_t uIndexOffset = Head.ulIndex;

            for(ulIdx = 0; ulIdx<Head.ulCount; ulIdx++)
            {
                ubyte_t* lpucData = NULL;

                if(!BvIORndPull(lpbvioRagp, uIndexOffset, &File, sizeof(File), &uPulled) || sizeof(File)!=uPulled)
                {
                    break;
                }

                uIndexOffset+= uPulled;

                if(File.ulLength>0)
                {
                    if(!MemTAllocEx(&lpucData, File.ulLength))
                    {
                        continue;
                    }

                    if(!BvIORndPull(lpbvioRagp, File.ulOffset, lpucData, File.ulLength, &uPulled) || File.ulLength!=uPulled)
                    {
                        MemTFree(&lpucData);
                        continue;
                    }
                }

                if(!lpfnForEach(File.szName, lpucData, File.ulLength, lpContext))
                {
                    MemTFree(&lpucData);
                    break;
                }

                MemTFree(&lpucData);
            }

            bSuccess = ulIdx==Head.ulCount;
        }
    }

    return bSuccess;
}
