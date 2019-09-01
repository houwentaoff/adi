/**
 * \file talise_config.c
 * \brief Contains Talise configuration settings for the Talise API
 *
 * Copyright 2015-2017 Analog Devices Inc.
 * Released under the AD9378-AD9379 API license, for more information see the "LICENSE.txt" file in this zip file.
 *
 * The top level structure taliseDevice_t talDevice uses keyword
 * extern to allow the application layer main() to have visibility
 * to these settings.
 */

#include "talise_types.h"
#include "talise_config.h"
#include "talise_error.h"

#ifdef ADI_ZYNQ_PLATFORM
#include "zynq_platform.h"
#endif
int16_t txFirCoefs[20] = {-54, 46, -127, 196, -359, 801, -2119, 19171, -1980, 1131, -487, 204, -129, 67, -12, 51, 0, 0, 0, 0};

int16_t rxFirCoefs[48] = {49, -71, -108, 146, 201, -261, -349, 439,
                         571, -701, -902, 1083, 1396, -1655, -2182, 2562,
                         3576, -4335, -7104, 9273, 30894, 30894, 9273, -7104,
                         -4335, 3576, 2562, -2182, -1655, 1396, 1083, -902,
                         -701, 571, 439, -349, -261, 201, 146, -108,
                         -71, 49, 33, -23, -7, 0, 0, 0};

int16_t obsrxFirCoefs[24] = {-15, 9, -13, 19, -28, 38, -73, 110,
                             -195, 409, -1006, 17711, -1006, 409, -195, 110,
                             -73, 38, -28, 19, -13, 9, -15, 0};
#ifdef ADI_ZYNQ_PLATFORM /** < Insert Customer Platform HAL State Container here>*/
/*
 * Platform Layer SPI settings - this structure is specific to ADI's platform layer code.
 * User should replace with their own structure or settings for their hardware
 */
zynqSpiSettings_t spiDev1 =
{
    .chipSelectIndex = 1,
    .writeBitPolarity = 0,
    .longInstructionWord = 1,
    .CPHA = 0,
    .CPOL = 0,
    .mode = 0,
    .spiClkFreq_Hz = 25000000
};

/*
 * Platform Layer settings - this structure is specific to ADI's platform layer code.
 * User should replace with their own structure or settings for their hardware
 * The structure is held in taliseDevice_t below as a void pointer, allowing
 * the customer to pass any information for their specific hardware down to the
 * hardware layer code.
 */
zynqAdiDev_t talDevHalInfo =
{
    .devIndex = 1,
    .spiSettings = &spiDev1,
    .spiErrCode = 0,
    .timerErrCode = 0,
    .gpioErrCode = 0,
    .logLevel = ADIHAL_LOG_ALL
};
#endif
/**
 *  TalDevice a structure used by the Talise API to hold the platform hardware
 *  structure information, as well as an internal Talise API state container
 *  (devStateInfo) of runtime information used by the API.
 **/
taliseDevice_t talDevice =
{
#ifdef ADI_ZYNQ_PLATFORM
    /* Void pointer of users platform HAL settings to pass to HAL layer calls
     * Talise API does not use the devHalInfo member */
     .devHalInfo = &talDevHalInfo,
#else
     .devHalInfo = NULL,     /*/** < Insert Customer Platform HAL State Container here>*/
#endif
     /* devStateInfo is maintained internal to the Talise API, just create the memory */
     .devStateInfo = {0}

};

