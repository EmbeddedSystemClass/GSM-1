#include "GSM_Client.h"

GSM_Client::GSM_Client( SoftwareSerial& serial ) : GSM( serial )
{}

int GSM_Client::connect( const char *host, uint16_t port )
{
    int return_val = 1;
	GSM_Init(3);
	TCP_Init();
	TCP_Connect();
	return(return_val);
}

uint8_t GSM_Client::connected( void )
{
	uint8_t return_val = 0;
	if ( TCP_Connected() )
    {
        return_val = 1;
    }
	return( return_val );
}

void GSM_Client::stop()
{
    TCP_Close();
}

void GSM_Client::flush()
{}

size_t GSM_Client::write(uint8_t data)
{
    size_t return_val = 0;    
    if ( TCP_Send(reinterpret_cast<char*>(&data), 1) )
    {
        return_val = 1;
    }
    return( return_val );
}

size_t GSM_Client::write(const uint8_t *buf, size_t size)
{
    size_t return_val = 0;    
    if ( TCP_Send(reinterpret_cast<char*>( const_cast<uint8_t*>(buf) ), size ) )
    {
        return_val = size;
    }
    return( return_val );
}

int GSM_Client::connect(IPAddress ip, uint16_t port){}
int GSM_Client::available(){}
int GSM_Client::read(){}
int GSM_Client::read(uint8_t *buf, size_t size){}
int GSM_Client::peek(){}
GSM_Client::operator bool(){}