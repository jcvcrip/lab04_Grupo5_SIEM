/*
 * CAPÍTULO IV: PERIFÉRICOS ESPECIALES
 * CÓDIGO 15: CAD Y SERVO
 * OBJETIVO: Realizar el giro de un servo motor hasta el límite ingresado por comunicación serial
 * FUNCIONES: analogRead(conversor)
 *            conversor-> A0-A5
 *            analogWrite(pin, dutty)                    
 *            pin-> pines con ~ , dutty->0-255
 */

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);


Servo servo1;
int conversor1;
float voltaje1;
int dutty1;
String dato;
int giro = 0;
String keyInput = "";
int gradosServo1 = 0;
int gradosServo2 = 0;
bool escribiendo = false;

// Variables para mantener los últimos valores válidos
int lastValidGiro = 0;
int lastValidGradosServo2 = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  servo1.attach(2);


}

void loop() {
 
  conversor1 = analogRead(A1);
  voltaje1 = (conversor1 * 5.0) / 1023.0;
  dutty1 = conversor1 / 4;
  analogWrite(6, dutty1);

  gradosServo1 = map(conversor1, 0, 1023, 0, 180);
  servo1.write(gradosServo1);


  lcd.setCursor(0, 0);
  lcd.print("P: ");
  lcd.print(conversor1);
    lcd.print("       ");

  lcd.setCursor(0, 1);
  lcd.print("G: ");
  lcd.print(gradosServo1);
  //lcd.print(" V1:");
  //lcd.print(voltaje1, 2);

  lcd.print(" PWM:"); // Impresión de datos en LCD
  lcd.print((dutty1*100)/255); // Impresión de datos en LCD
  
  lcd.print("       ");

 
 
 

  delay(300);
  delay(15);
}
