/*
 * CAPÍTULO IV: PERIFÉRICOS ESPECIALES
 * CÓDIGO 14: CAD Y PWM
 * OBJETIVO: Realizar lecturas del converso análogo digital para
 *           determinar la velocidad de un motor en DC
 * FUNCIONES: analogRead(conversor)
 *                        conversor-> A0-A5
 *            analogWrite(pin, dutty)                    
 *                        pin-> pines con ~ , dutty->0-255
 *                 
 */
#include <Wire.h> // Necesario para I2C
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int conversor; // variable para adquirir datos del conversor entre 0-1023
float voltaje; // variable de conversión de conversor a voltaje
int dutty;   // variable para uso de pwm
const int motorPin = 10; // Pin PWM para controlar el motor

void setup() {
  lcd.init();  // Inicialización del LCD
  lcd.backlight();  // Encender la luz de fondo del LCD
  pinMode(motorPin, OUTPUT); // Configurar el pin del motor como salida
}

void loop() {
  conversor = analogRead(A0); // Leer valor del potenciómetro
  voltaje = (conversor / 1023.0) * 5.0; // Convertir a voltaje
  lcd.setCursor(0, 0);
  lcd.print("Voltaje: ");
  lcd.print(voltaje);
  lcd.print(" V");
  
  // Determinar la velocidad del motor basada en el valor del potenciómetro
  if (conversor < 256) {
    dutty = 0; // Motor detenido
   lcd.init();  // Inicialización del LCD
  lcd.backlight();  // Encender la luz de fondo del LCD
  } else if (conversor < 512) {
    dutty = 85; // Baja velocidad
    lcd.setCursor(0, 1);
    lcd.print("Velocidad: 1");
  } else if (conversor < 768) {
    dutty = 170; // Velocidad media
    lcd.setCursor(0, 1);
    lcd.print("Velocidad: 2");
  } else {
    dutty = 255; // Alta velocidad
    lcd.setCursor(0, 1);
    lcd.print("Velocidad: 3");
  }
  
  analogWrite(motorPin, dutty); // Escribir el valor de PWM al pin del motor
  delay(100); // Pequeña pausa para evitar lecturas excesivas
}
