#include "Arduino.h"
#include "PCF8574.h"

#define buttonPrevPin 2
#define buttonNextPin 3
#define buttonEnterPin 4

void blink();
void detectButtons();
void executeCommand();

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

  pinMode(buttonPrevPin, INPUT);
  pinMode(buttonNextPin, INPUT);
  pinMode(buttonEnterPin, INPUT);
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
	
	Serial.print("Init pcf8574...");
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
//    setPrevCommand();
  }
  if (btnNextState && !buttonFlag && millis() - buttonTimer > 50) {
    buttonFlag = true;
    buttonTimer = millis();
    //do something;
//    setNextCommand();
  }
  if (btnEnterState && !buttonFlag && millis() - buttonTimer > 50) {
    buttonFlag = true;
    buttonTimer = millis();
    //do something;
    executeCommand(P0, 23535);
  }
  
  if ((!btnPrevState || !btnNextState || !btnEnterState) && buttonFlag && millis() - buttonTimer > 200) {
    buttonFlag = false;
    buttonTimer = millis();
    //Serial.println("released");
  }
}

void executeCommand(int cell, int data){
  setMemoryCell(cell);
  buttonReset();
  sendData(data); 
  buttonEnter();
}

void sendData(unsigned long value){
  //принять число и отправлять на pcfButtons P2 1 и на P3 0
  for (int i=32;i<0;i--){
    if(bit_is_set(value, i)) buttonHIGH();
    if(bit_is_clear(value, i)) buttonLOW();
  }
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