taliseInit_t talInit =
{
    /* SPI settings */
    .spiSettings =
    {
        .MSBFirst            = 1,  /* 1 = MSBFirst, 0 = LSBFirst */
        .enSpiStreaming      = 0,  /* Not implemented in ADIs platform layer. SW feature to improve SPI throughput */
        .autoIncAddrUp       = 1,  /* Not implemented in ADIs platform layer. For SPI Streaming, set address increment direction. 1= next addr = addr+1, 0:addr=addr-1 */
        .fourWireMode        = 1,  /* 1: Use 4-wire SPI, 0: 3-wire SPI (SDIO pin is bidirectional). NOTE: ADI's FPGA platform always uses 4-wire mode */
        .cmosPadDrvStrength  = TAL_CMOSPAD_DRV_2X /* Drive strength of CMOS pads when used as outputs (SDIO, SDO, GP_INTERRUPT, GPIO 1, GPIO 0) */
    },

    /* Rx settings */
    .rx =
    {
        .rxProfile =
        {
            /* Rx 200MHz @ 245.76, DEC5 */
            .rxFir =
            {
                .gain_dB = -6,      /* filter gain */
                .numFirCoefs = 48, /* number of coefficients in the FIR filter */
                .coefs = &rxFirCoefs[0]
            },
            .rxFirDecimation    = 2,      /*  Rx FIR decimation (1,2,4) */
            .rxDec5Decimation   = 5,      /*  Decimation of Dec5 or Dec4 filter (5,4) */
            .rhb1Decimation     = 1,      /*  RX Half band 1 decimation (1 or 2) */
            .rxOutputRate_kHz   = 245760, /*  Rx output data rate in kHz */
            .rfBandwidth_Hz     = 100000000,/*  The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz = 100000,/*  Rx BBF 3dB corner in kHz */
            .rxAdcProfile = {202, 134, 163, 91, 1280, 666, 1371, 175, 1297, 67, 1012, 29, 46, 45, 35, 205, 0, 0, 0, 0, 51, 0, 7, 6, 42, 0, 7, 6, 42, 0, 25, 27, 0, 0, 25, 27, 0, 0, 165, 44, 21, 905},
            .rxDdcMode = TAL_RXDDC_BYPASS,	 /* Rx DDC mode */
			.rxNcoShifterCfg = 
			{
				.bandAInputBandWidth_kHz = 0,
				.bandAInputCenterFreq_kHz = 0,
				.bandANco1Freq_kHz = 0,
				.bandANco2Freq_kHz = 0,
				.bandBInputBandWidth_kHz = 0,
				.bandBInputCenterFreq_kHz = 0,
				.bandBNco1Freq_kHz = 0,
				.bandBNco2Freq_kHz = 0
			}
        },
        .framerSel = TAL_FRAMER_A,       /*  Rx JESD204b framer configuration */
        .rxGainCtrl =
        {
            .gainMode        = TAL_MGC,  /* taliserxGainMode_t gainMode; */
            .rx1GainIndex    = 255,  /* uint8_t rx1GainIndex */
            .rx2GainIndex    = 255,  /* uint8_t rx2GainIndex */
            .rx1MaxGainIndex = 255,  /* uint8_t rx1MaxGainIndex; */
            .rx1MinGainIndex = 195,  /* uint8_t rx1MinGainIndex; */
            .rx2MaxGainIndex = 255,  /* uint8_t rx2MaxGainIndex; */
            .rx2MinGainIndex = 195   /* uint8_t rx2MinGainIndex; */
        },
        .rxChannels = TAL_RXOFF      /*  The desired Rx Channels to enable during initialization */
    },

    /* Tx settings */
    .tx =
    {
        .txProfile =
        {
            .dacDiv = 1,	/* The divider used to generate the DAC clock */
            .txFir =
            {
                .gain_dB = 6,       /* filter gain */
                .numFirCoefs = 20, /* number of coefficients in the FIR filter */
                .coefs = &txFirCoefs[0]
            },
            .txFirInterpolation      = 1,       /* The Tx digital FIR filter interpolation (1,2,4) */
            .thb1Interpolation       = 2,       /* Tx Halfband1 filter interpolation (1,2) */
            .thb2Interpolation       = 2,       /* Tx Halfband2 filter interpolation (1,2) */
            .thb3Interpolation       = 1,       /* Tx Halfband3 (HB3) filter interpolation (1,2) */
            .txInt5Interpolation     = 1,       /* Tx Int5 filter interpolation (1,5) */
            .txInputRate_kHz         = 491520,  /* Tx input data rate in kHz */
            .primarySigBandwidth_Hz  = 75000000,    /* Primary Signal BW */
            .rfBandwidth_Hz          = 200000000,   /* The Tx RF passband bandwidth for the profile */
            .txDac3dBCorner_kHz      = 450000,      /* The DAC filter 3dB corner in kHz */
            .txBbf3dBCorner_kHz      = 225000,       /* Tx BBF 3dB corner in kHz */
            .loopBackAdcProfile = {203, 139, 158, 90, 1280, 639, 1435, 172, 1273, 62, 1007, 30, 48, 44, 35, 207, 0, 0, 0, 0, 52, 0, 7, 6, 42, 0, 7, 6, 42, 0, 25, 27, 0, 0, 25, 27, 0, 0, 165, 44, 31, 905}
        },
        .deframerSel = TAL_DEFRAMER_A,       /* Talise JESD204b deframer config for the Tx data path */
        .txChannels = TAL_TX1TX2,               /* The desired Tx channels to enable during initialization */
        .txAttenStepSize = TAL_TXATTEN_0P05_DB,  /* Tx Attenuation step size */
        .tx1Atten_mdB = 10000,               /* Initial and current Tx1 Attenuation */
        .tx2Atten_mdB = 10000,              /* Initial and current Tx2 Attenuation */
        .disTxDataIfPllUnlock    = TAL_TXDIS_TX_RAMP_DOWN_TO_ZERO  /* Options to disable the transmit data when the RFPLL unlocks. */
    },

    /* ObsRx settings */
    .obsRx =
    {
        .orxProfile =
        {
            /* ORx 450Mhz @ 491.52 MSPS, DEC5  */
            .rxFir =
            {
                .gain_dB = 6,               /* filter gain */
                .numFirCoefs = 24,          /* number of coefficients in the FIR filter */
                .coefs = &obsrxFirCoefs[0]
            },
            .rxFirDecimation    = 1,        /* Rx FIR decimation (1,2,4) */
            .rxDec5Decimation   = 5,        /* Decimation of Dec5 or Dec4 filter (5,4) */
            .rhb1Decimation     = 1,        /* RX Half band 1 decimation (1 or 2) */
            .orxOutputRate_kHz  = 491520,   /* ORx output data rate in kHz */
            .rfBandwidth_Hz     = 200000000,/* The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz = 100000,   /* Rx BBF 3dB corner in kHz */
            .orxLowPassAdcProfile = {175, 155, 160, 90, 1280,   896,  1671, 472, 1094, 150,  1160,  21, 48, 35, 34, 204, 0, 0, 0, 0,  51, 0, 7, 6, 42, 0, 7, 6, 42, 0, 25, 27, 0, 0, 25, 27, 0, 0, 165, 44, 31, 905},
            .orxBandPassAdcProfile = {115, 129, 149, 88, 1280,  2619,  1670,  64, 1066, 879,   874, 107, 17, 15, 26, 177, 0, 0, 0, 0, 44, 0, 7, 6,  42, 0, 7, 6, 42, 0, 25, 27, 0, 0, 25, 27, 0, 0, 165, 44, 31, 905},
            .orxDdcMode            = TAL_ORXDDC_DISABLED, /* ORx DDC mode */
            .orxMergeFilter =   {-98,413,-310,-382,987,-488,-1131,2215,-622,-4016,9277,21173}
        },
        .orxGainCtrl =
        {
            .gainMode 	      = TAL_MGC,
            .orx1GainIndex    = 255,
            .orx2GainIndex    = 255,
            .orx1MaxGainIndex = 255,
            .orx1MinGainIndex = 195,
            .orx2MaxGainIndex = 255,
            .orx2MinGainIndex = 195
        },
        .framerSel = TAL_FRAMER_B,          /* ObsRx JESD204b framer configuration structure */
        .obsRxChannelsEnable = TAL_ORX1,    /* The desired ObsRx Channels to enable during initialization */
        .obsRxLoSource = TAL_AUX_PLL            /* The ORx mixers can use the TX_PLL*/
    },

    /* Digital Clock settings */
    .clocks =
    {
        .deviceClock_kHz    = 245760,  		/* CLKPLL and device reference clock frequency in kHz */
        .clkPllVcoFreq_kHz  = 9830400,  	/* CLKPLL VCO frequency in kHz */
        .clkPllHsDiv        = TAL_HSDIV_2,  /* CLKPLL high speed clock divider */
        .rfPllUseExternalLo = 0,         	/*  1= Use external LO for RF PLL, 0 = use internal LO generation for RF PLL */
        .rfPllPhaseSyncMode = TAL_RFPLLMCS_NOSYNC           /* RFPLL MCS (Phase sync) mode */
    },

    /* JESD204B settings */
    .jesd204Settings =
    {
        /* Framer A settings */
        .framerA =
        {
            .bankId     = 0,    /* JESD204B Configuration Bank ID -extension to Device ID (Valid 0..15) */
            .deviceId   = 0,    /* JESD204B Configuration Device ID - link identification number. (Valid 0..255) */
            .lane0Id    = 0,    /* JESD204B Configuration starting Lane ID.  If more than one lane used, each lane will increment from the Lane0 ID. (Valid 0..31) */
            .M = 4,             /* number of ADCs (0, 2, or 4) - 2 ADCs per receive chain */
            .K = 32,            /* number of frames in a multiframe (default=32), F*K must be a multiple of 4. (F=2*M/numberOfLanes) */
            .F = 1,             /* F (number of bytes per JESD204 Frame) */
            .Np = 16,           /* Np (converter sample resolution) */
            .scramble = 1,      /* scrambling off if framerScramble= 0, if framerScramble>0 scramble is enabled. */
            .externalSysref = 1, /* 0=use internal SYSREF, 1= use external SYSREF */
            .serializerLanesEnabled = 0x03, /* serializerLanesEnabled - bit per lane, [0] = Lane0 enabled, [1] = Lane1 enabled */
            .serializerLaneCrossbar = 0xE4, /* serializerLaneCrossbar; */
            .lmfcOffset = 0,    /* lmfcOffset - LMFC offset value for deterministic latency setting */
            .newSysrefOnRelink = 0, /* newSysrefOnRelink */
            .syncbInSelect = 0,     /* syncbInSelect; */
            .overSample = 0,         /* 1=overSample, 0=bitRepeat */
            .syncbInLvdsMode = 1,
            .syncbInLvdsPnInvert = 0,
			.enableManualLaneXbar = 0 /* 0=auto, 1=manual */
        },
        /* Framer B settings */
        .framerB =
        {
            .bankId = 0,      /* JESD204B Configuration Bank ID -extension to Device ID (Valid 0..15) */
            .deviceId = 0,    /* JESD204B Configuration Device ID - link identification number. (Valid 0..255) */
            .lane0Id = 0,     /* JESD204B Configuration starting Lane ID.  If more than one lane used, each lane will increment from the Lane0 ID. (Valid 0..31) */
            .M = 2,           /* number of ADCs (0, 2, or 4) - 2 ADCs per receive chain */
            .K = 32,          /* number of frames in a multiframe (default=32), F*K must be a multiple of 4. (F=2*M/numberOfLanes) */
            .F = 1,           /* F (number of bytes per JESD204 Frame) */
            .Np = 16,         /* Np (converter sample resolution) */
            .scramble = 1,    /* scrambling off if framerScramble= 0, if framerScramble>0 scramble is enabled. */
            .externalSysref = 1, /* 0=use internal SYSREF, 1= use external SYSREF */
            .serializerLanesEnabled  = 0x0C, /* serializerLanesEnabled - bit per lane, [0] = Lane0 enabled, [1] = Lane1 enabled */
            .serializerLaneCrossbar  = 0xE4, /* serializerLaneCrossbar; */
            .lmfcOffset = 0,                 /* lmfcOffset - LMFC offset value for deterministic latency setting */
            .newSysrefOnRelink = 0,    /* newSysrefOnRelink */
            .syncbInSelect = 1,        /* SyncbInSelect; */
            .overSample = 1,            /* 1=overSample, 0=bitRepeat */
            .syncbInLvdsMode = 1,
            .syncbInLvdsPnInvert = 0,
			.enableManualLaneXbar = 0 /* 0=auto, 1=manual */
        },
        /* Deframer A settings */
        .deframerA =
        {
            .bankId = 0,    /* bankId extension to Device ID (Valid 0..15) */
            .deviceId = 0,  /* deviceId  link identification number. (Valid 0..255) */
            .lane0Id = 0,   /* lane0Id Lane0 ID. (Valid 0..31) */
            .M = 4,         /* M  number of DACss (0, 2, or 4) - 2 DACs per transmit chain */
            .K = 32,        /* K  #frames in a multiframe (default=32), F*K=multiple of 4. (F=2*M/numberOfLanes) */
            //.S = 1,         /* S */
            .scramble = 1,  /* scramble  scrambling off if scramble= 0 */
            .externalSysref = 1,    /* externalSysref  0= use internal SYSREF, 1= external SYSREF */
            .deserializerLanesEnabled = 0x0F,   /* deserializerLanesEnabled  bit per lane, [0] = Lane0 enabled */
            .deserializerLaneCrossbar = 0xE4,   /* deserializerLaneCrossbar */
            .lmfcOffset = 0,                    /* lmfcOffset LMFC offset value to adjust deterministic latency */
            .newSysrefOnRelink = 0,             /* newSysrefOnRelink */
            .syncbOutSelect = 0,                /* SYNCBOUT0/1 select */
            .Np = 16,          /* Np (converter sample resolution) */
            .syncbOutLvdsMode = 1,
            .syncbOutLvdsPnInvert = 0,
            .syncbOutCmosSlewRate = 0,
            .syncbOutCmosDriveLevel = 0,
			.enableManualLaneXbar = 0 /* 0=auto, 1=manual */
       },
        /* Deframer B settings */
        .deframerB =
        {
            .bankId = 0,    /* bankId extension to Device ID (Valid 0..15) */
            .deviceId = 0,    /* deviceId  link identification number. (Valid 0..255) */
            .lane0Id  = 0,    /* lane0Id Lane0 ID. (Valid 0..31) */
            .M  = 0,    /* M  number of DACss (0, 2, or 4) - 2 DACs per transmit chain */
            .K  = 32,   /* K  #frames in a multiframe (default=32), F*K=multiple of 4. (F=2*M/numberOfLanes) */
            //.S = 1,    /* S */
            .scramble = 1,    /* scramble  scrambling off if scramble= 0 */
            .externalSysref = 1,    /* externalSysref  0= use internal SYSREF, 1= external SYSREF */
            .deserializerLanesEnabled = 0x00, /* deserializerLanesEnabled  bit per lane, [0] = Lane0 enabled */
            .deserializerLaneCrossbar = 0xE4, /* deserializerLaneCrossbar */
            .lmfcOffset = 0,    /* lmfcOffset    LMFC offset value to adjust deterministic latency */
            .newSysrefOnRelink = 0,    /* newSysrefOnRelink */
            .syncbOutSelect           = 1,     /* syncbOutSelect;  */
            .Np = 16,          /* Np (converter sample resolution) */
            .syncbOutLvdsMode = 1,
            .syncbOutLvdsPnInvert = 0,
            .syncbOutCmosSlewRate = 0,
            .syncbOutCmosDriveLevel = 0,
			.enableManualLaneXbar = 0 /* 0=auto, 1=manual */
        },
        .serAmplitude          = 15, /* Serializer amplitude setting. Default = 15. Range is 0..15 */
        .serPreEmphasis        = 1,  /* Serializer pre-emphasis setting. Default = 1 Range is 0..4 */
        .serInvertLanePolarity = 0,  /* Serializer Lane PN inversion select. Default = 0. Where, bit[0] = 0 will invert lane [0], bit[1] = 0 will invert lane 1, etc. */
        .desInvertLanePolarity = 0,  /* Deserializer Lane PN inversion select.  bit[0] = 1 Invert PN of Lane 0, bit[1] = Invert PN of Lane 1, etc */
        .desEqSetting          = 1,   /* Deserializer Equalizer setting. Applied to all deserializer lanes. Range is 0..4 */
        .sysrefLvdsMode        = 1,  /* Use LVDS inputs on Talise for SYSREF */
        .sysrefLvdsPnInvert    = 0   /*0= Do not PN invert SYSREF */
    }
};
