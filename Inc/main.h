/*
 * main.h
 *
 *  Created on: 13 џэт. 2015 у.
 *      Author: User
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_
#include "stdbool.h"
CanTxMsgTypeDef TxMessage;
CanTxMsgTypeDef framedMessagesArr[10];
typedef struct
{
	short LAx;			//linearic acceleration on X-axis
	short LAy;			//linearic acceleration on Y-axis
	short LAz;			//linearic acceleration on Z-axis
	uint8_t service;			//sevice byte
	uint8_t CSL;				//checksum
}MDLUdataType;

typedef struct
{
	short ARx;			//angle rate on X-axis
	short ARy;			//angle rate on Y-axis
	short ARz;			//angle rate on Z-axis
	uint8_t service;			//service byte
	uint8_t CSL;				//checksum
}MDUSdataType;

typedef struct
{
	int pressure;
	int temperature;
	uint8_t service;
	uint8_t CSL;
	uint16_t offset;
}MDADdataType;


typedef struct
{

	unsigned messageType:3;			//ID0-ID2
	unsigned moduleType:4;			//ID3-ID6  module type inertial 0001, sattelite 0010, interface module 0011, radio modem 0100, UHF 0101
	unsigned transmitionType:2;		//ID7-ID8  0 - single packet; 3 - 1st packet, has packet amount; 2 - CS of all packets, last frame; 1 - not 1s nor last packet, has number of packet;
	unsigned priorityLevel:2;		//ID9-ID10
	unsigned unused:21;

} standardID;



typedef struct
{
	unsigned deviceSerialNumber:12;		//EID11-EID0 serial number of device
	unsigned messageMode:2; 					//EID13, EID12 0 - single packet; 3 - 1st packet, has packet amount; 2 - CS of all packets, last frame; 1 - not 1s nor last packet, has number of packet;
	unsigned messageType:4; 				//EID17-EID14   0b0000 command; 0b0101 answer; 0b1010 data;
	unsigned deviceType:7;				//ID6-ID0  0b00000000 MDLU; 0b00101010 MDUS; 0b01010101 MDAD;
	unsigned priorityLevel:4;				//ID10-ID7  by default priority level is 0111
	unsigned unused:3;

}extendedID;

//typedef INT12U type = (aRecord->bytes[0] & 0xF0) >> 4;
/*
 *  ICL:ICM:ICH 		0xE7E7E7 - disable MDLU polling, 0xAAAAAA - enable MDLU polling
 *						0xC3C3C3 - disable MDUS polling, 0xA5A5A5 - enable MDUS polling
 *						0xDBDBDB - disable MDAD polling, 0xBDBDBD - enable MDAD polling
 *	IDL:IDH:DIDL:DIDH 	0x0001..0x01f4 - frequency of MDLU polling, 0x0001..0x0050 - MDUS, 0x0001..0x0019 - MDAD
 *
 */
typedef struct
{
	char instructionCode[3];     			// ICL:ICM:ICH
	char configData[2]; 					// IDL:IDH
	char dConfigData[2];					// DIDL:DIDH
	char CSL;							// low byte of checksum CSL = DATA0+DATA1+...+DATA6

}configIIM;


typedef enum {BasicModule = 0,MDLU=0x01,MDUS = 0x02, MDAD = 0x03, RadioTransmitter = 0x04, SatteliteModule = 5, NetworkModule = 6, RadioModem = 7}IIMmoduleType;
typedef enum {Command=0x00,Answer = 0x5, Data = 0xA}IIMmessageType;

typedef enum {IIM_enablePolling_MDLU = 0xAA,IIM_disablePolling_MDLU = 0xE7,
						IIM_enablePolling_MDUS = 0xA5,IIM_disablePolling_MDUS = 0xC3,
						IIM_enablePolling_MDAD = 0xBD,IIM_disablePolling_MDAD = 0xDB
}IIMInstructionCode;


IIMmoduleType IIMTyp, IIMTransmitType;
IIMInstructionCode IIMinstr;
extendedID *EXTID;
standardID STDID;
MDLUdataType MDLUdata;
MDUSdataType MDUSdata;
MDADdataType MDADdata;
MDLUdataType MDLUTransmitData;
MDUSdataType MDUSTransmitData;
MDADdataType MDADTransmitData;
CAN_HandleTypeDef CanHandle;
configIIM IIMConf;

int counter;

char makeFramedCANMessage(int *currentArrIndex, uint8_t *outArr, uint8_t *mode);
char parseArray(uint8_t *inArr, uint16_t inArrLength, int arrType, uint8_t *outArr, uint16_t *outArrLength);
uint8_t calcCSofArr(uint8_t *arr, uint8_t arrSize);
void prepareEXTID(short serial, char msgtype,char devtype,char priority, char msgMode);
void prepareSTDID( char mestype, char mtype, char ttype, char priority);
#endif /* INC_MAIN_H_ */
