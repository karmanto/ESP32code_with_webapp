#include <MatrixHardware_ESP32_V0.h>
#include <SmartMatrix.h>

#include <Wire.h>
#include <RtcDS3231.h>
#include <PrayerTimes.h>
#include <DFPlayerMini_Fast.h>

#include "SPIFFS.h"
//#include "FS.h"
//#include <FS.h>

#include <HijriCalendar.h>

//#include <ESP8266WiFi.h>
#include <WiFi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include "firstPage.h"
#include "textFile.h"
#include "generalSettingPage.h"
#include "alarmSettingPage.h"
#include "timeSettingPage.h"
#include "sahurSettingPage.h"
#include "autoMurottal1page.h"
#include "autoMurottal2page.h"
#include "autoMurottal3page.h"
#include "wifiSettingPage.h"
#include "alarmAckPage.h"
#include "lisensi.h"
#include "iqamahSettingPage.h"
#include "changeTimeTrial.h"
#include "styleCSS.h"
#include "styleCSS2.h"
#include "styleCSS3.h"
//#include "testPage.h"

#define COLOR_DEPTH 24
const uint16_t kMatrixWidth = 64;
const uint16_t kMatrixHeight = 32;
const uint8_t kRefreshDepth = 36;
const uint8_t kDmaBufferRows = 4;
const uint8_t kPanelType = SM_PANELTYPE_HUB75_32ROW_MOD16SCAN;
const uint32_t kMatrixOptions = (SM_HUB75_OPTIONS_NONE);
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

DFPlayerMini_Fast myDFPlayer;
void printDetail(uint8_t type, int value);

String APssid = "WEMDIGITAL";
String APpassword = "WEMDIGITAL";
String http_username = "WEMDIGITAL";
String http_password = "WEMDIGITAL";
const char* PARAM_INPUT_1 = "state";

AsyncWebServer server(80);
RtcDS3231<TwoWire> Rtc(Wire);

const int mp3Inp = 34; //led1 connected to D0
const int alarmAckBtn = 35;
bool LEDstat = false;
bool alarmSahur = false;
bool alarmTahajjud = false;
bool alarmWOmurottal = false;
bool masukWaktuSholat = false;
bool menjelangWaktuSholat = false;
bool alarmWithmurottal = false;
byte sahurMomentStat[10] = {0,0,0,0,0,0,0,0,0,0};
byte tahajjudMomentStat = 0;
byte WOmurottalMomentStat[2] = {0,0};
byte masukWaktuSholatStat[8] = {0,0,0,0,0,0,0,0};
byte menjelangWaktuSholatStat[5] = {0,0,0,0,0};
byte WithmurottalMomentStat[3] = {0,0,0};
byte count1 = 0; //count display pray time and datetime
long count2 = 0; //count to get RTC info
long count3 = 0; 
long count4 = 0;
long count5 = 0;
int count6 = 0;
const long changeDisp = 1000;
int brightness = 1;
int volume = 1;
int volume2 = 1;
long waktuPemutaranMurottal;
long waktuPemutaranMurottal1;
long waktuPemutaranMurottal2;
long waktuPemutaranMurottal3;
byte statInfoMurottalSpeech = 0;
byte statInfoMurottalSpeech1 = 0;
byte statInfoMurottalSpeech2 = 0;
byte statInfoMurottalSpeech3 = 0;
int countMp3Failed1 = 0;
int countMp3Failed1_1 = 0;
int countMp3Failed1_2 = 0;
int countMp3Failed1_3 = 0;
const int countMp3Failed = 30;
bool murottalState = false;
bool murottalState1 = false;
bool murottalState2 = false;
bool murottalState3 = false;
long milisRec;
long milisRec2;
long milisRec3;
long milisRec4;
int hours, minutes;

unsigned long currentMillis;
unsigned long currentMillis2;
const rgb24 defaultBackgroundColor = {0, 0, 0};
char charHour[3];
char charMinute[3];
char charSecond[3];
int intWeekDay;
char charDay[3];
int intMonth;
char charYear[3];
char charDayH[3];
int intMonthH;
char charYearH[3];
int cache1 = 0;
int fallRainIntensity = 5;  //makin kecil makin banyak
int rainIntensity = 5;  //makin kecil makin banyak
int starCount = 5;    //makin kecil makin banyak
int runningLineCount = 5;    //makin kecil makin banyak
int flagAnim = 0;  //makin kecil makin cepat
int gap = 0;

int tomorrow[] = {0,0,0};
int prayTimeSav[8][2];
int fileFolderMurottal[4];
int fileFolderMurottal1[4];
int fileFolderMurottal2[4];
int fileFolderMurottal3[4];
double waktu[sizeof(TimeName) / sizeof(char*)];
const byte timingDisp[] = {4,8,12,16,20,24,28,32,36,40,44};
const char weekDay[][7] = {"AHAD", "SENIN", "SELASA", "RABU", "KAMIS", "JUM'AT", "SABTU", "AHAD"};
const char monthYear[][10] = { "DES.", "JAN.", "FEB.", "MAR.", "APR.", "MEI", "JUN.", "JUL.", "AGT.", "SEP.", "OKT.", "NOV.", "DES." };
const char monthYearH[][10] = { "ZULH.", "MUH.", "SFR.", "R.AWL", "R.AHR", "J.AWL", "J.AHR", "RJB.", "SBN.", "RAM.", "SWL", "ZULQ.", "ZULH."};

const String waktuSholatString[8] = {"IMSAK|","SUBUH|","SYURUK|","DHUHA|","ZUHUR|","ASAR|","MAGRIB|","ISYA|"};
const String waktuSholatString1[8] = {"IMS|","SBH|","SRK|","DHA|","ZHR|","ASR|","MRB|","ISY|"};

int playPropertiesMurottal[8];
int playPropertiesMurottal1[8];
int playPropertiesMurottal2[8];
int playPropertiesMurottal3[8];
int murottalRecord1[3];
int murottalRecord2[3];
int murottalRecord3[3];

int digitStat = 0;
byte digitArr[8];
byte digitArr2[8];
bool digitArr2Decode = true;

bool spiffsStat = false;

uint32_t serialNumber;
String serialNumberString;
String lisensiSaveString;
String lisensiString;   //reverse serial number, add with 14021991 %10
String lisensiStatString = "uji coba";
int limitTrial;
bool trialStat = false;
int firstRun = 0;

String testData = "";
long microSave = 0;
int testDataTol = 250;
int testDataStat = 0;
int testDataBuff[100];
bool testDataChange = true;
long testDataResult;
byte dispStat[2] = {1,0}; //murottal stat (0 no murottall - 1 with murottal) --- display stat (0 all in - 1 only one jadwal - 2 no jadwal)
bool updateStat = false;
int showVolume2Disp = 0;
char charVolume2[7];

char runPrayTime[9][16];
char runPrayTime1[9][16];
                               
int murottalPlayingStat[] = {
                            0,  // 0 control playing
                            0,  // 1 status initializing
                            0,  // 2 status playing --> 0 no activity --> 1 command to play --> 2 playing --> 3 finish
                            0,  // 3 status repeating
                            0,  // 4 current playing folder
                            0,  // 5 current playing file
                            0,   // 6 current repeat times
                            0,  // 7 current playing folder for playing to dfplayer
                            0   // 8 current playing file for playing to dfplayer
                            };

int murottalPlayingStat1[] = {
                            0,  // 0 control playing
                            0,  // 1 status initializing
                            0,  // 2 status playing --> 0 no activity --> 1 command to play --> 2 playing --> 3 finish
                            0,  // 3 status repeating
                            0,  // 4 current playing folder
                            0,  // 5 current playing file
                            0,   // 6 current repeat times
                            0,  // 7 current playing folder for playing to dfplayer
                            0   // 8 current playing file for playing to dfplayer
                            };

int murottalPlayingStat2[] = {
                            0,  // 0 control playing
                            0,  // 1 status initializing
                            0,  // 2 status playing --> 0 no activity --> 1 command to play --> 2 playing --> 3 finish
                            0,  // 3 status repeating
                            0,  // 4 current playing folder
                            0,  // 5 current playing file
                            0,   // 6 current repeat times
                            0,  // 7 current playing folder for playing to dfplayer
                            0   // 8 current playing file for playing to dfplayer
                            };

int murottalPlayingStat3[] = {
                            0,  // 0 control playing
                            0,  // 1 status initializing
                            0,  // 2 status playing --> 0 no activity --> 1 command to play --> 2 playing --> 3 finish
                            0,  // 3 status repeating
                            0,  // 4 current playing folder
                            0,  // 5 current playing file
                            0,   // 6 current repeat times
                            0,  // 7 current playing folder for playing to dfplayer
                            0   // 8 current playing file for playing to dfplayer
                            };

struct TimeNow {
  int secondNow;
  int minuteNow;
  int hourNow;
  int dayNow;
  int dowNow;
  int monthNow;
  int yearNow;
  int dayH;
  int monthH;
  int yearH;
};

struct TimeMin {
  int Now;
  int Imsak;
  int Subuh;
  int Syuruk;
  int Dhuha;
  int Zuhur;
  int Asar;
  int Magrib;
  int Isya;
};

struct TimeAlarm {  
  int sahur; //waktu jelang imsak
  int subuh; //waktu jelang subuh
  int zuhur; //waktu jelang zuhur
  int asar; //waktu jelang asar
  int magrib; //waktu jelang magrib
  int isya; //waktu jelang isya
  int tahajjud[2]; //fix
  int custom1[2]; //fix
  int custom2[2]; //fix
  int custom3[2]; //fix
  int custom4[2]; //fix
  int custom5[2]; //fix
  bool adzan;
  bool countdown;
};

struct TimeIqamah {  
  int subuh; //waktu iqamah subuh
  int zuhur; //waktu iqamah zuhur
  int asar; //waktu iqamah asar
  int magrib; //waktu iqamah magrib
  int isya; //waktu iqamah isya
};

struct AlarmWithMurottal {
  int subuh;
  int zuhur;
  int asar;
  int magrib;
  int isya;
  int custom1; //disable
  int custom2; //disable
  int custom3;
  int custom4;
  int custom5;
};

struct SahurStat { 
  bool sahurSyawal;
  bool sahurZulhijah;
  bool sahurRamadan;
  bool sahurAsyuro;
  bool sahurAyamulbidh;
  bool sahurSenin;
  bool sahurKamis;
  bool sahurDaud;
  bool sahurTasua;
  bool sahurArafah;
  int awalSyawal;
};

struct DataWS {
  float _lat;    // lintang
  float _lng;   // bujur
  int koordinatNum;
  int _timezone1;        //  WIB=7, WITA=8, WIT=9
  int koreksi_imsak;
  int koreksi_subuh;
  int koreksi_terbit;
  int koreksi_dhuha;
  int koreksi_zuhur;
  int koreksi_ashar;
  int koreksi_terbenam;
  int koreksi_magrib;
  int koreksi_isya;
  int _sudut_subuh;
  int _sudut_isyak;
  int koreksi_hijri;
};

TimeNow timeNow;
TimeMin timeMin;
TimeAlarm timeAlarm;
SahurStat sahurStat;
DataWS dataWS;
AlarmWithMurottal alarmWithMurottal;
TimeIqamah timeIqamah;

void cek_menit() {
  if (minutes == 60) {
    minutes = 0;
    hours = hours + 1;
  }
  if (minutes <= -1) {
    minutes = 60 - (-minutes);
    hours = hours - 1;
  }
  if (minutes >= 61) {
    minutes = minutes - 60;
    hours = hours + 1;
  }
}

void IRAM_ATTR handleInterrupt1() {
  long a = micros()-microSave;
  microSave = micros();

  if(testDataChange){
    if(a>8500 & a<9500){
      testDataBuff[0] = a;
      testDataStat = 1;
    }
    else if((a>4000 & a<5000) & testDataStat==1){
      testDataBuff[1] = a;
      testDataStat = 2;
    }
    else if(testDataStat>=2 & testDataStat<100){
      testDataBuff[testDataStat] = a;
      testDataStat++;
    }
  }
}

