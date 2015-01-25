/**
  ******************************************************************************
  * File Name          : main.c
  * Date               : 23/01/2015 17:35:46
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define RXBUFFERSIZE 1000
 #define	sIDLE		0
 #define	sHEAD		1
 #define	sDATA		2
 #define	sEND		3
 #define	sPRECRC		4
 #define	sPOSTCRC	5
 #define	sCHECK		6
 #define	sDONE		7
#define DLEBINR 16
#define ETXBINR 3
#define framedArrSIZE 49
uint16_t DLINA_AT;
char AT_COM[64];
int TxBusy = 0;
char mas[1];
char 	TxNMEA0183[500];			//����� �����������
uint8_t		TxCntNMEA0183;				//������� �����������
char 	RxNMEA0183[500];			//����� �����������
uint8_t		RxCntNMEA0183;				//������� �����������
bool	reciveNMEA0183;				//���� �������� ����� ������
bool	sendNMEA0183;				//���� ��������� ��������
bool	T7Flag;						//���� ������������ ������� �7
char 	TxRS485[500];			//����� �����������
uint8_t		TxCntRS485;				//������� �����������
char 	RxRS485[500];			//����� �����������
uint8_t		RxCntRS485;				//������� �����������
bool	reciveRS485;				//���� �������� ����� ������
bool	sendRS485;				//���� ��������� ��������
bool	reciveADRRS485;				//���� ���������� ��������� � �������� ������
uint8_t aRxBuffer[RXBUFFERSIZE];
char 	TxBINR[500];			//����� �����������
uint16_t		TxCntBINR;				//������� �����������
char 	RxBINR[2048];			//����� �����������
uint16_t		RxCntBINR;				//������� �����������
bool	reciveBINR;				//���� �������� ����� ������
bool	sendBINR;				//���� ��������� ��������
// bool idleCatched = false;
bool	flagnewbinr;
uint16_t 		RxWrIndex;
bool 	RxBinrBuffOverrun;
uint8_t		NumberSatateRxBINR;		//���� ��������� ��������
#ifdef yesCRC
	bool	CRCok;					//���� ���������� ����������� ����
	uint8_t		NumberbyteCRC;			//������� ���� ����������� �����
	uint8_t		MCRC[5];				//������ ���� ����������� �����
#endif
uint8_t		hE4BINRmas[530];		//������ � �������� ������� �4
bool	hE4BINRok;				//���� ��������� ������� �4
uint8_t		h41BINRmas[12];			//������ � �������� ������� 41
bool	h41BINRok;				//���� ��������� ������� 41
uint8_t		h52BINRmas[224];		//������ � �������� ������� 52
bool	h52BINRok;				//���� ��������� ������� 52
uint8_t		h88BINRmas[69];			//������ � �������� ������� 88
bool	h88BINRok;				//���� ��������� ������� 88
int dataArrIndex;
uint8_t modeCANmsg;
uint8_t arrCANdata[8];
unsigned short Serial;//
unsigned char MsgType,Devtype,Priority,MsgMode;
uint8_t framedArrCANTx[framedArrSIZE];
//TIM_Base_InitTypeDef TimHandle;
bool checkMessageFlag;
int idleCounter;
int rxCounter;
bool beginCANTransmitFlag;
int canMessCounter=0;
char inc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
#define CAN_SHDN_PORT GPIOA
#define CAN_SHDN_PIN GPIO_PIN_15

#define RESET_CSM_PORT GPIOB
#define RESET_CSM_PIN GPIO_PIN_7

#define __HAL_CAN_CANCEL_TRANSMIT(__HANDLE__, __TRANSMITMAILBOX__)\
(((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_0)? ((__HANDLE__)->Instance->TSR |= CAN_TSR_ABRQ0) :\
 ((__TRANSMITMAILBOX__) == CAN_TXMAILBOX_1)? ((__HANDLE__)->Instance->TSR |= CAN_TSR_ABRQ1) :\
 ((__HANDLE__)->Instance->TSR |= CAN_TSR_ABRQ2))
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	beginCANTransmitFlag = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN2_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  inc = 0;
  HAL_GPIO_WritePin(RESET_CSM_PORT,RESET_CSM_PIN,1);
  HAL_UART_Receive_IT(&huart4, mas, 1); //��������� � ������ ����� ������

  AT_COM[0] = 0x10;
  AT_COM[1] = 0x0e;
  AT_COM[2] = 0x10;
  AT_COM[3] = 0x03;
  HAL_UART_Transmit_IT(&huart4, AT_COM, 4);
  HAL_Delay(1000);

  AT_COM[0] = 0x10;
  AT_COM[1] = 0x13;
  AT_COM[2] = 0x01;
  AT_COM[3] = 0x10;
  AT_COM[4] = 0x03;
  HAL_UART_Transmit_IT(&huart4, AT_COM, 5);
  HAL_Delay(100);
  AT_COM[0] = 0x10;
  AT_COM[1] = 0x27;
  AT_COM[2] = 0x01;
  AT_COM[3] = 0x10;
  AT_COM[4] = 0x03;
  HAL_UART_Transmit_IT(&huart4, AT_COM, 5);
  HAL_Delay(100);
  AT_COM[0] = 0x10;
  AT_COM[1] = 0xd7;
  AT_COM[2] = 0x02;
  AT_COM[3] = 0x0a;
  AT_COM[4] = 0x10;
  AT_COM[5] = 0x03;
  HAL_UART_Transmit_IT(&huart4, AT_COM, 6);
  HAL_Delay(100);
  /* USER CODE END 2 */

  /* USER CODE BEGIN 3 */
  HAL_GPIO_WritePin(CAN_SHDN_PORT,CAN_SHDN_PIN,0);

  HAL_CAN_Receive_IT(&hcan2,CAN_FIFO0);
  /* Infinite loop */
  while (1)
  {
	  if(beginCANTransmitFlag)
	  {
		  beginCANTransmitFlag = false;
			IIMinstr = IIM_disablePolling_MDUS;
			char msgtype = 0x0;
			char devtype = SatteliteModule;
			short serial = 0x0011;
			char priority = 0, mesMode = 0;
			uint8_t *resArr1,*resArr2;
			int arr1len,arr2len;
			prepareEXTID(serial,msgtype,devtype,priority,modeCANmsg);
//			setTxDataMessage(SatteliteModule);
//			parseArray(h41BINRmas,12,0x41,resArr1,&arr1len);
//			parseArray(h88BINRmas,69,0x88,resArr2,&arr2len);
			volatile uint8_t resArr[49];
//			volatile int sor = sizeof(resArr);
			for(int i = 0; i < 12; i++)
			{
				 if(i < 8)
				 {
					 resArr[i] = h41BINRmas[i];
				 }
			}
			for(int i = 0; i < 69; i++)
			{
				 if(i < 40)
				 {
					 resArr[i+8] = h88BINRmas[i];
				 }
				 else if(i==68)
					 resArr[i] = h88BINRmas[i];
			}
			int arrIndex=0;
			for(int j = 0; j < 10; j++)
			{
				for(int a = 0; a < 8; a++)
				{
					mesMode = 1;
					if(a!=7)
						framedMessagesArr[j].Data[a] = resArr[arrIndex++];
					else
						framedMessagesArr[j].Data[a] = j;
				}
				if(j==0)
				{
					mesMode = 3;
					framedMessagesArr[j].Data[7] = 7;
				}
				if(j>=6)
				{
					mesMode = 2;
					framedMessagesArr[j].Data[7] = calcCSofArr(resArr,sizeof(resArr));
				}

			}
			for(int b = 0; b < 8; b++)
				framedMessagesArr[canMessCounter].Data[b] = inc;
			inc++;
			prepareSTDID(3,2,3,3);
			framedMessagesArr[canMessCounter].IDE = CAN_ID_STD;
			framedMessagesArr[canMessCounter].DLC = 8;
			volatile uint32_t tmpval = *(uint32_t*)&STDID;
			framedMessagesArr[canMessCounter].StdId = tmpval;

			hcan2.pTxMsg = &framedMessagesArr[canMessCounter++];
			volatile HAL_StatusTypeDef hererr= HAL_CAN_Transmit_IT(&hcan2);
			//HAL_Delay(0);
			volatile uint32_t her = hcan2.Instance->ESR;
			//__HAL_CAN_CANCEL_TRANSMIT(&hcan2, 0);
			//__HAL_CAN_CANCEL_TRANSMIT(&hcan2, 1);
			//__HAL_CAN_CANCEL_TRANSMIT(&hcan2, 2);
			volatile uint32_t i=5*her;
			HAL_Delay(10);
//			HAL_CAN_Transmit_IT(&hcan2);
//			beginCANTransmitFlag = false;
//			tmpstatus = HAL_CAN_Transmit(&CanHandle,10);
			//HAL_Delay(2);
//			someCounter++;
//			while(makeFramedCANMessage(&dataArrIndex,arrCANdata,&modeCANmsg)!=0)
//			{
//
//			}
	  }
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);

  HAL_RCC_EnableCSS();

}

