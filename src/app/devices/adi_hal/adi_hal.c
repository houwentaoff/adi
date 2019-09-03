/**
 * \file adi_hal.c
 * \brief Contains ADI Hardware Abstraction functions
 *        End user must provide implementation based on their
 *        application and hardware platform.
 *
 *        This file provides the ADI Evaluation platform implementation
 *        that will not be compiled unless the compiler option ADI_ZYNQ_PLATFORM
 *        is defined at compile time.
 *
 * Copyright 2015-2017 Analog Devices Inc.
 * Released under the AD9378-AD9379 API license, for more information see the "LICENSE.txt" file in this zip file.
 */

#include "adi_hal.h"
#ifdef ADI_ZYNQ_PLATFORM
#include "zynq_platform.h"
#endif

#include <linux/spi/spidev.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

adiHalErr_t ADIHAL_setTimeout(void *devHalInfo, uint32_t halTimeout_ms)
{
    adiHalErr_t retVal = ADIHAL_OK;
#ifdef ADI_ZYNQ_PLATFORM
    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;
    devHalData->waitTimeout = halTimeout_ms;
    HAL_writeToLogFile("ADIHAL_setTimeout at index %d to ", devHalData->devIndex, halTimeout_ms);
#endif
    return retVal;
}

adiHalErr_t ADIHAL_openHw(void *devHalInfo, uint32_t halTimeout_ms)
{
    adiHalErr_t retVal = ADIHAL_OK;
    char        dev_node[16];
    int fd = 0;
    int ret = 0;
    int mode = 0, bits = 16, speed = 100 * 1000 * 1000;

    sprintf(dev_node, "/dev/spidev%d.0", 1);
    fd = open(dev_node, O_RDWR);
    if (fd < 0) {
        perror("open");
        return retVal=ADIHAL_SPI_FAIL;
    }
    *(int *)devHalInfo = fd;
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret < 0) {
        perror("SPI_IOC_WR_MODE");
        return retVal=ADIHAL_SPI_FAIL;
    }
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret < 0) {
        perror("SPI_IOC_WR_BITS_PER_WORD");
        return retVal=ADIHAL_SPI_FAIL;
    }
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret < 0) {
        perror("SPI_IOC_WR_MAX_SPEED_HZ");
        return retVal=ADIHAL_SPI_FAIL;
    }
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }
    devHalData->waitTimeout = halTimeout_ms;

    switch(devHalData->devIndex)
    {
        /* SPI Devices*/
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            error = HAL_initSpiDriver(devHalData->spiSettings);
            break;
        default:
            HAL_writeToLogFile("ERROR: ADIHAL_openHw: Unknown device index %d", devHalData->devIndex);
            return (ADIHAL_GEN_SW);
    }

    HAL_writeToLogFile("ADIHAL_openHw: device index %d", devHalData->devIndex);

    if (error != 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: ADIHAL_openHw: HAL_initSpiDriver() failed with error %d for device %d", error, devHalData->devIndex);
        retVal = ADIHAL_SPI_FAIL;
    }

#endif

    return (retVal);
}

adiHalErr_t ADIHAL_closeHw(void *devHalInfo)
{
    adiHalErr_t retVal = ADIHAL_OK;
    int fd = 0;
    fd = *(int *)devHalInfo;
    close(fd);
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    switch(devHalData->devIndex)
    {
        /* SPI Devices */
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            error = HAL_closeSpiDriver(devHalData->spiSettings);
            break;
        default:
            HAL_writeToLogFile("ERROR: ADIHAL_closeHw(): Unknown device index %d", devHalData->devIndex);
            return (ADIHAL_GEN_SW);
    }

    HAL_writeToLogFile("ADIHAL_closeHw: device index %d", devHalData->devIndex);

    if (error != 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: ADIHAL_closeHw: HAL_closeSpiDriver() failed with error %d for device %d", error, devHalData->devIndex);
        return (ADIHAL_SPI_FAIL);
    }

#endif

    return(retVal);
}

