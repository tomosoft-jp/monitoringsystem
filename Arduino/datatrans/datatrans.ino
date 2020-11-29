/**********************************************
  Arduino Nano I2C(Slave)
               SPI(sleave)
**********************************************/
#include <Wire.h>
#include <SPI.h>


// 0:Header
// 1:heart
// 2:sp02
// 3:temp
uint8_t sendbuf[4] = {0xf2,1,2,80};
uint8_t spipint = 1;
volatile uint8_t rxdata;
volatile uint8_t intrpt = 0;
volatile uint8_t rcvbuf[5] = {0xf2,1,2,80,0};

void setup() {
  Serial.begin(9600);
  Serial.println("/-----START Arduino Nano-----/");
  Serial.println("/----------I2C Slave---------/");
  Serial.println("/----------SPI Slave---------/");

  Wire.begin(0x21);// Slave ID #21
  Wire.onRequest(requestEvent);
  
  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  SPI.attachInterrupt();
}

void loop() {
  if(intrpt==1){
    intrpt = 0;
    

  }
}

//----------I2C requestEvent---------
void requestEvent() {
  Wire.write(sendbuf[0]);
  Wire.write(sendbuf[1]);
  Wire.write(sendbuf[2]);
  Wire.write(sendbuf[3]);
}

//----------SPI Interrupt---------
ISR(SPI_STC_vect) {
  rxdata = SPDR;
  intrpt= 1;
  
    if (rxdata == rcvbuf[0]){
      if(spipint == 5)
      {
        if(rcvbuf[4] == (uint8_t)((rcvbuf[0]+rcvbuf[1]+rcvbuf[2]+rcvbuf[3]) & 0x7f))
        {
          for (int i=1; i <= 3; i++)
          {
            sendbuf[i] = rcvbuf[i];
          }
        }
      }
      spipint = 1;
    }
    else{
      if (spipint >= 5)
      {
        spipint = 1;
      }
      else
      {
        rcvbuf[spipint] = rxdata;
      //Serial.print(spipint);
      //Serial.print(" ");
      //Serial.println(rxdata);
        spipint++;
      }
    }
  
  
  
//  Serial.print("ISR! ");
//  Serial.println(rxdata);
}
