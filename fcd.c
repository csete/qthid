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
#include "hidapi.h"
#include "fcd.h"

const unsigned short _usVID=0x04D8;
const unsigned short _usPID=0xFB56;

#define FCDCMDAPPSETFREQKHZ 100
#define FCDCMDAPPSENDI2CBYTE 200
#define FCDCMDAPPRECVI2CBYTE 201
#define FCDCMDAPPRESET 255

#define FCDCMDBLQUERY 1
#define FCDCMDBLRESET 8
#define FCDCMDBLERASE 24
#define FCDCMDBLSETBYTEADDR 25
#define FCDCMDBLGETBYTEADDRRANGE 26
#define FCDCMDBLWRITEFLASHBLOCK 27
#define FCDCMDBLREADFLASHBLOCK 28

#define FALSE 0
#define TRUE 1

typedef int BOOL;

static hid_device *FCDOpen(void)
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

    if ((phd=hid_open_path(pszPath))==NULL)
    {
        free(pszPath);
        pszPath=NULL;
        return NULL;
    }
    free(pszPath);
    pszPath=NULL;
    return phd;
}

static void FCDClose(hid_device *phd)
{
    hid_close(phd);
}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDGetMode(void)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];
    FCDMODEENUM fcd_mode = FME_NONE;


    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }

    // Send a BL Query Command
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLQUERY;
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    // If it's in bootloader mode, we get a sensible response, in app mode the response to this command isn't understood
    if (aucBufIn[0]==FCDCMDBLQUERY && aucBufIn[1]==1 && strncmp((char *)(aucBufIn+2),"FCDBL",5)==0)
    {
        fcd_mode = FME_BL;
    }
    else {
        fcd_mode = FME_APP;
    }

    FCDClose(phd);
    phd=NULL;


#if 0
    /** This code will be useful with FW 18f or later **/
    /* first check status bytes then check which mode */
    if (aucBufIn[0]==FCDCMDBLQUERY && aucBufIn[1]==1) {

        /* In bootloader mode we have the string "FCDBL" starting at acBufIn[2] **/
        if (strncmp((char *)(aucBufIn+2), "FCDBL", 5) == 0) {
            FCDClose(phd);
            phd = NULL;
            fcd_mode = FME_BL;
        }
        /* In application mode we have "FCDAPP_18.06" where the number is the FW version */
        else if (strncmp((char *)(aucBufIn+2), "FCDAPP", 6) == 0) {
            printf("Version: %s", (char *)(aucBufIn+8));
            FCDClose(phd);
            phd = NULL;
            fcd_mode = FME_APP;
        }
        else {
            FCDClose(phd);
            phd = NULL;
            fcd_mode = FME_APP;
        }
    }
#endif


    return fcd_mode;
}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDAppReset(void)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }
    // Send an App reset command
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDAPPRESET;
    hid_write(phd,aucBufOut,65);

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

    FCDClose(phd);
    phd=NULL;

    return FME_NONE;

}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDAppSetFreqkHz(int nFreq)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }
    // Send an App reset command
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDAPPSETFREQKHZ;
    aucBufOut[2]=(unsigned char)nFreq;
    aucBufOut[3]=(unsigned char)(nFreq>>8);
    aucBufOut[4]=(unsigned char)(nFreq>>16);
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]==FCDCMDAPPSETFREQKHZ && aucBufIn[1]==1)
    {
        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }
    FCDClose(phd);
    phd=NULL;
    return FME_BL;
}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLReset(void)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }
    // Send an BL reset command
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLRESET;
    hid_write(phd,aucBufOut,65);

    /** FIXME: hid_read() will hang due to a pthread_cond_wait() never returning.
        It seems that the read_callback() in hid-libusb.c will never receive any
        data during the reconfiguration. Since the same logic works in the native
        windows application, it could be a libusb thing. Anyhow, since the value
        returned by this function is not used, we may as well jsut skip the hid_read()
        and return FME_NONE.
        Correct switch from BL to APP mode can be observed in /var/log/messages (linux)
        (when in bootloader mode the device version includes 'BL')
    */
    /*
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]==FCDCMDBLRESET && aucBufIn[1]==1)
    {
        FCDClose(phd);
        phd=NULL;
        return FME_BL;
    }
    FCDClose(phd);
    phd=NULL;
    return FME_APP;
    */

    FCDClose(phd);
    phd=NULL;

    return FME_NONE;

}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLErase(void)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }
    // Send an App reset command
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLERASE;
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]==FCDCMDBLERASE && aucBufIn[1]==1)
    {
        FCDClose(phd);
        phd=NULL;
        return FME_BL;
    }
    FCDClose(phd);
    phd=NULL;
    return FME_APP;
}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLWriteFirmware(char *pc,int64_t n64Size)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];
    uint32_t u32AddrStart;
    uint32_t u32AddrEnd;
    uint32_t u32Addr;
    BOOL bFinished=FALSE;

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }

    // Get the valid flash address range
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLGETBYTEADDRRANGE;
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]!=FCDCMDBLGETBYTEADDRRANGE || aucBufIn[1]!=1)
    {

        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }
    u32AddrStart=
        aucBufIn[2]+
        (((uint32_t)aucBufIn[3])<<8)+
        (((uint32_t)aucBufIn[4])<<16)+
        (((uint32_t)aucBufIn[5])<<24);
    u32AddrEnd=
        aucBufIn[6]+
        (((uint32_t)aucBufIn[7])<<8)+
        (((uint32_t)aucBufIn[8])<<16)+
        (((uint32_t)aucBufIn[9])<<24);

    // Set start address for flash
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLSETBYTEADDR;
    aucBufOut[2]=((unsigned char)u32AddrStart);
    aucBufOut[3]=((unsigned char)(u32AddrStart>>8));
    aucBufOut[4]=((unsigned char)(u32AddrStart>>16));
    aucBufOut[5]=((unsigned char)(u32AddrStart>>24));
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]!=FCDCMDBLSETBYTEADDR || aucBufIn[1]!=1)
    {

        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }

    // Write blocks
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLWRITEFLASHBLOCK;
    for (u32Addr=u32AddrStart;u32Addr+47<u32AddrEnd && u32Addr+47<n64Size && !bFinished;u32Addr+=48)
    {
        memcpy(&aucBufOut[3],&pc[u32Addr],48);

        hid_write(phd,aucBufOut,65);
        memset(aucBufIn,0xCC,65); // Clear out the response buffer
        hid_read(phd,aucBufIn,65);

        if (aucBufIn[0]!=FCDCMDBLWRITEFLASHBLOCK || aucBufIn[1]!=1)
        {
            bFinished=TRUE;
            FCDClose(phd);
            phd=NULL;
            return FME_APP;
        }
    }

    FCDClose(phd);
    phd=NULL;
    return FME_BL;
}