/* USER CODE BEGIN 4 */
void prepareSTDID(char mestype, char mtype, char ttype, char priority)
{
//	STDID->messageType = mestype&0x07;
//	STDID->moduleType = mtype&0x0f;
//	STDID->transmitionType = ttype&0x03;
//	STDID->priorityLevel = priority&0x03;
//	STDID->unused = 0;
	STDID.messageType = mestype;
	STDID.moduleType = mtype;
	STDID.transmitionType = ttype;
	STDID.priorityLevel = priority;
	STDID.unused = 0;
}

void setTxDataMessage(IIMmoduleType module)
{
	uint32_t *eid;
	eid  = (uint32_t *)EXTID;
	TxMessage.ExtId = *eid;
	TxMessage.StdId = 0;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.DLC = 8;
//	if(module == MDLU)
//	{
//		TxMessage.Data[0] = (char)MDLUTransmitData.LAx;
//		TxMessage.Data[1] = (char)(MDLUTransmitData.LAx>>8);
//		TxMessage.Data[2] = (char)MDLUTransmitData.LAy;
//		TxMessage.Data[3] = (char)(MDLUTransmitData.LAy>>8);
//		TxMessage.Data[4] = (char)MDLUTransmitData.LAz;
//		TxMessage.Data[5] = (char)(MDLUTransmitData.LAz>>8);
//		TxMessage.Data[6] = MDLUTransmitData.service;
//		TxMessage.Data[7] = MDLUTransmitData.CSL;				//constant for a some time
//	}
//	if(module == MDUS)
//	{
//		TxMessage.Data[0] = (char)MDUSTransmitData.ARx;
//		TxMessage.Data[1] = (char)(MDUSTransmitData.ARx>>8);
//		TxMessage.Data[2] = (char)MDUSTransmitData.ARy;
//		TxMessage.Data[3] = (char)(MDUSTransmitData.ARy>>8);
//		TxMessage.Data[4] = (char)MDUSTransmitData.ARz;
//		TxMessage.Data[5] = (char)(MDUSTransmitData.ARz>>8);
//		TxMessage.Data[6] = MDUSTransmitData.service;
//		TxMessage.Data[7] = MDUSTransmitData.CSL;				//constant for a some time
//	}
//	if(module == MDAD)
//	{
//		TxMessage.Data[0] = (char)MDADTransmitData.pressure;
//		TxMessage.Data[1] = (char)(MDADTransmitData.pressure>>8);
//		TxMessage.Data[2] = (char)(MDADTransmitData.pressure>>16);
//		TxMessage.Data[3] = (char)MDADTransmitData.temperature;
//		TxMessage.Data[4] = (char)(MDADTransmitData.temperature>>8);
//		TxMessage.Data[5] = (char)(MDADTransmitData.temperature>>16);
//		TxMessage.Data[6] = MDADTransmitData.service;
//		TxMessage.Data[7] = MDADTransmitData.CSL;				//constant for a some time
//	}
	if(module == SatteliteModule)
	{
//		TxMessage.Data = (unsigned char)arrCANdata;
//		CanHandle.pTxMsg->Data
//		CanHandle.pTxMsg->Data[0] = arrCANdata[0];
//		CanHandle.pTxMsg->Data[1] = arrCANdata[1];
//		CanHandle.pTxMsg->Data[2] = arrCANdata[2];
//		CanHandle.pTxMsg->Data[3] = arrCANdata[3];
//		CanHandle.pTxMsg->Data[4] = arrCANdata[4];
//		CanHandle.pTxMsg->Data[5] = arrCANdata[5];
//		CanHandle.pTxMsg->Data[6] = arrCANdata[6];
//		CanHandle.pTxMsg->Data[7] = arrCANdata[7];
		TxMessage.Data[0] = arrCANdata[0];
		TxMessage.Data[1] = arrCANdata[1];
		TxMessage.Data[2] = arrCANdata[2];
		TxMessage.Data[3] = arrCANdata[3];
		TxMessage.Data[4] = arrCANdata[4];
		TxMessage.Data[5] = arrCANdata[5];
		TxMessage.Data[6] = arrCANdata[6];
		TxMessage.Data[7] = arrCANdata[7];
	}

}

