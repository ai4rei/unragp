#ifndef UNRAGP_H
#define UNRAGP_H

#ifdef __cplusplus
extern "C"
{
#endif  /* __cplusplus */

bool __WDECL UnRagp(BVIO* const lpbvioRagp, bool (__WDECL* lpfnForEach)(char const* const lpszPath, void const* const lpData, size_t const uDataLength, void* lpContext), void* lpContext);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* UNRAGP_H */
