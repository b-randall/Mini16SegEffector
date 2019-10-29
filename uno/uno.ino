const int button1 = 12;
#include "Tlc5940.h"
//Sliders
#define VEFX A1
#define LOWEQ A2
#define HIEQ A3
#define FILTER A4
#define PLAYVOL A5
#define REFRESH 300
#define DEADZONE 5
int timer =0;
int count =0;

//74hc595 pin setup
//
// SH_CP; 11
const int clockPin = 4;
// DS; 14
const int dataPin = 2;
// ST_CP; 12
const int latchPin = 7;
//OE 13
const int OE = 6;


int vefxState = 0;
int lowEqState = 0;
int hiEqState = 0;
int filterState = 0;
int playVolState = 0;
int oldvefxState = 0;
int oldlowEqState = 0;
int oldhiEqState = 0;
int oldfilterState = 0;
int oldplayVolState = 0;

bool estConnect = false;

char inChar = ' ' ;
String inString = "";
int segCount = 0;
int sliderCount = 0;

char segChar[12] = "   hello!   ";
void setup() {
  Tlc.init(0);
  
  pinMode(OE, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(0, OUTPUT);

  pinMode(5, OUTPUT);
  displayOff();

  
  delay(1000); //let USB MCU setup
  // put your setup code here, to run once:
  Serial.begin(115200);
  EstablishConnection();
}

void loop() {
  // put your main code here, to run repeatedly:
  SendSliderVals();
  timer++;
  if(timer >=200){
    count ++;
    timer =0;
  }
  //setTLC(segCount);
  updateSeg();
  delayMicroseconds(800);
}

void GetSliderVals(){
  switch(sliderCount){
    case 0:
      vefxState = analogRead(VEFX)/4;
      sliderCount++;
      break;
    case 1:
      lowEqState = analogRead(LOWEQ)/4;
      sliderCount++;
      break;
    case 2:
      hiEqState = analogRead(HIEQ)/4;
      sliderCount++;
      break;
    case 3:
      filterState = analogRead(FILTER)/4;
      sliderCount++;
      break;
    case 4:
      playVolState = analogRead(PLAYVOL)/4;
      sliderCount = 0;
      break;
  }
}

void SendSliderVals(){
  GetSliderVals();
    //only send slider update if there is a change and USB MCU asking for update
    if(abs(vefxState-oldvefxState)>DEADZONE and Serial.available()>0 ){
     SendAnalogs();
    }else if(abs(lowEqState-oldlowEqState)>DEADZONE and Serial.available()>0){
      SendAnalogs();
    }else if(abs(hiEqState-oldhiEqState)>DEADZONE and Serial.available()>0){
     SendAnalogs();
    }else if(abs(filterState-oldfilterState)>DEADZONE and Serial.available()>0){
      SendAnalogs();
    }else if(abs(playVolState-oldplayVolState)>DEADZONE and Serial.available()>0){
      SendAnalogs();
  }
  
}

void SendAnalogs(){
    ReadChar();
    Serial.write(vefxState);
    Serial.write(lowEqState);
    Serial.write(hiEqState);
    Serial.write(filterState);
    Serial.write(playVolState);
    oldvefxState = vefxState;
    oldlowEqState = lowEqState;
    oldhiEqState = hiEqState;
    oldfilterState = filterState;
    oldplayVolState = playVolState;
   
}

void ReadChar(){
  String newString = String();
  while(Serial.available() > 0){
    inChar = Serial.read();
    newString +=inChar;
  }
  inString = newString;
  //Serial.print(inString);
}

void EstablishConnection(){
  while(Serial.available() <=0){
    Serial.print('A');
    delay(300);
  }
  digitalWrite(OE, LOW);
}

void updateSeg(){
  switch(segCount){
    case 0:
      setTlcChar(segChar[count%12]);
      digitalWrite(5, 0);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000001);
      digitalWrite(latchPin, HIGH);
      break;
    case 1:
      setTlcChar(segChar[count+1%12]);      
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000010);
      digitalWrite(latchPin, HIGH);
      break;
    case 2:
      setTlcChar(segChar[count+2%12]);
      digitalWrite(8, 0);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000100);
      digitalWrite(latchPin, HIGH);
      break;
    case 3:
    setTlcChar('x');
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00001000);
      digitalWrite(latchPin, HIGH);
      break;
    case 4:
    setTlcChar('y');
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00010000);
      digitalWrite(latchPin, HIGH);
      break;
    case 5:    
    setTlcChar('z');
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00100000);
      digitalWrite(latchPin, HIGH);
      break;
    case 6:
    setTlcChar('r');
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B01000000);
      digitalWrite(latchPin, HIGH);
      break;
    case 7:
    setTlcChar('s');
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B10000000);
      digitalWrite(latchPin, HIGH);
      break;
    case 8:
    setTlcChar('t');
      displayOff();
      digitalWrite(5, 1);
      break;
  }
  segCount++;
  if(segCount > 8) segCount = 0;  
  //TODO - get 16 seg string from USB hid and set displayOff  
  if(count > 12){
    count =0;
  }
}

