#include <GSM.h>

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
GSM::GSM( SoftwareSerial& serial )
{
	_serial = &serial;
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::GSM_Init( byte retry_count )
{
	bool return_val = false;
	byte counter = 0;
	
	while( ( counter < retry_count ) && ( return_val == false ) )
	{
	 	/// Check if GSM module is working
		if ( Send_AT_Command( F("AT"), OK_numeric ) )
		{
			Serial.println("Modem Initialized");
			/// Disable Echo
			if ( Send_AT_Command( F("ATE0"), OK_numeric ) )
			{
				Serial.println("Echo Disabled");
			}
			/// Enable numeric response
			if ( Send_AT_Command( F("ATV0"), OK_numeric ) )
			{
				Serial.println("Numeric Response Set");
			}
			/// Save settings
			if ( Send_AT_Command( F("AT&W"), OK_numeric ) )
			{
				Serial.println("Setting Saved");
			}
			return_val = true;
		}
		++counter;
	}
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::Read_Check_Data( FlashStringPtr expected_reply )
{
	bool return_val = false;
	uint8_t counter = 0;
	String data = "\0";

	Clear_Buffer();	//< Clear the buffer
	while ( ( _serial->available() > 0 ) && ( counter < BUFFER_SIZE ) )
	{
		_receive_buffer[counter] = _serial->read();
		Serial.println(_receive_buffer[counter], HEX);
		++counter;
		delay(1);
	}

	data = String(_receive_buffer);	//< Convert to string object
	#ifdef DEBUG
	Serial.print("<--- ");
	Serial.print(data);
	#endif
	if ( data.equals( expected_reply )  )
	{
		#ifdef DEBUG
		Serial.println("SUCCESS");
		#endif
		return_val = true;
	}
	else
	{
		#ifdef DEBUG
		Serial.println("FAILED");
		#endif
	}
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::Send_AT_Command( FlashStringPtr AT_command, FlashStringPtr expected_reply,
	unsigned int delay_time )
{
	bool return_val = false;
	
	#ifdef DEBUG
	Serial.println("/******************************************/");
	Serial.print("---> ");
	Serial.println(AT_command);
	#endif
	_serial->println( AT_command );
	delay(delay_time);
	if ( Read_Check_Data( expected_reply ) )
	{
		return_val = true;
	}

	#ifdef DEBUG
	Serial.println("/******************************************/");
	#endif
	return( return_val );
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::Send_AT_Command( char* AT_command, uint16_t len, FlashStringPtr expected_reply,
	unsigned int delay_time )
{
	bool return_val = false;
	
	#ifdef DEBUG
	Serial.println("/******************************************/");
	Serial.print("---> ");
	Serial.println(AT_command);
	#endif
	_serial->write( AT_command, len );
	delay(delay_time);
	if ( Read_Check_Data( expected_reply ) )
	{
		return_val = true;
	}

	#ifdef DEBUG
	Serial.println("/******************************************/");
	#endif
	return( return_val );
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
void GSM::Clear_Buffer( void )
{
	byte counter = 0;
	for ( counter = 0; counter < BUFFER_SIZE; counter ++ )
	{
		_receive_buffer[counter] = '\0';
	}
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::TCP_Init( void )
{
	bool return_val = false;
	if ( Send_AT_Command( F("AT+CIPSHUT"), F("\r\nSHUT OK\r\n"), 2000 ) )
	{
		Serial.println( "Deactivate GPRS" );
	}

	if ( Send_AT_Command( F("AT+CIPMODE=0"), OK_numeric ) )
	{
		Serial.println( "Non-Transparent mode enable" );
	}
	if ( Send_AT_Command( F("AT+CSTT=\"imis/internet\",\"\",\"\""), OK_numeric ) )
	{
		Serial.println( "APN set" );
	}
	if ( Send_AT_Command( F("AT+CIICR"), OK_numeric, 5000 ) )
	{
		Serial.println( "Connected to GPRS" );
	}
	return( return_val );
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::TCP_Connect( void )
{
	bool return_val = false;
	Get_IP();
	if ( Send_AT_Command( F("AT+CIPSTART=\"TCP\",\"m16.cloudmqtt.com\",\"12529\""),
		F("0\r\n\r\nCONNECT OK\r\n"), 5000 ) )
	{
		Serial.println( "TCP Connection Established" );
	}
	return( return_val );
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::TCP_Close( void )
{
	bool return_val = false;
	if ( Send_AT_Command( F("AT+CIPCLOSE"), OK_numeric ) )
	{
		Serial.println( "TCP Connection Closed" );
		return_val = true;
	}

	if ( Send_AT_Command( F("AT+CIPSHUT"), F("\r\nSHUT OK\r\n"), 2000 ) )
	{
		Serial.println( "Deactivate GPRS" );
		return_val = true;
	}
	return(return_val);
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::Get_IP( void )
{
	bool return_val = false;
	if ( Send_AT_Command( F("AT+CIFSR"), OK_numeric ) )
	{
		Serial.println( "IP allotted" );
		return_val = true;
	}

	return(return_val);
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::TCP_Connected( void )
{
	bool return_val = false;
	if ( Send_AT_Command( F("AT+CIPSTATUS"), F("\r\nCONNECT OK\r\n") ) )
	{
		Serial.println( "Connection OK" );
		return_val = true;
	}
	return( return_val );
}

/*
 ******************************************************************************
 * Please read header file for more information
 ******************************************************************************
 */
bool GSM::TCP_Send(char* data, uint16_t len)
{
	bool return_val = false;
	if ( Send_AT_Command( F("AT+CIPSEND"), F("\r\n> ") ) )
	{
		if ( Send_AT_Command( data, len, F("\r\nSEND OK\r\n") ), 3000 )
		{
			Serial.println( "Send Successful" );
			return_val = true;
		}
	}
	return( return_val );
}