void getfileFolderMurottal(int x1, int y1, int x2, int y2, int z){
  spiffsStat = true;
  if(z<=1){
    int a;
    int b;
    int c;
    String Line;
    File file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {
      a = 1;
      c = 0;
      while(a<x1){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y1;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal[0] = a + z*25;
      fileFolderMurottal[1] = b;
      
      file.close();
    }
  
    file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {  
      a = 1;
      c = 0;
      while(a<x2){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y2;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal[2] = a + z*25;
      fileFolderMurottal[3] = b;
      file.close();
    }
  }
  else if(z>1 & z<=10){
    fileFolderMurottal[0] = z + 50;
    fileFolderMurottal[1] = x1;
    fileFolderMurottal[2] = z + 50;
    fileFolderMurottal[3] = x2;
    playPropertiesMurottal[4] = 0;    
  }
  else if(z==11){
    fileFolderMurottal[0] = 1;
    fileFolderMurottal[1] = 43;
    fileFolderMurottal[2] = 1;
    fileFolderMurottal[3] = 43;  
    playPropertiesMurottal[4] = 0;  
  }
  else if(z==12){
    fileFolderMurottal[0] = 1;
    fileFolderMurottal[1] = 41;
    fileFolderMurottal[2] = 1;
    fileFolderMurottal[3] = 41; 
    playPropertiesMurottal[4] = 0;   
  }
  else if(z==13){
    fileFolderMurottal[0] = 1;
    fileFolderMurottal[1] = 42;
    fileFolderMurottal[2] = 1;
    fileFolderMurottal[3] = 42;
    playPropertiesMurottal[4] = 0;    
  }
  spiffsStat = false;
}

void getfileFolderMurottal1(int x1, int y1, int x2, int y2, int z){
  spiffsStat = true;
  if(z<=1){
    int a;
    int b;
    int c;
    String Line;
    File file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {
      a = 1;
      c = 0;
      while(a<x1){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y1;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal1[0] = a + z*25;
      fileFolderMurottal1[1] = b;
      
      file.close();
    }
  
    file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {  
      a = 1;
      c = 0;
      while(a<x2){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y2;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal1[2] = a + z*25;
      fileFolderMurottal1[3] = b;
      file.close();
    }
  }
  else if(z>1 & z<=10){
    fileFolderMurottal1[0] = z + 50;
    fileFolderMurottal1[1] = x1;
    fileFolderMurottal1[2] = z + 50;
    fileFolderMurottal1[3] = x2;
    playPropertiesMurottal1[4] = 0;    
  }
  else if(z==11){
    fileFolderMurottal1[0] = 1;
    fileFolderMurottal1[1] = 43;
    fileFolderMurottal1[2] = 1;
    fileFolderMurottal1[3] = 43;  
    playPropertiesMurottal1[4] = 0;  
  }
  else if(z==12){
    fileFolderMurottal1[0] = 1;
    fileFolderMurottal1[1] = 41;
    fileFolderMurottal1[2] = 1;
    fileFolderMurottal1[3] = 41; 
    playPropertiesMurottal1[4] = 0;   
  }
  else if(z==13){
    fileFolderMurottal1[0] = 1;
    fileFolderMurottal1[1] = 42;
    fileFolderMurottal1[2] = 1;
    fileFolderMurottal1[3] = 42;
    playPropertiesMurottal1[4] = 0;    
  }
  spiffsStat = false;
}

void getfileFolderMurottal2(int x1, int y1, int x2, int y2, int z){
  spiffsStat = true;
  if(z<=1){
    int a;
    int b;
    int c;
    String Line;
    File file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {
      a = 1;
      c = 0;
      while(a<x1){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y1;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal2[0] = a + z*25;
      fileFolderMurottal2[1] = b;
      
      file.close();
    }
  
    file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {  
      a = 1;
      c = 0;
      while(a<x2){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y2;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal2[2] = a + z*25;
      fileFolderMurottal2[3] = b;
      file.close();
    }
  }
  else if(z>1 & z<=10){
    fileFolderMurottal2[0] = z + 50;
    fileFolderMurottal2[1] = x1;
    fileFolderMurottal2[2] = z + 50;
    fileFolderMurottal2[3] = x2;
    playPropertiesMurottal2[4] = 0;    
  }
  else if(z==11){
    fileFolderMurottal2[0] = 1;
    fileFolderMurottal2[1] = 43;
    fileFolderMurottal2[2] = 1;
    fileFolderMurottal2[3] = 43;  
    playPropertiesMurottal2[4] = 0;  
  }
  else if(z==12){
    fileFolderMurottal2[0] = 1;
    fileFolderMurottal2[1] = 41;
    fileFolderMurottal2[2] = 1;
    fileFolderMurottal2[3] = 41; 
    playPropertiesMurottal2[4] = 0;   
  }
  else if(z==13){
    fileFolderMurottal2[0] = 1;
    fileFolderMurottal2[1] = 42;
    fileFolderMurottal2[2] = 1;
    fileFolderMurottal2[3] = 42;
    playPropertiesMurottal2[4] = 0;    
  }
  spiffsStat = false;
}

void getfileFolderMurottal3(int x1, int y1, int x2, int y2, int z){
  spiffsStat = true;
  if(z<=1){
    int a;
    int b;
    int c;
    String Line;
    File file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {
      a = 1;
      c = 0;
      while(a<x1){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y1;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal3[0] = a + z*25;
      fileFolderMurottal3[1] = b;
      
      file.close();
    }
  
    file = SPIFFS.open("/jumlah-ayat.txt", "r");
    if (file.available()) {  
      a = 1;
      c = 0;
      while(a<x2){
        Line = file.readStringUntil('\n');
        a++;
        b = Line.toInt();
        c = c + b;
      }
      c = c + y2;
      a = (c / 255) + 1 + 1;
      b = c % 255;
      fileFolderMurottal3[2] = a + z*25;
      fileFolderMurottal3[3] = b;
      file.close();
    }
  }
  else if(z>1 & z<=10){
    fileFolderMurottal3[0] = z + 50;
    fileFolderMurottal3[1] = x1;
    fileFolderMurottal3[2] = z + 50;
    fileFolderMurottal3[3] = x2;
    playPropertiesMurottal3[4] = 0;    
  }
  else if(z==11){
    fileFolderMurottal3[0] = 1;
    fileFolderMurottal3[1] = 43;
    fileFolderMurottal3[2] = 1;
    fileFolderMurottal3[3] = 43;  
    playPropertiesMurottal3[4] = 0;  
  }
  else if(z==12){
    fileFolderMurottal3[0] = 1;
    fileFolderMurottal3[1] = 41;
    fileFolderMurottal3[2] = 1;
    fileFolderMurottal3[3] = 41; 
    playPropertiesMurottal3[4] = 0;   
  }
  else if(z==13){
    fileFolderMurottal3[0] = 1;
    fileFolderMurottal3[1] = 42;
    fileFolderMurottal3[2] = 1;
    fileFolderMurottal3[3] = 42;
    playPropertiesMurottal3[4] = 0;    
  }
  spiffsStat = false;
}

String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void getLisensiSave(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/lisensiSave.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    Line.replace("\n","");
    Line.replace("\r","");
    Line.replace(" ","");
    lisensiSaveString = Line;
  }
  file.close();
  spiffsStat = false;
}

void getWifiSetting(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/wifiPassword.txt", "r");
  if (file.available()) { 
    Line = file.readStringUntil('\n'); 
    Line.replace("\n","");
    Line.replace("\r","");
    Line.replace(" ","");
    APssid = Line;
    Line = file.readStringUntil('\n'); 
    Line.replace("\n","");
    Line.replace("\r","");
    Line.replace(" ","");
    APpassword = Line;
    Line = file.readStringUntil('\n');  
    Line.replace("\n","");
    Line.replace("\r","");
    Line.replace(" ","");
    http_username = Line;
    Line = file.readStringUntil('\n'); 
    Line.replace("\n","");
    Line.replace("\r","");
    Line.replace(" ","");
    http_password = Line;
  }
  file.close();
  spiffsStat = false;
}

void getAlarmWithMurottal(){
  spiffsStat = true;
  int a;
  String Line;
  File file = SPIFFS.open("/alarmWithMurottal.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    alarmWithMurottal.subuh = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.zuhur = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.asar = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.magrib = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.isya = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.custom1 = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.custom2 = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.custom3 = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.custom4 = Line.toInt();
    Line = file.readStringUntil('\n');
    alarmWithMurottal.custom5 = Line.toInt();
  }
  file.close();
  spiffsStat = false;
}

void getplayPropertiesMurottal(){
  spiffsStat = true;
  int a;
  String Line;
  File file = SPIFFS.open("/playPropertiesMurottal.txt", "r");
  if (file.available()) {
    for (a = 0; a<8; a++){
      Line = file.readStringUntil('\n');
      playPropertiesMurottal[a] = Line.toInt();
    }    
  }
  file.close();
  spiffsStat = false;
}

void getplayPropertiesMurottal1(bool b){
  spiffsStat = true;
  int a;
  String Line;
  File file = SPIFFS.open("/playPropertiesMurottal1.txt", "r");
  if (file.available()) {
    for (a = 0; a<8; a++){
      Line = file.readStringUntil('\n');
      playPropertiesMurottal1[a] = Line.toInt();
    }    
  }
  file.close();

  if(b==true){
    File file2 = SPIFFS.open("/Murottal1Record.txt", "r");
    if (file2.available()) {
      for (a = 0; a<3; a++){
        Line = file2.readStringUntil('\n');
        murottalRecord1[a] = Line.toInt();
      }    
    }
    file2.close();
  }
  spiffsStat = false;
}

void getplayPropertiesMurottal2(bool b){
  spiffsStat = true;
  int a;
  String Line;
  File file = SPIFFS.open("/playPropertiesMurottal2.txt", "r");
  if (file.available()) {
    for (a = 0; a<8; a++){
      Line = file.readStringUntil('\n');
      playPropertiesMurottal2[a] = Line.toInt();
    }    
  }
  file.close();

  if(b==true){
    File file2 = SPIFFS.open("/Murottal2Record.txt", "r");
    if (file2.available()) {
      for (a = 0; a<3; a++){
        Line = file2.readStringUntil('\n');
        murottalRecord2[a] = Line.toInt();
      }    
    }
    file2.close();
  }
  spiffsStat = false;
}

void getplayPropertiesMurottal3(bool b){
  spiffsStat = true;
  int a;
  String Line;
  File file = SPIFFS.open("/playPropertiesMurottal3.txt", "r");
  if (file.available()) {
    for (a = 0; a<8; a++){
      Line = file.readStringUntil('\n');
      playPropertiesMurottal3[a] = Line.toInt();
    }    
  }
  file.close();

  if(b==true){
    File file2 = SPIFFS.open("/Murottal3Record.txt", "r");
    if (file2.available()) {
      for (a = 0; a<3; a++){
        Line = file2.readStringUntil('\n');
        murottalRecord3[a] = Line.toInt();
      }    
    }
    file2.close();
  }
  spiffsStat = false;
}

void getBrightness(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/brightness.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    brightness = Line.toInt();
  }
  file.close();
  spiffsStat = false;
}

void getVolume(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/volume.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    volume = Line.toInt();
  }
  file.close();

  File file2 = SPIFFS.open("/volume2.txt", "r");
  if (file2.available()) {    
    Line = file2.readStringUntil('\n');
    volume2 = Line.toInt();
  }
  file2.close();
  spiffsStat = false;
}

void getKoordinat(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/koordinat.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    dataWS.koordinatNum = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS._timezone1 = Line.toInt();
  }
  file.close();

  File file2 = SPIFFS.open("/databaseKoordinat.txt", "r");
  if (file2.available()) {
    for(int b=0; b<=dataWS.koordinatNum; b++){
      Line = file2.readStringUntil('\n');
    }
    dataWS._lat = getValue(Line, ',', 1).toFloat();
    dataWS._lng = getValue(Line, ',', 2).toFloat();
  }
  file2.close();
  spiffsStat = false;
}

void getKoreksi(){
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/koreksiWaktuSholat.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    dataWS.koreksi_imsak = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_subuh = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_terbit = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_dhuha = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_zuhur = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_ashar = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_terbenam = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_magrib = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_isya = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS._sudut_subuh = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS._sudut_isyak = Line.toInt();
    Line = file.readStringUntil('\n');
    dataWS.koreksi_hijri = Line.toInt();
  }
  file.close();
  spiffsStat = false;
}

void getTimeAlarm(){
  spiffsStat = true;
  String Line;
  int a;
  File file = SPIFFS.open("/timeAlarm.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    timeAlarm.sahur = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.subuh = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.zuhur = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.asar = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.magrib = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.isya = Line.toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.tahajjud[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.tahajjud[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.custom1[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.custom1[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.custom2[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.custom2[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.custom3[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.custom3[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.custom4[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.custom4[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    timeAlarm.custom5[0] = getValue(Line, ':', 0).toInt();
    timeAlarm.custom5[1] = getValue(Line, ':', 1).toInt();
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    timeAlarm.adzan = false;
    if(a==1){
      timeAlarm.adzan = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    timeAlarm.countdown = false;
    if(a==1){
      timeAlarm.countdown = true;
    }
  }
  file.close();
  spiffsStat = false;
}

void getTimeIqamah(){
  String Line;
  int a;
  File file = SPIFFS.open("/timeIqamah.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    timeIqamah.subuh = Line.toInt();
    Line = file.readStringUntil('\n');
    timeIqamah.zuhur = Line.toInt();
    Line = file.readStringUntil('\n');
    timeIqamah.asar = Line.toInt();
    Line = file.readStringUntil('\n');
    timeIqamah.magrib = Line.toInt();
    Line = file.readStringUntil('\n');
    timeIqamah.isya = Line.toInt();
  }
  file.close();
}

void getSahurStat(){ 
  spiffsStat = true;
  String Line;
  int a;
  File file = SPIFFS.open("/sahurStat.txt", "r");
  if (file.available()) {
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurSyawal = false;
    if(a==1){
      sahurStat.sahurSyawal = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurZulhijah = false;
    if(a==1){
      sahurStat.sahurZulhijah = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurRamadan = false;
    if(a==1){
      sahurStat.sahurRamadan = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurAsyuro = false;
    if(a==1){
      sahurStat.sahurAsyuro = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurAyamulbidh = false;
    if(a==1){
      sahurStat.sahurAyamulbidh = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurSenin = false;
    if(a==1){
      sahurStat.sahurSenin = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurKamis = false;
    if(a==1){
      sahurStat.sahurKamis = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurDaud = false;
    if(a==1){
      sahurStat.sahurDaud = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurTasua = false;
    if(a==1){
      sahurStat.sahurTasua = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.sahurArafah = false;
    if(a==1){
      sahurStat.sahurArafah = true;
    }
    Line = file.readStringUntil('\n');
    a = Line.toInt();
    sahurStat.awalSyawal = a;
    
  }
  file.close();
  spiffsStat = false;
}

void murottalPlayingFunc(){
  if(murottalPlayingStat[0]==1 & (waktuPemutaranMurottal+(playPropertiesMurottal[7]*60000))>millis()){            //control playing ON
    if(murottalPlayingStat[1]==0){          //initializing not yet
      murottalPlayingStat[1]=1;                       
      if(playPropertiesMurottal[4]==0){               //check repeating mode on playPropertiesMorottal
        murottalPlayingStat[3]=0;
      }
      else{                                           //check repeating mode on playPropertiesMorottal
        murottalPlayingStat[3]=1;
      }
      murottalPlayingStat[2]=1;                       //change status playing to --> command to play
      murottalPlayingStat[4]=fileFolderMurottal[0];
      murottalPlayingStat[5]=fileFolderMurottal[1];
      murottalPlayingStat[7]=murottalPlayingStat[4];
      murottalPlayingStat[8]=murottalPlayingStat[5];
      murottalPlayingStat[6]=0;
      myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
      countMp3Failed1 = 0;
    }
    else{
      if(murottalPlayingStat[2]==3){   //jika sudah selesai diputar
        if(playPropertiesMurottal[4]==0){  //metode repeat biasa
          murottalPlayingStat[5]++;
          if(murottalPlayingStat[5]>255){
            murottalPlayingStat[5]=1;
            murottalPlayingStat[4]++;
          }
          if(murottalPlayingStat[4]>fileFolderMurottal[2]){
            murottalPlayingStat[4]=fileFolderMurottal[0];
            murottalPlayingStat[5]=fileFolderMurottal[1];
            murottalPlayingStat[7]=murottalPlayingStat[4];
            murottalPlayingStat[8]=murottalPlayingStat[5];
            murottalPlayingStat[2]=1;        
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }
          else if(murottalPlayingStat[4]==fileFolderMurottal[2]){
            if(murottalPlayingStat[5]>fileFolderMurottal[3]){
              murottalPlayingStat[4]=fileFolderMurottal[0];
              murottalPlayingStat[5]=fileFolderMurottal[1];
              murottalPlayingStat[7]=murottalPlayingStat[4];
              murottalPlayingStat[8]=murottalPlayingStat[5];
              murottalPlayingStat[2]=1;        
              myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
              countMp3Failed1 = 0;
            }
            else{
              murottalPlayingStat[2]=1;
              murottalPlayingStat[7]=murottalPlayingStat[4];
              murottalPlayingStat[8]=murottalPlayingStat[5];
              myDFPlayer.playNext();
              countMp3Failed1 = 0;
            }
          }
          else{
            murottalPlayingStat[2]=1;
            murottalPlayingStat[7]=murottalPlayingStat[4];
            murottalPlayingStat[8]=murottalPlayingStat[5];
            myDFPlayer.playNext();
            countMp3Failed1 = 0;
          }
        }
        else{                              //metode repeat tikrar
          if(murottalPlayingStat[3]==1){  //current mode repeat one
            murottalPlayingStat[6]++;
            if(murottalPlayingStat[6]>=playPropertiesMurottal[5]){ //last play
              //-------------------
              if(murottalPlayingStat[4]==fileFolderMurottal[0] & murottalPlayingStat[5]==fileFolderMurottal[1]){
                murottalPlayingStat[5]++;
                if(murottalPlayingStat[5]>255){
                  murottalPlayingStat[5]=1;
                  murottalPlayingStat[4]++;
                }
                if(murottalPlayingStat[4]>fileFolderMurottal[2]){
                  //change current mode to repeat one
                  murottalPlayingStat[4]=fileFolderMurottal[0];
                  murottalPlayingStat[5]=fileFolderMurottal[1];
                  murottalPlayingStat[3]=1;
                  murottalPlayingStat[6]=0;  
                  murottalPlayingStat[7]=murottalPlayingStat[4];
                  murottalPlayingStat[8]=murottalPlayingStat[5];
                  murottalPlayingStat[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                  countMp3Failed1 = 0;      
                }
                else if(murottalPlayingStat[4]==fileFolderMurottal[2]){
                  if(murottalPlayingStat[5]>fileFolderMurottal[3]){
                    //change current mode to repeat one
                    murottalPlayingStat[4]=fileFolderMurottal[0];
                    murottalPlayingStat[5]=fileFolderMurottal[1];
                    murottalPlayingStat[3]=1;
                    murottalPlayingStat[6]=0;
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    murottalPlayingStat[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                  else{
                    //change current mode to repeat one
                    murottalPlayingStat[3]=1;
                    murottalPlayingStat[6]=0;
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    murottalPlayingStat[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                }
                else{
                  //change current mode to repeat one
                  murottalPlayingStat[3]=1;
                  murottalPlayingStat[6]=0;
                  murottalPlayingStat[7]=murottalPlayingStat[4];
                  murottalPlayingStat[8]=murottalPlayingStat[5];
                  murottalPlayingStat[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                  countMp3Failed1 = 0;
                }
              }
              else{
                //change current mode to repeat all
                murottalPlayingStat[6]=0;
                murottalPlayingStat[3]=0;
                murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat[7]=fileFolderMurottal[0];
                murottalPlayingStat[8]=fileFolderMurottal[1];
                myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                countMp3Failed1 = 0;
              }
            }
            else{
              murottalPlayingStat[2]=1;
              myDFPlayer.resume();
              countMp3Failed1 = 0;
            }
          }
          else{                           //current mode repeat all
            murottalPlayingStat[8]++;
            if(murottalPlayingStat[8]>255){
              murottalPlayingStat[8]=1;
              murottalPlayingStat[7]++;
            }
            if(murottalPlayingStat[7]>murottalPlayingStat[4]){
              murottalPlayingStat[6]++;
              if(murottalPlayingStat[6]>=playPropertiesMurottal[5]){
                murottalPlayingStat[3]=1;
                murottalPlayingStat[6]=0;
                murottalPlayingStat[5]++;
                if(murottalPlayingStat[5]>255){
                  murottalPlayingStat[5]=1;
                   murottalPlayingStat[4]++;
                }
                if(murottalPlayingStat[4]>fileFolderMurottal[2]){
                  murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat[4]=fileFolderMurottal[0];
                  murottalPlayingStat[5]=fileFolderMurottal[1];
                  murottalPlayingStat[7]=murottalPlayingStat[4];
                  murottalPlayingStat[8]=murottalPlayingStat[5];
                  myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                  countMp3Failed1 = 0;
                }
                else if(murottalPlayingStat[4]==fileFolderMurottal[2]){
                  if(murottalPlayingStat[5]>fileFolderMurottal[3]){
                    murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat[4]=fileFolderMurottal[0];
                    murottalPlayingStat[5]=fileFolderMurottal[1];
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                  else{
                    murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                }
                else{
                  murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat[7]=murottalPlayingStat[4];
                  murottalPlayingStat[8]=murottalPlayingStat[5];
                  myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                  countMp3Failed1 = 0;
                }
              }
              else{
                murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat[7]=fileFolderMurottal[0];
                murottalPlayingStat[8]=fileFolderMurottal[1];
                myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                countMp3Failed1 = 0;
              }
            }
            else if(murottalPlayingStat[7]==murottalPlayingStat[4]){
              if(murottalPlayingStat[8]>murottalPlayingStat[5]){
                murottalPlayingStat[6]++;
                if(murottalPlayingStat[6]>=playPropertiesMurottal[5]){
                  murottalPlayingStat[3]=1;
                  murottalPlayingStat[6]=0;
                  murottalPlayingStat[5]++;
                  if(murottalPlayingStat[5]>255){
                    murottalPlayingStat[5]=1;
                     murottalPlayingStat[4]++;
                  }
                  if(murottalPlayingStat[4]>fileFolderMurottal[2]){
                    murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat[4]=fileFolderMurottal[0];
                    murottalPlayingStat[5]=fileFolderMurottal[1];
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                  else if(murottalPlayingStat[4]==fileFolderMurottal[2]){
                    if(murottalPlayingStat[5]>fileFolderMurottal[3]){
                      murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat[4]=fileFolderMurottal[0];
                      murottalPlayingStat[5]=fileFolderMurottal[1];
                      murottalPlayingStat[7]=murottalPlayingStat[4];
                      murottalPlayingStat[8]=murottalPlayingStat[5];
                      myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                      countMp3Failed1 = 0;
                    }
                    else{
                      murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat[7]=murottalPlayingStat[4];
                      murottalPlayingStat[8]=murottalPlayingStat[5];
                      myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                      countMp3Failed1 = 0;
                    }
                  }
                  else{
                    murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat[7]=murottalPlayingStat[4];
                    murottalPlayingStat[8]=murottalPlayingStat[5];
                    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                    countMp3Failed1 = 0;
                  }
                }
                else{
                  murottalPlayingStat[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat[7]=fileFolderMurottal[0];
                  murottalPlayingStat[8]=fileFolderMurottal[1];
                  myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
                  countMp3Failed1 = 0;
                }
              }
              else{
                murottalPlayingStat[2]=1;
                myDFPlayer.playNext();
                countMp3Failed1 = 0;
              }
            }
            else{
              murottalPlayingStat[2]=1;
              myDFPlayer.playNext();
              countMp3Failed1 = 0;
            }
          }
        }
      }
    }
  }
  else if(murottalPlayingStat[0]==1 & (waktuPemutaranMurottal+(playPropertiesMurottal[7]*60000))<=millis()){     //timeout
    murottalState=false;
    statInfoMurottalSpeech==3;
  }
}

void checkMurottalPlayingStat(){
  if(murottalPlayingStat[2]==1 & !digitalRead(mp3Inp)){
    murottalPlayingStat[2]=2; //change status playing to --> playing
  }
  else if(murottalPlayingStat[2]==2 & digitalRead(mp3Inp)){
    murottalPlayingStat[2]=3; //change status playing to --> finish
  }

  if(murottalPlayingStat[2]==1){        //check apakah mp3 sudah diputar atau belum, kalau belum perintahkan putar lagi
    countMp3Failed1++;
    if(countMp3Failed1>=countMp3Failed){
      myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
      countMp3Failed1 = 0;
    }
  }
}

void memutarMurottalSpeech(){
  if(murottalState==true & statInfoMurottalSpeech==0){
    myDFPlayer.pause();
    statInfoMurottalSpeech = 1;
    waktuPemutaranMurottal = millis();
  }
  else if(murottalState==false & statInfoMurottalSpeech==3){
    myDFPlayer.pause();
    statInfoMurottalSpeech = 4; 
  } 

  if(statInfoMurottalSpeech==1 & digitalRead(mp3Inp)){
    murottalPlayingStat[2]=1;        
    murottalPlayingStat[7] = 1;
    murottalPlayingStat[8] = 13;
    myDFPlayer.volume(volume2);
    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
    countMp3Failed1 = 0;
    statInfoMurottalSpeech = 2;
  }
  else if(statInfoMurottalSpeech==2 & murottalPlayingStat[2]==3){
    murottalPlayingStat[2]=0;        
    statInfoMurottalSpeech = 3;
    murottalPlayingStat[0] = 1;
  }
  else if(statInfoMurottalSpeech==4 & digitalRead(mp3Inp)){
    murottalPlayingStat[2]=1;        
    murottalPlayingStat[7] = 1;
    murottalPlayingStat[8] = 14;
    myDFPlayer.volume(volume2);
    myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
    countMp3Failed1 = 0;
    statInfoMurottalSpeech = 5;
  }
  else if(statInfoMurottalSpeech==5 & murottalPlayingStat[2]==3){
    murottalPlayingStat[2]=0;        
    statInfoMurottalSpeech = 0;
    murottalPlayingStat[0] = 0;
    murottalPlayingStat[1]=0;
    myDFPlayer.pause();
  }
}

void murottalPlayingFunc1(){
  if(murottalPlayingStat1[0]==1 & (waktuPemutaranMurottal1+(playPropertiesMurottal1[7]*60000))>millis()){            //control playing ON
    if(murottalPlayingStat1[1]==0){          //initializing not yet
      murottalPlayingStat1[1]=1;                       
      if(playPropertiesMurottal1[4]==0){               //check repeating mode on playPropertiesMorottal
        murottalPlayingStat1[3]=0;
      }
      else{                                           //check repeating mode on playPropertiesMorottal
        murottalPlayingStat1[3]=1;
      }
      murottalPlayingStat1[2]=1;                       //change status playing to --> command to play

      if(playPropertiesMurottal1[4]==0 & murottalRecord1[0]==playPropertiesMurottal1[6] &
         (murottalRecord1[1]>fileFolderMurottal1[0] | 
         (murottalRecord1[1]==fileFolderMurottal1[0] & murottalRecord1[2]>=fileFolderMurottal1[1]))){
        murottalRecord1[2]++;
        if(murottalRecord1[2]>255){
          murottalRecord1[1]++;
          murottalRecord1[2] = 1;
        }
        if(murottalRecord1[1]<fileFolderMurottal1[2]){
          murottalPlayingStat1[4] = murottalRecord1[1];
          murottalPlayingStat1[5] = murottalRecord1[2];
        }
        else if(murottalRecord1[1]==fileFolderMurottal1[2] & murottalRecord1[2]<=fileFolderMurottal1[3]){
          murottalPlayingStat1[4] = murottalRecord1[1];
          murottalPlayingStat1[5] = murottalRecord1[2];
        }
        else{
          murottalPlayingStat1[4]=fileFolderMurottal1[0];
          murottalPlayingStat1[5]=fileFolderMurottal1[1];
        }
      }
      else{
        murottalPlayingStat1[4]=fileFolderMurottal1[0];
        murottalPlayingStat1[5]=fileFolderMurottal1[1];
      }
      
      murottalPlayingStat1[7]=murottalPlayingStat1[4];
      murottalPlayingStat1[8]=murottalPlayingStat1[5];
      murottalPlayingStat1[6]=0;
      myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
      countMp3Failed1_1 = 0;
    }
    else{
      if(murottalPlayingStat1[2]==3){   //jika sudah selesai diputar
        if(playPropertiesMurottal1[4]==0){  //metode repeat biasa
          murottalPlayingStat1[5]++;
          if(murottalPlayingStat1[5]>255){
            murottalPlayingStat1[5]=1;
            murottalPlayingStat1[4]++;
          }
          if(murottalPlayingStat1[4]>fileFolderMurottal1[2]){
            murottalPlayingStat1[4]=fileFolderMurottal1[0];
            murottalPlayingStat1[5]=fileFolderMurottal1[1];
            murottalPlayingStat1[7]=murottalPlayingStat1[4];
            murottalPlayingStat1[8]=murottalPlayingStat1[5];
            murottalPlayingStat1[2]=1;        
            myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
            countMp3Failed1_1 = 0;
          }
          else if(murottalPlayingStat1[4]==fileFolderMurottal1[2]){
            if(murottalPlayingStat1[5]>fileFolderMurottal1[3]){
              murottalPlayingStat1[4]=fileFolderMurottal1[0];
              murottalPlayingStat1[5]=fileFolderMurottal1[1];
              murottalPlayingStat1[7]=murottalPlayingStat1[4];
              murottalPlayingStat1[8]=murottalPlayingStat1[5];
              murottalPlayingStat1[2]=1;        
              myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
              countMp3Failed1_1 = 0;
            }
            else{
              murottalPlayingStat1[2]=1;
              murottalPlayingStat1[7]=murottalPlayingStat1[4];
              murottalPlayingStat1[8]=murottalPlayingStat1[5];
              myDFPlayer.playNext();
              countMp3Failed1_1 = 0;
            }
          }
          else{
            murottalPlayingStat1[2]=1;
            murottalPlayingStat1[7]=murottalPlayingStat1[4];
            murottalPlayingStat1[8]=murottalPlayingStat1[5];
            myDFPlayer.playNext();
            countMp3Failed1_1 = 0;
          }
        }
        else{                              //metode repeat tikrar
          if(murottalPlayingStat1[3]==1){  //current mode repeat one
            murottalPlayingStat1[6]++;
            if(murottalPlayingStat1[6]>=playPropertiesMurottal1[5]){ //last play
              //-------------------
              if(murottalPlayingStat1[4]==fileFolderMurottal1[0] & murottalPlayingStat1[5]==fileFolderMurottal1[1]){
                murottalPlayingStat1[5]++;
                if(murottalPlayingStat1[5]>255){
                  murottalPlayingStat1[5]=1;
                  murottalPlayingStat1[4]++;
                }
                if(murottalPlayingStat1[4]>fileFolderMurottal1[2]){
                  //change current mode to repeat one
                  murottalPlayingStat1[4]=fileFolderMurottal1[0];
                  murottalPlayingStat1[5]=fileFolderMurottal1[1];
                  murottalPlayingStat1[3]=1;
                  murottalPlayingStat1[6]=0;  
                  murottalPlayingStat1[7]=murottalPlayingStat1[4];
                  murottalPlayingStat1[8]=murottalPlayingStat1[5];
                  murottalPlayingStat1[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                  countMp3Failed1_1 = 0;      
                }
                else if(murottalPlayingStat1[4]==fileFolderMurottal1[2]){
                  if(murottalPlayingStat1[5]>fileFolderMurottal1[3]){
                    //change current mode to repeat one
                    murottalPlayingStat1[4]=fileFolderMurottal1[0];
                    murottalPlayingStat1[5]=fileFolderMurottal1[1];
                    murottalPlayingStat1[3]=1;
                    murottalPlayingStat1[6]=0;
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    murottalPlayingStat1[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                  else{
                    //change current mode to repeat one
                    murottalPlayingStat1[3]=1;
                    murottalPlayingStat1[6]=0;
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    murottalPlayingStat1[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                }
                else{
                  //change current mode to repeat one
                  murottalPlayingStat1[3]=1;
                  murottalPlayingStat1[6]=0;
                  murottalPlayingStat1[7]=murottalPlayingStat1[4];
                  murottalPlayingStat1[8]=murottalPlayingStat1[5];
                  murottalPlayingStat1[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                  countMp3Failed1_1 = 0;
                }
              }
              else{
                //change current mode to repeat all
                murottalPlayingStat1[6]=0;
                murottalPlayingStat1[3]=0;
                murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat1[7]=fileFolderMurottal1[0];
                murottalPlayingStat1[8]=fileFolderMurottal1[1];
                myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                countMp3Failed1_1 = 0;
              }
            }
            else{
              murottalPlayingStat1[2]=1;
              myDFPlayer.resume();
              countMp3Failed1_1 = 0;
            }
          }
          else{                           //current mode repeat all
            murottalPlayingStat1[8]++;
            if(murottalPlayingStat1[8]>255){
              murottalPlayingStat1[8]=1;
              murottalPlayingStat1[7]++;
            }
            if(murottalPlayingStat1[7]>murottalPlayingStat1[4]){
              murottalPlayingStat1[6]++;
              if(murottalPlayingStat1[6]>=playPropertiesMurottal1[5]){
                murottalPlayingStat1[3]=1;
                murottalPlayingStat1[6]=0;
                murottalPlayingStat1[5]++;
                if(murottalPlayingStat1[5]>255){
                  murottalPlayingStat1[5]=1;
                   murottalPlayingStat1[4]++;
                }
                if(murottalPlayingStat1[4]>fileFolderMurottal1[2]){
                  murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat1[4]=fileFolderMurottal1[0];
                  murottalPlayingStat1[5]=fileFolderMurottal1[1];
                  murottalPlayingStat1[7]=murottalPlayingStat1[4];
                  murottalPlayingStat1[8]=murottalPlayingStat1[5];
                  myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                  countMp3Failed1_1 = 0;
                }
                else if(murottalPlayingStat1[4]==fileFolderMurottal1[2]){
                  if(murottalPlayingStat1[5]>fileFolderMurottal1[3]){
                    murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat1[4]=fileFolderMurottal1[0];
                    murottalPlayingStat1[5]=fileFolderMurottal1[1];
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                  else{
                    murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                }
                else{
                  murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat1[7]=murottalPlayingStat1[4];
                  murottalPlayingStat1[8]=murottalPlayingStat1[5];
                  myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                  countMp3Failed1_1 = 0;
                }
              }
              else{
                murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat1[7]=fileFolderMurottal1[0];
                murottalPlayingStat1[8]=fileFolderMurottal1[1];
                myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                countMp3Failed1_1 = 0;
              }
            }
            else if(murottalPlayingStat1[7]==murottalPlayingStat1[4]){
              if(murottalPlayingStat1[8]>murottalPlayingStat1[5]){
                murottalPlayingStat1[6]++;
                if(murottalPlayingStat1[6]>=playPropertiesMurottal1[5]){
                  murottalPlayingStat1[3]=1;
                  murottalPlayingStat1[6]=0;
                  murottalPlayingStat1[5]++;
                  if(murottalPlayingStat1[5]>255){
                    murottalPlayingStat1[5]=1;
                     murottalPlayingStat1[4]++;
                  }
                  if(murottalPlayingStat1[4]>fileFolderMurottal1[2]){
                    murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat1[4]=fileFolderMurottal1[0];
                    murottalPlayingStat1[5]=fileFolderMurottal1[1];
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                  else if(murottalPlayingStat1[4]==fileFolderMurottal1[2]){
                    if(murottalPlayingStat1[5]>fileFolderMurottal1[3]){
                      murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat1[4]=fileFolderMurottal1[0];
                      murottalPlayingStat1[5]=fileFolderMurottal1[1];
                      murottalPlayingStat1[7]=murottalPlayingStat1[4];
                      murottalPlayingStat1[8]=murottalPlayingStat1[5];
                      myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                      countMp3Failed1_1 = 0;
                    }
                    else{
                      murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat1[7]=murottalPlayingStat1[4];
                      murottalPlayingStat1[8]=murottalPlayingStat1[5];
                      myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                      countMp3Failed1_1 = 0;
                    }
                  }
                  else{
                    murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat1[7]=murottalPlayingStat1[4];
                    murottalPlayingStat1[8]=murottalPlayingStat1[5];
                    myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                    countMp3Failed1_1 = 0;
                  }
                }
                else{
                  murottalPlayingStat1[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat1[7]=fileFolderMurottal1[0];
                  murottalPlayingStat1[8]=fileFolderMurottal1[1];
                  myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
                  countMp3Failed1_1 = 0;
                }
              }
              else{
                murottalPlayingStat1[2]=1;
                myDFPlayer.playNext();
                countMp3Failed1_1 = 0;
              }
            }
            else{
              murottalPlayingStat1[2]=1;
              myDFPlayer.playNext();
              countMp3Failed1_1 = 0;
            }
          }
        }
      }
    }
  }
  else if(murottalPlayingStat1[0]==1 & (waktuPemutaranMurottal1+(playPropertiesMurottal1[7]*60000))<=millis()){     //timeout
    murottalState1=false;
  }
}

void checkMurottalPlayingStat1(){
  if(murottalPlayingStat1[2]==1 & !digitalRead(mp3Inp)){
    murottalPlayingStat1[2]=2; //change status playing to --> playing
  }
  else if(murottalPlayingStat1[2]==2 & digitalRead(mp3Inp)){
    murottalPlayingStat1[2]=3; //change status playing to --> finish
  }

  if(murottalPlayingStat1[2]==1){        //check apakah mp3 sudah diputar atau belum, kalau belum perintahkan putar lagi
    countMp3Failed1_1++;
    if(countMp3Failed1_1>=countMp3Failed){
      myDFPlayer.playFolder(murottalPlayingStat1[7], murottalPlayingStat1[8]);
      countMp3Failed1_1 = 0;
    }
  }
}

void memutarMurottalSpeech1(){
  if(murottalState1==true & statInfoMurottalSpeech1==0){
    myDFPlayer.pause();
    waktuPemutaranMurottal1 = millis();
    murottalPlayingStat1[2]=0;        
    murottalPlayingStat1[0] = 1;
    statInfoMurottalSpeech1 = 1;
    myDFPlayer.volume(volume2);
  }
  else if(murottalState1==false & statInfoMurottalSpeech1==1){
    spiffsStat = true;
    murottalRecord1[0] = playPropertiesMurottal1[6];
    murottalRecord1[1] = murottalPlayingStat1[7];
    murottalRecord1[2] = murottalPlayingStat1[8];    
    File fileToWrite2 = SPIFFS.open("/Murottal1Record.txt", "w");
    String abc = String(murottalRecord1[0]);
    abc += "\n" + String(murottalRecord1[1]);
    abc += "\n" + String(murottalRecord1[2]);
    fileToWrite2.println(abc);
    fileToWrite2.close();
    delay(500);    

    myDFPlayer.pause();
    murottalPlayingStat1[2]=0;        
    murottalPlayingStat1[0] = 0;
    murottalPlayingStat1[1]=0;
    statInfoMurottalSpeech1 = 0;
    spiffsStat = false;
  }
}

void murottalPlayingFunc2(){
  if(murottalPlayingStat2[0]==1 & (waktuPemutaranMurottal2+(playPropertiesMurottal2[7]*60000))>millis()){            //control playing ON
    if(murottalPlayingStat2[1]==0){          //initializing not yet
      murottalPlayingStat2[1]=1;                       
      if(playPropertiesMurottal2[4]==0){               //check repeating mode on playPropertiesMorottal
        murottalPlayingStat2[3]=0;
      }
      else{                                           //check repeating mode on playPropertiesMorottal
        murottalPlayingStat2[3]=1;
      }
      murottalPlayingStat2[2]=1;                       //change status playing to --> command to play

      if(playPropertiesMurottal2[4]==0 & murottalRecord2[0]==playPropertiesMurottal2[6] &
         (murottalRecord2[1]>fileFolderMurottal2[0] | 
         (murottalRecord2[1]==fileFolderMurottal2[0] & murottalRecord2[2]>=fileFolderMurottal2[1]))){
        murottalRecord2[2]++;
        if(murottalRecord2[2]>255){
          murottalRecord2[1]++;
          murottalRecord2[2] = 1;
        }
        if(murottalRecord2[1]<fileFolderMurottal2[2]){
          murottalPlayingStat2[4] = murottalRecord2[1];
          murottalPlayingStat2[5] = murottalRecord2[2];
        }
        else if(murottalRecord2[1]==fileFolderMurottal2[2] & murottalRecord2[2]<=fileFolderMurottal2[3]){
          murottalPlayingStat2[4] = murottalRecord2[1];
          murottalPlayingStat2[5] = murottalRecord2[2];
        }
        else{
          murottalPlayingStat2[4]=fileFolderMurottal2[0];
          murottalPlayingStat2[5]=fileFolderMurottal2[1];
        }
      }
      else{
        murottalPlayingStat2[4]=fileFolderMurottal2[0];
        murottalPlayingStat2[5]=fileFolderMurottal2[1];
      }
      
      murottalPlayingStat2[7]=murottalPlayingStat2[4];
      murottalPlayingStat2[8]=murottalPlayingStat2[5];
      murottalPlayingStat2[6]=0;
      myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
      countMp3Failed1_2 = 0;
    }
    else{
      if(murottalPlayingStat2[2]==3){   //jika sudah selesai diputar
        if(playPropertiesMurottal2[4]==0){  //metode repeat biasa
          murottalPlayingStat2[5]++;
          if(murottalPlayingStat2[5]>255){
            murottalPlayingStat2[5]=1;
            murottalPlayingStat2[4]++;
          }
          if(murottalPlayingStat2[4]>fileFolderMurottal2[2]){
            murottalPlayingStat2[4]=fileFolderMurottal2[0];
            murottalPlayingStat2[5]=fileFolderMurottal2[1];
            murottalPlayingStat2[7]=murottalPlayingStat2[4];
            murottalPlayingStat2[8]=murottalPlayingStat2[5];
            murottalPlayingStat2[2]=1;        
            myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
            countMp3Failed1_2 = 0;
          }
          else if(murottalPlayingStat2[4]==fileFolderMurottal2[2]){
            if(murottalPlayingStat2[5]>fileFolderMurottal2[3]){
              murottalPlayingStat2[4]=fileFolderMurottal2[0];
              murottalPlayingStat2[5]=fileFolderMurottal2[1];
              murottalPlayingStat2[7]=murottalPlayingStat2[4];
              murottalPlayingStat2[8]=murottalPlayingStat2[5];
              murottalPlayingStat2[2]=1;        
              myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
              countMp3Failed1_2 = 0;
            }
            else{
              murottalPlayingStat2[2]=1;
              murottalPlayingStat2[7]=murottalPlayingStat2[4];
              murottalPlayingStat2[8]=murottalPlayingStat2[5];
              myDFPlayer.playNext();
              countMp3Failed1_2 = 0;
            }
          }
          else{
            murottalPlayingStat2[2]=1;
            murottalPlayingStat2[7]=murottalPlayingStat2[4];
            murottalPlayingStat2[8]=murottalPlayingStat2[5];
            myDFPlayer.playNext();
            countMp3Failed1_2 = 0;
          }
        }
        else{                              //metode repeat tikrar
          if(murottalPlayingStat2[3]==1){  //current mode repeat one
            murottalPlayingStat2[6]++;
            if(murottalPlayingStat2[6]>=playPropertiesMurottal2[5]){ //last play
              //-------------------
              if(murottalPlayingStat2[4]==fileFolderMurottal2[0] & murottalPlayingStat2[5]==fileFolderMurottal2[1]){
                murottalPlayingStat2[5]++;
                if(murottalPlayingStat2[5]>255){
                  murottalPlayingStat2[5]=1;
                  murottalPlayingStat2[4]++;
                }
                if(murottalPlayingStat2[4]>fileFolderMurottal2[2]){
                  //change current mode to repeat one
                  murottalPlayingStat2[4]=fileFolderMurottal2[0];
                  murottalPlayingStat2[5]=fileFolderMurottal2[1];
                  murottalPlayingStat2[3]=1;
                  murottalPlayingStat2[6]=0;  
                  murottalPlayingStat2[7]=murottalPlayingStat2[4];
                  murottalPlayingStat2[8]=murottalPlayingStat2[5];
                  murottalPlayingStat2[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                  countMp3Failed1_2 = 0;      
                }
                else if(murottalPlayingStat2[4]==fileFolderMurottal2[2]){
                  if(murottalPlayingStat2[5]>fileFolderMurottal2[3]){
                    //change current mode to repeat one
                    murottalPlayingStat2[4]=fileFolderMurottal2[0];
                    murottalPlayingStat2[5]=fileFolderMurottal2[1];
                    murottalPlayingStat2[3]=1;
                    murottalPlayingStat2[6]=0;
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    murottalPlayingStat2[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                  else{
                    //change current mode to repeat one
                    murottalPlayingStat2[3]=1;
                    murottalPlayingStat2[6]=0;
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    murottalPlayingStat2[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                }
                else{
                  //change current mode to repeat one
                  murottalPlayingStat2[3]=1;
                  murottalPlayingStat2[6]=0;
                  murottalPlayingStat2[7]=murottalPlayingStat2[4];
                  murottalPlayingStat2[8]=murottalPlayingStat2[5];
                  murottalPlayingStat2[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                  countMp3Failed1_2 = 0;
                }
              }
              else{
                //change current mode to repeat all
                murottalPlayingStat2[6]=0;
                murottalPlayingStat2[3]=0;
                murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat2[7]=fileFolderMurottal2[0];
                murottalPlayingStat2[8]=fileFolderMurottal2[1];
                myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                countMp3Failed1_2 = 0;
              }
            }
            else{
              murottalPlayingStat2[2]=1;
              myDFPlayer.resume();
              countMp3Failed1_2 = 0;
            }
          }
          else{                           //current mode repeat all
            murottalPlayingStat2[8]++;
            if(murottalPlayingStat2[8]>255){
              murottalPlayingStat2[8]=1;
              murottalPlayingStat2[7]++;
            }
            if(murottalPlayingStat2[7]>murottalPlayingStat2[4]){
              murottalPlayingStat2[6]++;
              if(murottalPlayingStat2[6]>=playPropertiesMurottal2[5]){
                murottalPlayingStat2[3]=1;
                murottalPlayingStat2[6]=0;
                murottalPlayingStat2[5]++;
                if(murottalPlayingStat2[5]>255){
                  murottalPlayingStat2[5]=1;
                   murottalPlayingStat2[4]++;
                }
                if(murottalPlayingStat2[4]>fileFolderMurottal2[2]){
                  murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat2[4]=fileFolderMurottal2[0];
                  murottalPlayingStat2[5]=fileFolderMurottal2[1];
                  murottalPlayingStat2[7]=murottalPlayingStat2[4];
                  murottalPlayingStat2[8]=murottalPlayingStat2[5];
                  myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                  countMp3Failed1_2 = 0;
                }
                else if(murottalPlayingStat2[4]==fileFolderMurottal2[2]){
                  if(murottalPlayingStat2[5]>fileFolderMurottal2[3]){
                    murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat2[4]=fileFolderMurottal2[0];
                    murottalPlayingStat2[5]=fileFolderMurottal2[1];
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                  else{
                    murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                }
                else{
                  murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat2[7]=murottalPlayingStat2[4];
                  murottalPlayingStat2[8]=murottalPlayingStat2[5];
                  myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                  countMp3Failed1_2 = 0;
                }
              }
              else{
                murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat2[7]=fileFolderMurottal2[0];
                murottalPlayingStat2[8]=fileFolderMurottal2[1];
                myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                countMp3Failed1_2 = 0;
              }
            }
            else if(murottalPlayingStat2[7]==murottalPlayingStat2[4]){
              if(murottalPlayingStat2[8]>murottalPlayingStat2[5]){
                murottalPlayingStat2[6]++;
                if(murottalPlayingStat2[6]>=playPropertiesMurottal2[5]){
                  murottalPlayingStat2[3]=1;
                  murottalPlayingStat2[6]=0;
                  murottalPlayingStat2[5]++;
                  if(murottalPlayingStat2[5]>255){
                    murottalPlayingStat2[5]=1;
                     murottalPlayingStat2[4]++;
                  }
                  if(murottalPlayingStat2[4]>fileFolderMurottal2[2]){
                    murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat2[4]=fileFolderMurottal2[0];
                    murottalPlayingStat2[5]=fileFolderMurottal2[1];
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                  else if(murottalPlayingStat2[4]==fileFolderMurottal2[2]){
                    if(murottalPlayingStat2[5]>fileFolderMurottal2[3]){
                      murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat2[4]=fileFolderMurottal2[0];
                      murottalPlayingStat2[5]=fileFolderMurottal2[1];
                      murottalPlayingStat2[7]=murottalPlayingStat2[4];
                      murottalPlayingStat2[8]=murottalPlayingStat2[5];
                      myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                      countMp3Failed1_2 = 0;
                    }
                    else{
                      murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat2[7]=murottalPlayingStat2[4];
                      murottalPlayingStat2[8]=murottalPlayingStat2[5];
                      myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                      countMp3Failed1_2 = 0;
                    }
                  }
                  else{
                    murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat2[7]=murottalPlayingStat2[4];
                    murottalPlayingStat2[8]=murottalPlayingStat2[5];
                    myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                    countMp3Failed1_2 = 0;
                  }
                }
                else{
                  murottalPlayingStat2[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat2[7]=fileFolderMurottal2[0];
                  murottalPlayingStat2[8]=fileFolderMurottal2[1];
                  myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
                  countMp3Failed1_2 = 0;
                }
              }
              else{
                murottalPlayingStat2[2]=1;
                myDFPlayer.playNext();
                countMp3Failed1_2 = 0;
              }
            }
            else{
              murottalPlayingStat2[2]=1;
              myDFPlayer.playNext();
              countMp3Failed1_2 = 0;
            }
          }
        }
      }
    }
  }
  else if(murottalPlayingStat2[0]==1 & (waktuPemutaranMurottal2+(playPropertiesMurottal2[7]*60000))<=millis()){     //timeout
    murottalState2=false;
  }
}

void checkMurottalPlayingStat2(){
  if(murottalPlayingStat2[2]==1 & !digitalRead(mp3Inp)){
    murottalPlayingStat2[2]=2; //change status playing to --> playing
  }
  else if(murottalPlayingStat2[2]==2 & digitalRead(mp3Inp)){
    murottalPlayingStat2[2]=3; //change status playing to --> finish
  }

  if(murottalPlayingStat2[2]==1){        //check apakah mp3 sudah diputar atau belum, kalau belum perintahkan putar lagi
    countMp3Failed1_2++;
    if(countMp3Failed1_2>=countMp3Failed){
      myDFPlayer.playFolder(murottalPlayingStat2[7], murottalPlayingStat2[8]);
      countMp3Failed1_2 = 0;
    }
  }
}

void memutarMurottalSpeech2(){
  if(murottalState2==true & statInfoMurottalSpeech2==0){
    myDFPlayer.pause();
    waktuPemutaranMurottal2 = millis();
    murottalPlayingStat2[2]=0;        
    murottalPlayingStat2[0] = 1;
    statInfoMurottalSpeech2 = 1;
    myDFPlayer.volume(volume2);
  }
  else if(murottalState2==false & statInfoMurottalSpeech2==1){
    spiffsStat = true;
    murottalRecord2[0] = playPropertiesMurottal2[6];
    murottalRecord2[1] = murottalPlayingStat2[7];
    murottalRecord2[2] = murottalPlayingStat2[8];    
    File fileToWrite2 = SPIFFS.open("/Murottal2Record.txt", "w");
    String abc = String(murottalRecord2[0]);
    abc += "\n" + String(murottalRecord2[1]);
    abc += "\n" + String(murottalRecord2[2]);
    fileToWrite2.println(abc);
    fileToWrite2.close();
    delay(500);

    myDFPlayer.pause();
    murottalPlayingStat2[2]=0;        
    murottalPlayingStat2[0] = 0;
    murottalPlayingStat2[1]=0;
    statInfoMurottalSpeech2 = 0;
    spiffsStat = false;
  }
}

void murottalPlayingFunc3(){
  if(murottalPlayingStat3[0]==1 & (waktuPemutaranMurottal3+(playPropertiesMurottal3[7]*60000))>millis()){            //control playing ON
    if(murottalPlayingStat3[1]==0){          //initializing not yet
      murottalPlayingStat3[1]=1;                       
      if(playPropertiesMurottal3[4]==0){               //check repeating mode on playPropertiesMorottal
        murottalPlayingStat3[3]=0;
      }
      else{                                           //check repeating mode on playPropertiesMorottal
        murottalPlayingStat3[3]=1;
      }
      murottalPlayingStat3[2]=1;                       //change status playing to --> command to play

      if(playPropertiesMurottal3[4]==0 & murottalRecord3[0]==playPropertiesMurottal3[6] &
         (murottalRecord3[1]>fileFolderMurottal3[0] | 
         (murottalRecord3[1]==fileFolderMurottal3[0] & murottalRecord3[2]>=fileFolderMurottal3[1]))){
        murottalRecord3[2]++;
        if(murottalRecord3[2]>255){
          murottalRecord3[1]++;
          murottalRecord3[2] = 1;
        }
        if(murottalRecord3[1]<fileFolderMurottal3[2]){
          murottalPlayingStat3[4] = murottalRecord3[1];
          murottalPlayingStat3[5] = murottalRecord3[2];
        }
        else if(murottalRecord3[1]==fileFolderMurottal3[2] & murottalRecord3[2]<=fileFolderMurottal3[3]){
          murottalPlayingStat3[4] = murottalRecord3[1];
          murottalPlayingStat3[5] = murottalRecord3[2];
        }
        else{
          murottalPlayingStat3[4]=fileFolderMurottal3[0];
          murottalPlayingStat3[5]=fileFolderMurottal3[1];
        }
      }
      else{
        murottalPlayingStat3[4]=fileFolderMurottal3[0];
        murottalPlayingStat3[5]=fileFolderMurottal3[1];
      }
      
      murottalPlayingStat3[7]=murottalPlayingStat3[4];
      murottalPlayingStat3[8]=murottalPlayingStat3[5];
      murottalPlayingStat3[6]=0;
      myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
      countMp3Failed1_3 = 0;
    }
    else{
      if(murottalPlayingStat3[2]==3){   //jika sudah selesai diputar
        if(playPropertiesMurottal3[4]==0){  //metode repeat biasa
          murottalPlayingStat3[5]++;
          if(murottalPlayingStat3[5]>255){
            murottalPlayingStat3[5]=1;
            murottalPlayingStat3[4]++;
          }
          if(murottalPlayingStat3[4]>fileFolderMurottal3[2]){
            murottalPlayingStat3[4]=fileFolderMurottal3[0];
            murottalPlayingStat3[5]=fileFolderMurottal3[1];
            murottalPlayingStat3[7]=murottalPlayingStat3[4];
            murottalPlayingStat3[8]=murottalPlayingStat3[5];
            murottalPlayingStat3[2]=1;        
            myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
            countMp3Failed1_3 = 0;
          }
          else if(murottalPlayingStat3[4]==fileFolderMurottal3[2]){
            if(murottalPlayingStat3[5]>fileFolderMurottal3[3]){
              murottalPlayingStat3[4]=fileFolderMurottal3[0];
              murottalPlayingStat3[5]=fileFolderMurottal3[1];
              murottalPlayingStat3[7]=murottalPlayingStat3[4];
              murottalPlayingStat3[8]=murottalPlayingStat3[5];
              murottalPlayingStat3[2]=1;        
              myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
              countMp3Failed1_3 = 0;
            }
            else{
              murottalPlayingStat3[2]=1;
              murottalPlayingStat3[7]=murottalPlayingStat3[4];
              murottalPlayingStat3[8]=murottalPlayingStat3[5];
              myDFPlayer.playNext();
              countMp3Failed1_3 = 0;
            }
          }
          else{
            murottalPlayingStat3[2]=1;
            murottalPlayingStat3[7]=murottalPlayingStat3[4];
            murottalPlayingStat3[8]=murottalPlayingStat3[5];
            myDFPlayer.playNext();
            countMp3Failed1_3 = 0;
          }
        }
        else{                              //metode repeat tikrar
          if(murottalPlayingStat3[3]==1){  //current mode repeat one
            murottalPlayingStat3[6]++;
            if(murottalPlayingStat3[6]>=playPropertiesMurottal3[5]){ //last play
              //-------------------
              if(murottalPlayingStat3[4]==fileFolderMurottal3[0] & murottalPlayingStat3[5]==fileFolderMurottal3[1]){
                murottalPlayingStat3[5]++;
                if(murottalPlayingStat3[5]>255){
                  murottalPlayingStat3[5]=1;
                  murottalPlayingStat3[4]++;
                }
                if(murottalPlayingStat3[4]>fileFolderMurottal3[2]){
                  //change current mode to repeat one
                  murottalPlayingStat3[4]=fileFolderMurottal3[0];
                  murottalPlayingStat3[5]=fileFolderMurottal3[1];
                  murottalPlayingStat3[3]=1;
                  murottalPlayingStat3[6]=0;  
                  murottalPlayingStat3[7]=murottalPlayingStat3[4];
                  murottalPlayingStat3[8]=murottalPlayingStat3[5];
                  murottalPlayingStat3[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                  countMp3Failed1_3 = 0;      
                }
                else if(murottalPlayingStat3[4]==fileFolderMurottal3[2]){
                  if(murottalPlayingStat3[5]>fileFolderMurottal3[3]){
                    //change current mode to repeat one
                    murottalPlayingStat3[4]=fileFolderMurottal3[0];
                    murottalPlayingStat3[5]=fileFolderMurottal3[1];
                    murottalPlayingStat3[3]=1;
                    murottalPlayingStat3[6]=0;
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    murottalPlayingStat3[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                  else{
                    //change current mode to repeat one
                    murottalPlayingStat3[3]=1;
                    murottalPlayingStat3[6]=0;
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    murottalPlayingStat3[2]=1;        
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                }
                else{
                  //change current mode to repeat one
                  murottalPlayingStat3[3]=1;
                  murottalPlayingStat3[6]=0;
                  murottalPlayingStat3[7]=murottalPlayingStat3[4];
                  murottalPlayingStat3[8]=murottalPlayingStat3[5];
                  murottalPlayingStat3[2]=1;        
                  myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                  countMp3Failed1_3 = 0;
                }
              }
              else{
                //change current mode to repeat all
                murottalPlayingStat3[6]=0;
                murottalPlayingStat3[3]=0;
                murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat3[7]=fileFolderMurottal3[0];
                murottalPlayingStat3[8]=fileFolderMurottal3[1];
                myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                countMp3Failed1_3 = 0;
              }
            }
            else{
              murottalPlayingStat3[2]=1;
              myDFPlayer.resume();
              countMp3Failed1_3 = 0;
            }
          }
          else{                           //current mode repeat all
            murottalPlayingStat3[8]++;
            if(murottalPlayingStat3[8]>255){
              murottalPlayingStat3[8]=1;
              murottalPlayingStat3[7]++;
            }
            if(murottalPlayingStat3[7]>murottalPlayingStat3[4]){
              murottalPlayingStat3[6]++;
              if(murottalPlayingStat3[6]>=playPropertiesMurottal3[5]){
                murottalPlayingStat3[3]=1;
                murottalPlayingStat3[6]=0;
                murottalPlayingStat3[5]++;
                if(murottalPlayingStat3[5]>255){
                  murottalPlayingStat3[5]=1;
                   murottalPlayingStat3[4]++;
                }
                if(murottalPlayingStat3[4]>fileFolderMurottal3[2]){
                  murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat3[4]=fileFolderMurottal3[0];
                  murottalPlayingStat3[5]=fileFolderMurottal3[1];
                  murottalPlayingStat3[7]=murottalPlayingStat3[4];
                  murottalPlayingStat3[8]=murottalPlayingStat3[5];
                  myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                  countMp3Failed1_3 = 0;
                }
                else if(murottalPlayingStat3[4]==fileFolderMurottal3[2]){
                  if(murottalPlayingStat3[5]>fileFolderMurottal3[3]){
                    murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat3[4]=fileFolderMurottal3[0];
                    murottalPlayingStat3[5]=fileFolderMurottal3[1];
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                  else{
                    murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                }
                else{
                  murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat3[7]=murottalPlayingStat3[4];
                  murottalPlayingStat3[8]=murottalPlayingStat3[5];
                  myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                  countMp3Failed1_3 = 0;
                }
              }
              else{
                murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                murottalPlayingStat3[7]=fileFolderMurottal3[0];
                murottalPlayingStat3[8]=fileFolderMurottal3[1];
                myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                countMp3Failed1_3 = 0;
              }
            }
            else if(murottalPlayingStat3[7]==murottalPlayingStat3[4]){
              if(murottalPlayingStat3[8]>murottalPlayingStat3[5]){
                murottalPlayingStat3[6]++;
                if(murottalPlayingStat3[6]>=playPropertiesMurottal3[5]){
                  murottalPlayingStat3[3]=1;
                  murottalPlayingStat3[6]=0;
                  murottalPlayingStat3[5]++;
                  if(murottalPlayingStat3[5]>255){
                    murottalPlayingStat3[5]=1;
                     murottalPlayingStat3[4]++;
                  }
                  if(murottalPlayingStat3[4]>fileFolderMurottal3[2]){
                    murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat3[4]=fileFolderMurottal3[0];
                    murottalPlayingStat3[5]=fileFolderMurottal3[1];
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                  else if(murottalPlayingStat3[4]==fileFolderMurottal3[2]){
                    if(murottalPlayingStat3[5]>fileFolderMurottal3[3]){
                      murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat3[4]=fileFolderMurottal3[0];
                      murottalPlayingStat3[5]=fileFolderMurottal3[1];
                      murottalPlayingStat3[7]=murottalPlayingStat3[4];
                      murottalPlayingStat3[8]=murottalPlayingStat3[5];
                      myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                      countMp3Failed1_3 = 0;
                    }
                    else{
                      murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                      murottalPlayingStat3[7]=murottalPlayingStat3[4];
                      murottalPlayingStat3[8]=murottalPlayingStat3[5];
                      myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                      countMp3Failed1_3 = 0;
                    }
                  }
                  else{
                    murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                    murottalPlayingStat3[7]=murottalPlayingStat3[4];
                    murottalPlayingStat3[8]=murottalPlayingStat3[5];
                    myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                    countMp3Failed1_3 = 0;
                  }
                }
                else{
                  murottalPlayingStat3[2]=1;                       //change status playing to --> command to play
                  murottalPlayingStat3[7]=fileFolderMurottal3[0];
                  murottalPlayingStat3[8]=fileFolderMurottal3[1];
                  myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
                  countMp3Failed1_3 = 0;
                }
              }
              else{
                murottalPlayingStat3[2]=1;
                myDFPlayer.playNext();
                countMp3Failed1_3 = 0;
              }
            }
            else{
              murottalPlayingStat3[2]=1;
              myDFPlayer.playNext();
              countMp3Failed1_3 = 0;
            }
          }
        }
      }
    }
  }
  else if(murottalPlayingStat3[0]==1 & (waktuPemutaranMurottal3+(playPropertiesMurottal3[7]*60000))<=millis()){     //timeout
    murottalState3=false;
  }
}

void checkMurottalPlayingStat3(){
  if(murottalPlayingStat3[2]==1 & !digitalRead(mp3Inp)){
    murottalPlayingStat3[2]=2; //change status playing to --> playing
  }
  else if(murottalPlayingStat3[2]==2 & digitalRead(mp3Inp)){
    murottalPlayingStat3[2]=3; //change status playing to --> finish
  }

  if(murottalPlayingStat3[2]==1){        //check apakah mp3 sudah diputar atau belum, kalau belum perintahkan putar lagi
    countMp3Failed1_3++;
    if(countMp3Failed1_3>=countMp3Failed){
      myDFPlayer.playFolder(murottalPlayingStat3[7], murottalPlayingStat3[8]);
      countMp3Failed1_3 = 0;
    }
  }
}

void memutarMurottalSpeech3(){
  if(murottalState3==true & statInfoMurottalSpeech3==0){
    myDFPlayer.pause();
    waktuPemutaranMurottal3 = millis();
    murottalPlayingStat3[2]=0;        
    murottalPlayingStat3[0] = 1;
    statInfoMurottalSpeech3 = 1;
    myDFPlayer.volume(volume2);
  }
  else if(murottalState3==false & statInfoMurottalSpeech3==1){
    spiffsStat = true;
    murottalRecord3[0] = playPropertiesMurottal3[6];
    murottalRecord3[1] = murottalPlayingStat3[7];
    murottalRecord3[2] = murottalPlayingStat3[8];    
    File fileToWrite2 = SPIFFS.open("/Murottal3Record.txt", "w");
    String abc = String(murottalRecord3[0]);
    abc += "\n" + String(murottalRecord3[1]);
    abc += "\n" + String(murottalRecord3[2]);
    fileToWrite2.println(abc);
    fileToWrite2.close();
    delay(500); 

    myDFPlayer.pause();
    murottalPlayingStat3[2]=0;        
    murottalPlayingStat3[0] = 0;
    murottalPlayingStat3[1]=0;
    statInfoMurottalSpeech3 = 0;
    spiffsStat = false;
  }
}

String processor(const String &var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();
    buttons+= "<p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label></p>";
    return buttons;
  }
  if (var == "STATE"){
    if(murottalPlayingStat[0]){
      return "ON";
    }
    else {
      return "OFF";
    }
  }
  return String();
}

String outputState(){ 
  if(murottalPlayingStat[0]){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void countTime(bool xyz){
  RtcDateTime now = Rtc.GetDateTime();
  timeNow.secondNow = now.Second();
  timeNow.minuteNow = now.Minute();
  timeNow.hourNow = now.Hour();
  timeNow.dayNow = now.Day();
  timeNow.monthNow = now.Month();
  timeNow.yearNow = now.Year();
  timeNow.dowNow = now.DayOfWeek();
  timeMin.Now = timeNow.hourNow*60 + timeNow.minuteNow;

  String a;
  int b;
  String jam1;
  String menit1;
  int m;
  static int n;
  int i;

  n++;
  if(n>32){
    n=0;
  }
  i = n/4;
  
  CalcTomorrow(g(timeNow.yearNow-2000,timeNow.monthNow,timeNow.dayNow),dataWS.koreksi_hijri);

  timeNow.dayH = tomorrow[2];
  timeNow.monthH = tomorrow[1];
  timeNow.yearH = tomorrow[0];
  
  gregorianToHijri(timeNow.monthH , timeNow.dayH , timeNow.yearH);
  get_prayer_times(timeNow.yearNow, timeNow.monthNow, timeNow.dayNow, dataWS._lat, dataWS._lng, dataWS._timezone1, waktu);
  get_float_time_parts(waktu[i], hours, minutes);
  if (i == 0) { //Imsak
    minutes = minutes + dataWS.koreksi_imsak;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Imsak = hours*60 + minutes;
  }
  else if (i == 1) { //Subuh
    minutes = minutes + dataWS.koreksi_subuh ;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Subuh = hours*60 + minutes;
  } 
  else if (i == 2) { //Terbit
    minutes = minutes + dataWS.koreksi_terbit;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Syuruk = hours*60 + minutes;
  }  
  else if (i == 3) { //Dhuha
    minutes = minutes + dataWS.koreksi_dhuha;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Dhuha = hours*60 + minutes;
    
  }  
  else if (i == 4) { //Zhuhur
    minutes = minutes + dataWS.koreksi_zuhur;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Zuhur = hours*60 + minutes;
  }  
  else if (i == 5) { //Ashar
    minutes = minutes + dataWS.koreksi_ashar;
    cek_menit();
    prayTimeSav[i][0] = hours;
    prayTimeSav[i][1] = minutes;
    timeMin.Asar = hours*60 + minutes;
  }  
  else if (i == 6) { //Terbenam
    minutes = minutes + dataWS.koreksi_terbenam;
    cek_menit();
  }  
  else if (i == 7) { //Magrib
    minutes = minutes + dataWS.koreksi_magrib;
    cek_menit();
    prayTimeSav[i-1][0] = hours;
    prayTimeSav[i-1][1] = minutes;
    timeMin.Magrib = hours*60 + minutes;
  }  
  else if (i == 8) { //Isya
    minutes = minutes + dataWS.koreksi_isya ;
    cek_menit();
    prayTimeSav[i-1][0] = hours;
    prayTimeSav[i-1][1] = minutes;
    timeMin.Isya = hours*60 + minutes;
  }
  
  if(i!=6){
    if(i>=7){
      m = i-1;
    }
    else{
      m = i;
    }
    
    jam1 = String(prayTimeSav[m][0]/10) + String(prayTimeSav[m][0]%10);
    menit1 = String(prayTimeSav[m][1]/10) + String(prayTimeSav[m][1]%10);
    a = waktuSholatString[m] + String(jam1) + ":" + String(menit1);
    b = a.length() + 1;
    a.toCharArray(runPrayTime[m], b);
    a = waktuSholatString1[m] + String(jam1) + ":" + String(menit1);
    b = a.length() + 1;
    a.toCharArray(runPrayTime1[m], b);
  }
  
  int k = timeNow.hourNow;
  charHour[0] = k/10 + '0';
  charHour[1] = k%10 + '0';
  k = timeNow.minuteNow;
  charMinute[0] = k/10 + '0';
  charMinute[1] = k%10 + '0';
  k = timeNow.secondNow;
  charSecond[0] = k/10 + '0';
  charSecond[1] = k%10 + '0';

  intWeekDay = timeNow.dowNow;
  
  k = timeNow.dayNow;
  charDay[0] = k/10 +'0';
  charDay[1] = k%10 + '0';
  
  intMonth = timeNow.monthNow;
  
  k = timeNow.yearNow;
  charYear[0] = k%1000%100/10 + '0';
  charYear[1] = k%1000%100%10 + '0';

  charDayH[0] = timeNow.dayH/10 +'0';
  charDayH[1] = timeNow.dayH%10 + '0';
  intMonthH = timeNow.monthH;
  charYearH[0] = timeNow.yearH%1000%100/10 + '0';
  charYearH[1] = timeNow.yearH%1000%100%10 + '0';
}

void calculateLisensi(){
  String a = WiFi.macAddress();

  a.replace(":", "");
  serialNumberString = a;

  String b  = "#" + String(a.charAt(7)) + String(a.charAt(4)) + String(a.charAt(5)) + String(a.charAt(2)) + String(a.charAt(3))
                         + String(a.charAt(0)) + String(a.charAt(1));
  long number = strtol( &b[1], NULL, 16);
  String c = String(number);   

  long number2 = 0;
  for(int j=0; j<8; j++){
    number2 += String(c.charAt(j)).toInt() * pow(10,j);
  }

  if(number2<10000000){
    lisensiString = "0" + String(number2);
  }
  else{
    lisensiString = String(number2);
  }

  if(lisensiSaveString!=lisensiString){
    lisensiStatString = "uji coba";
  }
  else{
    lisensiStatString = "berbayar";
    trialStat = false;
  }
}

void getLimitTrial(){
  milisRec3 = millis();
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/limitTrial.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    limitTrial = Line.toInt();
  }
  file.close();
  spiffsStat = false;
}

void getFirstRun(){
  milisRec3 = millis();
  spiffsStat = true;
  String Line;
  File file = SPIFFS.open("/firstRun.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    firstRun = Line.toInt();
  }
  file.close();
  spiffsStat = false;
}

void getDispStat(){
  String Line;
  File file = SPIFFS.open("/dispStat.txt", "r");
  if (file.available()) {    
    Line = file.readStringUntil('\n');
    dispStat[0] = Line.toInt();
    Line = file.readStringUntil('\n');
    dispStat[1] = Line.toInt();
  }
  file.close();
}

void reorderFileSystem(){
  File fileToWrite = SPIFFS.open("/playPropertiesMurottal.txt", "w");
  String abc = String(playPropertiesMurottal[0]);
  abc += "\n" + String(playPropertiesMurottal[1]);
  abc += "\n" + String(playPropertiesMurottal[2]);
  abc += "\n" + String(playPropertiesMurottal[3]);
  abc += "\n" + String(playPropertiesMurottal[4]);
  abc += "\n" + String(playPropertiesMurottal[5]);
  abc += "\n" + String(playPropertiesMurottal[6]);
  abc += "\n" + String(playPropertiesMurottal[7]);
  fileToWrite.println(abc);
  fileToWrite.close();
  getfileFolderMurottal(playPropertiesMurottal[0], playPropertiesMurottal[1], playPropertiesMurottal[2], playPropertiesMurottal[3], playPropertiesMurottal[6]);

  fileToWrite = SPIFFS.open("/playPropertiesMurottal1.txt", "w");
  abc = String(playPropertiesMurottal1[0]);
  abc += "\n" + String(playPropertiesMurottal1[1]);
  abc += "\n" + String(playPropertiesMurottal1[2]);
  abc += "\n" + String(playPropertiesMurottal1[3]);
  abc += "\n" + String(playPropertiesMurottal1[4]);
  abc += "\n" + String(playPropertiesMurottal1[5]);
  abc += "\n" + String(playPropertiesMurottal1[6]);
  abc += "\n" + String(playPropertiesMurottal1[7]);
  fileToWrite.println(abc);
  fileToWrite.close();
  fileToWrite = SPIFFS.open("/Murottal1Record.txt", "w");
  abc = "99\n0\n0";
  fileToWrite.println(abc);
  fileToWrite.close();
  murottalRecord1[0] = 99;
  murottalRecord1[1] = 0;
  murottalRecord1[2] = 0;  
  getfileFolderMurottal1(playPropertiesMurottal1[0], playPropertiesMurottal1[1], playPropertiesMurottal1[2], playPropertiesMurottal1[3], playPropertiesMurottal1[6]);

  fileToWrite = SPIFFS.open("/playPropertiesMurottal2.txt", "w");
  abc = String(playPropertiesMurottal2[0]);
  abc += "\n" + String(playPropertiesMurottal2[1]);
  abc += "\n" + String(playPropertiesMurottal2[2]);
  abc += "\n" + String(playPropertiesMurottal2[3]);
  abc += "\n" + String(playPropertiesMurottal2[4]);
  abc += "\n" + String(playPropertiesMurottal2[5]);
  abc += "\n" + String(playPropertiesMurottal2[6]);
  abc += "\n" + String(playPropertiesMurottal2[7]);
  fileToWrite.println(abc);
  fileToWrite.close();  
  fileToWrite = SPIFFS.open("/Murottal2Record.txt", "w");
  abc = "99\n0\n0";
  fileToWrite.println(abc);
  fileToWrite.close();
  murottalRecord2[0] = 99;
  murottalRecord2[1] = 0;
  murottalRecord2[2] = 0;
  getfileFolderMurottal2(playPropertiesMurottal2[0], playPropertiesMurottal2[1], playPropertiesMurottal2[2], playPropertiesMurottal2[3], playPropertiesMurottal2[6]);

  fileToWrite = SPIFFS.open("/playPropertiesMurottal3.txt", "w");
  abc = String(playPropertiesMurottal3[0]);
  abc += "\n" + String(playPropertiesMurottal3[1]);
  abc += "\n" + String(playPropertiesMurottal3[2]);
  abc += "\n" + String(playPropertiesMurottal3[3]);
  abc += "\n" + String(playPropertiesMurottal3[4]);
  abc += "\n" + String(playPropertiesMurottal3[5]);
  abc += "\n" + String(playPropertiesMurottal3[6]);
  abc += "\n" + String(playPropertiesMurottal3[7]);
  fileToWrite.println(abc);
  fileToWrite.close();
  fileToWrite = SPIFFS.open("/Murottal3Record.txt", "w");
  abc = "99\n0\n0";
  fileToWrite.println(abc);
  fileToWrite.close();
  murottalRecord3[0] = 99;
  murottalRecord3[1] = 0;
  murottalRecord3[2] = 0;
  getfileFolderMurottal3(playPropertiesMurottal3[0], playPropertiesMurottal3[1], playPropertiesMurottal3[2], playPropertiesMurottal3[3], playPropertiesMurottal3[6]);

  fileToWrite = SPIFFS.open("/koreksiWaktuSholat.txt", "w");
  abc = String(dataWS.koreksi_imsak);
  abc += "\n" + String(dataWS.koreksi_subuh);
  abc += "\n" + String(dataWS.koreksi_terbit);
  abc += "\n" + String(dataWS.koreksi_dhuha);
  abc += "\n" + String(dataWS.koreksi_zuhur);
  abc += "\n" + String(dataWS.koreksi_ashar);
  abc += "\n" + String(dataWS.koreksi_terbenam);
  abc += "\n" + String(dataWS.koreksi_magrib);
  abc += "\n" + String(dataWS.koreksi_isya);
  abc += "\n" + String(dataWS._sudut_subuh);
  abc += "\n" + String(dataWS._sudut_isyak); 
  abc += "\n" + String(dataWS.koreksi_hijri);      
  fileToWrite.println(abc);
  fileToWrite.close();
  fileToWrite = SPIFFS.open("/volume.txt", "w");
  abc = String(volume); 
  fileToWrite.println(abc);
  fileToWrite.close();
  fileToWrite = SPIFFS.open("/volume2.txt", "w");
  abc = String(volume2); 
  fileToWrite.println(abc);
  fileToWrite.close();  
  fileToWrite = SPIFFS.open("/brightness.txt", "w");
  abc = String(brightness); 
  fileToWrite.println(abc);
  fileToWrite.close(); 
  fileToWrite = SPIFFS.open("/koordinat.txt", "w");
  abc = String(dataWS.koordinatNum);
  abc += "\n" + String(dataWS._timezone1); 
  fileToWrite.println(abc);
  fileToWrite.close();

  fileToWrite = SPIFFS.open("/timeAlarm.txt", "w");
  String stringTemp = String(timeAlarm.sahur);
  stringTemp += "\n" + String(timeAlarm.subuh);
  stringTemp += "\n" + String(timeAlarm.zuhur);
  stringTemp += "\n" + String(timeAlarm.asar);
  stringTemp += "\n" + String(timeAlarm.magrib);
  stringTemp += "\n" + String(timeAlarm.isya);
  stringTemp += "\n" + String(timeAlarm.tahajjud[0]/10) + String(timeAlarm.tahajjud[0]%10);
  stringTemp += ":" + String(timeAlarm.tahajjud[1]/10) + String(timeAlarm.tahajjud[1]%10);
  stringTemp += "\n" + String(timeAlarm.custom1[0]/10) + String(timeAlarm.custom1[0]%10);
  stringTemp += ":" + String(timeAlarm.custom1[1]/10) + String(timeAlarm.custom1[1]%10);
  stringTemp += "\n" + String(timeAlarm.custom2[0]/10) + String(timeAlarm.custom2[0]%10);
  stringTemp += ":" + String(timeAlarm.custom2[1]/10) + String(timeAlarm.custom2[1]%10);
  stringTemp += "\n" + String(timeAlarm.custom3[0]/10) + String(timeAlarm.custom3[0]%10);
  stringTemp += ":" + String(timeAlarm.custom3[1]/10) + String(timeAlarm.custom3[1]%10);
  stringTemp += "\n" + String(timeAlarm.custom4[0]/10) + String(timeAlarm.custom4[0]%10);
  stringTemp += ":" + String(timeAlarm.custom4[1]/10) + String(timeAlarm.custom4[1]%10);
  stringTemp += "\n" + String(timeAlarm.custom5[0]/10) + String(timeAlarm.custom5[0]%10);
  stringTemp += ":" + String(timeAlarm.custom5[1]/10) + String(timeAlarm.custom5[1]%10);
  String a = "0";
  if(timeAlarm.adzan==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(timeAlarm.countdown==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  fileToWrite.println(stringTemp);
  fileToWrite.close();
  fileToWrite = SPIFFS.open("/alarmWithMurottal.txt", "w");
  stringTemp = String(alarmWithMurottal.subuh);
  stringTemp += "\n" + String(alarmWithMurottal.zuhur);
  stringTemp += "\n" + String(alarmWithMurottal.asar);
  stringTemp += "\n" + String(alarmWithMurottal.magrib);
  stringTemp += "\n" + String(alarmWithMurottal.isya);
  stringTemp += "\n" + String(alarmWithMurottal.custom1);
  stringTemp += "\n" + String(alarmWithMurottal.custom2);
  stringTemp += "\n" + String(alarmWithMurottal.custom3);
  stringTemp += "\n" + String(alarmWithMurottal.custom4);
  stringTemp += "\n" + String(alarmWithMurottal.custom5);
  fileToWrite.println(stringTemp);
  fileToWrite.close();

  fileToWrite = SPIFFS.open("/timeIqamah.txt", "w");
  stringTemp = String(timeIqamah.subuh);
  stringTemp += "\n" + String(timeIqamah.zuhur);
  stringTemp += "\n" + String(timeIqamah.asar);
  stringTemp += "\n" + String(timeIqamah.magrib);
  stringTemp += "\n" + String(timeIqamah.isya);
  fileToWrite.println(stringTemp);
  fileToWrite.close();

  fileToWrite = SPIFFS.open("/dispStat.txt", "w");
  stringTemp = String(dispStat[0]);
  stringTemp += "\n" + String(dispStat[1]);
  fileToWrite.println(stringTemp);
  fileToWrite.close();

  fileToWrite = SPIFFS.open("/sahurStat.txt", "w");
  stringTemp = "";
  a = "0";
  if(sahurStat.sahurSyawal==true){
    a = "1";
  }
  stringTemp = a;
  a = "0";
  if(sahurStat.sahurZulhijah==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurRamadan==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurAsyuro==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurAyamulbidh==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurSenin==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurKamis==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurDaud==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurTasua==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  a = "0";
  if(sahurStat.sahurArafah==true){
    a = "1";
  }
  stringTemp += "\n" + a;
  stringTemp += "\n" + String(sahurStat.awalSyawal); 
  fileToWrite.println(stringTemp);
  fileToWrite.close();

  fileToWrite = SPIFFS.open("/firstRun.txt", "w");
  stringTemp = String(firstRun);
  fileToWrite.println(stringTemp);
  fileToWrite.close();   
}

void setup(){  
  SPIFFS.begin();
  getKoordinat();
  getKoreksi();
  getplayPropertiesMurottal();
  getplayPropertiesMurottal1(true);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
  getplayPropertiesMurottal2(true);
  getplayPropertiesMurottal3(true);
  getBrightness();
  getVolume();
  getTimeAlarm();
  getTimeIqamah();
  getSahurStat();
  getAlarmWithMurottal();
  getWifiSetting();
  getDispStat();
  getLisensiSave();
  calculateLisensi();
  getLimitTrial();  
  getFirstRun();

  if(firstRun!=1){
    firstRun=1;
    reorderFileSystem();
  }

  Rtc.Begin();

  RtcDateTime now = Rtc.GetDateTime();
    
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

  Serial.begin(9600);
  myDFPlayer.begin(Serial,false);
  myDFPlayer.setTimeout(1000); //Set serial communictaion time out 1000ms
  myDFPlayer.playbackSource(2);

  //WiFi.mode(WIFI_AP);
  //WiFi.softAP(APssid, APpassword);
  WiFi.softAP(APssid.c_str(), APpassword.c_str());
  //WiFi.begin("Redmi Note 8","12345678");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", homePage);
  });

  server.on("/generalSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", generalSetting);
  });

  server.on("/alarmSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", alarmSetting);
  });

  server.on("/iqamahSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html",iqamahSetting);
  });

  server.on("/sahurSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", sahurSetting);
  });

  server.on("/timeSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", timeSetting);
  });

  server.on("/murottalSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", murottalSetting, processor );
  });

  server.on("/alarmAck", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", alarmAck);
  });

  server.on("/auto1Setting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", murottalAuto1Setting);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/css", styleCSS);
  });

  server.on("/style2.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/css", styleCSS2);
  });

  server.on("/style3.css", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/css", styleCSS3);
  });

  server.on("/auto2Setting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", murottalAuto2Setting);
  });

  server.on("/auto3Setting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", murottalAuto3Setting);
  });

  server.on("/wifiSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", wifiSetting);
  });

  server.on("/lisensiSetting", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", lisensiSetting);
  });

  server.on("/hackhackhackambyar", HTTP_GET, [](AsyncWebServerRequest *request){
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    request->send_P(200, "text/html", changeTimeTrialSetting);
  });

  server.on("/daftar-isi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", daftarisiquran);
  });

  server.on("/koordinat", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(dataWS.koordinatNum);
    stringTemp += "\n" + String(dataWS._timezone1);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/koreksi", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(dataWS.koreksi_imsak);
    stringTemp += "\n" + String(dataWS.koreksi_subuh);
    stringTemp += "\n" + String(dataWS.koreksi_terbit);
    stringTemp += "\n" + String(dataWS.koreksi_dhuha);
    stringTemp += "\n" + String(dataWS.koreksi_zuhur);
    stringTemp += "\n" + String(dataWS.koreksi_ashar);
    stringTemp += "\n" + String(dataWS.koreksi_terbenam);
    stringTemp += "\n" + String(dataWS.koreksi_magrib);
    stringTemp += "\n" + String(dataWS.koreksi_isya);
    stringTemp += "\n" + String(dataWS._sudut_subuh);
    stringTemp += "\n" + String(dataWS._sudut_isyak);
    stringTemp += "\n" + String(dataWS.koreksi_hijri);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/wifiPassword", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = APssid;
    stringTemp += "\n" + APpassword;
    stringTemp += "\n" + http_username;
    stringTemp += "\n" + http_password;
    request->send(200, "text/plain", stringTemp);
  });
  
  server.on("/dispStat", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(dispStat[0]);
    stringTemp += "\n" + String(dispStat[1]);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(brightness);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/lisensiSave", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = lisensiSaveString;
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/limitTrial", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(limitTrial);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/lisensiStat", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = lisensiStatString;
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/volume", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(volume);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/volume2", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(volume2);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/playPropertiesMurottal", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(playPropertiesMurottal[0]);
    stringTemp += "\n" + String(playPropertiesMurottal[1]);
    stringTemp += "\n" + String(playPropertiesMurottal[2]);
    stringTemp += "\n" + String(playPropertiesMurottal[3]);
    stringTemp += "\n" + String(playPropertiesMurottal[4]);
    stringTemp += "\n" + String(playPropertiesMurottal[5]);
    stringTemp += "\n" + String(playPropertiesMurottal[6]);
    stringTemp += "\n" + String(playPropertiesMurottal[7]);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/playPropertiesMurottal1", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(playPropertiesMurottal1[0]);
    stringTemp += "\n" + String(playPropertiesMurottal1[1]);
    stringTemp += "\n" + String(playPropertiesMurottal1[2]);
    stringTemp += "\n" + String(playPropertiesMurottal1[3]);
    stringTemp += "\n" + String(playPropertiesMurottal1[4]);
    stringTemp += "\n" + String(playPropertiesMurottal1[5]);
    stringTemp += "\n" + String(playPropertiesMurottal1[6]);
    stringTemp += "\n" + String(playPropertiesMurottal1[7]);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/playPropertiesMurottal2", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(playPropertiesMurottal2[0]);
    stringTemp += "\n" + String(playPropertiesMurottal2[1]);
    stringTemp += "\n" + String(playPropertiesMurottal2[2]);
    stringTemp += "\n" + String(playPropertiesMurottal2[3]);
    stringTemp += "\n" + String(playPropertiesMurottal2[4]);
    stringTemp += "\n" + String(playPropertiesMurottal2[5]);
    stringTemp += "\n" + String(playPropertiesMurottal2[6]);
    stringTemp += "\n" + String(playPropertiesMurottal2[7]);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/playPropertiesMurottal3", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(playPropertiesMurottal3[0]);
    stringTemp += "\n" + String(playPropertiesMurottal3[1]);
    stringTemp += "\n" + String(playPropertiesMurottal3[2]);
    stringTemp += "\n" + String(playPropertiesMurottal3[3]);
    stringTemp += "\n" + String(playPropertiesMurottal3[4]);
    stringTemp += "\n" + String(playPropertiesMurottal3[5]);
    stringTemp += "\n" + String(playPropertiesMurottal3[6]);
    stringTemp += "\n" + String(playPropertiesMurottal3[7]);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/alarmWithMurottal", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(alarmWithMurottal.subuh);
    stringTemp += "\n" + String(alarmWithMurottal.zuhur);
    stringTemp += "\n" + String(alarmWithMurottal.asar);
    stringTemp += "\n" + String(alarmWithMurottal.magrib);
    stringTemp += "\n" + String(alarmWithMurottal.isya);
    stringTemp += "\n" + String(alarmWithMurottal.custom1);
    stringTemp += "\n" + String(alarmWithMurottal.custom2);
    stringTemp += "\n" + String(alarmWithMurottal.custom3);
    stringTemp += "\n" + String(alarmWithMurottal.custom4);
    stringTemp += "\n" + String(alarmWithMurottal.custom5);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/timeAlarm", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(timeAlarm.sahur);
    stringTemp += "\n" + String(timeAlarm.subuh);
    stringTemp += "\n" + String(timeAlarm.zuhur);
    stringTemp += "\n" + String(timeAlarm.asar);
    stringTemp += "\n" + String(timeAlarm.magrib);
    stringTemp += "\n" + String(timeAlarm.isya);
    stringTemp += "\n" + String(timeAlarm.tahajjud[0]/10) + String(timeAlarm.tahajjud[0]%10);
    stringTemp += ":" + String(timeAlarm.tahajjud[1]/10) + String(timeAlarm.tahajjud[1]%10);
    stringTemp += "\n" + String(timeAlarm.custom1[0]/10) + String(timeAlarm.custom1[0]%10);
    stringTemp += ":" + String(timeAlarm.custom1[1]/10) + String(timeAlarm.custom1[1]%10);
    stringTemp += "\n" + String(timeAlarm.custom2[0]/10) + String(timeAlarm.custom2[0]%10);
    stringTemp += ":" + String(timeAlarm.custom2[1]/10) + String(timeAlarm.custom2[1]%10);
    stringTemp += "\n" + String(timeAlarm.custom3[0]/10) + String(timeAlarm.custom3[0]%10);
    stringTemp += ":" + String(timeAlarm.custom3[1]/10) + String(timeAlarm.custom3[1]%10);
    stringTemp += "\n" + String(timeAlarm.custom4[0]/10) + String(timeAlarm.custom4[0]%10);
    stringTemp += ":" + String(timeAlarm.custom4[1]/10) + String(timeAlarm.custom4[1]%10);
    stringTemp += "\n" + String(timeAlarm.custom5[0]/10) + String(timeAlarm.custom5[0]%10);
    stringTemp += ":" + String(timeAlarm.custom5[1]/10) + String(timeAlarm.custom5[1]%10);
    String a = "0";
    if(timeAlarm.adzan==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(timeAlarm.countdown==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/timeIqamah", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = String(timeIqamah.subuh);
    stringTemp += "\n" + String(timeIqamah.zuhur);
    stringTemp += "\n" + String(timeIqamah.asar);
    stringTemp += "\n" + String(timeIqamah.magrib);
    stringTemp += "\n" + String(timeIqamah.isya);
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/sahurStat", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = "";
    String a = "0";
    if(sahurStat.sahurSyawal==true){
      a = "1";
    }
    stringTemp = a;
    a = "0";
    if(sahurStat.sahurZulhijah==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurRamadan==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurAsyuro==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurAyamulbidh==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurSenin==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurKamis==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurDaud==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurTasua==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    a = "0";
    if(sahurStat.sahurArafah==true){
      a = "1";
    }
    stringTemp += "\n" + a;
    stringTemp += "\n" + String(sahurStat.awalSyawal);    
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/jumlahayat", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", jumlahayat);
  });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout, processor);
  });

  server.on("/update1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if(inputMessage.toInt()==0){
        if(masukWaktuSholat==false & menjelangWaktuSholat==false & alarmSahur==false &
           alarmTahajjud==false & alarmWOmurottal==false & dispStat[0]==1){
          murottalState = true;
          statInfoMurottalSpeech=0;
        }
      }
      else if(inputMessage.toInt()==1){
        murottalState = false;
        statInfoMurottalSpeech=3;
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/update2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if(inputMessage.toInt()==1){
        sahurMomentStat[0]=20; sahurMomentStat[1]=20; sahurMomentStat[2]=20; sahurMomentStat[3]=20; sahurMomentStat[4]=20; 
        sahurMomentStat[5]=20; sahurMomentStat[6]=20; sahurMomentStat[7]=20; sahurMomentStat[8]=20; sahurMomentStat[9]=20;
        tahajjudMomentStat=20;
        WOmurottalMomentStat[0]=20; WOmurottalMomentStat[1]=20;
        masukWaktuSholatStat[0]=20; masukWaktuSholatStat[1]=20; masukWaktuSholatStat[2]=20; masukWaktuSholatStat[3]=20; 
        masukWaktuSholatStat[4]=20; masukWaktuSholatStat[5]=20; masukWaktuSholatStat[6]=20; masukWaktuSholatStat[7]=20;
        menjelangWaktuSholatStat[0]=20; menjelangWaktuSholatStat[1]=20; menjelangWaktuSholatStat[2]=20; 
        menjelangWaktuSholatStat[3]=20; menjelangWaktuSholatStat[4]=20; 
        WithmurottalMomentStat[0]=20; WithmurottalMomentStat[1]=20; WithmurottalMomentStat[2]=20; 
        myDFPlayer.pause();
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/update3", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if(inputMessage.toInt()==128){
        updateStat = true;
        myDFPlayer.pause();
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/serialNumber", HTTP_GET, [](AsyncWebServerRequest *request){
    String stringTemp = serialNumberString;
    request->send(200, "text/plain", stringTemp);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // if(!request->authenticate(http_username.c_str(), http_password.c_str()))
      // return request->requestAuthentication();
    String inputMessage;
    if(request->hasParam("murottalSettingCB")){
      if (request->hasParam("firstSurah")) {
        inputMessage = request->getParam("firstSurah")->value();
        playPropertiesMurottal[0] = inputMessage.toInt();
      }
      if (request->hasParam("firstAyat")) {
        inputMessage = request->getParam("firstAyat")->value();
        playPropertiesMurottal[1] = inputMessage.toInt();
      }
      if (request->hasParam("lastSurah")) {
        inputMessage = request->getParam("lastSurah")->value();
        playPropertiesMurottal[2] = inputMessage.toInt();
      }
      if (request->hasParam("lastAyat")) {
        inputMessage = request->getParam("lastAyat")->value();
        playPropertiesMurottal[3] = inputMessage.toInt();
      }
      if (request->hasParam("metode")) {
        inputMessage = request->getParam("metode")->value();
        playPropertiesMurottal[4] = inputMessage.toInt();
      }
      if (request->hasParam("repeatNum")) {
        inputMessage = request->getParam("repeatNum")->value();
        playPropertiesMurottal[5] = inputMessage.toInt();
      }
      if (request->hasParam("qari")) {
        inputMessage = request->getParam("qari")->value();
        playPropertiesMurottal[6] = inputMessage.toInt();
      }
      if (request->hasParam("waktuPutar")) {
        inputMessage = request->getParam("waktuPutar")->value();
        playPropertiesMurottal[7] = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/playPropertiesMurottal.txt", "w");
      String abc = String(playPropertiesMurottal[0]);
      abc += "\n" + String(playPropertiesMurottal[1]);
      abc += "\n" + String(playPropertiesMurottal[2]);
      abc += "\n" + String(playPropertiesMurottal[3]);
      abc += "\n" + String(playPropertiesMurottal[4]);
      abc += "\n" + String(playPropertiesMurottal[5]);
      abc += "\n" + String(playPropertiesMurottal[6]);
      abc += "\n" + String(playPropertiesMurottal[7]);
      fileToWrite.println(abc);
      fileToWrite.close();
    
      getfileFolderMurottal(playPropertiesMurottal[0], playPropertiesMurottal[1], playPropertiesMurottal[2], playPropertiesMurottal[3], playPropertiesMurottal[6]);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/murottalSetting\";}, 1500);</script></body></html>");  
    }

    else if(request->hasParam("auto1SettingCB")){
      if (request->hasParam("firstSurah")) {
        inputMessage = request->getParam("firstSurah")->value();
        playPropertiesMurottal1[0] = inputMessage.toInt();
      }
      if (request->hasParam("firstAyat")) {
        inputMessage = request->getParam("firstAyat")->value();
        playPropertiesMurottal1[1] = inputMessage.toInt();
      }
      if (request->hasParam("lastSurah")) {
        inputMessage = request->getParam("lastSurah")->value();
        playPropertiesMurottal1[2] = inputMessage.toInt();
      }
      if (request->hasParam("lastAyat")) {
        inputMessage = request->getParam("lastAyat")->value();
        playPropertiesMurottal1[3] = inputMessage.toInt();
      }
      if (request->hasParam("metode")) {
        inputMessage = request->getParam("metode")->value();
        playPropertiesMurottal1[4] = inputMessage.toInt();
      }
      if (request->hasParam("repeatNum")) {
        inputMessage = request->getParam("repeatNum")->value();
        playPropertiesMurottal1[5] = inputMessage.toInt();
      }
      if (request->hasParam("qari")) {
        inputMessage = request->getParam("qari")->value();
        playPropertiesMurottal1[6] = inputMessage.toInt();
      }
      if (request->hasParam("waktuPutar")) {
        inputMessage = request->getParam("waktuPutar")->value();
        playPropertiesMurottal1[7] = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/playPropertiesMurottal1.txt", "w");
      String abc = String(playPropertiesMurottal1[0]);
      abc += "\n" + String(playPropertiesMurottal1[1]);
      abc += "\n" + String(playPropertiesMurottal1[2]);
      abc += "\n" + String(playPropertiesMurottal1[3]);
      abc += "\n" + String(playPropertiesMurottal1[4]);
      abc += "\n" + String(playPropertiesMurottal1[5]);
      abc += "\n" + String(playPropertiesMurottal1[6]);
      abc += "\n" + String(playPropertiesMurottal1[7]);
      fileToWrite.println(abc);
      fileToWrite.close();

      File fileToWrite2 = SPIFFS.open("/Murottal1Record.txt", "w");
      abc = "99\n0\n0";
      fileToWrite2.println(abc);
      fileToWrite2.close();
    
      murottalRecord1[0] = 99;
      murottalRecord1[1] = 0;
      murottalRecord1[2] = 0;
      getfileFolderMurottal1(playPropertiesMurottal1[0], playPropertiesMurottal1[1], playPropertiesMurottal1[2], playPropertiesMurottal1[3], playPropertiesMurottal1[6]);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/auto1Setting\";}, 1500);</script></body></html>");
    }

    else if(request->hasParam("auto2SettingCB")){
      if (request->hasParam("firstSurah")) {
        inputMessage = request->getParam("firstSurah")->value();
        playPropertiesMurottal2[0] = inputMessage.toInt();
      }
      if (request->hasParam("firstAyat")) {
        inputMessage = request->getParam("firstAyat")->value();
        playPropertiesMurottal2[1] = inputMessage.toInt();
      }
      if (request->hasParam("lastSurah")) {
        inputMessage = request->getParam("lastSurah")->value();
        playPropertiesMurottal2[2] = inputMessage.toInt();
      }
      if (request->hasParam("lastAyat")) {
        inputMessage = request->getParam("lastAyat")->value();
        playPropertiesMurottal2[3] = inputMessage.toInt();
      }
      if (request->hasParam("metode")) {
        inputMessage = request->getParam("metode")->value();
        playPropertiesMurottal2[4] = inputMessage.toInt();
      }
      if (request->hasParam("repeatNum")) {
        inputMessage = request->getParam("repeatNum")->value();
        playPropertiesMurottal2[5] = inputMessage.toInt();
      }
      if (request->hasParam("qari")) {
        inputMessage = request->getParam("qari")->value();
        playPropertiesMurottal2[6] = inputMessage.toInt();
      }
      if (request->hasParam("waktuPutar")) {
        inputMessage = request->getParam("waktuPutar")->value();
        playPropertiesMurottal2[7] = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/playPropertiesMurottal2.txt", "w");
      String abc = String(playPropertiesMurottal2[0]);
      abc += "\n" + String(playPropertiesMurottal2[1]);
      abc += "\n" + String(playPropertiesMurottal2[2]);
      abc += "\n" + String(playPropertiesMurottal2[3]);
      abc += "\n" + String(playPropertiesMurottal2[4]);
      abc += "\n" + String(playPropertiesMurottal2[5]);
      abc += "\n" + String(playPropertiesMurottal2[6]);
      abc += "\n" + String(playPropertiesMurottal2[7]);
      fileToWrite.println(abc);
      fileToWrite.close();
      
      File fileToWrite2 = SPIFFS.open("/Murottal2Record.txt", "w");
      abc = "99\n0\n0";
      fileToWrite2.println(abc);
      fileToWrite2.close();
    
      murottalRecord2[0] = 99;
      murottalRecord2[1] = 0;
      murottalRecord2[2] = 0;
      getfileFolderMurottal2(playPropertiesMurottal2[0], playPropertiesMurottal2[1], playPropertiesMurottal2[2], playPropertiesMurottal2[3], playPropertiesMurottal2[6]);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/auto2Setting\";}, 1500);</script></body></html>"); 
    }

    else if(request->hasParam("auto3SettingCB")){
      if (request->hasParam("firstSurah")) {
        inputMessage = request->getParam("firstSurah")->value();
        playPropertiesMurottal3[0] = inputMessage.toInt();
      }
      if (request->hasParam("firstAyat")) {
        inputMessage = request->getParam("firstAyat")->value();
        playPropertiesMurottal3[1] = inputMessage.toInt();
      }
      if (request->hasParam("lastSurah")) {
        inputMessage = request->getParam("lastSurah")->value();
        playPropertiesMurottal3[2] = inputMessage.toInt();
      }
      if (request->hasParam("lastAyat")) {
        inputMessage = request->getParam("lastAyat")->value();
        playPropertiesMurottal3[3] = inputMessage.toInt();
      }
      if (request->hasParam("metode")) {
        inputMessage = request->getParam("metode")->value();
        playPropertiesMurottal3[4] = inputMessage.toInt();
      }
      if (request->hasParam("repeatNum")) {
        inputMessage = request->getParam("repeatNum")->value();
        playPropertiesMurottal3[5] = inputMessage.toInt();
      }
      if (request->hasParam("qari")) {
        inputMessage = request->getParam("qari")->value();
        playPropertiesMurottal3[6] = inputMessage.toInt();
      }
      if (request->hasParam("waktuPutar")) {
        inputMessage = request->getParam("waktuPutar")->value();
        playPropertiesMurottal3[7] = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/playPropertiesMurottal3.txt", "w");
      String abc = String(playPropertiesMurottal3[0]);
      abc += "\n" + String(playPropertiesMurottal3[1]);
      abc += "\n" + String(playPropertiesMurottal3[2]);
      abc += "\n" + String(playPropertiesMurottal3[3]);
      abc += "\n" + String(playPropertiesMurottal3[4]);
      abc += "\n" + String(playPropertiesMurottal3[5]);
      abc += "\n" + String(playPropertiesMurottal3[6]);
      abc += "\n" + String(playPropertiesMurottal3[7]);
      fileToWrite.println(abc);
      fileToWrite.close();

      File fileToWrite2 = SPIFFS.open("/Murottal3Record.txt", "w");
      abc = "99\n0\n0";
      fileToWrite2.println(abc);
      fileToWrite2.close();
    
      murottalRecord3[0] = 99;
      murottalRecord3[1] = 0;
      murottalRecord3[2] = 0;
      getfileFolderMurottal3(playPropertiesMurottal3[0], playPropertiesMurottal3[1], playPropertiesMurottal3[2], playPropertiesMurottal3[3], playPropertiesMurottal3[6]);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/auto3Setting\";}, 1500);</script></body></html>");   
    }

    else if(request->hasParam("koordinat") & request->hasParam("timezone")){
      if (request->hasParam("volume")) {
        inputMessage = request->getParam("volume")->value();
        volume = inputMessage.toInt();
      }
      if (request->hasParam("volume2")) {
        inputMessage = request->getParam("volume2")->value();
        volume2 = inputMessage.toInt();
      }
      if (request->hasParam("brightness")) {
        inputMessage = request->getParam("brightness")->value();
        brightness = inputMessage.toInt();
      }
      if (request->hasParam("koordinat")) {
        inputMessage = request->getParam("koordinat")->value();
        dataWS.koordinatNum = inputMessage.toInt();
      }
      if (request->hasParam("timezone")) {
        inputMessage = request->getParam("timezone")->value();
        dataWS._timezone1 = inputMessage.toInt();
      }
      if (request->hasParam("koreksiImsak")) {
        inputMessage = request->getParam("koreksiImsak")->value();
        dataWS.koreksi_imsak = inputMessage.toInt();
      }
      if (request->hasParam("koreksiSubuh")) {
        inputMessage = request->getParam("koreksiSubuh")->value();
        dataWS.koreksi_subuh = inputMessage.toInt();
      }
      if (request->hasParam("koreksiTerbit")) {
        inputMessage = request->getParam("koreksiTerbit")->value();
        dataWS.koreksi_terbit = inputMessage.toInt();
      }
      if (request->hasParam("koreksiDhuha")) {
        inputMessage = request->getParam("koreksiDhuha")->value();
        dataWS.koreksi_dhuha = inputMessage.toInt();
      }
      if (request->hasParam("koreksiZuhur")) {
        inputMessage = request->getParam("koreksiZuhur")->value();
        dataWS.koreksi_zuhur = inputMessage.toInt();
      }
      if (request->hasParam("koreksiAsar")) {
        inputMessage = request->getParam("koreksiAsar")->value();
        dataWS.koreksi_ashar = inputMessage.toInt();
      }
      if (request->hasParam("koreksiMagrib")) {
        inputMessage = request->getParam("koreksiMagrib")->value();
        dataWS.koreksi_magrib = inputMessage.toInt();
      }
      if (request->hasParam("koreksiIsya")) {
        inputMessage = request->getParam("koreksiIsya")->value();
        dataWS.koreksi_isya = inputMessage.toInt();
      }
      if (request->hasParam("sudutSubuh")) {
        inputMessage = request->getParam("sudutSubuh")->value();
        dataWS._sudut_subuh = inputMessage.toInt();
      }
      if (request->hasParam("sudutIsya")) {
        inputMessage = request->getParam("sudutIsya")->value();
        dataWS._sudut_isyak = inputMessage.toInt();
      } 
      if (request->hasParam("koreksiHijri")) {
        inputMessage = request->getParam("koreksiHijri")->value();
        dataWS.koreksi_hijri = inputMessage.toInt();
      }   
    
      File fileToWrite = SPIFFS.open("/koreksiWaktuSholat.txt", "w");
      String abc = String(dataWS.koreksi_imsak);
      abc += "\n" + String(dataWS.koreksi_subuh);
      abc += "\n" + String(dataWS.koreksi_terbit);
      abc += "\n" + String(dataWS.koreksi_dhuha);
      abc += "\n" + String(dataWS.koreksi_zuhur);
      abc += "\n" + String(dataWS.koreksi_ashar);
      abc += "\n" + String(dataWS.koreksi_terbenam);
      abc += "\n" + String(dataWS.koreksi_magrib);
      abc += "\n" + String(dataWS.koreksi_isya);
      abc += "\n" + String(dataWS._sudut_subuh);
      abc += "\n" + String(dataWS._sudut_isyak); 
      abc += "\n" + String(dataWS.koreksi_hijri);      
      fileToWrite.println(abc);
      fileToWrite.close();
      
      fileToWrite = SPIFFS.open("/volume.txt", "w");
      abc = String(volume); 
      fileToWrite.println(abc);
      fileToWrite.close();

      fileToWrite = SPIFFS.open("/volume2.txt", "w");
      abc = String(volume2); 
      fileToWrite.println(abc);
      fileToWrite.close();
      
      fileToWrite = SPIFFS.open("/brightness.txt", "w");
      abc = String(brightness); 
      fileToWrite.println(abc);
      fileToWrite.close();
      
      fileToWrite = SPIFFS.open("/koordinat.txt", "w");
      abc = String(dataWS.koordinatNum);
      abc += "\n" + String(dataWS._timezone1); 
      fileToWrite.println(abc);
      fileToWrite.close();
    
      getKoordinat();
      if(murottalState == true | murottalState1 == true | murottalState2 == true | murottalState3 == true){
        myDFPlayer.volume(volume2);  //Set volume value (0~30).
      }
      
      matrix.setBrightness(brightness*10);
      
      set_fajr_angle(dataWS._sudut_subuh);     // sudut waktu subuh
      set_isha_angle(dataWS._sudut_isyak);     // sudut waktu isyak
      count2 = 60000;
      count3 = millis();
      countTime(true);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/generalSetting\";}, 1500);</script></body></html>");   
    }
    
    else if(request->hasParam("time") & request->hasParam("date")){
      int minute1;
      int hour1;
      int date1;
      int month1;
      int year1;
      String dateChar;
      String timeChar;
      char monthChar[][4] = {"Jan","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Des"};
      if (request->hasParam("time")) {
        inputMessage = request->getParam("time")->value();
        hour1 = inputMessage.substring(0,2).toInt();
        minute1 = inputMessage.substring(3,5).toInt();
      }
      if (request->hasParam("date")) {
        inputMessage = request->getParam("date")->value();
        year1 = getValue(inputMessage, '-', 0).toInt();
        month1 = getValue(inputMessage, '-', 1).toInt();
        date1 = getValue(inputMessage, '-', 2).toInt();
      }
      char userTime[9];
      char userDate[12];
      userTime[0] = hour1 / 10 + '0';
      userTime[1] = hour1 % 10 + '0';
      userTime[2] = ':';
      userTime[3] = minute1 / 10 + '0';
      userTime[4] = minute1 % 10 + '0';
      userTime[5] = ':';
      userTime[6] = '3';
      userTime[7] = '0';
      userDate[0] = monthChar[month1][0];
      userDate[1] = monthChar[month1][1];
      userDate[2] = monthChar[month1][2];
      userDate[3] = ' ';
      userDate[4] = date1 / 10 + '0';
      userDate[5] = date1 % 10 + '0';
      userDate[6] = ' ';
      userDate[7] = year1 / 1000 + '0';
      userDate[8] = year1 % 1000 / 100 + '0';
      userDate[9] = year1 % 1000 % 100 / 10 + '0';
      userDate[10] = year1 % 1000 % 100 % 10 + '0';
      RtcDateTime manual = RtcDateTime(userDate, userTime);
      Rtc.SetDateTime(manual);
      count3 = millis();
      countTime(true);
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/timeSetting\";}, 1500);</script></body></html>");  
    }

    else if(request->hasParam("wifiName")){
      if (request->hasParam("wifiName")) {
        inputMessage = request->getParam("wifiName")->value();
        APssid = inputMessage;
      }
      if (request->hasParam("wifiPassword")) {
        inputMessage = request->getParam("wifiPassword")->value();
        APpassword = inputMessage;
      }
      if (request->hasParam("loginName")) {
        inputMessage = request->getParam("loginName")->value();
        http_username = inputMessage;
      }
      if (request->hasParam("loginPassword")) {
        inputMessage = request->getParam("loginPassword")->value();
        http_password = inputMessage;
      }
    
      File fileToWrite = SPIFFS.open("/wifiPassword.txt", "w");
      String abc = APssid;
      abc += "\n" + APpassword;
      abc += "\n" + http_username;
      abc += "\n" + http_password; 
      fileToWrite.println(abc);
      fileToWrite.close();
    
      WiFi.softAP(APssid.c_str(), APpassword.c_str());
      
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/wifiSetting\";}, 1500);</script></body></html>"); 
    }

    else if(request->hasParam("alarmSettingCB")){
      timeAlarm.sahur = 999;
      timeAlarm.subuh = 999;
      timeAlarm.zuhur = 999;
      timeAlarm.asar = 999;
      timeAlarm.magrib = 999;
      timeAlarm.isya = 999;
      timeAlarm.tahajjud[0] = 99;
      timeAlarm.tahajjud[1] = 99;
      timeAlarm.custom1[0] = 99;
      timeAlarm.custom1[1] = 99;
      timeAlarm.custom2[0] = 99;
      timeAlarm.custom2[1] = 99;
      timeAlarm.custom3[0] = 99;
      timeAlarm.custom3[1] = 99;
      timeAlarm.custom4[0] = 99;
      timeAlarm.custom4[1] = 99;
      timeAlarm.custom5[0] = 99;
      timeAlarm.custom5[1] = 99;
      timeAlarm.adzan = false;
      timeAlarm.countdown = false;
      alarmWithMurottal.subuh = 0;
      alarmWithMurottal.zuhur = 0;
      alarmWithMurottal.asar = 0;
      alarmWithMurottal.magrib = 0;
      alarmWithMurottal.isya = 0;
      alarmWithMurottal.custom1 = 0;
      alarmWithMurottal.custom2 = 0;
      alarmWithMurottal.custom3 = 0;
      alarmWithMurottal.custom4 = 0;
      alarmWithMurottal.custom5 = 0;
      if (request->hasParam("countdownCB")) {
        inputMessage = request->getParam("countdownCB")->value();
        if(inputMessage.toInt()==1){
          timeAlarm.countdown = true;
        }
      }
      if (request->hasParam("adzanCB")) {
        inputMessage = request->getParam("adzanCB")->value();
        if(inputMessage.toInt()==1){
          timeAlarm.adzan = true;
        }
      }
      if (request->hasParam("timeSahur")) {
        inputMessage = request->getParam("timeSahur")->value();
        timeAlarm.sahur = inputMessage.toInt();
      }
      if (request->hasParam("timeSubuh")) {
        inputMessage = request->getParam("timeSubuh")->value();
        timeAlarm.subuh = inputMessage.toInt();
      }
      if (request->hasParam("timeZuhur")) {
        inputMessage = request->getParam("timeZuhur")->value();
        timeAlarm.zuhur = inputMessage.toInt();
      }
      if (request->hasParam("timeAsar")) {
        inputMessage = request->getParam("timeAsar")->value();
        timeAlarm.asar = inputMessage.toInt();
      }
      if (request->hasParam("timeMagrib")) {
        inputMessage = request->getParam("timeMagrib")->value();
        timeAlarm.magrib = inputMessage.toInt();
      }
      if (request->hasParam("timeIsya")) {
        inputMessage = request->getParam("timeIsya")->value();
        timeAlarm.isya = inputMessage.toInt();
      }
      if (request->hasParam("timeTahajjud")) {
        inputMessage = request->getParam("timeTahajjud")->value();
        timeAlarm.tahajjud[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.tahajjud[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("timeCustom1")) {
        inputMessage = request->getParam("timeCustom1")->value();
        timeAlarm.custom1[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.custom1[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("timeCustom2")) {
        inputMessage = request->getParam("timeCustom2")->value();
        timeAlarm.custom2[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.custom2[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("timeCustom3")) {
        inputMessage = request->getParam("timeCustom3")->value();
        timeAlarm.custom3[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.custom3[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("timeCustom4")) {
        inputMessage = request->getParam("timeCustom4")->value();
        timeAlarm.custom4[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.custom4[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("timeCustom5")) {
        inputMessage = request->getParam("timeCustom5")->value();
        timeAlarm.custom5[0] = getValue(inputMessage, ':', 0).toInt();
        timeAlarm.custom5[1] = getValue(inputMessage, ':', 1).toInt();
      }
      if (request->hasParam("murottalSubuh")) {
        inputMessage = request->getParam("murottalSubuh")->value();
        alarmWithMurottal.subuh = inputMessage.toInt();
      }
      if (request->hasParam("murottalZuhur")) {
        inputMessage = request->getParam("murottalZuhur")->value();
        alarmWithMurottal.zuhur = inputMessage.toInt();
      }
      if (request->hasParam("murottalAsar")) {
        inputMessage = request->getParam("murottalAsar")->value();
        alarmWithMurottal.asar = inputMessage.toInt();
      }
      if (request->hasParam("murottalMagrib")) {
        inputMessage = request->getParam("murottalMagrib")->value();
        alarmWithMurottal.magrib = inputMessage.toInt();
      }
      if (request->hasParam("murottalIsya")) {
        inputMessage = request->getParam("murottalIsya")->value();
        alarmWithMurottal.isya = inputMessage.toInt();
      }
      if (request->hasParam("murottalCustom1")) {
        inputMessage = request->getParam("murottalCustom1")->value();
        alarmWithMurottal.custom1 = inputMessage.toInt();
      }
      if (request->hasParam("murottalCustom2")) {
        inputMessage = request->getParam("murottalCustom2")->value();
        alarmWithMurottal.custom2 = inputMessage.toInt();
      }
      if (request->hasParam("murottalCustom3")) {
        inputMessage = request->getParam("murottalCustom3")->value();
        alarmWithMurottal.custom3 = inputMessage.toInt();
      }
      if (request->hasParam("murottalCustom4")) {
        inputMessage = request->getParam("murottalCustom4")->value();
        alarmWithMurottal.custom4 = inputMessage.toInt();
      }
      if (request->hasParam("murottalCustom5")) {
        inputMessage = request->getParam("murottalCustom5")->value();
        alarmWithMurottal.custom5 = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/timeAlarm.txt", "w");
      String stringTemp = String(timeAlarm.sahur);
      stringTemp += "\n" + String(timeAlarm.subuh);
      stringTemp += "\n" + String(timeAlarm.zuhur);
      stringTemp += "\n" + String(timeAlarm.asar);
      stringTemp += "\n" + String(timeAlarm.magrib);
      stringTemp += "\n" + String(timeAlarm.isya);
      stringTemp += "\n" + String(timeAlarm.tahajjud[0]/10) + String(timeAlarm.tahajjud[0]%10);
      stringTemp += ":" + String(timeAlarm.tahajjud[1]/10) + String(timeAlarm.tahajjud[1]%10);
      stringTemp += "\n" + String(timeAlarm.custom1[0]/10) + String(timeAlarm.custom1[0]%10);
      stringTemp += ":" + String(timeAlarm.custom1[1]/10) + String(timeAlarm.custom1[1]%10);
      stringTemp += "\n" + String(timeAlarm.custom2[0]/10) + String(timeAlarm.custom2[0]%10);
      stringTemp += ":" + String(timeAlarm.custom2[1]/10) + String(timeAlarm.custom2[1]%10);
      stringTemp += "\n" + String(timeAlarm.custom3[0]/10) + String(timeAlarm.custom3[0]%10);
      stringTemp += ":" + String(timeAlarm.custom3[1]/10) + String(timeAlarm.custom3[1]%10);
      stringTemp += "\n" + String(timeAlarm.custom4[0]/10) + String(timeAlarm.custom4[0]%10);
      stringTemp += ":" + String(timeAlarm.custom4[1]/10) + String(timeAlarm.custom4[1]%10);
      stringTemp += "\n" + String(timeAlarm.custom5[0]/10) + String(timeAlarm.custom5[0]%10);
      stringTemp += ":" + String(timeAlarm.custom5[1]/10) + String(timeAlarm.custom5[1]%10);
      String a = "0";
      if(timeAlarm.adzan==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(timeAlarm.countdown==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      fileToWrite.println(stringTemp);
      fileToWrite.close();
      
      fileToWrite = SPIFFS.open("/alarmWithMurottal.txt", "w");
      stringTemp = String(alarmWithMurottal.subuh);
      stringTemp += "\n" + String(alarmWithMurottal.zuhur);
      stringTemp += "\n" + String(alarmWithMurottal.asar);
      stringTemp += "\n" + String(alarmWithMurottal.magrib);
      stringTemp += "\n" + String(alarmWithMurottal.isya);
      stringTemp += "\n" + String(alarmWithMurottal.custom1);
      stringTemp += "\n" + String(alarmWithMurottal.custom2);
      stringTemp += "\n" + String(alarmWithMurottal.custom3);
      stringTemp += "\n" + String(alarmWithMurottal.custom4);
      stringTemp += "\n" + String(alarmWithMurottal.custom5);
      fileToWrite.println(stringTemp);
      fileToWrite.close();
    
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/alarmSetting\";}, 1500);</script></body></html>");  
    }

    else if(request->hasParam("iqamahSettingCB")){
      timeIqamah.subuh = 999;
      timeIqamah.zuhur = 999;
      timeIqamah.asar = 999;
      timeIqamah.magrib = 999;
      timeIqamah.isya = 999;
      if (request->hasParam("iqamahSubuh")) {
        inputMessage = request->getParam("iqamahSubuh")->value();
        timeIqamah.subuh = inputMessage.toInt();
      }
      if (request->hasParam("iqamahZuhur")) {
        inputMessage = request->getParam("iqamahZuhur")->value();
        timeIqamah.zuhur = inputMessage.toInt();
      }
      if (request->hasParam("iqamahAsar")) {
        inputMessage = request->getParam("iqamahAsar")->value();
        timeIqamah.asar = inputMessage.toInt();
      }
      if (request->hasParam("iqamahMagrib")) {
        inputMessage = request->getParam("iqamahMagrib")->value();
        timeIqamah.magrib = inputMessage.toInt();
      }
      if (request->hasParam("iqamahIsya")) {
        inputMessage = request->getParam("iqamahIsya")->value();
        timeIqamah.isya = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/timeIqamah.txt", "w");
      String stringTemp = String(timeIqamah.subuh);
      stringTemp += "\n" + String(timeIqamah.zuhur);
      stringTemp += "\n" + String(timeIqamah.asar);
      stringTemp += "\n" + String(timeIqamah.magrib);
      stringTemp += "\n" + String(timeIqamah.isya);
      fileToWrite.println(stringTemp);
      fileToWrite.close();
    
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/iqamahSetting\";}, 1500);</script></body></html>");  
    }

    else if(request->hasParam("sahurSettingCB")){
      sahurStat.sahurSyawal = false;
      sahurStat.sahurZulhijah = false;
      sahurStat.sahurRamadan = false;
      sahurStat.sahurAsyuro = false;
      sahurStat.sahurAyamulbidh = false;
      sahurStat.sahurSenin = false;
      sahurStat.sahurKamis = false;
      sahurStat.sahurDaud = false;
      sahurStat.sahurTasua = false;
      sahurStat.sahurArafah = false;
      sahurStat.awalSyawal = 2;
      if (request->hasParam("syawalCB")) {
        inputMessage = request->getParam("syawalCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurSyawal = true;
        }
      }
      if (request->hasParam("zulhijahCB")) {
        inputMessage = request->getParam("zulhijahCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurZulhijah = true;
        }
      }
      if (request->hasParam("ramadanCB")) {
        inputMessage = request->getParam("ramadanCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurRamadan = true;
        }
      }
      if (request->hasParam("asyuroCB")) {
        inputMessage = request->getParam("asyuroCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurAsyuro = true;
        }
      }
      if (request->hasParam("ayamulbidhCB")) {
        inputMessage = request->getParam("ayamulbidhCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurAyamulbidh = true;
        }
      }
      if (request->hasParam("seninCB")) {
        inputMessage = request->getParam("seninCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurSenin = true;
        }
      }
      if (request->hasParam("kamisCB")) {
        inputMessage = request->getParam("kamisCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurKamis = true;
        }
      }
      if (request->hasParam("daudCB")) {
        inputMessage = request->getParam("daudCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurDaud = true;
        }
      }
      if (request->hasParam("tasuaCB")) {
        inputMessage = request->getParam("tasuaCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurTasua = true;
        }
      }
      if (request->hasParam("arafahCB")) {
        inputMessage = request->getParam("arafahCB")->value();
        if(inputMessage.toInt()==1){
          sahurStat.sahurArafah = true;
        }
      }
      if (request->hasParam("awalSyawal")) {
        inputMessage = request->getParam("awalSyawal")->value();
        sahurStat.awalSyawal = inputMessage.toInt();
      }
    
      File fileToWrite = SPIFFS.open("/sahurStat.txt", "w");
      String stringTemp = "";
      String a = "0";
      if(sahurStat.sahurSyawal==true){
        a = "1";
      }
      stringTemp = a;
      a = "0";
      if(sahurStat.sahurZulhijah==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurRamadan==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurAsyuro==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurAyamulbidh==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurSenin==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurKamis==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurDaud==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurTasua==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      a = "0";
      if(sahurStat.sahurArafah==true){
        a = "1";
      }
      stringTemp += "\n" + a;
      stringTemp += "\n" + String(sahurStat.awalSyawal); 
      fileToWrite.println(stringTemp);
      fileToWrite.close();
    
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/sahurSetting\";}, 1500);</script></body></html>");
    }

    else if(request->hasParam("lisensi")){
      if (request->hasParam("lisensi")) {
        inputMessage = request->getParam("lisensi")->value();
        lisensiSaveString = inputMessage;
      }
          
      File fileToWrite = SPIFFS.open("/lisensiSave.txt", "w");
      String abc = lisensiSaveString;
      fileToWrite.println(abc);
      fileToWrite.close();
          
      calculateLisensi();
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/lisensiSetting\";}, 1500);</script></body></html>"); 
    }

    else if(request->hasParam("hackhackhackambyarambyar")){
      if (request->hasParam("hackhackhackambyarambyar")) {
        inputMessage = request->getParam("hackhackhackambyarambyar")->value();
        limitTrial = inputMessage.toInt();
      }
      if (request->hasParam("murottalDispStat")) {
        inputMessage = request->getParam("murottalDispStat")->value();
        dispStat[0] = inputMessage.toInt();
      }
      if (request->hasParam("DispStat")) {
        inputMessage = request->getParam("DispStat")->value();
        dispStat[1] = inputMessage.toInt();
      }
          
      File fileToWrite = SPIFFS.open("/limitTrial.txt", "w");
      String abc = String(limitTrial);
      fileToWrite.println(abc);
      fileToWrite.close();

      File fileToWrite2 = SPIFFS.open("/dispStat.txt", "w");
      String abc2 = String(dispStat[0]);
      abc2 += "\n" + String(dispStat[1]);
      fileToWrite2.println(abc2);
      fileToWrite2.close();
          
      calculateLisensi();
      request->send(200, "text/html", "<!DOCTYPE HTML><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><body style=\"background-color: black; text-align:center; color:aqua; font-family: sans-serif;\"><p>Input berhasil. Mohon Tunggu... </p><script>setTimeout(function(){window.location.href =\"/hackhackhackambyar\";}, 1500);</script></body></html>"); 
    }
  });
  
  AsyncElegantOTA.begin(&server); 
  server.begin();
  
  set_calc_method(ISNA);  // metode perhitungan
  set_asr_method(Shafii); // madzhab?
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(dataWS._sudut_subuh);     // sudut waktu subuh
  set_isha_angle(dataWS._sudut_isyak);     // sudut waktu isyak
  
  matrix.addLayer(&backgroundLayer); 
  matrix.begin();
  matrix.setBrightness(brightness*10);
  backgroundLayer.fillScreen(defaultBackgroundColor);
  backgroundLayer.swapBuffers();
  
  milisRec = millis();
  
  countTime(true);

  getfileFolderMurottal(playPropertiesMurottal[0], playPropertiesMurottal[1], playPropertiesMurottal[2], playPropertiesMurottal[3], playPropertiesMurottal[6]);
  getfileFolderMurottal1(playPropertiesMurottal1[0], playPropertiesMurottal1[1], playPropertiesMurottal1[2], playPropertiesMurottal1[3], playPropertiesMurottal1[6]);
  getfileFolderMurottal2(playPropertiesMurottal2[0], playPropertiesMurottal2[1], playPropertiesMurottal2[2], playPropertiesMurottal2[3], playPropertiesMurottal2[6]);
  getfileFolderMurottal3(playPropertiesMurottal3[0], playPropertiesMurottal3[1], playPropertiesMurottal3[2], playPropertiesMurottal3[3], playPropertiesMurottal3[6]);

  pinMode(mp3Inp, INPUT_PULLUP);
  pinMode(alarmAckBtn, INPUT_PULLUP);

  attachInterrupt(alarmAckBtn, handleInterrupt1, CHANGE);
}

int zeroIfSubMin(int x, int y){
  int z = x - y;
  if(z<0){
    return 0;
  }
  else{
    return z;
  }
}

void animeRunningLineRev() {
  static int dotRunningLine[32][5];
  static int runningLineCountRec;
  runningLineCountRec++;
  if(runningLineCountRec>=runningLineCount){
    runningLineCountRec = 0;
    int a = random(0,2);
    for (int b=0; b<a; b++){
      int c = random(0,31);
      if(dotRunningLine[c][0]==0){
        dotRunningLine[c][0]=700;
        int d = random(1,3);
        dotRunningLine[c][1]=d;
        int b = random(30*d,80*d);
        dotRunningLine[c][2]=b;
        b = random(30*d,80*d);
        dotRunningLine[c][3]=b;
        b = random(30*d,80*d);
        dotRunningLine[c][4]=b;
      }
    }
  }
  
  for(int x=0; x<32; x++){
    if(dotRunningLine[x][0]!=0){
      dotRunningLine[x][0] = dotRunningLine[x][0] - dotRunningLine[x][1]*5;
      if(dotRunningLine[x][0]>0){        
         backgroundLayer.drawLine((dotRunningLine[x][0]/10), x, (dotRunningLine[x][0]/10)-dotRunningLine[x][1], x, {dotRunningLine[x][2], dotRunningLine[x][3], dotRunningLine[x][4]});
      }
      else{
        dotRunningLine[x][0] = 0;
      }
    }
  }
}

void animeRunningLineRev2() {
  static int dotRunningLine[32][5];
  static int runningLineCountRec;
  runningLineCountRec++;
  if(runningLineCountRec>=runningLineCount){
    runningLineCountRec = 0;
    int a = random(0,2);
    for (int b=0; b<a; b++){
      int c = random(0,31);
      if(dotRunningLine[c][0]==0){
        dotRunningLine[c][0]=700;
        int d = random(1,3);
        dotRunningLine[c][1]=d;
        int b = random(30*d,80*d);
        dotRunningLine[c][2]=b;
        dotRunningLine[c][3]=b;
        dotRunningLine[c][4]=b;
      }
    }
  }
  
  for(int x=0; x<32; x++){
    if(dotRunningLine[x][0]!=0){
      dotRunningLine[x][0] = dotRunningLine[x][0] - dotRunningLine[x][1]*5;
      if(dotRunningLine[x][0]>0){        
         backgroundLayer.drawLine((dotRunningLine[x][0]/10), x, (dotRunningLine[x][0]/10)-dotRunningLine[x][1], x, {dotRunningLine[x][2], dotRunningLine[x][3], dotRunningLine[x][4]});
      }
      else{
        dotRunningLine[x][0] = 0;
      }
    }
  }
}

long g(int y, int m, int d) {
  //http://alcor.concordia.ca/~gpkatch/gdate-algorithm.html
  m = (m + 9) % 12;
  y = y - m / 10;
  return 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (d - 1);
}

void  CalcTomorrow(long g, int h) {
  g = g + h; // add one day

  int y = (10000 * g + 14780) / 3652425;
  int ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    if (ddd < 0) {
      y = y - 1;
      ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    }
    int mi = (100 * ddd + 52) / 3060;
    int mm = (mi + 2) % 12 + 1;
    y = y + (mi + 2) / 12;
    int dd = ddd - (mi * 306 + 5) / 10 + 1;

    tomorrow[0] = y + 2000;
    tomorrow[1] = mm;
    tomorrow[2] = dd;
}

void stopMurottal(){
  statInfoMurottalSpeech = 0;
  murottalPlayingStat[0] = 0;
  murottalPlayingStat[1] = 0;
  murottalState = false;
  murottalState1 = false;
  murottalState2 = false;
  murottalState3 = false;
  
  myDFPlayer.pause();
}

void sahurMoment(){ 
  long b = g(timeNow.yearNow, timeNow.monthNow, timeNow.dayNow);
  long c = b%2;
  if(timeAlarm.sahur!=999 & masukWaktuSholat==false & menjelangWaktuSholat==false & !(
    (timeNow.monthH==10 & timeNow.dayH==1) | (timeNow.monthH==12 & (timeNow.dayH==10 | timeNow.dayH==11 | timeNow.dayH==12 | timeNow.dayH==13))
    )){
    if(timeMin.Imsak-timeMin.Now<=timeAlarm.sahur & timeMin.Imsak-timeMin.Now>10){
      if(sahurStat.sahurRamadan==true & timeNow.monthH==9){
        sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[0]==0){
          stopMurottal();
          sahurMomentStat[0]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[0]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[0]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 23;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[0]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[0]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurDaud==true & c==0){
        sahurMomentStat[0] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[1]==0){
          stopMurottal();
          sahurMomentStat[1]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[1]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[1]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 27;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[1]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[1]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurSyawal==true & timeNow.monthH==10 & timeNow.dayH>=sahurStat.awalSyawal & timeNow.dayH<sahurStat.awalSyawal+6){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[2]==0){
          stopMurottal();
          sahurMomentStat[2]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[2]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[2]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 29;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[2]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[2]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurZulhijah==true & timeNow.monthH==12 & timeNow.dayH<9){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[3]==0){
          stopMurottal();
          sahurMomentStat[3]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[3]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[3]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 33;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[3]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[3]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurArafah==true & timeNow.monthH==12 & timeNow.dayH==9){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[4]==0){
          stopMurottal();
          sahurMomentStat[4]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[4]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[4]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 31;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[4]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[4]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurTasua==true & timeNow.monthH==1 & timeNow.dayH==9){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[5]==0){
          stopMurottal();
          sahurMomentStat[5]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[5]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[5]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 30;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[5]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[5]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurAsyuro==true & timeNow.monthH==1 & timeNow.dayH==10){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[6]==0){
          stopMurottal();
          sahurMomentStat[6]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[6]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[6]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 24;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[6]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[6]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurAyamulbidh==true & (timeNow.dayH==13 | timeNow.dayH==14 | timeNow.dayH==15)){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[7]==0){
          stopMurottal();
          sahurMomentStat[7]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[7]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[7]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 25;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[7]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[7]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurSenin==true & timeNow.dowNow==1){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[9] = 0;
        if(sahurMomentStat[8]==0){
          stopMurottal();
          sahurMomentStat[8]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[8]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[8]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 28;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[8]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[8]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else if(sahurStat.sahurKamis==true & timeNow.dowNow==4){
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0;
        if(sahurMomentStat[9]==0){
          stopMurottal();
          sahurMomentStat[9]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[9]==1 & murottalPlayingStat[2]==3){
          sahurMomentStat[9]=2;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 26;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
        else if(sahurMomentStat[9]==2 & murottalPlayingStat[2]==3){
          sahurMomentStat[9]=1;
          if(dispStat[0]==1){
            murottalPlayingStat[2]=1;        
            murottalPlayingStat[7] = 1;
            murottalPlayingStat[8] = 32;
            myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
            countMp3Failed1 = 0;
          }          
        }
      }
      else{
        sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0; 
      }
    }
    else{
      sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0; 
    }
  }
  else{
    sahurMomentStat[0] = 0; sahurMomentStat[1] = 0; sahurMomentStat[2] = 0; sahurMomentStat[3] = 0; sahurMomentStat[4] = 0; sahurMomentStat[5] = 0; sahurMomentStat[6] = 0; sahurMomentStat[7] = 0; sahurMomentStat[8] = 0; sahurMomentStat[9] = 0; 
  }

  if(sahurMomentStat[0]==1 | sahurMomentStat[0] == 2 | sahurMomentStat[1]==1 | sahurMomentStat[1] == 2 | sahurMomentStat[2]==1 | sahurMomentStat[2] == 2 | sahurMomentStat[3]==1 | sahurMomentStat[3] == 2 | sahurMomentStat[4]==1 | sahurMomentStat[4] == 2 | 
     sahurMomentStat[5]==1 | sahurMomentStat[5] == 2 | sahurMomentStat[6]==1 | sahurMomentStat[6] == 2 | sahurMomentStat[7]==1 | sahurMomentStat[7] == 2 | sahurMomentStat[8]==1 | sahurMomentStat[8] == 2 | sahurMomentStat[9]==1 | sahurMomentStat[9] == 2){
    alarmSahur = true;
    }
  else{
    alarmSahur = false;
  }
}

void tahajjudMoment(){
  if(timeAlarm.tahajjud[0]!=99 & masukWaktuSholat==false & menjelangWaktuSholat==false & alarmSahur==false){
    int tahajjudMin = timeAlarm.tahajjud[0] * 60 + timeAlarm.tahajjud[1];
    if(tahajjudMin-timeMin.Now<=0 & tahajjudMin-timeMin.Now>-30){
      if(tahajjudMomentStat==0){
        stopMurottal();
        tahajjudMomentStat=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;          
        }
      }
      else if(tahajjudMomentStat==1 & murottalPlayingStat[2]==3){
        tahajjudMomentStat=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 34;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;       
        }
      }
      else if(tahajjudMomentStat==2 & murottalPlayingStat[2]==3){
        tahajjudMomentStat=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;   
        }
      }
    }
    else{
      tahajjudMomentStat=0;
    }
  }
  else{
    tahajjudMomentStat=0;
  }

  if(tahajjudMomentStat==1 | tahajjudMomentStat==2){
    alarmTahajjud = true;
  }
  else{
    alarmTahajjud = false;
  }
}

void woMurottalMoment(){
  if((timeAlarm.custom1[0]!=99 | timeAlarm.custom2[0]!=99) & masukWaktuSholat==false & menjelangWaktuSholat==false & alarmSahur==false & alarmTahajjud==false){
    int custom1Min = timeAlarm.custom1[0] * 60 + timeAlarm.custom1[1];
    int custom2Min = timeAlarm.custom2[0] * 60 + timeAlarm.custom2[1];
    if(timeAlarm.custom1[0]!=99 & custom1Min-timeMin.Now<=0 & custom1Min-timeMin.Now>-30){
      WOmurottalMomentStat[1]=0;
      if(WOmurottalMomentStat[0]==0){
        stopMurottal();
        WOmurottalMomentStat[0]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;        
        }
      }
      else if(WOmurottalMomentStat[0]==1 & murottalPlayingStat[2]==3){
        WOmurottalMomentStat[0]=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 35;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;   
        }
      }
      else if(WOmurottalMomentStat[0]==2 & murottalPlayingStat[2]==3){
        WOmurottalMomentStat[0]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(timeAlarm.custom1[1]!=99 & custom2Min-timeMin.Now<=0 & custom2Min-timeMin.Now>=-30){
      WOmurottalMomentStat[0]=0;
      if(WOmurottalMomentStat[1]==0){
        stopMurottal();
        WOmurottalMomentStat[1]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WOmurottalMomentStat[1]==1 & murottalPlayingStat[2]==3){
        WOmurottalMomentStat[1]=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 36;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WOmurottalMomentStat[1]==2 & murottalPlayingStat[2]==3){
        WOmurottalMomentStat[1]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else{
      WOmurottalMomentStat[0]=0;
      WOmurottalMomentStat[1]=0;
    }
  }
  else{
    WOmurottalMomentStat[0]=0;
    WOmurottalMomentStat[1]=0;
  }

  if(WOmurottalMomentStat[0]==1 | WOmurottalMomentStat[0]==2 | WOmurottalMomentStat[1]==1 | WOmurottalMomentStat[1]==2){
    alarmWOmurottal = true;
  }
  else{
    alarmWOmurottal = false;
  }
}

void withMurottalMoment(){
  if((timeAlarm.custom3[0]!=99 | timeAlarm.custom4[0]!=99 | timeAlarm.custom5[0]!=99) & masukWaktuSholat==false & menjelangWaktuSholat==false & alarmSahur==false & alarmTahajjud==false & alarmWOmurottal==false){
    int custom3Min = timeAlarm.custom3[0] * 60 + timeAlarm.custom3[1];
    int custom4Min = timeAlarm.custom4[0] * 60 + timeAlarm.custom4[1];
    int custom5Min = timeAlarm.custom5[0] * 60 + timeAlarm.custom5[1];
    int a;
    int b;
    int c;
    switch (alarmWithMurottal.custom3){
      case 1:
        a = playPropertiesMurottal1[7];
        break;
      case 2:
        a = playPropertiesMurottal2[7];
        break;
      case 3:
        a = playPropertiesMurottal3[7];
        break;
    }
    switch (alarmWithMurottal.custom4){
      case 1:
        b = playPropertiesMurottal1[7];
        break;
      case 2:
        b = playPropertiesMurottal2[7];
        break;
      case 3:
        b = playPropertiesMurottal3[7];
        break;
    }
    switch (alarmWithMurottal.custom5){
      case 1:
        c = playPropertiesMurottal1[7];
        break;
      case 2:
        c = playPropertiesMurottal2[7];
        break;
      case 3:
        c = playPropertiesMurottal3[7];
        break;
    }
    if(timeAlarm.custom3[0]!=99 & timeMin.Now-custom3Min>=0 & timeMin.Now-custom3Min<a){
      WithmurottalMomentStat[1]=0; WithmurottalMomentStat[2]=0;
      if(WithmurottalMomentStat[0]==0){
        stopMurottal();
        WithmurottalMomentStat[0]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[0]==1 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[0]=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 37;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[0]==2 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[0]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.custom3){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else if(timeAlarm.custom4[0]!=99 & timeMin.Now-custom4Min>=0 & timeMin.Now-custom4Min<b){
      WithmurottalMomentStat[0]=0; WithmurottalMomentStat[2]=0;
      if(WithmurottalMomentStat[1]==0){
        stopMurottal();
        WithmurottalMomentStat[1]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[1]==1 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[1]=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 38;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[1]==2 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[1]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.custom4){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else if(timeAlarm.custom5[0]!=99 & timeMin.Now-custom5Min>=0 & timeMin.Now-custom5Min<c){
      WithmurottalMomentStat[0]=0; WithmurottalMomentStat[1]=0;
      if(WithmurottalMomentStat[2]==0){
        stopMurottal();
        WithmurottalMomentStat[2]=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[2]==1 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[2]=2;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 39;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(WithmurottalMomentStat[2]==2 & murottalPlayingStat[2]==3){
        WithmurottalMomentStat[2]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.custom5){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      WithmurottalMomentStat[0]=0;
      WithmurottalMomentStat[1]=0;
      WithmurottalMomentStat[2]=0;
    }
  }
  else{
    WithmurottalMomentStat[0]=0;
    WithmurottalMomentStat[1]=0;
    WithmurottalMomentStat[2]=0;
  }

  if((WithmurottalMomentStat[0]>=1 & WithmurottalMomentStat[0]<=3) | (WithmurottalMomentStat[1]>=1 & WithmurottalMomentStat[1]<=3) | (WithmurottalMomentStat[2]>=1 & WithmurottalMomentStat[2]<=3)){
    alarmWithmurottal = true;
  }
  else{
    alarmWithmurottal = false;
  }
}

void masukWaktuSholatMoment(){
  if(timeMin.Now-timeMin.Imsak==0){
    masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0; 
    if(masukWaktuSholatStat[0]==0){
      stopMurottal();
      masukWaktuSholatStat[0]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;      
      }
    }
    else if(masukWaktuSholatStat[0]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[0]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 22;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;    
      }
    }
  }
  else if(timeMin.Now-timeMin.Subuh>=0 & timeMin.Now-timeMin.Subuh<13+timeIqamah.subuh){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[1]==0){ 
      stopMurottal();
      masukWaktuSholatStat[1]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;  
      }
    }
    else if(masukWaktuSholatStat[1]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[1]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 15;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[1]==2 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[1]+=1;
      if(dispStat[0]==1){
        if(timeAlarm.adzan==true & timeMin.Now-timeMin.Subuh<3){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 50;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(((masukWaktuSholatStat[1]==3 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[1]==20) & timeMin.Now-timeMin.Subuh==3+timeIqamah.subuh){
      masukWaktuSholatStat[1]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if((masukWaktuSholatStat[1]==4 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[1]==21){
      masukWaktuSholatStat[1]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 52;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Syuruk==0){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[2]==0){ 
      stopMurottal();
      masukWaktuSholatStat[2]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[2]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[2]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 16;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Dhuha==0){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[3]==0){ 
      stopMurottal();
      masukWaktuSholatStat[3]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[3]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[3]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 17;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Zuhur>=0 & timeMin.Now-timeMin.Zuhur<13+timeIqamah.zuhur){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[4]==0){ 
      stopMurottal();
      masukWaktuSholatStat[4]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[4]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[4]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 18;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[4]==2 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[4]+=1;
      if(dispStat[0]==1){
        if(timeAlarm.adzan==true & timeMin.Now-timeMin.Zuhur<3){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 51;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(((masukWaktuSholatStat[4]==3 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[4]==20) & timeMin.Now-timeMin.Zuhur==3+timeIqamah.zuhur){
      masukWaktuSholatStat[4]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if((masukWaktuSholatStat[4]==4 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[4]==21){
      masukWaktuSholatStat[4]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 52;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Asar>=0 & timeMin.Now-timeMin.Asar<13+timeIqamah.asar){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[5]==0){ 
      stopMurottal();
      masukWaktuSholatStat[5]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[5]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[5]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 19;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[5]==2 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[5]+=1;
      if(dispStat[0]==1){
        if(timeAlarm.adzan==true & timeMin.Now-timeMin.Asar<3){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 51;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(((masukWaktuSholatStat[5]==3 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[5]==20) & timeMin.Now-timeMin.Asar==3+timeIqamah.asar){
      masukWaktuSholatStat[5]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if((masukWaktuSholatStat[5]==4 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[5]==21){
      masukWaktuSholatStat[5]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 52;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Magrib>=0 & timeMin.Now-timeMin.Magrib<13+timeIqamah.magrib){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[7]=0;
    if(masukWaktuSholatStat[6]==0){ 
      stopMurottal();
      masukWaktuSholatStat[6]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[6]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[6]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 20;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[6]==2 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[6]+=1;
      if(dispStat[0]==1){
        if(timeAlarm.adzan==true & timeMin.Now-timeMin.Magrib<3){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 51;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(((masukWaktuSholatStat[6]==3 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[6]==20) & timeMin.Now-timeMin.Magrib==3+timeIqamah.magrib){
      masukWaktuSholatStat[6]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if((masukWaktuSholatStat[6]==4 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[6]==21){
      masukWaktuSholatStat[6]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 52;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else if(timeMin.Now-timeMin.Isya>=0 & timeMin.Now-timeMin.Isya<13+timeIqamah.isya){
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0;
    if(masukWaktuSholatStat[7]==0){ 
      stopMurottal();
      masukWaktuSholatStat[7]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[7]==1 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[7]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 21;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if(masukWaktuSholatStat[7]==2 & murottalPlayingStat[2]==3){
      masukWaktuSholatStat[7]+=1;
      if(dispStat[0]==1){
        if(timeAlarm.adzan==true & timeMin.Now-timeMin.Isya<3){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 51;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
    }
    else if(((masukWaktuSholatStat[7]==3 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[7]==20) & timeMin.Now-timeMin.Isya==3+timeIqamah.isya){
      masukWaktuSholatStat[7]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 32;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
    else if((masukWaktuSholatStat[7]==4 & murottalPlayingStat[2]==3) | masukWaktuSholatStat[7]==21){
      masukWaktuSholatStat[7]+=1;
      if(dispStat[0]==1){
        murottalPlayingStat[2]=1;        
        murottalPlayingStat[7] = 1;
        murottalPlayingStat[8] = 52;
        myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
        countMp3Failed1 = 0;
      }
    }
  }
  else{
    masukWaktuSholatStat[0]=0; masukWaktuSholatStat[1]=0; masukWaktuSholatStat[2]=0; masukWaktuSholatStat[3]=0; masukWaktuSholatStat[4]=0; masukWaktuSholatStat[5]=0; masukWaktuSholatStat[6]=0; masukWaktuSholatStat[7]=0; 
  }

  if(masukWaktuSholatStat[0]==1 | masukWaktuSholatStat[0]==2 | (masukWaktuSholatStat[1]>=1 & masukWaktuSholatStat[1]<=3) | 
     masukWaktuSholatStat[2]==1 | masukWaktuSholatStat[2]==2 | masukWaktuSholatStat[3]==1 | masukWaktuSholatStat[3]==2 | 
     (masukWaktuSholatStat[4]>=1 & masukWaktuSholatStat[4]<=3) | (masukWaktuSholatStat[5]>=1 & masukWaktuSholatStat[5]<=3) | 
     (masukWaktuSholatStat[6]>=1 & masukWaktuSholatStat[6]<=3) | (masukWaktuSholatStat[7]>=1 & masukWaktuSholatStat[7]<=3) ){
    masukWaktuSholat = true;
    }
  else{
    masukWaktuSholat = false;
  }
}

void menjelangSholatMoment(){
  if(masukWaktuSholat==false & timeAlarm.subuh!=999 & timeMin.Subuh-timeMin.Now<=timeAlarm.subuh & timeMin.Subuh-timeMin.Now>0){
    menjelangWaktuSholatStat[1]=0; menjelangWaktuSholatStat[2]=0; menjelangWaktuSholatStat[3]=0; menjelangWaktuSholatStat[4]=0; 
    if(timeMin.Subuh-timeMin.Now==timeAlarm.subuh){
      if(menjelangWaktuSholatStat[0]==0){
        stopMurottal();
        menjelangWaktuSholatStat[0]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[0]==1 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[0]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          if(timeAlarm.subuh==5){
            murottalPlayingStat[8] = 2;
          }
          else if(timeAlarm.subuh==10){
            murottalPlayingStat[8] = 3;
          }
          else if(timeAlarm.subuh==15){
            murottalPlayingStat[8] = 4;
          }
          else if(timeAlarm.subuh==20){
            murottalPlayingStat[8] = 5;
          }
          else if(timeAlarm.subuh==30){
            murottalPlayingStat[8] = 6;
          }
          else if(timeAlarm.subuh==45){
            murottalPlayingStat[8] = 7;
          }
          else if(timeAlarm.subuh==60){
            murottalPlayingStat[8] = 1;
          }
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[0]==2 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[0]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 15;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[0]==3 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[0]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.subuh){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      if(menjelangWaktuSholatStat[0]<4){
        stopMurottal();
        menjelangWaktuSholatStat[0]=4;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.subuh){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
  }
  else if(masukWaktuSholat==false & timeAlarm.zuhur!=999 & timeMin.Zuhur-timeMin.Now<=timeAlarm.zuhur & timeMin.Zuhur-timeMin.Now>0){
    menjelangWaktuSholatStat[0]=0; menjelangWaktuSholatStat[2]=0; menjelangWaktuSholatStat[3]=0; menjelangWaktuSholatStat[4]=0; 
    if(timeMin.Zuhur-timeMin.Now==timeAlarm.zuhur){
      if(menjelangWaktuSholatStat[1]==0){
        stopMurottal();
        menjelangWaktuSholatStat[1]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[1]==1 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[1]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          if(timeAlarm.zuhur==5){
            murottalPlayingStat[8] = 2;
          }
          else if(timeAlarm.zuhur==10){
            murottalPlayingStat[8] = 3;
          }
          else if(timeAlarm.zuhur==15){
            murottalPlayingStat[8] = 4;
          }
          else if(timeAlarm.zuhur==20){
            murottalPlayingStat[8] = 5;
          }
          else if(timeAlarm.zuhur==30){
            murottalPlayingStat[8] = 6;
          }
          else if(timeAlarm.zuhur==45){
            murottalPlayingStat[8] = 7;
          }
          else if(timeAlarm.zuhur==60){
            murottalPlayingStat[8] = 1;
          }
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[1]==2 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[1]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 18;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[1]==3 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[1]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.zuhur){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      if(menjelangWaktuSholatStat[1]<4){
        stopMurottal();
        menjelangWaktuSholatStat[1]=4;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.zuhur){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
  }
  else if(masukWaktuSholat==false & timeAlarm.asar!=999 & timeMin.Asar-timeMin.Now<=timeAlarm.asar & timeMin.Asar-timeMin.Now>0){
    menjelangWaktuSholatStat[0]=0; menjelangWaktuSholatStat[1]=0; menjelangWaktuSholatStat[3]=0; menjelangWaktuSholatStat[4]=0; 
    if(timeMin.Asar-timeMin.Now==timeAlarm.asar){
      if(menjelangWaktuSholatStat[2]==0){
        stopMurottal();
        menjelangWaktuSholatStat[2]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[2]==1 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[2]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          if(timeAlarm.asar==5){
            murottalPlayingStat[8] = 2;
          }
          else if(timeAlarm.asar==10){
            murottalPlayingStat[8] = 3;
          }
          else if(timeAlarm.asar==15){
            murottalPlayingStat[8] = 4;
          }
          else if(timeAlarm.asar==20){
            murottalPlayingStat[8] = 5;
          }
          else if(timeAlarm.asar==30){
            murottalPlayingStat[8] = 6;
          }
          else if(timeAlarm.asar==45){
            murottalPlayingStat[8] = 7;
          }
          else if(timeAlarm.asar==60){
            murottalPlayingStat[8] = 1;
          }
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[2]==2 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[2]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 19;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[2]==3 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[2]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.asar){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      if(menjelangWaktuSholatStat[2]<4){
        stopMurottal();
        menjelangWaktuSholatStat[2]=4;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.asar){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
  }
  else if(masukWaktuSholat==false & timeAlarm.magrib!=999 & timeMin.Magrib-timeMin.Now<=timeAlarm.magrib & timeMin.Magrib-timeMin.Now>0){
    menjelangWaktuSholatStat[0]=0; menjelangWaktuSholatStat[1]=0; menjelangWaktuSholatStat[2]=0; menjelangWaktuSholatStat[4]=0; 
    if(timeMin.Magrib-timeMin.Now==timeAlarm.magrib){
      if(menjelangWaktuSholatStat[3]==0){
        stopMurottal();
        menjelangWaktuSholatStat[3]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[3]==1 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[3]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          if(timeAlarm.magrib==5){
            murottalPlayingStat[8] = 2;
          }
          else if(timeAlarm.magrib==10){
            murottalPlayingStat[8] = 3;
          }
          else if(timeAlarm.magrib==15){
            murottalPlayingStat[8] = 4;
          }
          else if(timeAlarm.magrib==20){
            murottalPlayingStat[8] = 5;
          }
          else if(timeAlarm.magrib==30){
            murottalPlayingStat[8] = 6;
          }
          else if(timeAlarm.magrib==45){
            murottalPlayingStat[8] = 7;
          }
          else if(timeAlarm.magrib==60){
            murottalPlayingStat[8] = 1;
          }
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[3]==2 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[3]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 20;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[3]==3 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[3]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.magrib){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      if(menjelangWaktuSholatStat[3]<4){
        stopMurottal();
        menjelangWaktuSholatStat[3]=4;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.magrib){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
  }
  else if(masukWaktuSholat==false & timeAlarm.isya!=999 & timeMin.Isya-timeMin.Now<=timeAlarm.isya & timeMin.Isya-timeMin.Now>0){
    menjelangWaktuSholatStat[0]=0; menjelangWaktuSholatStat[1]=0; menjelangWaktuSholatStat[2]=0; menjelangWaktuSholatStat[3]=0; 
    if(timeMin.Isya-timeMin.Now==timeAlarm.isya){
      if(menjelangWaktuSholatStat[4]==0){
        stopMurottal();
        menjelangWaktuSholatStat[4]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 32;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[4]==1 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[4]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          if(timeAlarm.isya==5){
            murottalPlayingStat[8] = 2;
          }
          else if(timeAlarm.isya==10){
            murottalPlayingStat[8] = 3;
          }
          else if(timeAlarm.isya==15){
            murottalPlayingStat[8] = 4;
          }
          else if(timeAlarm.isya==20){
            murottalPlayingStat[8] = 5;
          }
          else if(timeAlarm.isya==30){
            murottalPlayingStat[8] = 6;
          }
          else if(timeAlarm.isya==45){
            murottalPlayingStat[8] = 7;
          }
          else if(timeAlarm.isya==60){
            murottalPlayingStat[8] = 1;
          }
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[4]==2 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[4]+=1;
        if(dispStat[0]==1){
          murottalPlayingStat[2]=1;        
          murottalPlayingStat[7] = 1;
          murottalPlayingStat[8] = 21;
          myDFPlayer.playFolder(murottalPlayingStat[7], murottalPlayingStat[8]);
          countMp3Failed1 = 0;
        }
      }
      else if(menjelangWaktuSholatStat[4]==3 & murottalPlayingStat[2]==3){
        menjelangWaktuSholatStat[4]+=1;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.isya){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
    else{
      if(menjelangWaktuSholatStat[4]<4){
        stopMurottal();
        menjelangWaktuSholatStat[4]=4;
        if(dispStat[0]==1){
          switch (alarmWithMurottal.isya){
            case 1:
              murottalState1 = true;
              break;
            case 2:
              murottalState2 = true;
              break;
            case 3:
              murottalState3 = true;
              break;
          }
        }
      }
    }
  }
  else{
    menjelangWaktuSholatStat[0]=0; menjelangWaktuSholatStat[1]=0; menjelangWaktuSholatStat[2]=0; menjelangWaktuSholatStat[3]=0; menjelangWaktuSholatStat[4]=0;
  }

  if((menjelangWaktuSholatStat[0]>=1 & menjelangWaktuSholatStat[0] <= 4) | (menjelangWaktuSholatStat[1]>=1 & menjelangWaktuSholatStat[1] <= 4) | 
     (menjelangWaktuSholatStat[2]>=1 & menjelangWaktuSholatStat[2] <= 4) | (menjelangWaktuSholatStat[3]>=1 & menjelangWaktuSholatStat[3] <= 4) |
     (menjelangWaktuSholatStat[4]>=1 & menjelangWaktuSholatStat[4] <= 4)){
    menjelangWaktuSholat = true;
    }
  else{
    menjelangWaktuSholat = false;
  }
}

void TampilJamDinamis1() {
  int fontWidth = 5;
  String a = weekDay[intWeekDay];
  int b = a.length();
  int x = b+1;
  int c;
  char d[b+1];
  
  int delayTransition = 80;
  
  String e = charDay;
  String f = e;
  e = monthYear[intMonth];
  f = f + " " + e;
  e = charYear;
  f = f + " 20" + e;
  
  String g = f;   //a
  int h = g.length();     //b
  int y = h + 1;    //x
  int i;    //c
  char j[h+1];    //d

  String k = charDayH;
  String l = k;
  k = monthYearH[intMonthH];
  l = l + " " + k;
  k = charYearH;
  l = l + " 14" + k;

  String m = l;   //g
  int n = m.length();     //h
  int z = n + 1;    //y
  int o;    //i
  char p[n+1];    //j

  int q;
  static int r;
  static int s;
  int gapTemp = -1;
  int gapTemp1 = 1;

  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charHour);
  backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charHour);
  backgroundLayer.drawString(23+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charMinute);
  backgroundLayer.drawString(24, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charMinute);
  backgroundLayer.drawString(43+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charSecond);
  backgroundLayer.drawString(44, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charSecond);
  
  backgroundLayer.drawRectangle(21, 7 + gap + gapTemp, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
  backgroundLayer.drawRectangle(41, 7 + gap + gapTemp, 42, 8 + gap, {0xcc, 0xcc, 0xcc});

  backgroundLayer.setFont(font5x7);
  gapTemp = -1;

  if(cache1==0){
    cache1 = cache1;
    q = 0;
  }
  else if(cache1>0 & cache1<x){
    q = 1;
    a.toCharArray(d, cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=x & cache1<delayTransition+x){
    q = 2;
    a.toCharArray(d, x);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=delayTransition+x & cache1<delayTransition+x*2){
    q = 3;
    a.toCharArray(d, delayTransition+x*2-cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1==delayTransition+x*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition+x*2 & cache1<delayTransition+x*2+y){
    q = 1;
    g.toCharArray(j, cache1-(delayTransition+x*2));
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition+x*2+y & cache1<delayTransition*2+x*2+y){
    q = 2;
    g.toCharArray(j, y);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition*2+x*2+y & cache1<delayTransition*2+x*2+y*2){
    q = 3;
    g.toCharArray(j, delayTransition*2+x*2+y*2-cache1);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1==delayTransition*2+x*2+y*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition*2+x*2+y*2 & cache1<delayTransition*2+x*2+y*2+z){
    q = 1;
    m.toCharArray(p, cache1-(delayTransition*2+x*2+y*2));
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*2+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z){
    q = 2;
    m.toCharArray(p, z);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*3+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z*2){
    q = 3;
    m.toCharArray(p, delayTransition*3+x*2+y*2+z*2-cache1);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else{
    q = 0;
    cache1 = 0;
  }

  static float x1;
  static float x2;
  static float x3;
  float xyz;

  String jam1;
  String menit1;
  char t[16];

  backgroundLayer.setFont(font3x5);
  gapTemp = 1;

  if(s<8){
    jam1 = String(prayTimeSav[s][0]/10) + String(prayTimeSav[s][0]%10);
    menit1 = String(prayTimeSav[s][1]/10) + String(prayTimeSav[s][1]%10);
    a = waktuSholatString[s] + String(jam1) + ":" + String(menit1);
    b = a.length() + 1;
    a.toCharArray(t, b);
    c = (64-(4*(b-1)))/2;
    if(r==0){
      s++;
      b = 0xd0;
      x1 = b;
      b = 0x60;
      x2 = b;
      b = 0x80;
      x3 = b;
    }
  }
  else if(s>=8){
    s = 0;
    jam1 = String(prayTimeSav[s][0]/10) + String(prayTimeSav[s][0]%10);
    menit1 = String(prayTimeSav[s][1]/10) + String(prayTimeSav[s][1]%10);
    a = waktuSholatString[s] + String(jam1) + ":" + String(menit1);
    b = a.length() + 1;
    a.toCharArray(t, b);
    c = (64-(4*(b-1)))/2;
    if(r==0){
      s++;
      b = 0xa0;
      x1 = b;
      b = 0x50;
      x2 = b;
      b = 0x70;
      x3 = b;
    }
  }

  if(r<255){
    r = r + 10;
    if(r>255){
      r = 255;
    }
    xyz = r*(x1/255);
    x = xyz;
    xyz = r*(x2/255);
    y = xyz;
    xyz = r*(x3/255);
    z = xyz;
    backgroundLayer.drawString(c-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, t);
    backgroundLayer.drawString(c, 23 + gap + gapTemp, {x, y, z}, t);
  }
  else if(r==255 & q<=2){
    backgroundLayer.drawString(c-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, t);
    backgroundLayer.drawString(c, 23 + gap + gapTemp, {x1, x2, x3}, t);
  }
  else if(r==255 & q==3){
    backgroundLayer.drawString(c-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, t);
    backgroundLayer.drawString(c, 23 + gap + gapTemp, {x1, x2, x3}, t);
    r++;
  }
  else if(r>255 & r<511){
    r = r + 10;
    if(r>511){
      r = 511;
    }
    xyz = (511-r)*(x1/255);
    x = xyz;
    xyz = (511-r)*(x2/255);
    y = xyz;
    xyz = (511-r)*(x3/255);
    z = xyz;
    backgroundLayer.drawString(c, 23 + gap + gapTemp, {x, y, z}, t);
  }
  else{
    r = 0;
  }
}

void TampilJamDinamis2() {
  int fontWidth = 5;
  String a = weekDay[intWeekDay];
  int b = a.length();
  int x = b+1;
  int c;
  char d[b+1];
  
  int delayTransition = 80;
  
  String e = charDay;
  String f = e;
  e = monthYear[intMonth];
  f = f + " " + e;
  e = charYear;
  f = f + " 20" + e;
  
  String g = f;   //a
  int h = g.length();     //b
  int y = h + 1;    //x
  int i;    //c
  char j[h+1];    //d

  String k = charDayH;
  String l = k;
  k = monthYearH[intMonthH];
  l = l + " " + k;
  k = charYearH;
  l = l + " 14" + k;

  String m = l;   //g
  int n = m.length();     //h
  int z = n + 1;    //y
  int o;    //i
  char p[n+1];    //j

  int q;
  static int r;
  static int s;
  int gapTemp = -1;
  int gapTemp1 = 1;

  if(showVolume2Disp==0){
    backgroundLayer.setFont(font8x13);
    backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charHour);
    backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charHour);
    backgroundLayer.drawString(23+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charMinute);
    backgroundLayer.drawString(24, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charMinute);
    backgroundLayer.drawString(43+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charSecond);
    backgroundLayer.drawString(44, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charSecond);
    
    backgroundLayer.drawRectangle(21, 7 + gap + gapTemp, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
    backgroundLayer.drawRectangle(41, 7 + gap + gapTemp, 42, 8 + gap, {0xcc, 0xcc, 0xcc});    
  }
  else{    
    backgroundLayer.setFont(font8x13);
    backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charVolume2);
    backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charVolume2);
  }

  backgroundLayer.setFont(font5x7);
  gapTemp = -1;

  if(cache1==0){
    cache1 = cache1;
    q = 0;
  }
  else if(cache1>0 & cache1<x){
    q = 1;
    a.toCharArray(d, cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=x & cache1<delayTransition+x){
    q = 2;
    a.toCharArray(d, x);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=delayTransition+x & cache1<delayTransition+x*2){
    q = 3;
    a.toCharArray(d, delayTransition+x*2-cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1==delayTransition+x*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition+x*2 & cache1<delayTransition+x*2+y){
    q = 1;
    g.toCharArray(j, cache1-(delayTransition+x*2));
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition+x*2+y & cache1<delayTransition*2+x*2+y){
    q = 2;
    g.toCharArray(j, y);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition*2+x*2+y & cache1<delayTransition*2+x*2+y*2){
    q = 3;
    g.toCharArray(j, delayTransition*2+x*2+y*2-cache1);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1==delayTransition*2+x*2+y*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition*2+x*2+y*2 & cache1<delayTransition*2+x*2+y*2+z){
    q = 1;
    m.toCharArray(p, cache1-(delayTransition*2+x*2+y*2));
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*2+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z){
    q = 2;
    m.toCharArray(p, z);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*3+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z*2){
    q = 3;
    m.toCharArray(p, delayTransition*3+x*2+y*2+z*2-cache1);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else{
    q = 0;
    cache1 = 0;
  }

  backgroundLayer.setFont(font3x5);
  gapTemp = 1;
  int nb[] = {11, 11, 12, 11, 11, 10, 12, 10};
  int ob = 0;
  int p1b;
  int p2b;
  int p3b;
  int p4b; 
  int p3bFinalMove; 
  int sMin1;
  int sPlus1;
  int sMin2;

  if(q==2){
    if(r==-1){
      r = 0;
      s++;
      if(s>=8){
        s = 0;
      }
    }
    
    sMin1 = (8 + s - 1) % 8;
    sPlus1 = (s + 1) % 8;
    sMin2 = (8 + s - 2) % 8;
    
    p1b = (64 - (nb[sMin1]*4))/2 - 5 - (nb[sMin2]*4);

    p2b = (64 - (nb[sMin1]*4))/2;

    p3b = p2b + (nb[sMin1]*4) + 5;

    p4b = p3b + (nb[s]*4) + 5;

    p3bFinalMove = (64 - (nb[s]*4))/2;

    backgroundLayer.drawString(p1b-r-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[sMin2]);
    backgroundLayer.drawString(p1b-r, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[sMin2]);

    backgroundLayer.drawString(p2b-r-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[sMin1]);
    backgroundLayer.drawString(p2b-r, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[sMin1]);

    if(r == p3b - p3bFinalMove){
      backgroundLayer.drawString(p3b-r-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[s]);
      backgroundLayer.drawString(p3b-r, 23 + gap + gapTemp, {0xd0, 0x60, 0x80}, runPrayTime[s]);
    }
    else{
      backgroundLayer.drawString(p3b-r-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[s]);
      backgroundLayer.drawString(p3b-r, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[s]);
    }

    backgroundLayer.drawString(p4b-r-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[sPlus1]);
    backgroundLayer.drawString(p4b-r, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[sPlus1]);

    if(p3b-r>p3bFinalMove){
      r++;
      if(p3b-r-p3bFinalMove<0){
        r = p3b - p3bFinalMove;
      }
    }
  }
  
  else if(q!=2){
    r = -1;
    sMin1 = (8 + s - 1) % 8;
    sPlus1 = (s + 1) % 8;
    
    ob = (64 - (nb[s]*4))/2;
    ob = ob - 5;
    ob = ob - (nb[sMin1]*4);
    
    backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[sMin1]);
    backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[sMin1]);

    ob = (64 - (nb[s]*4))/2;

    backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[s]);
    backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0xd0, 0x60, 0x80}, runPrayTime[s]);

    ob = ob + (nb[s]*4) + 5;

    backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[sPlus1]);
    backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime[sPlus1]);
  }
}

void TampilMasukWaktuSholat1() {
  int aa;
  if(masukWaktuSholatStat[0]!=0){
    aa = 0;
  }
  else if(masukWaktuSholatStat[2]!=0){
    aa = 2;
  }
  else if(masukWaktuSholatStat[3]!=0){
    aa = 3;
  }
  
  int fontWidth = 5;
  String a = weekDay[intWeekDay];
  int b = a.length();
  int x = b+1;
  int c;
  char d[b+1];
  
  int delayTransition = 80;
  
  String e = charDay;
  String f = e;
  e = monthYear[intMonth];
  f = f + " " + e;
  e = charYear;
  f = f + " 20" + e;
  
  String g = f;   //a
  int h = g.length();     //b
  int y = h + 1;    //x
  int i;    //c
  char j[h+1];    //d

  String k = charDayH;
  String l = k;
  k = monthYearH[intMonthH];
  l = l + " " + k;
  k = charYearH;
  l = l + " 14" + k;

  String m = l;   //g
  int n = m.length();     //h
  int z = n + 1;    //y
  int o;    //i
  char p[n+1];    //j

  int q;
  int gapTemp = -1;
  int gapTemp1 = 1;

  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charHour);
  backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charHour);
  backgroundLayer.drawString(23+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charMinute);
  backgroundLayer.drawString(24, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charMinute);
  backgroundLayer.drawString(43+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charSecond);
  backgroundLayer.drawString(44, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charSecond);
  
  backgroundLayer.drawRectangle(21, 7 + gap + gapTemp, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
  backgroundLayer.drawRectangle(41, 7 + gap + gapTemp, 42, 8 + gap, {0xcc, 0xcc, 0xcc});

  backgroundLayer.setFont(font5x7);
  gapTemp = -1;

  if(cache1==0){
    cache1 = cache1;
    q = 0;
  }
  else if(cache1>0 & cache1<x){
    q = 1;
    a.toCharArray(d, cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=x & cache1<delayTransition+x){
    q = 2;
    a.toCharArray(d, x);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=delayTransition+x & cache1<delayTransition+x*2){
    q = 3;
    a.toCharArray(d, delayTransition+x*2-cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1==delayTransition+x*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition+x*2 & cache1<delayTransition+x*2+y){
    q = 1;
    g.toCharArray(j, cache1-(delayTransition+x*2));
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition+x*2+y & cache1<delayTransition*2+x*2+y){
    q = 2;
    g.toCharArray(j, y);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition*2+x*2+y & cache1<delayTransition*2+x*2+y*2){
    q = 3;
    g.toCharArray(j, delayTransition*2+x*2+y*2-cache1);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1==delayTransition*2+x*2+y*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition*2+x*2+y*2 & cache1<delayTransition*2+x*2+y*2+z){
    q = 1;
    m.toCharArray(p, cache1-(delayTransition*2+x*2+y*2));
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*2+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z){
    q = 2;
    m.toCharArray(p, z);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*3+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z*2){
    q = 3;
    m.toCharArray(p, delayTransition*3+x*2+y*2+z*2-cache1);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else{
    q = 0;
    cache1 = 0;
  }

  backgroundLayer.setFont(font3x5);
  gapTemp = 1;  
  int nb[] = {11, 11, 12, 11, 11, 10, 12, 10};
  int ob = (64 - (nb[aa]*4))/2;

  if(count6%3==0){
    backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[aa]);
    backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[aa]);
  }
  else{
    backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[aa]);
    backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0xd0, 0x60, 0x80}, runPrayTime[aa]);
  }
}

void TampilMasukWaktuSholat2() {
  int i;
  int j;
  int b;
  int c;
  int d;
  int e;
  int f;
  
  int gapTemp = -1;
  int gapTemp1 = 1;

  if(masukWaktuSholatStat[1]!=0){
    b = 1;
    c = timeMin.Now-timeMin.Subuh;
    d = timeIqamah.subuh;
  }
  else if(masukWaktuSholatStat[4]!=0){
    b = 4;
    c = timeMin.Now-timeMin.Zuhur;
    d = timeIqamah.zuhur;
  }
  else if(masukWaktuSholatStat[5]!=0){
    b = 5;
    c = timeMin.Now-timeMin.Asar;
    d = timeIqamah.asar;
  }
  else if(masukWaktuSholatStat[6]!=0){
    b = 6;
    c = timeMin.Now-timeMin.Magrib;
    d = timeIqamah.magrib;
  }
  else if(masukWaktuSholatStat[7]!=0){
    b = 7;
    c = timeMin.Now-timeMin.Isya;
    d = timeIqamah.isya;
  }

  if(c<3+d){
    backgroundLayer.setFont(font8x13);
    backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charHour);
    backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charHour);
    backgroundLayer.drawString(23+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charMinute);
    backgroundLayer.drawString(24, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charMinute);
    backgroundLayer.drawString(43+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charSecond);
    backgroundLayer.drawString(44, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charSecond);
    
    backgroundLayer.drawRectangle(21, 7 + gap + gapTemp, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
    backgroundLayer.drawRectangle(41, 7 + gap + gapTemp, 42, 8 + gap, {0xcc, 0xcc, 0xcc});
    
    backgroundLayer.setFont(font3x5);
    gapTemp = 1;  
    int nb[] = {11, 11, 12, 11, 11, 10, 12, 10};
    int ob = (64 - (nb[b]*4))/2;
    
    if(count6%3==0){
      backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[b]);
      backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[b]);
    }
    else{
      backgroundLayer.drawString(ob-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime[b]);
      backgroundLayer.drawString(ob, 23 + gap + gapTemp, {0xd0, 0x60, 0x80}, runPrayTime[b]);
    }
  }
  else{
    runningLineCount = 60;
  }
  
  backgroundLayer.setFont(font5x7);
  gapTemp = -1;

  String aa;
  char bb[16];
  int cc;
  char dd[16];
  char ee[16];
  char ff[16];
  
  if(c<3){
    aa = "AZAN";
    aa.toCharArray(bb, 5);
    cc = (64-(4*5))/2-1;
    backgroundLayer.drawString(cc+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, bb);
    backgroundLayer.drawString(cc+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, bb);
  }
  else if(c>=3 & c<3+d){
    aa = "IQAMAH";
    aa.toCharArray(dd, 7);
    e = d+3-c-1;
    f = 60 - timeNow.secondNow;
    if(timeNow.secondNow==0){
      e = d+3-c;
      f = 0;
    }
    ff[0] = e/10 + '0';
    ff[1] = e%10 + '0';
    ff[2] = ':';
    ff[3] = f/10 + '0';
    ff[4] = f%10 + '0';   
    
    backgroundLayer.drawString(2+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, dd);
    backgroundLayer.drawString(2+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, dd);

    backgroundLayer.drawString(37+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, ff);
    backgroundLayer.drawString(37+1, 16 + gap + gapTemp, {0xef, 0xe1, 0x87}, ff);
  }
  else{
    aa = "SHOLAT";
    aa.toCharArray(ee, 7);
    cc = (64-(6*5))/2-1;
    backgroundLayer.drawString(cc+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, ee);
    backgroundLayer.drawString(cc+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, ee);
  }
}

void TampilMenjelangWaktuSholat(){
  byte aa;
  long da = timeNow.hourNow*3600 + timeNow.minuteNow*60 + timeNow.secondNow;
  long ea;
  int fa;
  int ga;

  if(menjelangWaktuSholatStat[0]!=0){
    aa = 1;
    ea = timeMin.Subuh * 60;
  }
  else if(menjelangWaktuSholatStat[1]!=0){
    aa = 4;
    ea = timeMin.Zuhur * 60;
  }
  else if(menjelangWaktuSholatStat[2]!=0){
    aa = 5;
    ea = timeMin.Asar * 60;
  }
  else if(menjelangWaktuSholatStat[3]!=0){
    aa = 6;
    ea = timeMin.Magrib * 60;
  }
  else if(menjelangWaktuSholatStat[4]!=0){
    aa = 7;
    ea = timeMin.Isya * 60;
  }

  ea = ea - da;
  fa = ea / 60;
  ga = ea % 60;

  char ha[16];
  ha[0] = fa/10 + '0';
  ha[1] = fa%10 + '0';
  ha[2] = ':';
  ha[3] = ga/10 + '0';
  ha[4] = ga%10 + '0';

  int fontWidth = 5;
  String a = weekDay[intWeekDay];
  int b = a.length();
  int x = b+1;
  int c;
  char d[b+1];
  
  int delayTransition = 80;
  
  String e = charDay;
  String f = e;
  e = monthYear[intMonth];
  f = f + " " + e;
  e = charYear;
  f = f + " 20" + e;
  
  String g = f;   //a
  int h = g.length();     //b
  int y = h + 1;    //x
  int i;    //c
  char j[h+1];    //d

  String k = charDayH;
  String l = k;
  k = monthYearH[intMonthH];
  l = l + " " + k;
  k = charYearH;
  l = l + " 14" + k;

  String m = l;   //g
  int n = m.length();     //h
  int z = n + 1;    //y
  int o;    //i
  char p[n+1];    //j

  int q;
  int gapTemp = -1;
  int gapTemp1 = 1;

  backgroundLayer.setFont(font8x13);
  backgroundLayer.drawString(3+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charHour);
  backgroundLayer.drawString(4, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charHour);
  backgroundLayer.drawString(23+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charMinute);
  backgroundLayer.drawString(24, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charMinute);
  backgroundLayer.drawString(43+gapTemp1, 3 + gap + gapTemp, {0x00, 0x00, 0x00}, charSecond);
  backgroundLayer.drawString(44, 2 + gap + gapTemp, {0x50, 0x80, 0xa0}, charSecond);
  
  backgroundLayer.drawRectangle(21, 7 + gap + gapTemp, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
  backgroundLayer.drawRectangle(41, 7 + gap + gapTemp, 42, 8 + gap, {0xcc, 0xcc, 0xcc});

  backgroundLayer.setFont(font5x7);
  gapTemp = -1;

  if(cache1==0){
    cache1 = cache1;
    q = 0;
  }
  else if(cache1>0 & cache1<x){
    q = 1;
    a.toCharArray(d, cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=x & cache1<delayTransition+x){
    q = 2;
    a.toCharArray(d, x);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1>=delayTransition+x & cache1<delayTransition+x*2){
    q = 3;
    a.toCharArray(d, delayTransition+x*2-cache1);
    a = d;
    b = a.length();
    c = (64-(fontWidth*b))/2-1;
    backgroundLayer.drawString(c+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, d);
    backgroundLayer.drawString(c+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, d);
  }
  else if(cache1==delayTransition+x*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition+x*2 & cache1<delayTransition+x*2+y){
    q = 1;
    g.toCharArray(j, cache1-(delayTransition+x*2));
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition+x*2+y & cache1<delayTransition*2+x*2+y){
    q = 2;
    g.toCharArray(j, y);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1>=delayTransition*2+x*2+y & cache1<delayTransition*2+x*2+y*2){
    q = 3;
    g.toCharArray(j, delayTransition*2+x*2+y*2-cache1);
    g = j;
    h = g.length();
    i = (64-(fontWidth*h))/2-1;
    backgroundLayer.drawString(i+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, j);
    backgroundLayer.drawString(i+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, j);
  }
  else if(cache1==delayTransition*2+x*2+y*2){
    q = 0;
    cache1 = cache1;
  }
  else if(cache1>delayTransition*2+x*2+y*2 & cache1<delayTransition*2+x*2+y*2+z){
    q = 1;
    m.toCharArray(p, cache1-(delayTransition*2+x*2+y*2));
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*2+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z){
    q = 2;
    m.toCharArray(p, z);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else if(cache1>=delayTransition*3+x*2+y*2+z & cache1<delayTransition*3+x*2+y*2+z*2){
    q = 3;
    m.toCharArray(p, delayTransition*3+x*2+y*2+z*2-cache1);
    m = p;
    n = m.length();
    o = (64-(fontWidth*n))/2-1;
    backgroundLayer.drawString(o+gapTemp1, 17 + gap + gapTemp, {0x00, 0x00, 0x00}, p);
    backgroundLayer.drawString(o+1, 16 + gap + gapTemp, {0xff, 0xc1, 0x07}, p);
  }
  else{
    q = 0;
    cache1 = 0;
  }

  backgroundLayer.setFont(font3x5);
  gapTemp = 1;

  if(count6%6==0){
    backgroundLayer.drawString(2-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime1[aa]);
    backgroundLayer.drawString(2, 23 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime1[aa]);
  }
  else{
    backgroundLayer.drawString(2-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, runPrayTime1[aa]);
    backgroundLayer.drawString(2, 23 + gap + gapTemp, {0xff, 0xd0, 0x90}, runPrayTime1[aa]);
  }

  backgroundLayer.drawString(64-20-1-1+gapTemp1, 24 + gap + gapTemp, {0x00, 0x00, 0x00}, ha);
  backgroundLayer.drawString(64-20-1, 23 + gap + gapTemp, {0xd0, 0x60, 0x80}, ha);
}

void getDataIR(){
  testDataChange = false;
  
  byte a;
  byte b;
  long c = 0;
  for(a=0; a<32; a++){
    b = a + 1;
    if(562-testDataTol<testDataBuff[b*2] & testDataBuff[b*2]<562+testDataTol  & 562-testDataTol<testDataBuff[b*2+1] & testDataBuff[b*2+1]<562+testDataTol){
      c = c & ~(1<<(31-a));
    }
    else if(562-testDataTol<testDataBuff[b*2] & testDataBuff[b*2]<562+testDataTol  & 1687-testDataTol<testDataBuff[b*2+1] & testDataBuff[b*2+1]<1687+testDataTol){
      c = c | 1<<(31-a);
    }
  }
  testDataResult = c;
  testData = String(testDataResult, HEX);

  for(a=0; a<100; a++){
    testDataBuff[a] = 0;
  }

  testDataChange = true;
}

void millisHighSpeed(){
  runningLineCount = 2;
  backgroundLayer.fillScreen(defaultBackgroundColor);
  //animeRunningLineRev();
  //animeRunningLineRev2();
  TampilJamDinamis2();  
  backgroundLayer.swapBuffers();
}

void millisHighSpeed1(){
  runningLineCount = 8;
  backgroundLayer.fillScreen(defaultBackgroundColor);
  //animeRunningLineRev();
  //animeRunningLineRev2();
  TampilMenjelangWaktuSholat();  
  backgroundLayer.swapBuffers();
}

void millisHighSpeed21(){
  runningLineCount = 8;
  backgroundLayer.fillScreen(defaultBackgroundColor);
  //animeRunningLineRev();
  //animeRunningLineRev2();
  TampilMasukWaktuSholat1();  
  backgroundLayer.swapBuffers();
}

void millisHighSpeed22(){
  runningLineCount = 15;
  backgroundLayer.fillScreen(defaultBackgroundColor);
  //animeRunningLineRev();
  //animeRunningLineRev2();
  TampilMasukWaktuSholat2();  
  backgroundLayer.swapBuffers();
}

void millisMediumSpeed(){
  cache1++;
}

void loop() {
  if(trialStat==false){
    if(millis()-milisRec>=500){
      count6++;
      if(count6>=120){
        count6 = 0;
      }
      if(count4==0 | millis()-count4>=1000){
        count4 = millis();
        countTime(false);
      }
      if((murottalState == false & statInfoMurottalSpeech==0) & (murottalState1 == false & statInfoMurottalSpeech1==0) & (murottalState2 == false & statInfoMurottalSpeech2==0) & (murottalState3 == false & statInfoMurottalSpeech3==0)){
        myDFPlayer.volume(volume);  //Set volume value (0~30).
      }
      
      getDataIR();
      if(testData=="ff38c7"){
        sahurMomentStat[0]=20; sahurMomentStat[1]=20; sahurMomentStat[2]=20; sahurMomentStat[3]=20; sahurMomentStat[4]=20; 
        sahurMomentStat[5]=20; sahurMomentStat[6]=20; sahurMomentStat[7]=20; sahurMomentStat[8]=20; sahurMomentStat[9]=20;
        tahajjudMomentStat=20;
        WOmurottalMomentStat[0]=20; WOmurottalMomentStat[1]=20;
        masukWaktuSholatStat[0]=20; masukWaktuSholatStat[1]=20; masukWaktuSholatStat[2]=20; masukWaktuSholatStat[3]=20; 
        masukWaktuSholatStat[4]=20; masukWaktuSholatStat[5]=20; masukWaktuSholatStat[6]=20; masukWaktuSholatStat[7]=20;
        menjelangWaktuSholatStat[0]=20; menjelangWaktuSholatStat[1]=20; menjelangWaktuSholatStat[2]=20; 
        menjelangWaktuSholatStat[3]=20; menjelangWaktuSholatStat[4]=20; 
        WithmurottalMomentStat[0]=20; WithmurottalMomentStat[1]=20; WithmurottalMomentStat[2]=20; 
        myDFPlayer.pause();
      }
      else if(testData=="ff6897"){
        if(masukWaktuSholat==false & menjelangWaktuSholat==false & alarmSahur==false &
           alarmTahajjud==false & alarmWOmurottal==false & dispStat[0]==1){
          murottalState = true;
          statInfoMurottalSpeech=0;
        }
      }
      else if(testData=="ffb04f"){
        if(murottalState==true){
          murottalState = false;
          statInfoMurottalSpeech=3;
        }
      }      
      else if(testData=="ff5aa5"){
        if(murottalState==true & volume2<30){
          volume2++;
          myDFPlayer.volume(volume2);
          showVolume2Disp = 10;
          
        }
      }
      else if(testData=="ff10ef"){
        if(murottalState==true & volume2>1){
          volume2--;
          myDFPlayer.volume(volume2);
          showVolume2Disp = 10;
        }
      }
      if(showVolume2Disp>0){
        showVolume2Disp--;
      }
      charVolume2[0] = 'v';
      charVolume2[1] = 'o';
      charVolume2[2] = 'l';
      charVolume2[3] = ':';
      charVolume2[4] = volume2/10 + '0';
      charVolume2[5] = volume2%10 + '0';
      
      murottalPlayingFunc();
      checkMurottalPlayingStat();
      memutarMurottalSpeech();
      memutarMurottalSpeech1();
      murottalPlayingFunc1();
      checkMurottalPlayingStat1();
      memutarMurottalSpeech2();
      murottalPlayingFunc2();
      checkMurottalPlayingStat2();
      memutarMurottalSpeech3();
      murottalPlayingFunc3();
      checkMurottalPlayingStat3();
      masukWaktuSholatMoment();
      menjelangSholatMoment();
      sahurMoment();
      tahajjudMoment(); 
      woMurottalMoment();   
      withMurottalMoment(); 
       
      if(alarmWithmurottal==false & menjelangWaktuSholat==false){
        murottalState1 = false;
        murottalState2 = false;
        murottalState3 = false;
      }
      
      milisRec = millis();
    }
  
    if(millis()-currentMillis>20){
      currentMillis = millis();
      
      if(masukWaktuSholatStat[1]!=0 | masukWaktuSholatStat[4]!=0 | masukWaktuSholatStat[5]!=0 | 
              masukWaktuSholatStat[6]!=0 | masukWaktuSholatStat[7]!=0){
        millisHighSpeed22();  
      }
      else if(masukWaktuSholatStat[0]!=0 | masukWaktuSholatStat[2]!=0 | 
              masukWaktuSholatStat[3]!=0){
        millisHighSpeed21();  
      }      
      else if(menjelangWaktuSholatStat[0]!=0 | menjelangWaktuSholatStat[1]!=0 | menjelangWaktuSholatStat[2]!=0 | 
              menjelangWaktuSholatStat[3]!=0 | menjelangWaktuSholatStat[4]!=0){
        if(timeAlarm.countdown==true){
          millisHighSpeed1(); 
        }
        else{
          millisHighSpeed();  
        }
      }
      else{
        millisHighSpeed();  
      }
    }
  
    if(millis()-currentMillis2>=50){
      currentMillis2 = millis();
      millisMediumSpeed();
    }
  }

  if(millis()-milisRec3>60000){
    milisRec3 = millis();
    
    if(lisensiStatString!="berbayar"){
      limitTrial--;
      if(limitTrial<0){
        limitTrial=0;
        if(trialStat==false){
          trialStat=true;
          backgroundLayer.fillScreen(defaultBackgroundColor);

          backgroundLayer.setFont(font8x13);
          backgroundLayer.drawString(3, 3 + gap, {0x00, 0x00, 0x00}, charHour);
          backgroundLayer.drawString(4, 2 + gap, {0x00, 0xbc, 0xd4}, charHour);
          backgroundLayer.drawString(23, 3 + gap, {0x00, 0x00, 0x00}, charMinute);
          backgroundLayer.drawString(24, 2 + gap, {0x00, 0xbc, 0xd4}, charMinute);
          backgroundLayer.drawString(43, 3 + gap, {0x00, 0x00, 0x00}, charSecond);
          backgroundLayer.drawString(44, 2 + gap, {0x00, 0xbc, 0xd4}, charSecond);
          
          backgroundLayer.drawRectangle(21, 7 + gap, 22, 8 + gap, {0xcc, 0xcc, 0xcc});
          backgroundLayer.drawRectangle(41, 7 + gap, 42, 8 + gap, {0xcc, 0xcc, 0xcc});
    
          char d[15] = {"versi uji coba"};
          backgroundLayer.setFont(font3x5);
          backgroundLayer.drawString(0, 17 + gap, {0x00, 0x00, 0x00}, d);
          backgroundLayer.drawString(1, 16 + gap, {0xff, 0xc1, 0x07}, d);
          
          backgroundLayer.swapBuffers();
        }
      }
      if(limitTrial%60==5){
        spiffsStat = true;
        File fileToWrite2 = SPIFFS.open("/limitTrial.txt", "w");
        String abc = String(limitTrial);
        fileToWrite2.println(abc);
        fileToWrite2.close();
        spiffsStat = false;
      }
    }
  }
  
}
