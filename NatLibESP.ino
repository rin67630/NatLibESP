/*
This sketch runs on ESP32 and ESP8266 without change.

**Do not*** include other fancy Time libraries!** 
Everything is on board from Espressif and shoulr compile ooB when the board support is installed.
Tested on Wemos dev boards ESP32 Wroom and Lolin ESP8266 with Arduino IDE 1.8.12.

n.B. The ESP32 is daylightsaving aware, the ESP8266 is not, but else, everything is the same.

The main purpose is to provide a portable approach, with outputs that match the needs of noobs, 
and more exprerienced users altogether.
It is designed to integrate well with other libraries relying on data presented in various formats.

The code is licensed CC-BY-SA. Laszlo Lebrun.
improvements, issues on Github: https://github.com/rin67630/NaTLibESP
*/

#define OFFLINE // Remove to keep online

#if defined(ESP8266)
#include <ESP8266WiFi.h>  // default from Espressif
#include <WiFiUdp.h>      // default from Espressif  
#include <TZ.h>           // default from Espressif 
#elif defined(ESP32)
#include <WiFi.h>         // defaults from Espressif
#include <time.h>
#endif

#define WIFI_SSID "MY SSID"
#define WIFI_PASS "My Password"
#define HOST_NAME "ESP-IoT"
#define NTP_SERVER "de.pool.ntp.org"

// maintain for ESP32
#define TZ            1               // (utc+) TZ in hours
#define DST_MN        60              // use 60mn for summer time in some countries
#define GMT_OFFSET_SEC 3600 * TZ      // Do not change here...
#define DAYLIGHT_OFFSET_SEC 60 * DST_MN // Do not change here...
// maintain for ESP8266
#define MYTZ TZ_Europe_Paris

// Variables for Time
static int lastSecond;
tm*        timeinfo;                 //localtime returns a pointer to a tm structstatic int Second;
time_t     Epoch;
time_t     now;

int Second;
int Minute;
int Hour;
int Day;
int Month;
int Year;
int Weekday;
char DayName[12];
char MonthName[12];
char Time[10];
char Date[12];

static IPAddress ip;

//*** Buffers ***
char charbuff[80];    //Char buffer for many functions
//String text;                 //String buffer for many functions

// Constructors
   //not needed, yet

void getWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay ( 500 );
    Serial.print ( "." );
  }
  ip = WiFi.localIP();
  sprintf(charbuff, "Connected to IP: %03d.%03d.%03d.%03d", ip[0], ip[1], ip[2], ip[3]);
  Serial.println(charbuff);
}

void getNTP()
{
#if defined(ESP8266)
  configTime(MYTZ, NTP_SERVER);
#elif defined(ESP32)
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
#endif
  now = time(nullptr);
  Epoch = now;
}
void getEpoch()
{
  now = time(nullptr);
  Epoch = now;
}

void inputTime()
{
    if (Serial.available() > 0)
    {
      // read in the user input
      Day = Serial.parseInt();
      Month = Serial.parseInt();
      Year = Serial.parseInt();
      Hour = Serial.parseInt();
      Minute = Serial.parseInt();
      Second = Serial.parseInt();
      boolean validDate = (inRange(Day, 1, 31) && inRange(Month, 1, 12) && inRange(Year, 2021, 2031));
      boolean validTime = (inRange(Hour, 0, 23) && inRange(Minute, 0, 59) && inRange(Second, 0, 59));
      if (validTime && validDate)
      {
        configTime(0, 0, "pool.ntp.org");    // Repair timezone
        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 3);
        tzset();        
        struct tm t;                         //Prepare time strucure
        time_t t_of_day;
        t.tm_year = Year - 1900; // Year - 1900
        t.tm_mon = Month - 1;     // Month, where 0 = jan
        t.tm_mday = Day ;      // Day of the month
        t.tm_hour = Hour;
        t.tm_min = Minute;
        t.tm_sec = Second;
        t.tm_isdst = -1;         // Is DST on? 1 = yes, 0 = no, -1 = unknown
        t_of_day = mktime(&t);
        struct timeval tv;                   //Extending to mandatory microseconds
        tv.tv_sec = t_of_day;  // epoch time (seconds)
        tv.tv_usec = 0;    // microseconds
        settimeofday(&tv, 0);                //Setting Clock
      }
    }
}

void getTimeData()
{
  timeinfo  = localtime(&now);  // cf: https://www.cplusplus.com/reference/ctime/localtime/
  Second    = timeinfo->tm_sec;
  Minute    = timeinfo->tm_min;
  Hour      = timeinfo->tm_hour;
  Weekday   = timeinfo->tm_wday + 1 ;
  Day       = timeinfo->tm_mday;
  Month     = timeinfo->tm_mon + 1;
  Year      = timeinfo->tm_year + 1900; //returns years since 1900
  strftime (DayName , 12, "%A", timeinfo); //cf: https://www.cplusplus.com/reference/ctime/strftime/
  strftime (MonthName, 12, "%B", timeinfo);
  strftime (Time,10, "%T", timeinfo);
  strftime (Date,12, "%d/%m/%Y", timeinfo);
}

void disConnect()
{
  //  WiFi.disconnect(); //temporarily disconnect WiFi as it's no longer needed
  WiFi.mode(WIFI_OFF);
}

void reConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
    //    Serial.print(".");
  }
}


void setup()
{
  Serial.begin(115200);
  
  getWiFi();
  getNTP();
  getEpoch();            // writes the Epoch (Numbers of seconds till 1.1.1970...
  getTimeData();         // breaks down the Epoch into discrete values.
  
#if defined (OFFLINE)
  delay(1000);
  disConnect();
  Serial.println (F("Sketch is now running offline with own time"));
#endif
}

void loop()
{
  getEpoch();            // writes the Epoch (Numbers of seconds till 1.1.1970...
  getTimeData();         // breaks down the Epoch into discrete values.

  // Examples with Strings
  Serial.print(F("Thanks God, it is: "));   Serial.print( DayName );
  Serial.print(F("! Date is "));   Serial.print( Date );
  Serial.print(F(" and Time is "));   Serial.println( Time );

  // Examples with a character buffer (can be used for displays as well)
  sprintf(charbuff, "Now is %02d Hour, %02d Minutes and %02d Seconds. The Epoch is: %10lu" , Hour , Minute, Second, Epoch);
  Serial.println(charbuff);
  sprintf(charbuff, "Date is %s, %02d %s %04d ", DayName , Day , MonthName, Year);
  Serial.print(charbuff);
  Serial.println();

  // Examples with Posix expressions
  strftime (charbuff, 80, "Americans say it is %I:%M%p, but Europeans prefer using %T", timeinfo);
  Serial.println(charbuff);
  Serial.println();
  Serial.println();
  delay(1000);
}
