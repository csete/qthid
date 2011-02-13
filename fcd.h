#ifdef FCD
#define EXTERN
#define ASSIGN (x) =x
#else
#define EXTERN extern
#define ASSIGN(x)
#endif

#ifdef _WIN32
#define FCD_API_EXPORT __declspec(dllexport)
#define FCD_API_CALL  _stdcall
#else
#define FCD_API_EXPORT
#define FCD_API_CALL
#endif

#include <inttypes.h>
typedef enum {FME_NONE,FME_BL,FME_APP} FCDMODEENUM; // The current mode of the FCD: none inserted, in bootloader mode or in normal application mode

#ifdef __cplusplus
extern "C" {
#endif

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDGetMode(void);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDAppReset(void);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDAppSetFreqkHz(int nFreq);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLReset(void);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLErase(void);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLWriteFirmware(char *pc,int64_t n64Size);
EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLVerifyFirmware(char *pc,int64_t n64Size);

#ifdef __cplusplus
}
#endif

