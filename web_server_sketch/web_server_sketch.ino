/* Arduino Web Server
 *  
 * @author:   Esteban Buror
 * @date:     05/2022
 * @version:  1.0
*/

#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xFE, 0xED};
EthernetServer server(80);

File HTML_file;

void setup() {    
  Serial.begin(9600);
  
  Serial.println("Initializing SD Card...");
  if (!SD.begin(4)) {
    Serial.println("Failed to start SD Card.");
    return;
  }
  Serial.println("SD Card initialized!");
  
  if (!SD.exists("index.htm")) {
    Serial.println("Web file not found!");
    return;
  }
  Serial.println("Web file found!");

  Ethernet.begin(mac);
  server.begin();

  if (Ethernet.localIP() != 0,0,0,0) {
    Serial.println("Error to start Web Server!");
  } else {
    Serial.println("Starting Web Server...");
    Serial.print("IP: ");
    Serial.println(Ethernet.localIP());
  }
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          HTML_file = SD.open("index.htm");
          if (HTML_file) {
            while (HTML_file.available()){
              client.write(HTML_file.read());
            }
            HTML_file.close();
          }
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(10);
    client.stop();
  }

}
