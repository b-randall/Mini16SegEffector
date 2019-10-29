#include "HID-Project.h"
#include "HID-Settings.h"


char income;

bool debug = true;

int incomingByte = 0;


//Defining Button Pins (also used when setting button states);
#define P1 1
#define EFFECT 2
#define VEFX 3
#define P2 4
//button states
int P1State = 0;
int effState = 0;
int vefxState = 0;
int P2State = 0;

unsigned int slider1State = 0;
unsigned int slider2State = 0;
unsigned int slider3State = 0;
unsigned int slider4State = 0;
unsigned int slider5State = 0;

int serialCount = 0;
int inval= 0;

bool readyToRead = false;

int timer =0;
int counter =0;

unsigned int sliderVals[] = {0,0,0,0,0};


void setup() {
  //Setup Panel Inputs
  pinMode(P1, INPUT_PULLUP);
  pinMode(EFFECT, INPUT_PULLUP);
  pinMode(VEFX, INPUT_PULLUP);
  pinMode(P2, INPUT_PULLUP);

  //Begin Serial to MCU
  Serial1.begin(115200);

  //Declare Gamepad to PC
  Gamepad.begin();

  if(debug){
     Serial.begin(9600);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial1.available()>0) {
      inval = Serial1.read();
      Serial.write(inval);
      if(!readyToRead){
          if(char(inval)== 'A'){
            Serial1.write('~');
            readyToRead = true;
          }
      }else{
        //got all sliders vals
        sliderVals[serialCount] = inval;
        serialCount++;
        if(serialCount >4){
            slider1State = sliderVals[0];
            slider2State = sliderVals[1];
            slider3State = sliderVals[2];
            slider4State = sliderVals[3];
            slider5State = sliderVals[4];
            serialCount = 0;
            //break;
            Serial1.write('~');
          }
      }
  }


  GetAndSetStates();
  SetSliders();
   //give time for uno 16segdisplay
  timer++;
  if(timer>10){
    send16Seg(counter%9);
    counter++;
    timer=0;
  }
  Gamepad.write();
  delay(30);

}

void GetAndSetStates(){
  //read States
  P1State = digitalRead(P1);
  effState = digitalRead(EFFECT);
  vefxState = digitalRead(VEFX);
  P2State = digitalRead(P2);

  //set gamePad states
  if(P1State == LOW){
    Gamepad.press(P1);
  }else{
     Gamepad.release(P1);
  }
  if(effState == LOW){
   Gamepad.press(EFFECT);
  }else{
     Gamepad.release(EFFECT);
  }
  if(vefxState == LOW){
   Gamepad.press(VEFX);
  }else{
    Gamepad.release(VEFX);
  }
  if(P2State == LOW){
    Gamepad.press(P2);
  }else{
     Gamepad.release(P2);
  }
}
void send16Seg(int offset){
  //do 16 seg shit
  switch(offset){
    case 0:
      Serial1.write("@beatmania#");
    break;
    case 1:
    Serial1.write("@eatmaniab#");
    break;
    case 2:
    Serial1.write("@atmaniabe#");
    break;
    case 3:
    Serial1.write("@tmaniabea#");
    break;
    case 4:
    Serial1.write("@maniabeat#");
    break;
    case 5:
    Serial1.write("@aniabeatm#");
    break;
    case 6:
    Serial1.write("@niabeatma#");
    break;
    case 7:
    Serial1.write("@iabeatman#");
    break;
    case 8:    
    Serial1.write("@abeatmani#");
    break;
  }
}

void SetSliders(){
    Gamepad.xAxis(slider1State);
    Gamepad.yAxis(slider2State);
    Gamepad.zAxis(slider3State);
    Gamepad.rxAxis(slider4State);
    Gamepad.ryAxis(slider5State);
}
