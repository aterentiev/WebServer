#include <SPI.h>
#include <Ethernet.h>
#include <webserver.h>

EthernetServer ethernetServer(80); // Web Server Port
WebServer webServer(&ethernetServer);

void setup()
{
    byte mac[] = { 0x12, 0x34, 0x56, 0x00, 0x00, 0x01 };
    byte ip[] = { 192, 168, 1, 100 };
    webServer.Initialize(mac, ip);
    webServer.attachInterrupt(webServer_OnRequest);
}

void loop()
{
    webServer.Do();
}

void webServer_OnRequest(const char *request)
{
    if (webServer.isRequested("123")) 
    {
        webServer.http200();
        webServer.print(F("something 123 "));
        webServer.print(F("even more <b>something</b> 123"));
        webServer.send();
        return;
    }
    if (webServer.isRequested("345")) 
    {
        webServer.http200();
        webServer.print(F("something 345 "));
        webServer.print(F("even more something "));
        webServer.print(F("and more "));
        webServer.print(F("and more "));
        webServer.print(F("and more "));
        webServer.print(F("and more and more and more and more and more and more and more and more and more and more "));
        webServer.print(F("and more and more and more and more and more and more and more and more and more and more "));
        webServer.print(F("and more and more and more and more and more and more and more and more and more and more "));
        webServer.print(F("and more and more and more and more and more and more and more and more and more and more ")); // All these will be in only some PDUs
        webServer.send();
        return;
    }
    webServer.http404();
    webServer.send();
}


