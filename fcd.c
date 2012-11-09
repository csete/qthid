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

#define FCD
#include <string.h>
#ifdef WIN32
    #include <malloc.h>
#else
    #include <stdlib.h>
#endif
#include <stdio.h>
#include "hidapi.h"
#include "fcdhidcmd.h"
#include "fcd.h"


#define FALSE 0
#define TRUE 1
typedef int BOOL;


const unsigned short _usVID=0x04D8;  /*!< USB vendor ID. */
const unsigned short _usPID=0xFB31;  /*!< USB product ID. */



/** \brief Open FCD device.
  * \return Pointer to the FCD HID device or NULL if none found
  *
  * This function looks for FCD devices connected to the computer and
  * opens the first one found.
  */
static hid_device *fcdOpen(void)
{
    struct hid_device_info *phdi=NULL;
    hid_device *phd=NULL;
    char *pszPath=NULL;

    phdi=hid_enumerate(_usVID,_usPID);
    if (phdi==NULL)
    {
        return NULL; // No FCD device found
    }

    pszPath=strdup(phdi->path);
    if (pszPath==NULL)
    {
        return NULL;
    }

    hid_free_enumeration(phdi);
    phdi=NULL;

    if ((phd=hid_open_path(pszPath)) == NULL)
    {
        free(pszPath);
        pszPath=NULL;

        return NULL;
    }

    free(pszPath);
    pszPath=NULL;

    return phd;
}


/** \brief Close FCD HID device. */
static void fcdClose(hid_device *phd)
{
    hid_close(phd);
}


/** \brief Get FCD mode.
  * \return The current FCD mode.
  * \sa FCD_MODE_ENUM
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdGetMode(void)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];
    FCD_MODE_ENUM fcd_mode = FCD_MODE_NONE;


    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    /* Send a BL Query Command */
    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_BL_QUERY;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    /* first check status bytes then check which mode */
    if (aucBufIn[0]==FCD_CMD_BL_QUERY && aucBufIn[1]==1) {

        /* In bootloader mode we have the string "FCDBL" starting at acBufIn[2] **/
        if (strncmp((char *)(aucBufIn+2), "FCDBL", 5) == 0) {
            fcd_mode = FCD_MODE_BL;
        }
        /* In application mode we have "FCDAPP_18.06" where the number is the FW version */
        else if (strncmp((char *)(aucBufIn+2), "FCDAPP", 6) == 0) {
            fcd_mode = FCD_MODE_APP;
        }
        /* either no FCD or firmware less than 18f */
        else {
            fcd_mode = FCD_MODE_NONE;
        }
    }

    return fcd_mode;
}


/** \brief Get FCD firmware version as string.
  * \param str The returned vesion number as a 0 terminated string (must be pre-allocated)
  * \return The current FCD mode.
  * \sa FCD_MODE_ENUM
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdGetFwVerStr(char *str)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];
    FCD_MODE_ENUM fcd_mode = FCD_MODE_NONE;

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    /* Send a BL Query Command */
    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_BL_QUERY;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    /* first check status bytes then check which mode */
    if (aucBufIn[0]==FCD_CMD_BL_QUERY && aucBufIn[1]==1) {

        /* In bootloader mode we have the string "FCDBL" starting at acBufIn[2] **/
        if (strncmp((char *)(aucBufIn+2), "FCDBL", 5) == 0) {
            fcd_mode = FCD_MODE_BL;
        }
        /* In application mode we have "FCDAPP_18.06" where the number is the FW version */
        else if (strncmp((char *)(aucBufIn+2), "FCDAPP", 6) == 0) {
            strncpy(str, (char *)(aucBufIn+9), 5);
            str[5] = 0;
            fcd_mode = FCD_MODE_APP;
        }
        /* either no FCD or firmware less than 18f */
        else {
            fcd_mode = FCD_MODE_NONE;
        }
    }

    return fcd_mode;
}


