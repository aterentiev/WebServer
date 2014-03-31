#include "webserver.h"

WebServer::WebServer(EthernetServer *srv)
{
    _position = 0;
    _server = srv;
}

WebServer::~WebServer()
{

}

void WebServer::Initialize(byte mac[6], byte ip[4])
{
    Ethernet.begin(mac, ip);
    _server->begin();
}

void WebServer::Do()
{
    _client = _server->available();
    uint8_t i = 0;
    if (_client)
    {
        while (_client.connected())
        {
            if (_client.available())
            {
                char c = _client.read();
                if ((c != 0x0D) && (c != 0x0A) && (i < (MAX_REQUEST_SIZE + 4)))
                {
                    if ((c < 0x20) || (c > 0x7E)) break; // Sometimes strange things happen...
                    request[i] = c;
                    i++;
                    continue;
                }
                request[i] = 0;
                if (i > 4) {
                    if (!strncmp(request, "GET /", 5))
                    {
                        for (uint8_t i = 5; i < (MAX_REQUEST_SIZE + 4); i++) if (request[i] == ' ') { request[i] = 0; break; }
                        if (_callback) _callback(&request[5]);
                    }
                }
                break;
            }
        }
        delay(1);
        _client.stop();
    }
}

void WebServer::attachInterrupt(WebServerCallbackType callback)
{
    _callback = callback;
}

void WebServer::detachInterrupt()
{
    _callback = 0;
}

boolean WebServer::isRequested(const char *requested)
{
    return (strcmp(&request[5], requested) == 0);         
}

// Print possibilities

void WebServer::print(const __FlashStringHelper *str)
{
    const char PROGMEM *p = (const char PROGMEM *)str;
    while (1) {
        uint8_t c = (uint8_t)pgm_read_byte(p++);
        if (c == 0) break;
        _put(c);
    }
}

void WebServer::print(const String &str)
{
    for (uint16_t i = 0; i < str.length(); i++) {
        _put((uint8_t)str[i]);
    }
}

void WebServer::print(const char str[])
{
    uint16_t i = 0;
    while (str[i]) {
        _put((uint8_t)str[i]);
        i++;
    }
}

void WebServer::print(char c)
{
    _put((uint8_t)c);
}

// Println possibilities
void WebServer::println()
{
    print(F("\r\n"));
}

void WebServer::println(const __FlashStringHelper *str)
{
     print(str);
     println();
}

void WebServer::println(const String &str)
{
     print(str);
     println();
}

void WebServer::println(const char str[])
{
     print(str);
     println();
}

void WebServer::println(char c)
{
     print(c);
     println();
}


// Put one byte into buffer and send the buffer if it is full
void WebServer::_put(uint8_t c)
{
    _response[_position] = c;
    _position++;
    if (_position == RESPONSE_BUF_LEN) send();
}

// HTTP headers
void WebServer::http200()
{
    println(F("HTTP/1.1 200 OK"));
    println();
}

void WebServer::http200(unsigned long size, fileType type)
{
    println(F("HTTP/1.1 200 OK"));
    print(F("Content-size: "));
    printNumber(size, 10);
    println();
    if (type)
    {
        print(F("Content-type: "));
        switch (type)
        {
            case HTML:
                println(F("text/html")); break;
            case JPG:
                println(F("image/jpeg")); break;
            case PNG:
                println(F("image/png")); break;
            default: // == TXT
                println(F("text/plain")); break;
        }
    }
    println();
}

void WebServer::http404()
{
    println(F("HTTP/1.1 404 File not found"));
    println();
}

// Just send the buffer if any data there
void WebServer::send()
{
    if (_position) _client.write(_response, _position);
    _position = 0;
}

// Got from Print.cpp
void WebServer::printNumber(unsigned long n, uint8_t base) 
{
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1 or 0
    if (base < 2) base = 10;

    do {
        unsigned long m = n;
        n /= base;
        char c = m - base * n;
        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);

    print(str);
}