adiHalErr_t ADIHAL_resetHw(void *devHalInfo)
{
    adiHalErr_t retVal = ADIHAL_OK;
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    HAL_writeToLogFile("ADIHAL_resetHw at index %d", devHalData->devIndex);
    error = HAL_resetHw(devHalData->devIndex);
    if (error != 0)
    {
       devHalData->gpioErrCode = error;
       HAL_writeToLogFile("ERROR: ADIHAL_resetHw: HAL_resetHw() failed with error %d for device %d", error, devHalData->devIndex);
       return (ADIHAL_GPIO_FAIL);
    }
#endif

    return (retVal);
}

adiHalErr_t  ADIHAL_spiWriteByte(void *devHalInfo, uint16_t addr, uint8_t data)
{
    adiHalErr_t retVal = ADIHAL_OK;
    int fd = *(int *)devHalInfo;
    struct spi_ioc_transfer tr;
    unsigned char txbuf[2];
    int ret = 0;
    
    memset(&tr, 0, sizeof(tr));

    txbuf[0] = (addr >> 8) & 0x7F;                                 
    txbuf[1] = addr & 0xFF;
    txbuf[2] = data;
    tr.len = 3;
    tr.tx_buf = (long long)txbuf;
//    tr.bits_per_word = bits;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        perror("can't send spi message");
        retVal = ADIHAL_SPI_FAIL;
    }
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIWriteByte(devHalData->spiSettings, addr, data);

    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: ADIHAL_spiWriteByte(): HAL_SPIWriteByte() for device %d, chipSelect %d, returned error %d\n",
                devHalData->devIndex, devHalData->spiSettings->chipSelectIndex, error);
        return (ADIHAL_SPI_FAIL);
    }

    if (devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        HAL_writeToLogFile("SPIWrite: CS:%2d, ADDR:0x%03X, DATA:0x%02X \n", devHalData->spiSettings->chipSelectIndex, addr, data);
    }

#endif

    return (retVal);
}

adiHalErr_t  ADIHAL_spiWriteBytes(void *devHalInfo, uint16_t *addr, uint8_t *data, uint32_t count)
{
    adiHalErr_t retVal = ADIHAL_OK;
    int i;             

    for (i = 0; i < count; i++) {
        retVal = ADIHAL_spiWriteByte(devHalInfo, addr[i], data[i]);
        if (retVal)    
            return retVal;
    }           
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;
    uint32_t i = 0;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIWriteBytes(devHalData->spiSettings, addr, data, count);

    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: ADIHAL_spiWriteBytes(): HAL_SPIWriteBytes() for device %d, chipSelect %d, returned error %d\n",
                devHalData->devIndex, devHalData->spiSettings->chipSelectIndex, error);
        return (ADIHAL_SPI_FAIL);
    }

    if (devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        for(i = 0; i < count; i++)
        {
            HAL_writeToLogFile("SPIWrite: CS:%2d, ADDR:0x%03X, DATA:0x%02X \n", devHalData->spiSettings->chipSelectIndex, addr[i], data[i]);
        }

    }

#endif

    return (retVal);
}

