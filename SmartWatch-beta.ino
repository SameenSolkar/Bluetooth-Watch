#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

#define TIME_HEADER  "%"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

 byte data[70];
 char dateTime[14];
 String _time;
 String date;
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:  0     A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char epd_bitmap_instalogo [] PROGMEM = {
0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0, 0xf0, 0x00, 0xf0, 0xe0, 0x01, 0x70, 0xc0, 0x03, 0xb0, 0xc0, 
  0xf1, 0x30, 0xc3, 0xfc, 0x30, 0xc3, 0x0c, 0x30, 0xc6, 0x06, 0x30, 0xc6, 0x06, 0x30, 0xc6, 0x06, 
  0x30, 0xc6, 0x06, 0x30, 0xc3, 0x0c, 0x30, 0xc3, 0xfc, 0x30, 0xc0, 0xf0, 0x30, 0xc0, 0x00, 0x30, 
  0xe0, 0x00, 0x70, 0xf0, 0x00, 0xf0, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0
};

const unsigned char epd_bitmap_whatsapp [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x03, 0xfc, 0x00, 0x06, 0x03, 0x00, 0x0c, 0x01, 0x80, 0x18, 
  0x00, 0xc0, 0x33, 0x80, 0x40, 0x23, 0x80, 0x40, 0x23, 0x00, 0x60, 0x21, 0x00, 0x60, 0x21, 0x80, 
  0x60, 0x20, 0xcc, 0x60, 0x30, 0x7e, 0x40, 0x10, 0x1c, 0xc0, 0x10, 0x01, 0x80, 0x10, 0x03, 0x00, 
  0x3f, 0x0e, 0x00, 0x31, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_snapchat [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x03, 0xfc, 0x00, 0x07, 0xfe, 0x00, 0x07, 
  0xfe, 0x00, 0x07, 0xfe, 0x00, 0x07, 0xfe, 0x00, 0x07, 0xfe, 0x80, 0x1f, 0xff, 0x80, 0x07, 0xfe, 
  0x00, 0x07, 0xfe, 0x00, 0x0f, 0xff, 0x00, 0x1f, 0xff, 0x80, 0x3f, 0xff, 0xe0, 0x1f, 0xff, 0x80, 
  0x0f, 0xff, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_facebook [] PROGMEM = {
0x3f, 0xff, 0xc0, 0x7f, 0xff, 0xe0, 0xff, 0xff, 0xe0, 0xff, 0xf0, 0x70, 0xff, 0xe0, 0x70, 0xff, 
  0xe0, 0x70, 0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 0xff, 0x00, 0x70, 0xff, 0x00, 
  0x70, 0xff, 0x00, 0x70, 0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 
  0xff, 0xe3, 0xf0, 0xff, 0xe3, 0xf0, 0x7f, 0xe3, 0xe0, 0x3f, 0xe3, 0xc0
};

const unsigned char epd_bitmap_phone [] PROGMEM = {
0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 
  0x00, 0x00, 0x7c, 0x00, 0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
  0x00, 0x1e, 0x00, 0x00, 0x0f, 0x02, 0x00, 0x07, 0xc7, 0x80, 0x03, 0xef, 0xc0, 0x01, 0xff, 0xe0, 
  0x00, 0xff, 0xe0, 0x00, 0x3f, 0xc0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00
};

const unsigned char epd_bitmap_message [] PROGMEM = {
0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0xe0, 0x00, 0x70, 0xf0, 0x00, 0xf0, 0xd8, 0x01, 0xb0, 0xcc, 
  0x03, 0x30, 0xc6, 0x06, 0x30, 0xc3, 0x0c, 0x30, 0xc1, 0x98, 0x30, 0xc0, 0xf0, 0x30, 0xc0, 0x00, 
  0x30, 0xc0, 0x00, 0x30, 0xc0, 0x00, 0x30, 0xc0, 0x00, 0x30, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
//  display.display();
//  delay(2000); // Pause for 2 seconds

  // Clear the buffer
//  setSyncProvider(requestSync); 
  display.clearDisplay();
  displayText("S Watch",20,32,2); 

  while(Serial.available() == 0);
  
  processSyncMessage();
  
}

void loop() {
  String message;
  
 if (timeStatus()!= timeNotSet) {
  
       
  if(Serial.available()>0){

    
    size_t lenght = Serial.readBytes(data,70);
  
    for(int i=lenght-1;i>=0;i--){
      
      message = char(data[i]) + message;    
    }

 
  if(message.startsWith("1")){

    display.clearDisplay();
    String mainMessage = message.substring(1);
    displayTimeDate(date,_time,3,0,1);
    facebookIcon();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,35);
    display.println(mainMessage);
    display.display();      // Show initial text 
    delay(5000);  
  }
  
   if(message.startsWith("2")){
    display.clearDisplay();
    String mainMessage = message.substring(1);
    displayTimeDate(date,_time,3,0,1);
    whatsappIcon();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,35);
    display.println(mainMessage);
    display.display();      // Show initial text
    delay(5000);   
  }
  
  if(message.startsWith("3")){
    
    display.clearDisplay();
    String mainMessage = message.substring(1);
//    Serial.println(mainMessage);  
    displayTimeDate(date,_time,3,0,1);
    instagramIcon();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,35);
    display.println(mainMessage);
    display.display();      // Show initial text
    delay(5000);  
  }
  
   if(message.startsWith("4")){
    
    display.clearDisplay();
    String mainMessage = message.substring(1);
//    Serial.println(mainMessage);
    displayTimeDate(date,_time,3,0,1);
    snapchatIcon();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,35);
    display.println(mainMessage);
    display.display();      // Show initial text
    delay(5000);
  }

  if(message.startsWith("#")){
    
    display.clearDisplay();
    String mainMessage = message.substring(1);
    phoneIcon();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,50);
    display.println(mainMessage);
    display.display();      // Show initial text
    delay(5000);

    
    while(Serial.available() == 0);  
    while(!Serial.find('$'));
    }
    
  if(message.startsWith("*")){
    
    display.clearDisplay();
    messageIcon();
    String mainMessage = message.substring(1);
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(1,35);
    display.println(mainMessage);
    display.display();      // Show initial text
    delay(5000);
  }
  
