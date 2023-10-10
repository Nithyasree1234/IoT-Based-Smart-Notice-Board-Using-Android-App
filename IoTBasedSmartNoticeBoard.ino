#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <RoninDMD.h> 
#include <fonts/Arial_Black_16.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define FONT Arial_Black_16 
#define WIDTH 2
#define HEIGHT 1

RoninDMD P10(WIDTH, HEIGHT); 
SoftwareSerial mySerial(D1, D2); // TX, RX for DFPlayer Mini  
DFRobotDFPlayerMini myDFPlayer;

const char *ssid = "Redmi Note 10";
const char *password = "nithya1234";

WiFiClient client;

unsigned long counterChannelNumber = 2016792;
const char *myCounterReadAPIKey = "S5KLDTLH2N0MGTUT";
const int FieldNumber1 = 1;
String presentStr, previousStr = "";

void setup()
{
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  ThingSpeak.begin(client);

  P10.begin(); 
  P10.setFont(FONT); 
  P10.setBrightness(20); 
  Serial.begin(9600);  
  mySerial.begin(9600);
  myDFPlayer.begin(mySerial);
  delay(500);  
  
  myDFPlayer.setTimeOut(500); //Set serial communication time out 500ms
  myDFPlayer.volume(30); // set volume to 20 (0-30)  
  
}

void loop()
{
  presentStr = ThingSpeak.readStringField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);

  if (presentStr.charAt(0) == '0') {
    presentStr = presentStr.substring(1);
  }

  Serial.println(presentStr);

  if (presentStr != previousStr) {
    myDFPlayer.playMp3Folder(1); // play audio when presentStr is not equal to previousStr
  }

  Scrolling_text(0, 50, presentStr); 
  previousStr = presentStr;
}

void Scrolling_text(int text_height, int scroll_speed, String scroll_text)
{
  static uint32_t pM;
  pM = millis();
  static uint32_t x = 0;
  scroll_text = scroll_text + " ";

  bool scrl_while = 1;
  int dsp_width = P10.width();
  int txt_width = P10.textWidth(scroll_text);

  while (scrl_while == 1)
  {

    P10.loop();

    delay(1);
    if (millis() - pM > scroll_speed)
    {
      P10.setFont(FONT);
      P10.drawText(dsp_width - x, text_height, scroll_text);
      x++;
      if (x > txt_width + dsp_width)
      {
        x = 0;
        scrl_while = 0;
      }
      pM = millis();
    }
  }
}