char parseArray(uint8_t *inArr, uint16_t inArrLength, int arrType, uint8_t *outArr, uint16_t *outArrLength)
{
	char err=0;
//	uint8_t tmpArr[300];
	uint8_t i;
//	 vector<uint8_t> tmpArr;
//	 vector<uint8_t>::iterator tmpIterator;
//
//	 for(i = 0; i < inArrLength; i++)
//	 {
	//	 tmpArr.insert(*inMas);
//		 tmpArr.at(i) = *inMas;
//		 tmpArr.push_back(*inMas);
//		 tmpArr[i]
//		 inArr++;
//	 }
	switch (arrType)
	{

		case 0x41:
		{
			for(i = 0; i < inArrLength; i++)
			{
				if(i<8)
				{
					*outArrLength++;		//incrementing value
					*outArr =  *inArr;
					inArr++;				//incrementing address
				}

			}
			break;
		}
		case 0x88:
		{
			for(i = 0; i < inArrLength; i++)
			{
				if(i<40)
				{
					*outArrLength++;		//incrementing value
					*outArr =  *inArr;
					inArr++;				//incrementing address
				}
				if(i==68)
				{
					*outArrLength++;		//incrementing value
					*outArr =  *inArr;
					inArr++;				//incrementing address
				}

			}
			break;
		}
		default:
		{
			err = 1;
			break;
		}


	}
	return err;
}