//   if(message.startsWith("$")){
// 
//    display.clearDisplay();
//    displayText("S Watch",20,32,2);  
//  }
}

  else{
    
      display.clearDisplay();
      displayTime(hour(),minute(),weekday(),day(),month());
    }
}  
  Serial.flush();
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  String date_time;
  
  if(Serial.find(TIME_HEADER)) {
    
//     pctime = Serial.parseInt();
//     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
//       setTime(pctime); // Sync Arduino clock to the time received on the serial port
//     }
       size_t lenght =  Serial.readBytes(dateTime, 14);

      for(int i=lenght-1;i>=0;i--){
      
        date_time = char(dateTime[i]) + date_time;    
      }
//      Serial.println(date_time);
            
      int hrs = (int) date_time.substring(0,2).toInt() ;
      int mins = (int) date_time.substring(2,4).toInt();
      int sec = (int) date_time.substring(4,6).toInt();
      int weekDay = (int) date_time.substring(6,7).toInt();
      int Day = (int) date_time.substring(7,9).toInt();
      int Month = (int) date_time.substring(9,11).toInt();
      int Year = (int) date_time.substring(11,13).toInt();

      setTime(hrs,mins,sec,Day,Month,Year);
  } 
}

//time_t requestSync()
//{
//  Serial.write(TIME_REQUEST);  
//  return 0; // the time will be sent later in response to serial mesg
//}

void displayTime(byte hrs,byte mins,byte weekdays,byte days,byte months){

  int hrsformat12 = 0;

      if(hrs > 12){

          hrsformat12 = hrs-12;
        }
      else if(hrs == 0){

          hrsformat12 = hrs+12;
        }  
      else{

         hrsformat12 = hrs;
        }
  
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(15,25);

  if(hrsformat12 < 10){
      display.print('0');
      display.print(hrsformat12);
      display.print(':');
    }
  else{
      display.print(hrsformat12);
      display.print(':');
    }

  if(mins < 10){
      display.print('0');
      display.print(mins);
    }
  else{
      display.print(mins);
    }  
  
  
  if(isAM()){
      display.print(' ');
      display.print("AM");
    }
    else{
      
        display.print(' ');
        display.print("PM");
      }
  display.println();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
   display.setCursor(35,45);
  display.print(dayShortStr(weekdays));
  display.print(' ');
  display.print(days);
  display.print('/');
  display.print(monthShortStr(months));
  display.display();
 
}

void displayTimeDate(String idate,String itime,int x, int y, int fontSize){
  
//  display.clearDisplay();
  display.setTextSize(fontSize); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x,y);
  display.println(idate +' '+ itime );
  display.display();
}

void displayText(String data,int x,int y,int fontSize){
  
//  display.clearDisplay();
  display.setTextSize(fontSize); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x,y);
  display.println(data);
  display.display();
  delay(1000);
}

void instagramIcon(void) {
  display.clearDisplay();

#define _LOGO_HEIGHT   20
#define _LOGO_WIDTH    20

  display.drawBitmap(
     _LOGO_WIDTH/4,
    _LOGO_HEIGHT/2,
    epd_bitmap_instalogo, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
//  delay(1000);
}

void whatsappIcon(void) {
//  display.clearDisplay();


#define _LOGO_HEIGHT   20
#define _LOGO_WIDTH    20

  display.drawBitmap(
     _LOGO_WIDTH/4,
    _LOGO_HEIGHT/2,
    epd_bitmap_whatsapp, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
//  delay(1000);
}

void facebookIcon(void) {
  display.clearDisplay();


#define _LOGO_HEIGHT   20
#define _LOGO_WIDTH    20

  display.drawBitmap(
     _LOGO_WIDTH/4,
    _LOGO_HEIGHT/3,
    epd_bitmap_facebook, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
  //delay(1000);
}

void snapchatIcon(void) {
  display.clearDisplay();


#define _LOGO_HEIGHT   20
#define _LOGO_WIDTH    20

  display.drawBitmap(
     _LOGO_WIDTH/4,
    _LOGO_HEIGHT/2,
    epd_bitmap_snapchat, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
//  delay(1000);
}

void phoneIcon(void) {
  display.clearDisplay();


#define _LOGO_HEIGHT   20
#define _LOGO_WIDTH    20

  display.drawBitmap(
    (display.width()  - _LOGO_WIDTH ) / 2,
    (display.height() - _LOGO_HEIGHT) / 4,
    epd_bitmap_phone, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
//  delay(1000);
}

void messageIcon(void) {
  display.clearDisplay();


#define _LOGO_HEIGHT   16
#define _LOGO_WIDTH    20

  display.drawBitmap(
    (display.width()  - _LOGO_WIDTH ) / 2,
    (display.height() - _LOGO_HEIGHT) / 4,
    epd_bitmap_message, _LOGO_WIDTH, _LOGO_HEIGHT, 1);
  display.display();
//  delay(1000);
}
