/***************************************************************************
 *  This file is part of Qthid.
 * 
 *  Copyright (C) 2010  Howard Long, G6LVB
 * 
 *  Qthid is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

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

