/***************************************************************************
 *  This file is part of Qthid.
 *
 *  Copyright (C) 2010       Howard Long, G6LVB
 *  Copyright (C) 2011       Mario Lorenz, DL5MLO
 *  Copyright (C) 2011-2012  Alexandru Csete, OZ9AEC
 *
 *  Qthid is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Qthid is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Qthid.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#ifndef FCD_H
#define FCD_H 1


#ifdef FCD
#define EXTERN
#define ASSIGN(x) =x
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
#include "fcdhidcmd.h"


/** \brief FCD mode enumeration. */
typedef enum {
    FCD_MODE_NONE,  /*!< No FCD detected. */
    FCD_MODE_BL,    /*!< FCD present in bootloader mode. */
    FCD_MODE_APP    /*!< FCD present in application mode. */
} FCD_MODE_ENUM; // The current mode of the FCD: no FCD, in bootloader mode or in normal application mode

#ifdef __cplusplus
extern "C" {
#endif

/* Application functions */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdGetMode(void);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdGetFwVerStr(char *str);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppReset(void);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetFreqKhz(int nFreq);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetFreq(unsigned int uFreq, unsigned int *rFreq);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetFreq(unsigned int *rFreq);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetLna(char enabled);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetLna(char *enabled);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetRfFilter(tuner_rf_filter_t filter);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetRfFilter(tuner_rf_filter_t *filter);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetMixerGain(char enabled);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetMixerGain(char *enabled);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetIfGain(unsigned char gain);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetIfGain(unsigned char *gain);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetIfFilter(tuner_if_filter_t filter);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetIfFilter(tuner_if_filter_t *filter);

EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetBiasTee(char enabled);
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetBiasTee(char *enabled);

#ifdef __cplusplus
}
#endif

#endif // FCD_H
