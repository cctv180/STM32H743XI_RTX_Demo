/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ����RTX5����ģ�塣
*              ʵ��Ŀ�ģ�
*                1. ����RTX5����ģ�塣
*              ʵ�����ݣ�
*                1. K1�������£����ڴ�ӡ��
*                2. ��������ʵ�ֵĹ������£�
*                   AppTaskUserIF����   : ������Ϣ������
*                   AppTaskLED����      : LED��˸��
*                   AppTaskMsgPro����   : ��Ϣ������
*                   AppTaskStart����    : ��������Ҳ��������ȼ���������ʵ�ְ���ɨ�衣
*                   osRtxTimerThread����: ��ʱ��������δʹ�á�
*              ע�����
*                1. Ϊ��RTX5�ĵ����������ʹ�ã�RTX5�Ĺ���·���в�Ҫ�����ģ�·����Ҫ̫����
*                   �����˳�����״̬Ҫ������жϵ㡣
*                2. ��ʵ���Ƽ�ʹ�ô�������SecureCRT�鿴��ӡ��Ϣ��������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                3. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����        ˵��
*		V1.0    2019-04-10   Eric2013     1. CMSIS�����汾 V5.5.1
*                                         2. HAL��汾 V1.3.0
*                                         3. RTX5�汾5.5.0
*                                         4. Event Recorder�汾1.4.0
*
*	Copyright (C), 2019-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/	
#include "includes.h"

/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static void AppTaskCreate (void);
void AppTaskUserIF(void *argument);
void AppTaskLED(void *argument);
void AppTaskMsgPro(void *argument);
void AppTaskStart(void *argument);

/*
**********************************************************************************************************
											 ����
**********************************************************************************************************
*/
/* ������������� */
const osThreadAttr_t ThreadStart_Attr = 
{
	/* δʹ�� */
//	.cb_mem = &worker_thread_tcb_1,
//	.cb_size = sizeof(worker_thread_tcb_1),
//	.stack_mem = &worker_thread_stk_1[0],
//	.stack_size = sizeof(worker_thread_stk_1),
//	.priority = osPriorityAboveNormal,
//	.tz_module = 0
	
	.name = "osRtxStartThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh4,
	.stack_size = 2048,
};

const osThreadAttr_t ThreadMsgPro_Attr = 
{
	.name = "osRtxMsgProThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh3,
	.stack_size = 1024,
};

const osThreadAttr_t ThreadLED_Attr = 
{
	.name = "osRtxLEDThread",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh2,
	.stack_size = 512,
};

const osThreadAttr_t ThreadUserIF_Attr = 
{
	.name = "osRtxThreadUserIF",
	.attr_bits = osThreadDetached, 
	.priority = osPriorityHigh1,
	.stack_size = 1024,
};

/* ������ */
osThreadId_t ThreadIdTaskUserIF = NULL;
osThreadId_t ThreadIdTaskMsgPro = NULL;
osThreadId_t ThreadIdTaskLED = NULL;
osThreadId_t ThreadIdStart = NULL;

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main (void) 
{	
	/* HAL�⣬MPU��Cache��ʱ�ӵ�ϵͳ��ʼ�� */
	System_Init();

	/* �ں˿���ǰ�ر�HAL��ʱ���׼ */
	HAL_SuspendTick();
	
	/* �ں˳�ʼ�� */
	osKernelInitialize();                                  

	/* ������������ */
	ThreadIdStart = osThreadNew(AppTaskStart, NULL, &ThreadStart_Attr);  

	/* ���������� */
	osKernelStart();
	
	while(1);
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: ������Ϣ����		
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh1 (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
void AppTaskUserIF(void *argument)
{
	uint8_t ucKeyCode;

    while(1)
    {
//		ucKeyCode = bsp_GetKey();
//		
//		if (ucKeyCode != KEY_NONE)
//		{
//			switch (ucKeyCode)
//			{
//				/* K1�����£���ӡ����˵�� */
//				case KEY_DOWN_K1:
//					printf("KEY_DOWN_K1\r\n");
//					break;	

//				/* �����ļ�ֵ������ */
//				default:                     
//					break;
//			}
//		}
		
		osDelay(20);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskLED
*	����˵��: LED��˸��
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh2 
*********************************************************************************************************
*/
void AppTaskLED(void *argument)
{
	const uint16_t usFrequency = 200; /* �ӳ����� */
	uint32_t tick;

	/* ��ȡ��ǰʱ�� */
	tick = osKernelGetTickCount(); 
	
    while(1)
    {
//		bsp_LedToggle(2);
		/* ����ӳ� */
		tick += usFrequency;                          
		osDelayUntil(tick);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMsgPro
*	����˵��: ��Ϣ����������������֤Event Recoder��
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh3  
*********************************************************************************************************
*/
void AppTaskMsgPro(void *argument)
{
	while(1)
	{
		/* ��֤Event Recoder��ʱ��ͳ�ƹ��� */
//		EventStartA(1);
		osDelay(10);
//		EventStopA(1);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ����������������BSP������������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: osPriorityHigh4  
*********************************************************************************************************
*/
void AppTaskStart(void *argument)
{
	const uint16_t usFrequency = 1; /* �ӳ����� */
	uint32_t tick;
	
	/* ��ʼ������ */
	HAL_ResumeTick();
	bsp_Init();

	/* �������� */
	AppTaskCreate();

	/* ��ȡ��ǰʱ�� */
	tick = osKernelGetTickCount(); 
	
    while(1)
    {
		/* ��Ҫ�����Դ����ĳ��򣬶�Ӧ������̵��õ� */
		bsp_ProPer1ms();
		
		/* ����ӳ� */
		tick += usFrequency;                          
		osDelayUntil(tick);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
	ThreadIdTaskMsgPro = osThreadNew(AppTaskMsgPro, NULL, &ThreadMsgPro_Attr);  
	ThreadIdTaskLED = osThreadNew(AppTaskLED, NULL, &ThreadLED_Attr);  
	ThreadIdTaskUserIF = osThreadNew(AppTaskUserIF, NULL, &ThreadUserIF_Attr);  
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/