char makeFramedCANMessage(int *currentArrIndex, uint8_t *outArr, uint8_t *mode)
{
	char ret=1;
	uint8_t framesQuantity=0;
	uint8_t currentFrame = 0;
	uint8_t locmode = 0;
	uint8_t currentIndex = *currentArrIndex;
	currentFrame = currentIndex/7;
	framesQuantity = sizeof(framedArrCANTx)/7+1;
	if(currentIndex/7+1 <= framesQuantity)
	{
		for(int i=0; i < 8; i++)
		{
			if(i!=7)
			{
				*outArr = framedArrCANTx[currentIndex];
				currentIndex++;

			}
			else
			{
				if(currentIndex==7)
				{
					*outArr = framesQuantity;
					currentFrame++;
					locmode = 1;
											//also need to set correct mode flag in extID
				}
				else
				{
					currentFrame++;
					if(framesQuantity>currentFrame) //not first not last frame
						{
							*outArr = currentFrame;
							locmode = 2;
						}
					else
						{
							if(framesQuantity == currentFrame)
							{
								*outArr = calcCSofArr(framedArrCANTx,sizeof(framedArrCANTx));
								locmode = 3;
							}
							else
							{
								ret = 0;
								locmode = 0;
							}
						}



				}
			}
			outArr++;
		}
	}
	else
		{
			ret = 0;
			locmode = 0;
		}
	*mode = locmode;
	*currentArrIndex = currentIndex;
//	Serial=0x02;
	MsgMode = locmode;
	return ret;
}

