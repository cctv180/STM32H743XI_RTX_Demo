/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For STM32H7)
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��
*		V1.0    2018-07-29  Eric2013   ��ʽ����
*
*	Copyright (C), 2018-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_H_
#define _BSP_H_

/* ����ͷ�ļ� */
#include "stm32h7xx_hal.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ���� BSP �汾�� */
#define STM32_BOARD         "STM32V7"
#define STM32_BSP_VERSION   "V1.02"

/* ������ӡ���� */
#define BSP_INFO_EN

/* �������Դ�ӡ */
//#define DEBUG_MODE

/* ��������ģʽ  1���� 0�ر�  */
#define Enable_EventRecorder    0

/* RTOS_RTX����  1���� 0�ر� */
#define  USE_RTX                1

/* CPU����ʱִ�еĺ��� */
#define CPU_IDLE()          bsp_Idle()
#define ERROR_HANDLER()		Error_Handler(__FILE__, __LINE__);
/* ����ȫ���жϵĺ� */
#define ENABLE_INT()    __set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()   __set_PRIMASK(1)	/* ��ֹȫ���ж� */

typedef enum
{
    BSP_ERR_NULL = 0,
    BSP_ERR_01,
    BSP_ERR_02,
    BSP_ERR_03,
    BSP_ERR_04,
    BSP_ERR_05,
}BSP_ERR_E;

/*
*********************************************************************************************************
 * ���º��Զ���������ʾ
*********************************************************************************************************
 */
#ifdef DEBUG_MODE
//#define BSP_Printf(...) bsp_log_debug(__FILE__, __LINE__, __VA_ARGS__)
    #define BSP_Printf(...)                         \
    do                                                  \
    {                                                   \
        printf("[D/SYS] (%s:%d) ", __FILE__, __LINE__); \
        printf(__VA_ARGS__);                            \
        printf("\r\n");                                 \
    } while (0)

#else
    #define BSP_Printf(...)
#endif /* DEBUG_MODE END */

#ifdef BSP_INFO_EN
    //#define BSP_INFO(...)  bsp_log_info(__VA_ARGS__)
    #define BSP_INFO(...)                               \
    do                                                  \
    {                                                   \
        printf("[I/SYS] ");                             \
        printf(__VA_ARGS__);                            \
        printf("\r\n");                                 \
    } while (0)

#else
    #define BSP_INFO(...)
#endif

#if USE_RTX == 1
#ifndef RTE_CMSIS_RTOS2
#define RTE_CMSIS_RTOS2
#endif
#endif

/* ����Ƿ����˿������ͺ� */
#if !defined (STM32_BOARD)
    #error "Please define the board model : STM32_BOARD"
#endif

/* ���������ڵ��Խ׶��Ŵ�printf */
#if Enable_EventRecorder == 1
    #include "EventRecorder.h"
#endif

/* printf �����Ƹ�ʽ��� �� */
#define BYTE_TO_BINARY_PATTERN "0b%c%c%c%c%c%c%c%c"
/* printf �����Ƹ�ʽ��� �� */
#define BYTE_TO_BINARY(byte) \
        (byte & 0x80 ? '1' : '0'), \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

#ifndef TRUE
    #define TRUE  1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef NULL
    #define NULL 0
#endif

/* ͨ��ȡ��ע�ͻ������ע�͵ķ�ʽ�����Ƿ�����ײ�����ģ�� */
//#include "bsp_user_lib.h"
//#include "bsp_dwt.h"
//#include "bsp_beep.h"
//#include "bsp_key.h"
//#include "bsp_adc.h"
//#include "bsp_led.h"
//#include "bsp_tim_pwm.h"
//#include "bsp_timer.h"
//#include "bsp_encoder.h"
//#include "bsp_tm1638.h"
//#include "bsp_uart_fifo.h"
//#include "bsp_19264.h"
//#include "bsp_595.h"

//#include "bsp_i2c_gpio.h"
//#include "bsp_i2c_rx8025t.h"

//#include "bsp_spi_bus.h"

//#include "sfud.h"
//#include "sfud_cfg.h"
//#include "easyflash.h"
//#include "ef_cfg.h"
//#include "fal.h"
//#include "fal_cfg.h"

//#include "app.h"
//#include "TextString.h"
//#include "easyLIFO.h"

////#include "bsp_uart_esp8266.h"

//#include <re.h>

//#include "at_command_set.h"
//#include "at_sim7600result.h"

//#include "shell_port.h"
//#include "shell_cfg.h"

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);
void System_Init(void);
void Error_Handler(char *file, uint32_t line);

void bsp_RunPer1ms(void);
void bsp_RunPer10ms(void);

#endif
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
