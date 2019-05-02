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
	    virtual int available();
	    virtual int read();
	    virtual int read(uint8_t *buf, size_t size);
	    virtual int peek();
	    void flush();
	    void stop();
	    uint8_t connected();
	    virtual operator bool();
};

#endif // GSM_CLIENT_H