/** \brief Reset FCD to bootloader mode.
  * \return FCD_MODE_NONE
  *
  * This function is used to switch the FCD into bootloader mode in which
  * various firmware operations can be performed.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppReset(void)
{
    hid_device *phd=NULL;
    //unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    // Send an App reset command
    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_RESET;
    hid_write(phd, aucBufOut, 65);

    /** FIXME: hid_read() will occasionally hang due to a pthread_cond_wait() never returning.
        It seems that the read_callback() in hid-libusb.c will never receive any
        data during the reconfiguration. Since the same logic works in the native
        windows application, it could be a libusb thing. Anyhow, since the value
        returned by this function is not used, we may as well just skip the hid_read()
        and return FME_NONE.
        Correct switch from APP to BL mode can be observed in /var/log/messages (linux)
        (when in bootloader mode the device version includes 'BL')
    */
    /*
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]==FCDCMDAPPRESET && aucBufIn[1]==1)
    {
        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }
    FCDClose(phd);
    phd=NULL;
    return FME_BL;
    */

    fcdClose(phd);
    phd = NULL;

    return FCD_MODE_NONE;

}


/** \brief Set FCD frequency with kHz resolution.
  * \param nFreq The new frequency in kHz.
  * \return The FCD mode.
  *
  * This function sets the frequency of the FCD with 1 kHz resolution. The parameter
  * nFreq must already contain any necessary frequency corrention.
  *
  * \sa fcdAppSetFreq
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetFreqKhz(int nFreq)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    // Send an App reset command
    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_FREQ_KHZ;
    aucBufOut[2] = (unsigned char)nFreq;
    aucBufOut[3] = (unsigned char)(nFreq>>8);
    aucBufOut[4] = (unsigned char)(nFreq>>16);
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_SET_FREQ_KHZ && aucBufIn[1]==1)
    {
        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Set FCD frequency with Hz resolution.
  * \param nFreq The new frequency in Hz.
  * \param rFreq The actual frequency in Hz returned by the FCD (can be NULL).
  * \return The FCD mode.
  *
  * This function sets the frequency of the FCD with 1 Hz resolution. The parameter
  * nFreq must already contain any necessary frequency corrention. If rFreq is not NULL
  * it will contain the actual frequency as returned by the API call.
  *
  * \sa fcdAppSetFreqKhz
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetFreq(unsigned int uFreq, unsigned int *rFreq)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    // Send an App reset command
    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_FREQ_HZ;
    aucBufOut[2] = (unsigned char) uFreq;
    aucBufOut[3] = (unsigned char) (uFreq >> 8);
    aucBufOut[4] = (unsigned char) (uFreq >> 16);
    aucBufOut[5] = (unsigned char) (uFreq >> 24);
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_SET_FREQ_HZ && aucBufIn[1]==1)
    {
        if (rFreq != NULL)
        {
            *rFreq = 0;
            *rFreq = (unsigned int) aucBufIn[2];
            *rFreq += (unsigned int) (aucBufIn[3] << 8);
            *rFreq += (unsigned int) (aucBufIn[4] << 16);
            *rFreq += (unsigned int) (aucBufIn[5] << 24);
        }

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}

/** \brief Get current FCD frequency with Hz resolution.
  * \param rFreq The current frequency in Hz returned by the FCD.
  * \return The FCD mode.
  *
  * This function reads the frequency of the FCD with 1 Hz resolution. The parameter
  * nFreq must already contain any necessary frequency corrention.
  *
  * \sa fcdAppSetFreq
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetFreq(unsigned int *rFreq)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (rFreq == NULL)
    {
        return FCD_MODE_NONE;
    }

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_FREQ_HZ;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_FREQ_HZ && aucBufIn[1]==1)
    {
        *rFreq = 0;
        *rFreq = (unsigned int) aucBufIn[2];
        *rFreq += (unsigned int) (aucBufIn[3] << 8);
        *rFreq += (unsigned int) (aucBufIn[4] << 16);
        *rFreq += (unsigned int) (aucBufIn[5] << 24);

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Enable/disable LNA.
  * \param enabled Whether to enable or disable the LNA (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetLna(char enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_LNA_GAIN;
    aucBufOut[2] = (unsigned char) enabled;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_LNA_GAIN) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get LNA status
  * \param enabled The current staus of the LNA (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetLna(char *enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (enabled == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_LNA_GAIN;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_LNA_GAIN && aucBufIn[1]==1)
    {
        *enabled = aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Select RF filter.
  * \param filter The new RF filter.
  * \return The FCD mode.
  *
  * \note RF filter is selected by the FCD; using this function may not be very useful, except when
  *       firmware selects wrong filter.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetRfFilter(tuner_rf_filter_t filter)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (filter > TRFE_875_2000)
    {
        return FCD_MODE_NONE;
    }

    phd = fcdOpen();
    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_RF_FILTER;
    aucBufOut[2] = (unsigned char) filter;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_RF_FILTER) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get RF filter selection.
  * \param filter The current RF filter selection.
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetRfFilter(tuner_rf_filter_t *filter)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (filter == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_RF_FILTER;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_RF_FILTER && aucBufIn[1]==1)
    {
        *filter = (tuner_rf_filter_t) aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}

/** \brief Enable/disable Mixer gain.
  * \param enabled Whether to enable or disable the mixer gain (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetMixerGain(char enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_MIXER_GAIN;
    aucBufOut[2] = (unsigned char) enabled;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_MIXER_GAIN) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get mixer gain status
  * \param enabled The current staus of the mixer gain (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetMixerGain(char *enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (enabled == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_MIXER_GAIN;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_MIXER_GAIN && aucBufIn[1]==1)
    {
        *enabled = aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Set IF gain.
  * \param gain The new IF gain between 0 and 59 dB.
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetIfGain(unsigned char gain)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (gain > 59)
    {
        return FCD_MODE_NONE;
    }

    phd = fcdOpen();
    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_IF_GAIN;
    aucBufOut[2] = gain;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_IF_GAIN) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get IF gain.
  * \param filter The current IF gain between 0 and 59 dB.
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetIfGain(unsigned char *gain)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (gain == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_IF_GAIN;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_IF_GAIN && aucBufIn[1]==1)
    {
        *gain = aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Select IF filter.
  * \param filter The new IF filter.
  * \return The FCD mode.
  *
  * \note IF filter is selected by the FCD; using this function may not be very useful, except when
  *       firmware selects the wrong filter.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetIfFilter(tuner_if_filter_t filter)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (filter > TIFE_8MHZ)
    {
        return FCD_MODE_NONE;
    }

    phd = fcdOpen();
    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_IF_FILTER;
    aucBufOut[2] = (unsigned char) filter;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_IF_FILTER) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get IF filter selection.
  * \param filter The current IF filter selection.
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetIfFilter(tuner_if_filter_t *filter)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (filter == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_IF_FILTER;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_IF_FILTER && aucBufIn[1]==1)
    {
        *filter = (tuner_if_filter_t) aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}


/** \brief Enable/disable Bias T.
  * \param enabled Whether to enable or disable the Bias T (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppSetBiasTee(char enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd = fcdOpen();

    if (phd == NULL)
    {
        return FCD_MODE_NONE;
    }

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_SET_BIAS_TEE;
    aucBufOut[2] = (unsigned char) enabled;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    return (aucBufIn[0] == FCD_CMD_APP_SET_BIAS_TEE) ? FCD_MODE_APP : FCD_MODE_BL;
}

/** \brief Get Bias T status
  * \param enabled The current staus of the Bias T (1=ON 0=OFF).
  * \return The FCD mode.
  */
EXTERN FCD_API_EXPORT FCD_API_CALL FCD_MODE_ENUM fcdAppGetBiasTee(char *enabled)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    if (enabled == NULL)
        return FCD_MODE_NONE;

    phd = fcdOpen();
    if (phd == NULL)
        return FCD_MODE_NONE;

    aucBufOut[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd for one report
    aucBufOut[1] = FCD_CMD_APP_GET_BIAS_TEE;
    hid_write(phd, aucBufOut, 65);
    memset(aucBufIn, 0xCC, 65); // Clear out the response buffer
    hid_read(phd, aucBufIn, 65);

    fcdClose(phd);
    phd = NULL;

    if (aucBufIn[0]==FCD_CMD_APP_GET_BIAS_TEE && aucBufIn[1]==1)
    {
        *enabled = aucBufIn[2];

        return FCD_MODE_APP;
    }

    return FCD_MODE_BL;
}
