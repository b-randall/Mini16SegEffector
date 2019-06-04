const int button1 = 10;
//Sliders

#define VEFX A1
#define LOWEQ A2
#define HIEQ A3
#define FILTER A4
#define PLAYVOL A5
#define REFRESH 300
#define DEADZONE 5

// SH_CP; 11
const int clockPin = 4;
// DS; 15
const int dataPin = 2;
// ST_CP; 12
const int latchPin = 3;


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

void setup() {
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(5, OUTPUT);
  digitalWrite(5, 0);

  
  delay(1000); //let USB MCU setup
  // put your setup code here, to run once:
  Serial.begin(115200);
  EstablishConnection();
}

void loop() {
  // put your main code here, to run repeatedly:
  SendSliderVals();
  //updateSeg();
  //delayMicroseconds(REFRESH);
}

void GetSliderVals(){
  
  vefxState = analogRead(VEFX)/4;
  delay(2);
  lowEqState = analogRead(LOWEQ)/4;
  delay(2);
  hiEqState = analogRead(HIEQ)/4;
  delay(2);
  filterState = analogRead(FILTER)/4;
  delay(2);
  playVolState = analogRead(PLAYVOL)/4;
  delay(2);

}

void SendSliderVals(){
  GetSliderVals();
  //read word for 16 SEG
  //if(Serial.available() > 0){
    //ReadChar();
    if(abs(vefxState-oldvefxState)>DEADZONE){
     SendAnalogs();
    }else if(abs(lowEqState-oldlowEqState)>DEADZONE){
      SendAnalogs();
    }else if(abs(hiEqState-oldhiEqState)>DEADZONE){
     SendAnalogs();
    }else if(abs(filterState-oldfilterState)>DEADZONE){
      SendAnalogs();
    }else if(abs(playVolState-oldplayVolState)>DEADZONE){
      SendAnalogs();
  }
  
}

void SendAnalogs(){
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
}

void updateSeg(){
  switch(segCount){
    case 0:
      //refreshOff();
      digitalWrite(5, 0);
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000001);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 1:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000010);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 2:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000100);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 3:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00001000);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 4:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00010000);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 5:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00100000);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 6:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B01000000);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 7:
     // refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B10000000);
      digitalWrite(latchPin, HIGH);
      segCount++;
      break;
    case 8:
      //refreshOff();
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
      digitalWrite(latchPin, HIGH);
      digitalWrite(5, 1);
      segCount = 0;
      break;
  }
  //TODO - get 16 seg string from USB hid and set display
}

void refreshOff(){
  digitalWrite(5, 0);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B00000000);
  digitalWrite(latchPin, HIGH);
}

void setTLC(int SegNum){
  //do tlc stuff
}
