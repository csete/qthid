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
#ifndef FCD_HID_CMD_H
#define FCD_HID_CMD_H 1


/* Commands applicable in bootloader mode */
#define FCD_CMD_BL_QUERY                1  /*!< Returns string with "FCDAPP version". */

/* Commands applicable in application mode */
#define FCD_CMD_APP_SET_FREQ_KHZ     100 /*!< Send with 3 byte unsigned little endian frequency in kHz. */
#define FCD_CMD_APP_SET_FREQ_HZ      101 /*!< Send with 4 byte unsigned little endian frequency in Hz, returns with actual frequency set in Hz */
#define FCD_CMD_APP_GET_FREQ_HZ      102 /*!< Returns 4 byte unsigned little endian frequency in Hz. */

#define FCD_CMD_APP_SET_LNA_GAIN     110 /*!< Send one byte: 1=ON, 0=OFF. */
#define FCD_CMD_APP_SET_RF_FILTER    113 /*!< Send one byte, see tuner_rf_filter_t enum. */
#define FCD_CMD_APP_SET_MIXER_GAIN   114 /*!< Send one byte:1=ON, 0=OFF. */
#define FCD_CMD_APP_SET_IF_GAIN      117 /*!< Send one byte, valid value 0 to 59 (dB). */
#define FCD_CMD_APP_SET_IF_FILTER    122 /*!< Send one byte, see tuner_if_filter_t enum. */
#define FCD_CMD_APP_SET_BIAS_TEE     126 /*!< Bias T for ext LNA. Send with one byte: 1=ON, 0=OFF. */

#define FCD_CMD_APP_GET_LNA_GAIN     150 /*!< Returns one byte: 1=ON, 0=OFF. */
#define FCD_CMD_APP_GET_RF_FILTER    153 /*!< Returns one byte, see tuner_rf_filter_t enum. */
#define FCD_CMD_APP_GET_MIXER_GAIN   154 /*!< Returns one byte: 1=ON, 0=OFF. */
#define FCD_CMD_APP_GET_IF_GAIN      157 /*!< Returns one byte, valid value 0 to 59 (dB). */
#define FCD_CMD_APP_GET_IF_FILTER    162 /*!< Returns one byte, see tuner_if_filter_t enum. */
#define FCD_CMD_APP_GET_BIAS_TEE     166 /*!< Returns one byte: 1=ON, 0=OFF. */

#define FCD_CMD_APP_RESET            255 /*!< Reset to bootloader. */

typedef enum
{
    TRFE_0_4 = 0,
    TRFE_4_8,
    TRFE_8_16,
    TRFE_16_32,
    TRFE_32_75,
    TRFE_75_125,
    TRFE_125_250,
    TRFE_145,
    TRFE_410_875,
    TRFE_435,
    TRFE_875_2000
} tuner_rf_filter_t;

typedef enum
{
    TIFE_200KHZ = 0,
    TIFE_300KHZ,
    TIFE_600KHZ,
    TIFE_1536KHZ,
    TIFE_5MHZ,
    TIFE_6MHZ,
    TIFE_7MHZ,
    TIFE_8MHZ
} tuner_if_filter_t;


#endif // FCD_HID_CMD_H
