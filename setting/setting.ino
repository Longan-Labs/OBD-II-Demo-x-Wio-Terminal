// MASK&FILE EXAMPLE OF SERIAL CAN MODULE
// SUPPORT: joney.sui@longan-labs.cc
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>

Serial_CAN can;


#define STANDARD_CAN_11BIT      1       // That depands on your car. some 1 some 0. 

#if STANDARD_CAN_11BIT
unsigned long mask[4] = 
{
    0, 0x7FC,                // ext, maks 0
    0, 0x7FC,                // ext, mask 1
};

unsigned long filt[12] = 
{
    0, 0x7E8,                // ext, filt 0
    0, 0x7E8,                // ext, filt 1
    0, 0x7E8,                // ext, filt 2
    0, 0x7E8,                // ext, filt 3
    0, 0x7E8,                // ext, filt 4
    0, 0x7E8,                // ext, filt 5
};

#else
unsigned long mask[4] =
{
    1, 0x1fffffff,               // ext, maks 0
    1, 0x1fffffff,
};
 
unsigned long filt[12] =
{
    1, 0x18DAF110,                // ext, filt
    1, 0x18DAF110,                // ext, filt 1
    1, 0x18DAF110,                // ext, filt 2
    1, 0x18DAF110,                // ext, filt 3
    1, 0x18DAF110,                // ext, filt 4
    1, 0x18DAF110,                // ext, filt 5
};
#endif

void set_mask_filt()
{
    /*
     * set mask, set both the mask to 0x3ff
     */
MASK:
    if(can.setMask(mask))
    {
        Serial.println("set mask ok");
    }
    else
    {
        Serial.println("set mask fail, try again.");
        goto MASK;
    }
FILT:
    /*
     * set filter, we can receive id from 0x04 ~ 0x09
     */
    if(can.setFilt(filt))
    {
        Serial.println("set filt ok");
    }
    else 
    {
        Serial.println("set filt fail, try again");
        goto FILT;
    }
    
}

void setup()
{
    Serial.begin(9600);
    while(!Serial.available());
    can.begin(0, 0, 9600);      // tx, rx

    set_mask_filt();

}

void loop()
{

}

// END FILE