uint8_t calcCSofArr(uint8_t *arr, uint8_t arrSize)
{
	uint16_t tmpVal=0;
//	tmpVal = *arr;
	for(uint8_t i = 0; i < arrSize; i++)
	{
		tmpVal+=*arr;
		arr++;
	}
	return tmpVal&0x00ff;
}

void prepareEXTID(short serial, char msgtype,char devtype,char priority, char msgMode)
{


	EXTID->deviceSerialNumber =serial&0x0fff;//
	EXTID->messageType = msgtype&0x0f;
	EXTID->deviceType = devtype&0x7f;
	EXTID->priorityLevel = priority&0x0f;
	EXTID->messageMode = msgMode&0x03;
	EXTID->unused = 0;

}
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{

	if(hcan->Instance==CAN2)
	{
		HAL_CAN_Receive_IT(&hcan2,CAN_FIFO0);
	}
}
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	__IO uint32_t ERR;
	if(hcan->Instance==CAN2)
	{
		  ERR=hcan->ErrorCode;
		  hcan->ErrorCode=0;
		  if ( ((hcan->Instance->TSR&CAN_TSR_TERR0) == CAN_TSR_TERR0) )
		  {
			__HAL_CAN_CANCEL_TRANSMIT(hcan,0);
		  }
//		  if ((hcan->Instance->TSR&CAN_TSR_TME1) == 0)
//		  {
//			  __HAL_CAN_CANCEL_TRANSMIT(hcan,1);
//		  }
//		  if ((hcan->Instance->TSR&CAN_TSR_TME2) == 0)
//		  {
//			  __HAL_CAN_CANCEL_TRANSMIT(hcan,2);
//		  }
	}
}

//void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
//{
//	if(hcan->Instance==CAN2)
//	{
//		volatile int status = HAL_CAN_GetState(hcan);
//		HAL_CAN_Receive_IT(hcan,CAN_FIFO0);
//	}
//
//}
void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
	if(hcan->Instance==CAN2)
	{
		if(canMessCounter<10)
			{
				if(canMessCounter<8)
					prepareSTDID(1,2,3,3);
				else
					prepareSTDID(2,2,3,3);
				hcan2.pTxMsg = &framedMessagesArr[canMessCounter];
				framedMessagesArr[canMessCounter].IDE = CAN_ID_STD;
				framedMessagesArr[canMessCounter].DLC = 8;
				uint32_t tmp = *(uint32_t*)&STDID;
				framedMessagesArr[canMessCounter++].StdId =tmp;
				HAL_CAN_Transmit_IT(&hcan2);
		//		beginCANTransmitFlag = false;
			}
		else canMessCounter=0;
	}
//	if(HAL_CAN_GetState(&hcan2)==HAL_CAN_STATE_READY)
//	{

