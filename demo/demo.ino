// WIO TERMINAL X OBD-II DEMO
// 2020-9-18

#include"TFT_eSPI.h"
#include"Free_Fonts.h" //include the header file
#include <Serial_CAN_Module.h>

Serial_CAN can;

#define STANDARD_CAN_11BIT      1       // That depands on your car. some 1 some 0. 

#define PID_ENGIN_PRM       0x0C
#define PID_VEHICLE_SPEED   0x0D
#define PID_COOLANT_TEMP    0x05
#define PID_INTAKE_TEMP     0x0f

#if STANDARD_CAN_11BIT
#define CAN_ID_PID          0x7DF
#else
#define CAN_ID_PID          0x18db33f1
#endif

TFT_eSPI tft;

int nSpeed = 0;         // speed, 0-200
int nRpm = 0;           // engin speed, 0-999
int nCTemp = 0;         // cooltant temp: 0-999
int nITemp = 0;         //intake temp: 0-999


void sendPid(unsigned char __pid)
{
    unsigned char tmp[8] = {0x02, 0x01, __pid, 0, 0, 0, 0, 0};
    Serial.print("SEND PID: 0x");
    Serial.println(__pid, HEX);

#if STANDARD_CAN_11BIT
    can.send(CAN_ID_PID, 0, 0, 8, tmp);   // SEND TO ID:0X55
#else
    can.send(CAN_ID_PID, 1, 0, 8, tmp);   // SEND TO ID:0X55
#endif

}

unsigned char getPidFromCar(unsigned char __pid, unsigned char *dta)
{
    sendPid(__pid);
    
    unsigned long timer_s = millis();
    
    while(1)
    {
        if(millis()-timer_s > 500)
        {
            return 0;
        }
        
        unsigned char len = 0;
        unsigned long id = 0;
        if(can.recv(&id, dta))                   // check if get data
        {
            if(dta[1] == 0x41 && dta[2] == __pid)
            {
                return 1;
            }
        }
    }
    
    return 0;
}


int df = 40;
int wf = 80;

void setup() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK); //Black background
 
  tft.setFreeFont(&FreeSansBoldOblique12pt7b); //select Free, Sans, Bold, Oblique, 12pt.
  tft.drawString("Wio Terminal OBD-II Demo",0,20);//prints string at (70,80)
 
  tft.setFreeFont(FF10); //select Free, Mono, Oblique, 12pt.
  tft.drawString("SPEED: ",0,wf);//prints string at (70,110)
  tft.drawString("ENGIN PRM: ", 0, wf+df);
  tft.drawString("COOLANT TEMP: ", 0, wf+df*2);
  tft.drawString("INTAKE TEMP: ", 0, wf+df*3);

  Serial.begin(9600);
  
  //while(!Serial.available());
  Serial.println("sys start...");
  
  can.begin(0, 0, 9600);      // tx, rx
  Serial.println("hello world");
  
  //set_mask_filt();

}


void loop() 
{
    unsigned char dta[8];
    
    if(getPidFromCar(PID_VEHICLE_SPEED, dta))
    {
        nSpeed = dta[3];
    }
    delay(100);
    if(getPidFromCar(PID_ENGIN_PRM, dta))
    {
        nRpm = (256.0*(float)dta[3]+(float)dta[4])/4.0;
    }
    delay(100);
    if(getPidFromCar(PID_COOLANT_TEMP, dta))
    {
        nCTemp = dta[3];
        nCTemp = nCTemp-40;
    }
    delay(100);
    if(getPidFromCar(PID_COOLANT_TEMP, dta))
    {
        nCTemp = dta[3];
        nCTemp = nCTemp-40;
    }
    delay(100);
    upDateValue();
    delay(100);
}


void upDateValue()
{
    int NNN = 13;
    if(nSpeed < 10)
    {
        tft.drawString("     ", 14*NNN, wf);
        tft.drawNumber(nSpeed, 16*NNN, wf);
        
        
    }
    else if(nSpeed < 100)
    {
        tft.drawString("    ", 14*NNN, wf);
        tft.drawNumber(nSpeed, 15*NNN, wf);
    }
    else
    {
        tft.drawString("    ", 14*NNN, wf);
        tft.drawNumber(nSpeed, 14*NNN, wf);
    }
    
    tft.drawString("km/h", 18*NNN, wf);
    
    if(nRpm < 10)
    {
        tft.drawString("     ", 13*NNN, wf+df);
        tft.drawNumber(nRpm, 16*NNN, wf+df);
        
    }
    else if(nRpm < 100)
    {
        tft.drawString("      ", 13*NNN, wf+df);
        tft.drawNumber(nRpm, 15*NNN, wf+df);
    }
    else if(nRpm <1000)
    {
        tft.drawString("      ", 13*NNN, wf+df);
        tft.drawNumber(nRpm, 14*NNN, wf+df);
    }
    else
    {
        tft.drawString("      ", 13*NNN, wf+df);
        tft.drawNumber(nRpm, 13*NNN, wf+df);
    }
    
    tft.drawString("r/m", 18*NNN, wf+df);
    
    /*
      tft.drawString("SPEED: ",0,wf);//prints string at (70,110)
  tft.drawString("ENGIN PRM: ", 0, wf+df);
  tft.drawString("COOLANT TEMP: ", 0, wf+df*2);
  tft.drawString("INTAKE TEMP: ", 0, wf+df*3);
  */
    if(nCTemp < 10)
    {
        tft.drawString("     ", 14*NNN, wf+2*df);
        tft.drawNumber(nCTemp, 16*NNN, wf+2*df);
        
    }
    else if(nCTemp < 100)
    {
        tft.drawString("    ", 14*NNN, wf+2*df);
        tft.drawNumber(nCTemp, 15*NNN, wf+2*df);
    }
    else
    {
        tft.drawString("    ", 14*NNN, wf+2*df);
        tft.drawNumber(nCTemp, 14*NNN, wf+2*df);
    }
    
    tft.drawString("o", 18*NNN, wf+2*df-7);
    tft.drawString("C", 19*NNN+2, wf+2*df);
    
    if(nITemp < 10)
    {
        tft.drawString("     ", 14*NNN, wf+3*df);
        tft.drawNumber(nITemp, 16*NNN, wf+3*df);
        
    }
    else if(nITemp < 100)
    {
        tft.drawString("    ", 14*NNN, wf+3*df);
        tft.drawNumber(nITemp, 15*NNN, wf+3*df);
    }
    else
    {
        tft.drawString("    ", 14*NNN, wf+3*df);
        tft.drawNumber(nITemp, 14*NNN, wf+3*df);
    }
    
    tft.drawString("o", 18*NNN, wf+3*df-7);
    tft.drawString("C", 19*NNN+2, wf+3*df);
}

// END FILE