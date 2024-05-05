#ifndef TCS3200_H
#define TCS3200_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "tim.h"

    typedef enum
    {
        TCS3200_RED,
        TCS3200_BLUE,
        TCS3200_CLEAR,
        TCS3200_GREEN
    } TCS3200_Color;

    typedef enum
    {
        TCS3200_POWER_DOWN,
        TCS3200_FREQ_2,
        TCS3200_FREQ_20,
        TCS3200_FREQ_100
    } TCS3200_FreqScale;

    typedef struct
    {
        uint32_t Red;
        uint32_t Blue;
        uint32_t Clear;
        uint32_t Green;
    } TCS3200_ColorFreq;

    extern TIM_HandleTypeDef *TCS3200_htim;
    extern TCS3200_ColorFreq colorFreq;

    void TCS3200_Init();

    void TCS3200_SetFreqScale(TCS3200_FreqScale scale);

    void TCS3200_SetColor(TCS3200_Color color);

    void TCS3200_TimerCallback();

#ifdef __cplusplus
}
#endif

#endif // TCS3200_H
