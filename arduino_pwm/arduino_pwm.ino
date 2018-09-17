/* 
@Autor: Jorge Sanabria
*/

#include <Wire.h>  // Lo trae Arduino IDE
#include <LiquidCrystal_I2C.h> // Incluimos la libreria del LCD

// Direccion LCD a 0x27 para el display de 20 caracteres 4 lineas
//Pines sobre el chip I2C usados para las conexiones del LCD:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

unsigned int TH, TL;
float Amplitud;
unsigned int temp_TH, temp_TL;
float temp_Amplitud;

boolean up;
unsigned long int timeNow, timeEnd;
unsigned int timeLeft, temp_timeLeft;

char charUp = 0xFF;

const int N = 64;

void setup(){
  
  analogReference(DEFAULT);
  
  up = true;
  timeNow = 0;
  timeEnd = 0;
  timeLeft = 0;
  
  TH = 0;
  TL = 0;
  Amplitud = 0.0;
  
  lcd.begin(20,4);

  lcd.setCursor(13,0);
  lcd.print("seconds");
  
  lcd.setCursor(0,1);
  lcd.print("TH = ");
  lcd.print(TH);
  lcd.setCursor(13,1);
  lcd.print("seconds");
  
  lcd.setCursor(0,2);
  lcd.print("TL = ");
  lcd.print(TL);
  lcd.setCursor(13,2);
  lcd.print("seconds");
  
  lcd.setCursor(0,3);
  lcd.print("Value = ");
  lcd.print(Amplitud);
  lcd.setCursor(15,3);
  lcd.print("Volts");

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT); //For led
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

unsigned long int seconds(){
  return millis()/1000;
}

unsigned int analogReadN(int port){
  unsigned int sum = 0;
  
  for(int i = 0; i < N; i++){
    sum += analogRead(port);
    delay(1);
  }

  return sum / N;
}

void loop(){

  //Check step
  timeNow = seconds();
  
  if(timeNow >= timeEnd){
    lcd.setCursor(0,0);
    if(up){
      up = false;
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
      timeEnd = timeNow + TL;
      lcd.print("Low  ");      
      lcd.print("___");
    }
    else{
      up = true;
      digitalWrite(12, HIGH);
      digitalWrite(13, HIGH);
      timeEnd = timeNow + TH;
      lcd.print("High ");      
      lcd.print(charUp);
      lcd.print(charUp);
      lcd.print(charUp);
    }
  }

  temp_timeLeft = timeLeft;
  timeLeft = timeEnd - timeNow;
  if(temp_timeLeft != timeLeft){
    lcd.setCursor(9,0);
    lcd.print("   ");
    lcd.setCursor(9,0);
    lcd.print(timeLeft);
  }
  
  //Update parameters
  temp_TH = TH;
  TH = analogReadN(A0)/2;
  if (temp_TH != TH){
    lcd.setCursor(5,1);
    lcd.print("       ");
    lcd.setCursor(5,1);
    lcd.print(TH);    
    if(up){
      timeEnd = timeEnd - temp_TH + TH;
    }
  }
  
  temp_TL = TL;
  TL = analogReadN(A1)/2;
  if (temp_TL != TL){
    lcd.setCursor(5,2);
    lcd.print("       ");
    lcd.setCursor(5,2);
    lcd.print(TL);
    if (!up){
      timeEnd = timeEnd - temp_TL + TL;
    }
  }
  
  temp_Amplitud = Amplitud;
  Amplitud = analogReadN(A2)*5.0/1023.0;
  if (256*abs(temp_Amplitud - Amplitud) > 1){
    lcd.setCursor(8,3);
    lcd.print("       ");
    lcd.setCursor(8,3);
    lcd.print(Amplitud);
  }

  //Wait a moment
  delay(10);
}