void displayOff(){
  digitalWrite(5, 0);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
}
void setTLC(int SegNum){
  displayOff();
  Tlc.clear();
  setTlcChar('c');
  Tlc.update();  
  delay(1);
  if(count >= 16){
    count =0;
  }
}



void setTlcChar(char letter){
  displayOff();
  switch(letter){
    case 'a':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'b':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 4095);
      break;
    case 'c':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'd':
      Tlc.set(0, 4095);
      Tlc.set(1, 4095);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 4095);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 40950);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'e':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'f':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'g':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'h':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 0);
      break;
    case 'i':
      Tlc.set(0, 4095);
      Tlc.set(1, 4095);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 4095);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'j':
      Tlc.set(0, 4095);
      Tlc.set(1, 4095);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 4095);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'k':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 0);
      break;
    case 'l':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 0);
      break;
    case 'm':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 4095);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 4095);
      Tlc.set(15, 0);
      break;
    case 'n':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 4095);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 4095);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 0);
      break;
    case 'o':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'p':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'q':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 4095);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'r':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 4095);
      Tlc.set(12, 4095);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 's':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 4095);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 4095);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 't':
      Tlc.set(0, 4095);
      Tlc.set(1, 4095);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 4095);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 0);
      Tlc.set(15, 4095);
      break;
    case 'u':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 4095);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 4095);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 0);
      break;
    case 'v':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 4095);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 0);
      break;
    case 'w':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 4095);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 4095);
      Tlc.set(7, 4095);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 4095);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 4095);
      Tlc.set(14, 0);
      Tlc.set(15, 0);
      break;
    case 'x':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 4095);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 4095);
      Tlc.set(7, 0);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 4095);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 0);
      break;
    case 'y':
      Tlc.set(0, 0);
      Tlc.set(1, 0);
      Tlc.set(2, 4095);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 4095);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 0);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 0);
      break;
    case 'z':
      Tlc.set(0, 4095);
      Tlc.set(1, 0);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 4095);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 4095);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 4095);
      break;
    case '1':
      break;
    case '2':
      break;
    case '3':
      break;
    case '4':
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    case '8':
      break;
    case '9':
      break;
    case '0':
      break;
    case '!':
      Tlc.set(0, 0);
      Tlc.set(1, 4095);
      Tlc.set(2, 0);
      Tlc.set(3, 0);
      Tlc.set(4, 0);
      Tlc.set(5, 0);
      Tlc.set(6, 0);
      Tlc.set(7, 0);
      Tlc.set(8, 4095);
      Tlc.set(9, 0);
      Tlc.set(10, 0);
      Tlc.set(11, 0);
      Tlc.set(12, 0);
      Tlc.set(13, 0);
      Tlc.set(14, 4095);
      Tlc.set(15, 4095);
      break;
   case NULL:
      Tlc.clear();
      break;
    case ' ':
      Tlc.clear();
      break;
  }
  Tlc.update(); 
  delay(1); 
}
