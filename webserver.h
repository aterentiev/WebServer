#ifndef __webserver_h__
#define __webserver_h__

// Comment this for disabling SD card support
#define USE_SD_CARD 1

#ifdef USE_SD_CARD
#include <SD.h>
#endif

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>

// Maximum GET request length
#define MAX_REQUEST_SIZE 20

#define WEBSERVER_HTTP200 F("200 OK")
#define WEBSERVER_HTTP404 F("404 Not Found")

#define WEBSERVER_DEFAULTPAGE F("<!DOCTYPE html><html lang=\"de-DE\"><head><META http-equiv=\"refresh\" content=\"0;URL=/index.htm\"></head></html>")

class WebServer
{

    public:

        // Must be extended with supported data types
        enum fileType
        {
            None = 0, // If fileType == None, the Content-type parameter will not be generated
            TXT,      // Content-type: text/plain
            HTML,     // Content-type: text/html
            XML,      // Content-type: text/xml
            JPG,      // Content-type: image/jpeg
            PNG,      // Content-type: image/png
            ICO,      // Content-type: image/x-icon
            JSON,     // Content-type: application/json
            JS        // Content-type: application/x-javascript
        };

        typedef void (*WebServerCallbackType)(const char *request);
        char request[MAX_REQUEST_SIZE + 4];

        WebServer(EthernetServer *server, uint8_t bufsize = 144);
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
        char *Requested();

        // Print and println functions to put the data to the buffer and send if the buffer is full
        // This is the full list from Print except printing Printable
        void print(const __FlashStringHelper *);
        void print(const String &);
        void print(const char[]);
        void print(char);
        void print(unsigned char, int = 10);
        void print(int, int = 10);
        void print(unsigned int, int = 10);
        void print(long, int = 10);
        void print(unsigned long, int = 10);
        void print(double, int = 2);
        void println(const __FlashStringHelper *);
        void println(const String &s);
        void println(const char[]);
        void println(char);
        void println(unsigned char, int = 10);
        void println(int, int = 10);
        void println(unsigned int, int = 10);
        void println(long, int = 10);
        void println(unsigned long, int = 10);
        void println(double, int = 2);

        void println(void);

        void printNumber(unsigned long n, uint8_t base = 10);
        void printFloat(double number, uint8_t digits = 7);

#ifdef USE_SD_CARD
        boolean printFile(char *fileName);
#endif

        // Put the HTTP answer into the buffer
        void http200(fileType type);
        void http200(fileType type, unsigned long size);
        void http404();
        fileType contentType(char *filename);

        // Send the rest of the data buffer if any 
        void send();

    private:

        // Send buffer, its position and a function to put one byte into the buffer
        uint8_t _position;
        uint8_t *_response;
        uint8_t _bufsize;
        void _put(uint8_t c);

        // Client and server objects
        EthernetServer *_server;
        EthernetClient _client;

        // Helpers
        void _contentType(fileType type = None);

        // Callback placeholder
        WebServerCallbackType _callback;

};

#endif
