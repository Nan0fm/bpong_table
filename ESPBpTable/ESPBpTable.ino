#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <EEPROM.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define USE_SERIAL Serial
#define MIN_PULSE_WIDTH 0
#define MAX_PULSE_WIDTH 4090

#define PORT_A_RED 2
#define PORT_A_GREEN 4
#define PORT_A_BLUE 0
#define PORT_B_RED 8
#define PORT_B_GREEN 10
#define PORT_B_BLUE 6
#define PORT_C_RED 9
#define PORT_C_GREEN 7
#define PORT_C_BLUE 11
#define PORT_D_RED 3
#define PORT_D_GREEN 1
#define PORT_D_BLUE 5

#define RED_A_ADDR 1
#define GREEN_A_ADDR 2
#define BLUE_A_ADDR 3
#define RED_B_ADDR 4
#define GREEN_B_ADDR 5
#define BLUE_B_ADDR 6
#define RED_C_ADDR 7
#define GREEN_C_ADDR 8
#define BLUE_C_ADDR 9
#define RED_D_ADDR 10
#define GREEN_D_ADDR 11
#define BLUE_D_ADDR 12
String webPage;

ESP8266WiFiMulti WiFiMulti;
byte redA, greenA, blueA, redB, greenB, blueB, redC, greenC, blueC, redD, greenD, blueD;


#ifndef STASSID
#define STASSID "Your_ssid"
#define STAPSK  "Your_pw"
#endif

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
//char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char packetBuffer[29 + 1]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";       // a string to send back

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
   pwm.begin();
  pwm.setPWMFreq(400);  // This is the maximum PWM frequency

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  Udp.destinationIP().toString().c_str(), Udp.localPort(),
                  ESP.getFreeHeap());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, 29);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

 //   parseEvent();
parseEvent(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }

}

void parseEvent(  char* payload){
    
    Serial.println("parseEvent:");
    Serial.println(packetBuffer);
      char Dimmer [7];
      int rgbArr[12];
      for (int j = 0; j < 29;) {
        for (int i = 0; i < 7; i ++) {
          Dimmer [i] = + payload[j];
          j ++ ;
        }
        // decode rgb data
        uint32_t rgb = (uint32_t) strtol((const char *) &Dimmer[1], NULL, 16);
        Serial.println("rgb:");
        Serial.println(rgb);
         uint8_t red = rgb >> 16 & 0xFF;
        uint8_t green = rgb >> 8 & 0xFF;
        uint8_t blue = rgb >> 0 & 0xFF;
        Serial.println(Dimmer[0]);
        switch (Dimmer[0]) {
          case 'a':
            setRgbPwm(PORT_A_RED, PORT_A_GREEN, PORT_A_BLUE, red, green, blue);
            rgbArr[0] = red;
            rgbArr[1] = green;
            rgbArr[2] = blue;
            break;
          case 'b':
            setRgbPwm(PORT_B_RED, PORT_B_GREEN, PORT_B_BLUE, red, green, blue);
            rgbArr[3] = red;
            rgbArr[4] = green;
            rgbArr[5] = blue;
            break;
          case 'c':
            setRgbPwm(PORT_C_RED, PORT_C_GREEN, PORT_C_BLUE, red, green, blue);
            rgbArr[6] = red;
            rgbArr[7] = green;
            rgbArr[8] = blue;
            break;
          case 'd':
            setRgbPwm(PORT_D_RED, PORT_D_GREEN, PORT_D_BLUE, red, green, blue);
            rgbArr[9] = red;
            rgbArr[10] = green;
            rgbArr[11] = blue;
            break;
          case 's':
            Serial.println("save");
            for ( int i = 0; i < 12; i++) {
              Serial.println(rgbArr[i]);
              EEPROM.write(i + 1, rgbArr[i]);
              delay(100);
            }
            EEPROM.commit();
            break;
        }

      }
}

int pulseWidth(int analog) {
  int pulse_wide;
  pulse_wide = map(analog, 0, 255, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  return pulse_wide;
}
void setRgbPwm ( byte portRed, byte portGreen, byte portBlue, uint8_t red, uint8_t green, uint8_t blue) {
   Serial.println("setRgbPwm:");
    Serial.println(portRed);
    Serial.println(red);
   
  pwm.setPWM(portRed, 0, pulseWidth(red));
  pwm.setPWM(portGreen, 0, pulseWidth(green));
  pwm.setPWM(portBlue, 0, pulseWidth(blue));
}

/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/
