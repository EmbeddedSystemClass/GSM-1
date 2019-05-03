#ifndef GSM_H
#define GSM_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Client.h>

#define OK_numeric F("0\r\n")
#define OK_Alphabetic F("OK\r\n")

#define DEBUG
#ifdef DEBUG
	// #define DEBUG_LVL_1
	// #define DEBUG_LVL_2
#endif

typedef const __FlashStringHelper *	FlashStringPtr;

class GSM
{
	public:
		GSM( SoftwareSerial& serial );
		bool GSM_Init( byte retry_count );
		void SIM_Status();	///< Implement
		bool TCP_Init();
		bool TCP_Connect();
		bool TCP_Close();
		bool TCP_Send(char* data, uint16_t len);
		bool Get_IP();
		bool TCP_Connected();

	private:
		static const byte BUFFER_SIZE = 64;
		SoftwareSerial* _serial;
		char _receive_buffer[BUFFER_SIZE];
		void Clear_Buffer( void );
		bool Send_AT_Command( FlashStringPtr AT_command,
			FlashStringPtr expected_reply,
			unsigned int delay_time = 500 );
		bool Send_AT_Command( char* AT_command, uint16_t len,
			FlashStringPtr expected_reply,
			unsigned int delay_time = 500 );
		bool Read_Check_Data( FlashStringPtr expected_reply );
};

#endif	// _GSM_H_