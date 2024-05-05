#include "stm32f1xx_hal.h"
#include "usbd_cdc_if.h"

int _write(int file, char *data, int len)
{
    CDC_Transmit_FS((uint8_t *)data, (uint16_t)len);
    return len;
}