//			hcan2.pTxMsg = framedMessagesArr[canMessCounter++];
//			prepareSTDID((char)mesMode,2,3,3);
//			hcan2.pTxMsg = &framedMessagesArr[j];
//			framedMessagesArr[j].IDE = CAN_ID_STD;
//			framedMessagesArr[j].DLC = 8;
//			framedMessagesArr[j].StdId = STDID;
//			HAL_CAN_Transmit_IT(&hcan2);
//			HAL_CAN_Transmit_IT()
//		}
//	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		volatile uint16_t	CntRxBINR=0,CntU4=0;
		volatile char 	RBINR[2048];			//����� �����������
		volatile bool tmp2=false;
		uint8_t rxbyte=0,t,r;
  if(huart->Instance==UART4)
  {
	     	        rxbyte = (char)UART4->DR;
	  				switch (NumberSatateRxBINR)
	  				{
	  					case sIDLE://������������� ����������� ������ ������
	  					{
	  						if(rxbyte==DLEBINR)
	  						{
	  							NumberSatateRxBINR=sHEAD;
	  							flagnewbinr=true;
	  						}
	  						break;
	  					}

	  					case sHEAD://������ �������������� ������ ������
	  					{

	  		#ifdef yesCRC
	  						if((rxbyte==DLEBINR)||(rxbyte==CRCBINR)||(rxbyte==ETXBINR))
	  		#else
	  						if((rxbyte==DLEBINR)||(rxbyte==ETXBINR))
	  		#endif
	  						{
	  							NumberSatateRxBINR=sIDLE;
	  							flagnewbinr=false;
	  						}
	  						else
	  						{
	  							RxCntBINR=0;
	  							RxBINR[RxCntBINR] = rxbyte;//������ � ����� ��������� ID ������
	  							RBINR[RxCntBINR] = rxbyte;//������ � �����u8	rxbyte,t,r; ��������� ID ������
	  							NumberSatateRxBINR=sDATA;
	  						}
	  						break;
	  					}

	  					case sDATA://������ ������
	  					{
	  						if(rxbyte!=DLEBINR)
	  						{
	  							RxCntBINR++;
	  							RxBINR[RxCntBINR] = rxbyte;//������ � ����� ������ ������
	  							RBINR[RxCntBINR] = rxbyte;//������ � ����� ��������� ID ������
	  						}
	  						else
	  						{
	  							NumberSatateRxBINR=sEND;
	  						}
	  						break;
	  					}

	  					case sEND://�������� �����
	  					{
	  						if(rxbyte==DLEBINR)
	  						{
	  							RxCntBINR++;
	  							RxBINR[RxCntBINR] = rxbyte;//������ � ����� ������ ������
	  							RBINR[RxCntBINR] = rxbyte;//������ � ����� ��������� ID ������
	  							NumberSatateRxBINR=sDATA;//������ DLE
	  						}
	  		#ifdef yesCRC
	  						else if(rxbyte==CRCBINR)	//�������� �� ��
	  						{
	  							NumberSatateRxBINR=sPRECRC;
	  							NumberbyteCRC=0;
	  						}
	  		#endif
	  						else if(rxbyte==ETXBINR)	//�������� �� �����
	  						{
	  							NumberSatateRxBINR=sDONE;
	  						}
	  						else
	  						{
	  							NumberSatateRxBINR=sIDLE;//������
	  						}
	  						break;
	  					}
	  					//break;
	  		#ifdef yesCRC
	  					case sPRECRC://������ CRC
	  					{
	  						MCRC[NumberbyteCRC]=rxbyte;
	  						NumberbyteCRC++;
	  						if(NumberbyteCRC>2)//��� ������� 2 ����� CRC � ��� ���� ����
	  						{
	  							if(rxbyte==DLEBINR)
	  							{
	  								NumberSatate	  	if(UART4->SR&UART_FLAG_ORE)
	  RxBINR=sPOSTCRC;//������� � �������� ETX
	  							}
	  							else
	  							{
	  								NumberSatateRxBINR=sIDLE;//������
	  							}
	  						}
	  					}
	  					break;
	  					case sPOSTCRC://������ ������
	  					{
	  						if(rxbyte==ETXBINR)	//�������� �� �����
	  						{
	  							NumberSatateRxBINR=sCHECK;
	  						}
	  						else
	  						{
	  							NumberSatateRxBINR=sIDLE;//������
	  						}
	  					}
	  					//break;
	  					case sCHECK://�������� CRC
	  					{
	  						//��� ����� ��������� CRC
	  						CRCok=true;//��� ��� ��� ������� ���������� CRC, ������ ��� ������!
	  						if(CRCok)
	  						{
	  							NumberSatateRxBINR=sDONE;
	  						}
	  						else
	  						{
	  							NumberSatateRxBINR=sIDLE;//������
	  						}
	  					}
	  		#endif
	  					//break;
	  					case sDONE://������� ���������� �����
	  					{
	  						//������������ �������� ����� ���� ���� �����, � ������ ����
	  						reciveBINR=true;
	  						NumberSatateRxBINR=sHEAD;//not debug
	  						r=RxBINR[0];
	  						CntRxBINR=RxCntBINR;
	  						if(r==0x41)//��������������� � �������
	  						{
//	  							anotherCounter++;
	  //								uint8_t		h41BINRmas[12];			//������ � �������� ������� 41
	  //								bool	h41BINRok;
	  							for(CntU4=0;CntU4<=CntRxBINR;CntU4++)
	  							{
	  								h41BINRmas[CntU4]=RxBINR[CntU4+1];
	  							}
	  							h41BINRok=true;				//���� ��������� ������� �4
//	  							h88BINRok=true;				//���� ��������� ������� �4
	  							if(h88BINRok)
	  							{
//	  								HAL_GPIO_TogglePin(CAN_SHDN_PORT,CAN_SHDN_PIN);


	  								NumberSatateRxBINR=sHEAD;
	  							}
	  							else
	  								NumberSatateRxBINR=sHEAD;
	  //
	  //							else
	  							r=CntU4;
//	  							uartLostCyclesCounter=0;

	  //							r=0x41;
	  //							commandToSend = 0x27;
	  //							changeCommand = true;
	  						}
	  						if(r==0x88)//��������������� � �������
	  						{
//	  							anotherCounter1++;
	  		#ifdef debugprog
	  							for(CntU4=0;CntU4<=CntRxBINR;CntU4++)
	  							{
	  								masE4[CntU4]=RxBINR[CntU4+1];
	  							}
	  		#endif
	  							for(CntU4=0;CntU4<=CntRxBINR;CntU4++)
	  							{
	  								h88BINRmas[CntU4]=RxBINR[CntU4+1];
	  							}
	  							h88BINRok=true;
//	  							if(h41BINRok)
//	  							{
//	  											//���� ��������� ������� �4
//	  								NumberSatateRxBINR=sHEAD;
//	  							}
//	  							else
//	  							{
//	  								h88BINRok = false;
//	  								NumberSatateRxBINR=sIDLE;
//	  							}
	  //							h41BINRok=true;				//���� ��������� ������� �4

	  							r=CntU4;
//	  							uartLostCyclesCounter=0;
	  							if(h41BINRok)
	  							{
//	  								HAL_GPIO_TogglePin(CAN_SHDN_PORT,CAN_SHDN_PIN);
//	  								h41BINRok = false;
//	  								h88BINRok = false;
	  								beginCANTransmitFlag = true;
	  								NumberSatateRxBINR=sHEAD;
	  							}
	  							else
	  								NumberSatateRxBINR=sHEAD;
	  							r=0x88;
	  //							commandToSend = 0x13;
	  //							changeCommand = true;
	  						}
	  						break;
	  					}
	  //					break;
	  				}

	  //			}
//�������� ��������� ����� ����� ������� ����-������
	 HAL_UART_Receive_IT(&huart4, mas, 1);
  }

}

//void SendAT (const char * S)		//������� ������ ��������� ������� ������
//{
//
//	 TxBusy = 1;
//	 strcpy (AT_COM,S);
//	 DLINA_AT = strlen (AT_COM);
//	 HAL_UART_Transmit_IT(&huart4, AT_COM, DLINA_AT);
//	 HAL_Delay(1);
//
//}
/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
