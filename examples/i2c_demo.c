#include "drivers.h"

I2C_HandleTypeDef hi2c1;

uint8_t txData[] = {0x10,0x20,0x30,0x40};

uint8_t rxData[4];
void I2C_Complete(void)
{

}

int main(void)
{
    hi2c1.Instance = I2C1;

    hi2c1.Init.mode = I2C_MODE_MASTER;

    hi2c1.Init.addressMode = I2C_ADDRESS_7BIT;

    hi2c1.Init.clockSpeed = I2C_SPEED_STANDARD;

    hi2c1.Init.dutyCycle = I2C_DUTY_2;

    hi2c1.Init.ownAddress = 0x52;

    I2C_Init(&hi2c1);
    I2C_RegisterCallback(
&hi2c1,
I2C_Complete);

I2C_EnableInterrupt(
&hi2c1);

    I2C_MasterTransmit(&hi2c1,
                       0x68,
                       txData,
                       sizeof(txData));

    I2C_MasterReceive(&hi2c1,
                      0x68,
                      rxData,
                      sizeof(rxData));

    while(1)
    {

    }
}