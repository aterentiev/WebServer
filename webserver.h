#ifndef __webserver_h__
#define __webserver_h__

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>

// Maximum GET request size
#define MAX_REQUEST_SIZE 20

// Response buffer length: optimum 576
#define RESPONSE_BUF_LEN 144

class WebServer
{

    public:

        // Must be extended with supported data types
        enum fileType
        {
            None = 0,
            TXT,
            HTML,
            JPG,
            PNG
        };

        typedef void (*WebServerCallbackType)(const char *request);
        char request[MAX_REQUEST_SIZE + 4];

        WebServer(EthernetServer *server);
        ~WebServer();

        // MAC and IP addresses initializer
        void Initialize(byte mac[6], byte ip[4]);

        // Call it in the loop()
        void Do();

        // Attach and detach callback for GET request
        void attachInterrupt(WebServerCallbackType callback);
        void detachInterrupt();

        // Check if the "requested" file was requested
        boolean isRequested(const char *requested);

        // Print nad println functions to put the data to the buffer and send if the buffer is full
        void print(const __FlashStringHelper *);
        void print(const String &);
        void print(const char[]);
        void print(char);
/* ToDo:
        void print(unsigned char, int = DEC);
        void print(int, int = DEC);
        void print(unsigned int, int = DEC);
        void print(long, int = DEC);
        void print(unsigned long, int = DEC);
        void print(double, int = 2);
*/
        void println(const __FlashStringHelper *);
        void println(const String &s);
        void println(const char[]);
        void println(char);
/* ToDo:
        void println(unsigned char, int = DEC);
        void println(int, int = DEC);
        void println(unsigned int, int = DEC);
        void println(long, int = DEC);
        void println(unsigned long, int = DEC);
        void println(double, int = 2);
*/

        void println(void);

        void printNumber(unsigned long n, uint8_t base);

        // Put the HTTP answer into the buffer
        void http200();
        void http200(unsigned long size, fileType type = None);
        void http404();

        // Send the rest of the data buffer if any 
        void send();

    private:

        // Send buffer, its position and a function to put one byte into the buffer
        uint8_t _position;
        uint8_t _response[RESPONSE_BUF_LEN];
        void _put(uint8_t c);

        // Client and server objects
        EthernetServer *_server;
	EthernetClient _client;

        // Callback placeholder
        WebServerCallbackType _callback;

};

#endif
