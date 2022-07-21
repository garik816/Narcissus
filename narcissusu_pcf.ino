// SDA -> A4 to 15 pin on pcf
// SCL -> A5 to 14 pin on pcf

#include "Arduino.h"
#include "PCF8574.h"

#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))

#define buttonPrevPin 2
#define buttonNextPin 3
#define buttonEnterPin 4

long commandArray[] = { 0b1110100000100010101001110101, 0b1110000100101010101101011011, 0b01010100110110101000101010111, 0b1010011100010101010001010011,
                        0b1001000100100010101001110101, 0b1110000101001010101101011011, 0b01010100110110101000101000111, 0b1010011001010101010101010011,
                        0b1011000100010101001001110101, 0b1110100001011011010100100101, 0b01010101100101010100101100111, 0b1011011100010101010101010011 };

void blink();
void detectButtons();
void executeCommand(int cell, long data);
void sendData(long value);
void setMemoryCell(int Area);
void buttonEnter();
void buttonReset();
void buttonHIGH();
void buttonLOW();

// Set i2c address
PCF8574 pcfMemoryArea(0x20);
PCF8574 pcfMemoryCell(0x21);
PCF8574 pcfButtons(0x22);

static unsigned long blinkTimer = millis();
static unsigned long buttonTimer = millis();

bool buttonFlag = false;

void setup(){
	Serial.begin(115200);
	delay(1000);
  Serial.println("start");

  pinMode(buttonPrevPin, INPUT_PULLUP);
  pinMode(buttonNextPin, INPUT_PULLUP);
  pinMode(buttonEnterPin, INPUT_PULLUP);
	pcfMemoryArea.pinMode(P0, OUTPUT, HIGH);
  pcfMemoryArea.pinMode(P1, OUTPUT, HIGH);
  pcfMemoryArea.pinMode(P2, OUTPUT, HIGH);
  pcfMemoryArea.pinMode(P3, OUTPUT, HIGH);
  pcfMemoryCell.pinMode(P0, OUTPUT, HIGH);
  pcfMemoryCell.pinMode(P1, OUTPUT, HIGH);
  pcfMemoryCell.pinMode(P2, OUTPUT, HIGH);
  pcfMemoryCell.pinMode(P3, OUTPUT, HIGH);
  pcfButtons.pinMode(P0, OUTPUT, HIGH);
  pcfButtons.pinMode(P1, OUTPUT, HIGH);
  pcfButtons.pinMode(P2, OUTPUT, HIGH);
  pcfButtons.pinMode(P3, OUTPUT, HIGH);
	
	Serial.println("Init pcf8574...");
	if (pcfMemoryArea.begin()){
		Serial.println("pcfMemoryArea - OK");
	}
    else{
      Serial.println("pcfMemoryArea - Error");
    }
  if (pcfMemoryCell.begin()){
    Serial.println("pcfMemoryCell - OK");
   }
    else{
      Serial.println("pcfMemoryCell - Error");
    }
  if (pcfButtons.begin()){
    Serial.println("pcfButtons - OK");
   }
    else{
      Serial.println("pcfButtons - Error");
    }
}

//bool interruptFlag = false;

void loop(){
  blink();
  detectButtons();
}

void blink(void){
  if (millis() - blinkTimer > 1000) {
    blinkTimer = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void detectButtons(void){

  bool btnPrevState = !digitalRead(buttonPrevPin);
  bool btnNextState = !digitalRead(buttonNextPin);
  bool btnEnterState = !digitalRead(buttonEnterPin);

  if (btnPrevState && !buttonFlag && millis() - buttonTimer > 50) {
    buttonFlag = true;
    buttonTimer = millis();
    //do something;
    //setPrevCommand();
    executeCommand(P0, commandArray[0]);
    executeCommand(P1, commandArray[1]);
    executeCommand(P2, commandArray[2]);
    executeCommand(P3, commandArray[3]);
  }
  if (btnNextState && !buttonFlag && millis() - buttonTimer > 50) {
    buttonFlag = true;
    buttonTimer = millis();
    //do something;
    //setNextCommand();
    executeCommand(P0, commandArray[4]);
    executeCommand(P1, commandArray[5]);
    executeCommand(P2, commandArray[6]);
    executeCommand(P3, commandArray[7]);
  }
  if (btnEnterState && !buttonFlag && millis() - buttonTimer > 50) {
    buttonFlag = true;
    buttonTimer = millis();
    //do something;
    executeCommand(P0, commandArray[8]);
    executeCommand(P1, commandArray[9]);
    executeCommand(P2, commandArray[10]);
    executeCommand(P3, commandArray[11]);
  }
  
  if ((!btnPrevState || !btnNextState || !btnEnterState) && buttonFlag && millis() - buttonTimer > 200) {
    buttonFlag = false;
    buttonTimer = millis();
    //Serial.println("released");
  }
}

void executeCommand(int cell, long data){
  setMemoryCell(cell);
  buttonHIGH();
  buttonReset();
  sendData(data); 
  buttonEnter();
}

void sendData(long value){
  // Serial.println("======");
  // Serial.println(value, BIN);
  for (int i=28; i>0; i--){
    if(BIT_CHECK(value, i-1)) {buttonHIGH(); Serial.print("1");}
    else {buttonLOW(); Serial.print("0");}
  }
  Serial.println("");
}

void setMemoryCell(int Area){
  pcfMemoryCell.digitalWrite(Area, LOW);
  pcfMemoryArea.digitalWrite(Area, LOW);
}

void buttonEnter(){
  pcfButtons.digitalWrite(P0, LOW);
  pcfButtons.digitalWrite(P0, HIGH);
}

void buttonReset(){
  pcfButtons.digitalWrite(P1, LOW);
  pcfButtons.digitalWrite(P1, HIGH);
}

void buttonHIGH(){
  pcfButtons.digitalWrite(P2, LOW);
  pcfButtons.digitalWrite(P2, HIGH);
}

void buttonLOW(){
  pcfButtons.digitalWrite(P3, LOW);
  pcfButtons.digitalWrite(P3, HIGH);
}