adiHalErr_t ADIHAL_spiReadByte(void *devHalInfo, uint16_t addr, uint8_t *readdata)
{
    adiHalErr_t retVal = ADIHAL_OK;
    int fd = *(int *)devHalInfo;
    struct spi_ioc_transfer tr[2];
    unsigned char txbuf[2];
    uint8_t data; 
    int ret = 0;
    
    memset(tr, 0, sizeof(tr));

    txbuf[0] = (0x1<<7) | ((addr >> 8) & 0x7F);                                 
    txbuf[1] = addr & 0xFF;
    tr[0].len = 2;
    tr[0].tx_buf = (long long)txbuf;
    tr[1].len = 1;
    tr[1].rx_buf = (long long)&data;

    ret = ioctl(fd, SPI_IOC_MESSAGE(2), &tr);
    if (ret < 1)
    {
        perror("can't send spi message");
        retVal = ADIHAL_SPI_FAIL;
    }
    else
    {
        readdata = data;
    }
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIReadByte(devHalData->spiSettings, addr, readdata);
    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: HAL_SPIReadByte() returned error %d, reading from device %d, ChipSelect:%d\n",
                error, devHalData->devIndex, devHalData->spiSettings->chipSelectIndex);
        return (ADIHAL_SPI_FAIL);
    }

    if (devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        HAL_writeToLogFile("SPIRead: CS:%2d, ADDR:0x%03X, ReadData:0x%02X\n",
                devHalData->devIndex, addr, *readdata);
    }

#endif
    return (retVal);
}

adiHalErr_t ADIHAL_spiReadBytes(void *devHalInfo, uint16_t *addr, uint8_t *readdata, uint32_t count)
{
    adiHalErr_t retVal = ADIHAL_OK;
    int i;              
    
   for (i = 0; i < count; i++) {
       retVal = ADIHAL_spiReadByte(devHalInfo, addr[i], &readdata[i]);
       if (retVal)     
           return retVal;
  }              
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;
    uint32_t i = 0;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIReadBytes(devHalData->spiSettings, addr, readdata, count);
    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: ADIHAL_spiWriteBytes(): HAL_SPIReadBytes() for device %d, chipSelect %d, returned error %d\n",
                devHalData->devIndex, devHalData->spiSettings->chipSelectIndex, error);
        return (ADIHAL_SPI_FAIL);
    }

    if (devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        for(i = 0; i < count; i++)
        {
            HAL_writeToLogFile("SPIRead: CS:%2d, ADDR:0x%03X, ReadData:0x%02X\n",
                               devHalData->devIndex, addr[i], readdata[i]);
        }

    }

#endif
    return (retVal);
}

adiHalErr_t ADIHAL_spiWriteField(void *devHalInfo,
        uint16_t addr, uint8_t fieldVal, uint8_t mask, uint8_t startBit)
{
    adiHalErr_t retVal = ADIHAL_OK;
    uint8_t readVal;

    retVal = ADIHAL_spiReadByte(devHalInfo, addr, &readVal);                      
    if (retVal < 0)
        return retVal;
    readVal = (readVal & ~mask) | ((fieldVal << startBit) & mask);
                 
//    if (devHalData->logLevel & ADIHAL_LOG_SPI)
//    {          
//        printf("SPIWriteField: ADDR:0x%03X, FIELDVAL:0x%02X  , MASK:0x%02X, STARTBIT:%d\n",
//                        addr, fieldVal, mask, startBit);
//    }          
                       
    return ADIHAL_spiWriteByte(devHalInfo, addr, readVal);

#ifdef ADI_ZYNQ_PLATFORM
    uint8_t readVal = 0;
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIReadByte(devHalData->spiSettings, addr, &readVal);
    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: HAL_SPIReadByte() returned error %d in ADIHAL_spiWriteField()\n", error);
        return (ADIHAL_SPI_FAIL);
    }

    readVal = (readVal & ~mask) | ((fieldVal << startBit) & mask);

    error = HAL_SPIWriteByte(devHalData->spiSettings, addr, readVal);
    if (error < 0 )
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: HAL_SPIWriteByte() returned error %d in ADIHAL_spiWriteField()\n", error);
        return (ADIHAL_SPI_FAIL);
    }

    if (devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        HAL_writeToLogFile("SPIWriteField: CS:%2d, ADDR:0x%03X, FIELDVAL:0x%02X, MASK:0x%02X, STARTBIT:%d\n",
                devHalData->devIndex,addr, fieldVal, mask, startBit);
    }
#endif

//    return (retVal);
}

