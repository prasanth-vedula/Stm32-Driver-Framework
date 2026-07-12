#include "drivers.h"

/*==========================================================
                Global Objects
==========================================================*/
static SPI_HandleTypeDef hspi1;

static uint8_t txData[] = "STM32 SPI Demo";

static uint8_t rxData[20];

/*==========================================================
                Transfer Complete Callback
==========================================================*/
static void SPI_CompleteCallback(void)
{
    /* User Application Code */
}

/*==========================================================
                Main Application
==========================================================*/
int main(void)
{
    /* Enable SPI Clock */
    RCC_EnableSPIClock(SPI1);

    /* Configure SPI */
    hspi1.Instance = SPI1;

    hspi1.Init.mode = SPI_MODE_MASTER;
    hspi1.Init.cpol = SPI_CPOL_LOW;
    hspi1.Init.cpha = SPI_CPHA_FIRST_EDGE;
    hspi1.Init.dataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.firstBit = SPI_MSB_FIRST;

    /* Initialize Driver */
    SPI_Init(&hspi1);

    /* Register Callback */
    SPI_RegisterCallback(&hspi1,
                         SPI_CompleteCallback);

    /* Enable Interrupt */
    SPI_EnableInterrupt(&hspi1);

    /* Transmit Data */
    SPI_Transmit(&hspi1,
                 txData,
                 sizeof(txData));

    /* Error Check */
    if (SPI_GetError(&hspi1) != SPI_ERROR_NONE)
    {
        SPI_ClearError(&hspi1);
    }

    /* Receive Data */
    SPI_Receive(&hspi1,
                rxData,
                sizeof(rxData));

    /* Full Duplex Transfer */
    SPI_TransmitReceive(&hspi1,
                        txData,
                        rxData,
                        sizeof(txData));

    while (1)
    {
        /* Application Loop */
    }
}