EXTERN FCD_API_EXPORT FCD_API_CALL FCDMODEENUM FCDBLVerifyFirmware(char *pc,int64_t n64Size)
{
    hid_device *phd=NULL;
    unsigned char aucBufIn[65];
    unsigned char aucBufOut[65];
    uint32_t u32AddrStart;
    uint32_t u32AddrEnd;
    uint32_t u32Addr;
    BOOL bFinished=FALSE;

    phd=FCDOpen();

    if (phd==NULL)
    {
        return FME_NONE;
    }

    // Get the valid flash address range
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLGETBYTEADDRRANGE;
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]!=FCDCMDBLGETBYTEADDRRANGE || aucBufIn[1]!=1)
    {

        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }
    u32AddrStart=
        aucBufIn[2]+
        (((uint32_t)aucBufIn[3])<<8)+
        (((uint32_t)aucBufIn[4])<<16)+
        (((uint32_t)aucBufIn[5])<<24);
    u32AddrEnd=
        aucBufIn[6]+
        (((uint32_t)aucBufIn[7])<<8)+
        (((uint32_t)aucBufIn[8])<<16)+
        (((uint32_t)aucBufIn[9])<<24);

    // Set start address for flash
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLSETBYTEADDR;
    aucBufOut[2]=((unsigned char)u32AddrStart);
    aucBufOut[3]=((unsigned char)(u32AddrStart>>8));
    aucBufOut[4]=((unsigned char)(u32AddrStart>>16));
    aucBufOut[5]=((unsigned char)(u32AddrStart>>24));
    hid_write(phd,aucBufOut,65);
    memset(aucBufIn,0xCC,65); // Clear out the response buffer
    hid_read(phd,aucBufIn,65);

    if (aucBufIn[0]!=FCDCMDBLSETBYTEADDR || aucBufIn[1]!=1)
    {

        FCDClose(phd);
        phd=NULL;
        return FME_APP;
    }

    // Read blocks
    aucBufOut[0]=0; // Report ID, ignored
    aucBufOut[1]=FCDCMDBLREADFLASHBLOCK;
    for (u32Addr=u32AddrStart;u32Addr+47<u32AddrEnd && u32Addr+47<n64Size && !bFinished;u32Addr+=48)
    {
        hid_write(phd,aucBufOut,65);
        memset(aucBufIn,0xCC,65); // Clear out the response buffer
        hid_read(phd,aucBufIn,65);

        if (aucBufIn[0]!=FCDCMDBLREADFLASHBLOCK || aucBufIn[1]!=1)
        {
            bFinished=TRUE;
            FCDClose(phd);
            phd=NULL;
            return FME_APP;
        }
        if (memcmp(&aucBufIn[2],&pc[u32Addr],48)!=0)
        {
            bFinished=TRUE;
            FCDClose(phd);
            phd=NULL;
            return FME_APP;
        }
    }

    FCDClose(phd);
    phd=NULL;
    return FME_BL;
}