adiHalErr_t ADIHAL_spiReadField(void *devHalInfo, uint16_t addr, uint8_t *fieldVal, uint8_t mask, uint8_t startBit)
{
    adiHalErr_t retVal = ADIHAL_OK;
    uint8_t readVal;
                    
    retVal = ADIHAL_spiReadByte(devHalInfo, addr, &readVal);
    if (retVal < 0)
        return retVal;
                                
    *fieldVal = ((readVal & mask) >> startBit);
                                      
//    if(devHalData->logLevel & ADIHAL_LOG_SPI)
//    {          
//        printf("SPIReadField: ADDR:0x%03X, MASK:0x%02X, STA  RTBIT:%d, FieldVal:0x%02X\n",
//                addr, mask, startBit, *fieldVal);
//    }          

#ifdef ADI_ZYNQ_PLATFORM

    uint8_t readVal = 0;
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_SPIReadByte(devHalData->spiSettings, addr, &readVal);
    if (error < 0)
    {
        devHalData->spiErrCode = error;
        HAL_writeToLogFile("ERROR: HAL_SPIReadByte() returned error %d in ADIHAL_spiReadField()\n", error);
        return (ADIHAL_SPI_FAIL);
    }

    *fieldVal = ((readVal & mask) >> startBit);

    if(devHalData->logLevel & ADIHAL_LOG_SPI)
    {
        HAL_writeToLogFile("SPIReadField: CS:%2d, ADDR:0x%03X, MASK:0x%02X, STARTBIT:%d, FieldVal:0x%02X\n",
                devHalData->devIndex,addr, mask, startBit, *fieldVal);
    }

#endif
    return (retVal);
}

adiHalErr_t  ADIHAL_wait_us(void *devHalInfo, uint32_t time_us)
{
    adiHalErr_t retVal = ADIHAL_OK;
    usleep(time_us);
#ifdef ADI_ZYNQ_PLATFORM
    int32_t error = 0;
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    error = HAL_wait_us(time_us);
    if (error != 0)
    {
        devHalData->timerErrCode = error;
        HAL_writeToLogFile("ERROR: HAL_wait_us() returned error %d in ADIHAL_wait_us()\n", error);
        return (ADIHAL_TIMER_FAIL);
    }

#endif
    return (retVal);
}

adiHalErr_t ADIHAL_writeToLog(void *devHalInfo, adiLogLevel_t logLevel, uint32_t errorCode, const char *comment)
{
    adiHalErr_t retVal = ADIHAL_OK;
#ifdef ADI_ZYNQ_PLATFORM
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;
    int32_t error = 0;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    if((devHalData->logLevel & ADIHAL_LOG_ERR) && (logLevel == ADIHAL_LOG_ERR))
    {
        error = HAL_writeToLogFile("ERROR: %d: %s", (int)errorCode, comment);
    }
    else if((devHalData->logLevel & ADIHAL_LOG_WARN) && (logLevel == ADIHAL_LOG_WARN))
    {
        error = HAL_writeToLogFile("WARNING: %d: %s", (int)errorCode, comment);
    }
    else if((devHalData->logLevel & ADIHAL_LOG_MSG) && (logLevel == ADIHAL_LOG_MSG))
    {
        error = HAL_writeToLogFile("MESSAGE: %d: %s",(int)errorCode, comment);
    }
    else
    {
        error = HAL_writeToLogFile("Undefined Log Level: 0x%X: %d: %s", logLevel, (int)errorCode, comment);
    }

    if(error != 0)
    {
        retVal = ADIHAL_WARNING;
    }
#endif

    return (retVal);
}

adiHalErr_t ADIHAL_setLogLevel(void *devHalInfo, uint16_t logLevel)
{
    adiHalErr_t retVal = ADIHAL_OK;
#ifdef ADI_ZYNQ_PLATFORM
    zynqAdiDev_t *devHalData = (zynqAdiDev_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        return (ADIHAL_GEN_SW);
    }

    devHalData->logLevel = logLevel;
#endif
    return retVal;
}
