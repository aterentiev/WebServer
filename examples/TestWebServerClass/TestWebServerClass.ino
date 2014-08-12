#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>

EthernetServer ethernetServer(80); // Web Server Port
WebServer webServer(&ethernetServer); // Default buffer size is 144 bytes
//WebServer webServer(&ethernetServer, 200); // Example of defining other buffer size as 144

void setup()
{
    SD.begin(4);
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
    if (webServer.isRequested("")) // default website
    {
        webServer.http200(WebServer::HTML);
        webServer.print(F("<html><head><META http-equiv=\"refresh\" content=\"0;URL=/index.htm\"></head></html>"));
        webServer.send();
        return;
    }
    char *filename = webServer.Requested();
    if (webServer.printFile(filename)) return;
    webServer.http404();
    webServer.send();
}


