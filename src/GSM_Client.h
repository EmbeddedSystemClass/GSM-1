#ifndef GSM_CLIENT_H
#define GSM_CLIENT_H

#include "GSM.h"

class GSM_Client: public Client, public GSM {
    public:
	    GSM_Client(SoftwareSerial& serial);
	    virtual int connect(IPAddress ip, uint16_t port);
	    int connect(const char *host, uint16_t port);
	    size_t write(uint8_t);
	    size_t write(const uint8_t *buf, size_t size);
	    inline int available( void )
		{
    		return( Data_Available() );
		}
	    inline int read( void )
		{
			return( TCP_Read() );
		}

	    virtual int read(uint8_t *buf, size_t size);	//< Do not implement
	    inline int peek( void )
		{
			return( TCP_Peek() );
		}
	    inline void flush( void )
		{
			TCP_Flush();
		}
	    void stop();
	    uint8_t connected();
	    virtual operator bool();
};

#endif // GSM_CLIENT_H