/*
 lightning iot sword project for NT札幌 2019/11/30

 Special thanks to these examples and projects I especially refered
  -from FastLED.h  ColorPalette 
  -from WiFi.h  WiFiAccessPoint 
  -ESP32/ESP8266 RGB LED Strip with Color Picker Web Server
    (https://randomnerdtutorials.com/esp32-esp8266-rgb-led-strip-web-server/)
      by RANDOM NERD TUTORIALS 
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <FastLED.h>

// Set these to your desired credentials.
const char *ssid = "('ω')";
const char *password = "12345678";
WiFiServer server(80);

// Some for LEDs
#define LED_PIN     3
#define NUM_LEDS    38
#define BRIGHTNESS  120
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 65
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

String colorString = "0";
String numString = "1";
String red = "0";
String green = "0";
String blue = "0";
int flag = 0;
int pos1 = 0;
int pos2 = 0;
int num = 0;
int rr = 0;
int gg = 0;
int bb = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds
const long timeoutTime = 300;
// to do with the push button
int count = 0;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  pinMode(2, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println();
  wifisetup();
  ledsetup();
  goled();  
}

void goled(){
  Palette1();
  leds[0] = 0xff2222;
  leds[33] = 0xc71585;
  leds[34] = 0xc71585;
  leds[35] = 0xc71585;
  leds[36] = 0xc71585;
  leds[37] = 0xc71585;
  FastLED.show();
}



void loop() {
  wifi();
  if(flag==1){ChangePalettePeriodically();} //select random
  // turn off when pressed long
  if(digitalRead(2)){count+=1;}
  if (count > 100){
    while(digitalRead(2)){
      for(int i=0;i<38;i++){
        leds[i]=0x000000;
        FastLED.show();
      }
      delay(300);
      Serial.println("pushing");
    }
    goled();
    Serial.println("nukemashita");
    count=0;
  }
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors(startIndex);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}


void ledsetup() {
    delay( 2000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( BRIGHTNESS );
    Palette1();
    currentBlending = LINEARBLEND;
}


void wifisetup() {
  Serial.println("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
}


void wifi() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime){ // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // the content of the HTTP response follows the header:
            client.print("<!DOCTYPE html><html><head><meta name='viewport'charset='UTF-8'content='width=device-width',initial-scale=1.0,minimum-scale=1.0></head>"
                          "<body><style>body{margin:0 auto;text-align:center;background:linear-gradient(to bottom,#ffe4b5,#FFFAFA);height:900px}table{margin:0 30px 0 0;text-align:center;width:100%;padding:10% 0;}"
                          "h1{border-bottom:double 5px #d2691e;font-family:sans-serif;}.btn{font-family:sans-serif;font-size: 162%;display:inline-block;width:5.8em;height:3.2em;text-decoration:none;color:#FFF;border-bottom:solid 2px #627295;border-radius: 5px;}"
                          ".btn:active{-webkit-transform: translateY(2px);transform:translateY(2px);border-bottom: none;}.container{padding:0.5em;}div.selWrapper{background:#f0fff0;border-radius:4px;border:1px solid #0066ff;display:block;height:2em;margin:0;position:relative;width:30em;}"
                          """#styled{background-color:transparent;color:#000;display:block;font-size:16px;font-weight:bold;height:inherit;margin:0;padding-left:5px;text-overflow:ellipsis;width:100%;appearance:none;}div.wrapper > div{display:inline-block;margin:8em 0 0 0;text-align:center;vertical-align:top;width:3em;}"
                          "pre{display:inline;}select{margin-top:100px;}</style>"
                          "<div class='container'><div class='row'><h1>PRESS and PLAY</h1></div></div><table width='100%'><tbody><tr><td><button class='btn'style='background-color:#a0b4ce;'onclick='send(\"a\")'>Cloud</button></td><td><button class='btn'style='background-color:#228b22;'onclick='send(\"b\")'>Forest</button></td></tr>"
                          "<tr><td><button class='btn'style='background-color:#9932cc;'onclick='send(\"c\")'>Party</button></td><td><button class='btn'style='background-color:#cd5c5c;'onclick='send(\"d\")'>Lava</button></td></tr><tr><td><button class='btn'style='background-color:#1e90ff;'onclick='send(\"e\")'>Ocean</button></td>"
                          "<td><button class='btn'style='background-color:#ffa07a;'onclick='send(\"f\")'>Rainbow</button></td></tr><tr><td><button class='btn'style='background-color:#c0c0c0;'onclick='send(\"g\")'>?</button></td><td><button class='btn'style='background-color:#c0c0c0;'onclick='send(\"h\")'>random</button></td></tr></tbody></table>"
                          "<div class='wrapper'><div class='selWrapper'><select id='styled'name='styled'><option>1</option><option>2</option><option>3</option><option>4</option><option>5</option><option>6</option></select></div></div><br><button class='btn2'style='background-color:#fff;'onclick='col()'>↑番目のLEDを↓の色にする</button>"
                          "<div class='inp'><br><input type='color'id='rgb2'value='#0000ff'><br></div></body>"
                          "<script>function send(ch){var xh=new XMLHttpRequest();xh.onreadystatechange=function(){};xh.open('GET','/'+ch,true);xh.send();}"
                          "function col(){var a=document.getElementById('styled').value;var b=document.getElementById('rgb2').value.substr(1,2);var c=document.getElementById('rgb2').value.substr(3,2);var d=document.getElementById('rgb2').value.substr(5,2);"
                          "var one=String(parseInt(b,16));if(one.length==1){one='00'+one;}if(one.length==2){one='0'+one;}var two=String(parseInt(c,16));if(two.length==1){two='00'+two;}if(two.length==2){two='0'+two;}var three=String(parseInt(d,16));if(three.length==1){three='00'+three;}if(three.length==2){three='0'+three;}"
                          "var xh=new XMLHttpRequest();xh.onreadystatechange=function(){};xh.open('GET','/z/'+a+'/?'+one+two+three+'&',true);xh.send();}"
                          "</script></html>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // Some kind of rounting:
        if (currentLine.endsWith("GET /")){flag=0;Palette1();currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /a")){flag=0;currentPalette = CloudColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /b")){flag=0;currentPalette = ForestColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /c")){flag=0;currentPalette = PartyColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /d")){flag=0;currentPalette = LavaColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /e")){flag=0;currentPalette = OceanColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /f")){flag=0;currentPalette = RainbowColors_p;currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /g")){flag=0;Palette1();currentBlending = LINEARBLEND;}
        if (currentLine.endsWith("GET /h")){flag=1;}
        if (currentLine.endsWith("&")) { //parsing query parameters
          pos1 = currentLine.indexOf("?");
          pos2 = currentLine.indexOf("&");
          colorString = currentLine.substring(pos1+1, pos2);
          red = currentLine.substring(pos1+1, pos1+4);
          green = currentLine.substring(pos1+4, pos1+7);
          blue = currentLine.substring(pos1+7, pos1+10);
          numString = currentLine.substring(pos1-2,pos1-1);
          //Serial.println(numString);Serial.println(red);Serial.println(blue);Serial.println(green);
          rr = red.toInt();
          gg = green.toInt();
          bb = blue.toInt();
          num = numString.toInt();
          if(numString=="6"){
            leds[0].r=rr; leds[0].g=gg; leds[0].b=bb; FastLED.show();
          }else{
            leds[38-num].r=rr; leds[38-num].g=gg; leds[38-num].b=bb; FastLED.show();
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}


void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for( int i = 32; i>0; i--) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

//RainbowColors_p,RainbowStripeColors_p,OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p are available.

void ChangePalettePeriodically(){
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
  }
}


void SetupTotallyRandomPalette(){
  for( int i = 0; i < 16; i++) {
      currentPalette[i] = CHSV( random8(), 255, random8());
  }
}


void SetupBlackAndWhiteStripedPalette(){
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}


void SetupPurpleAndGreenPalette(){
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

// my original palette.
void Palette1(){
  CRGB blue  = CHSV( HUE_BLUE, 255, 255);
  CRGB black  = CRGB::Black;
  currentPalette = CRGBPalette16(
                                 black, black, black, 0xff00ff,
                                 blue,  black, black, black,
                                 black, black, black, black,
                                 black, black, black, black );
}