#include "TCS3200.h"
#include "gpio.h"
#include <stdio.h>

TIM_HandleTypeDef *TCS3200_htim = &htim2;

GPIO_TypeDef* S0_Port = TCS3200_S0_GPIO_Port;
uint16_t S0_Pin = TCS3200_S0_Pin;

GPIO_TypeDef* S1_Port = TCS3200_S1_GPIO_Port;
uint16_t S1_Pin = TCS3200_S1_Pin;

GPIO_TypeDef* S2_Port = TCS3200_S2_GPIO_Port;
uint16_t S2_Pin = TCS3200_S2_Pin;

GPIO_TypeDef* S3_Port = TCS3200_S3_GPIO_Port;
uint16_t S3_Pin = TCS3200_S3_Pin;

TCS3200_Color currentColor;
uint64_t startTick;
TCS3200_ColorFreq colorFreq;

void TCS3200_Init()
{
    TCS3200_SetFreqScale(TCS3200_FREQ_100);
    TCS3200_SetColor(TCS3200_RED);

    TCS3200_htim->Instance->CNT = 0;
    startTick = GetMicros();
}

void TCS3200_SetFreqScale(TCS3200_FreqScale scale)
{
    switch (scale)
    {
    case TCS3200_POWER_DOWN:
        HAL_GPIO_WritePin(S0_Port, S0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(S1_Port, S1_Pin, GPIO_PIN_RESET);
        break;
    case TCS3200_FREQ_2:
        HAL_GPIO_WritePin(S0_Port, S0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(S1_Port, S1_Pin, GPIO_PIN_SET);
        break;
    case TCS3200_FREQ_20:
        HAL_GPIO_WritePin(S0_Port, S0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(S1_Port, S1_Pin, GPIO_PIN_RESET);
        break;
    case TCS3200_FREQ_100:
        HAL_GPIO_WritePin(S0_Port, S0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(S1_Port, S1_Pin, GPIO_PIN_SET);
        break;
    }
}

void TCS3200_SetColor(TCS3200_Color color)
{
    switch (color)
    {
    case TCS3200_RED:
        HAL_GPIO_WritePin(S2_Port, S2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(S3_Port, S3_Pin, GPIO_PIN_RESET);
        break;
    case TCS3200_BLUE:
        HAL_GPIO_WritePin(S2_Port, S2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(S3_Port, S3_Pin, GPIO_PIN_SET);
        break;
    case TCS3200_CLEAR:
        HAL_GPIO_WritePin(S2_Port, S2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(S3_Port, S3_Pin, GPIO_PIN_RESET);
        break;
    case TCS3200_GREEN:
        HAL_GPIO_WritePin(S2_Port, S2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(S3_Port, S3_Pin, GPIO_PIN_SET);
        break;
    }
}

// It takes 230 ms to get the Timer Callback for ARR = 1000
void TCS3200_TimerCallback()
{
    uint64_t endTick = GetMicros();
    uint32_t freq  = (double)TCS3200_htim->Instance->ARR * 1000 / (endTick - startTick);

    switch (currentColor)
    {
    case TCS3200_RED:
        colorFreq.Red = freq;
        currentColor = TCS3200_BLUE;
        TCS3200_SetColor(TCS3200_BLUE);
        break;

    case TCS3200_BLUE:
        colorFreq.Blue = freq;
        currentColor = TCS3200_RED;
        TCS3200_SetColor(TCS3200_RED);
        break;

    case TCS3200_CLEAR:
        break;

    case TCS3200_GREEN:
        break;
    }

    startTick = GetMicros();
}