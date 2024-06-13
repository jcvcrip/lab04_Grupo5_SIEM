/* UNIVERSIDAD TECNICA DEL NORTE
      SISTEMAS EMBEBIDOS 
            LAB 04
           GRUPO 02
 INTEGRANTES: JEAN VISCAINO, JHOJAN RIVERA, DAMIAN SANCHEZ           
*/

#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pines
#define DHTPIN 2         //sensor de temperatura y humedad
#define DHTTYPE DHT11    // se establece el tipo de sensor DHT
#define MOTOR_PIN 3      // Pin PWM para controlar el motor
#define SERVO_PIN 9      // Pin para controlar el servo
#define BUTTON_MOTOR_PIN 5
#define BUTTON_SERVO_PIN 6
#define LED_PIN 4        // Pin para el LED

// Inicialización de objetos
DHT dht(DHTPIN, DHTTYPE);
Servo servo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables de estado
bool motorManualMode = false; // Iniciar en modo automático
bool servoManualMode = true; // Iniciar en modo automático
int motorSpeed = 0;
int servoAngle = 0;
int servoDirection = 1;
int angleStep = 4; // Incremento del ángulo: mayor valor = más rápido
int delayTime = 40; // Retardo en milisegundos: menor valor = más rápido

unsigned long lastDebounceTimeMotor = 0;
unsigned long lastDebounceTimeServo = 0;
unsigned long debounceDelay = 50;
bool lastButtonStateMotor = HIGH;
bool lastButtonStateServo = HIGH;
bool motorButtonPressed = false;
bool servoButtonPressed = false;

void setup() {
  // Inicialización de componentes
  dht.begin();
  servo.attach(SERVO_PIN);
  lcd.init();
  lcd.backlight();

  pinMode(BUTTON_MOTOR_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SERVO_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); // Configurar el pin del LED como salida
}

void loop() {
  // Leer temperatura y humedad
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Leer botones con debounce
  int readingMotor = digitalRead(BUTTON_MOTOR_PIN);
  if (readingMotor != lastButtonStateMotor) {
    lastDebounceTimeMotor = millis();
  }
  if ((millis() - lastDebounceTimeMotor) > debounceDelay) {
    if (readingMotor == LOW && !motorButtonPressed) {
      motorManualMode = !motorManualMode;
      displayModeChange("Motor", motorManualMode);
      motorButtonPressed = true;
    } else if (readingMotor == HIGH) {
      motorButtonPressed = false;
    }
  }
  lastButtonStateMotor = readingMotor;
  
  int readingServo = digitalRead(BUTTON_SERVO_PIN);
  if (readingServo != lastButtonStateServo) {
    lastDebounceTimeServo = millis();
  }
  if ((millis() - lastDebounceTimeServo) > debounceDelay) {
    if (readingServo == LOW && !servoButtonPressed) {
      servoManualMode = !servoManualMode;
      displayModeChange("Giro", servoManualMode);
      servoButtonPressed = true;
    } else if (readingServo == HIGH) {
      servoButtonPressed = false;
    }
  }
  lastButtonStateServo = readingServo;

  // Leer el valor del potenciómetro para la temperatura máxima
  int potValue = analogRead(A2);
  int maxTemp = map(potValue, 0, 1023, 20, 50); // Ajustar el rango de temperatura según sea necesario

  // Control del motor
  if (motorManualMode) {
    // Modo manual (controlar con una entrada de usuario, por ejemplo un potenciómetro)
    motorSpeed = analogRead(A0) / 4; // Ajusta esto según tu entrada manual
  } else {
    // Modo automático (dependiendo de la temperatura y humedad)
    if (humidity > 75) {
      motorSpeed = 255; // Máxima velocidad si la humedad es superior a 75%
    } else {
      motorSpeed = map(temp, 24, maxTemp, 0, 255);
      motorSpeed = constrain(motorSpeed, 0, 255);
    }
  }
  analogWrite(MOTOR_PIN, motorSpeed);
  
  // Control del servo
  if (servoManualMode) {
    // Modo manual (controlar con una entrada de usuario, por ejemplo un potenciómetro)
    servoAngle = analogRead(A1) / 6; // Ajusta esto según tu entrada manual
    servo.write(servoAngle);
  } else {
    // Modo automático (girar de 0° a 180° en bucle)
    if (motorSpeed != 85) { // Verificar que el PWM del motor no esté en 85
      servo.write(servoAngle);
      servoAngle += servoDirection * angleStep;

      if (servoAngle <= 0 || servoAngle >= 180) {
        servoDirection = -servoDirection;
        servoAngle = constrain(servoAngle, 0, 180); // Asegurarse de que el ángulo esté dentro de los límites
      }

      delay(delayTime); // Ajusta la velocidad del movimiento del servo
    }
  }
  
  // Control del LED según la humedad
  if (humidity > 75) {
    digitalWrite(LED_PIN, HIGH); // Encender el LED
  } else {
    digitalWrite(LED_PIN, LOW); // Apagar el LED
  }

  // Actualizar LCD
  if (humidity > 75) {
    // Mostrar mensaje de exceso de humedad
    lcd.setCursor(0, 0);
    lcd.print("   Exceso de        ");
    lcd.setCursor(0, 1);
    lcd.print("    humedad         ");
  } else if (temp > maxTemp) {
    // Mostrar mensaje de exceso de temperatura
    lcd.setCursor(0, 0);
    lcd.print("  Exceso de        ");
    lcd.setCursor(0, 1);
    lcd.print("   temperatura     ");
    lcd.print("               ");
  } else {
    // Mostrar temperatura y humedad
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp);
    lcd.setCursor(6, 0);
    lcd.print("C ");
    lcd.setCursor(8, 0);
    lcd.print("H:");
    lcd.print(humidity);
    lcd.print("%");
    lcd.print("               ");
    
    lcd.setCursor(0, 1);
    lcd.print("vel:");
    lcd.print(motorSpeed);
    lcd.print(" ");

    if (!motorManualMode) {
      lcd.setCursor(8, 1);
      lcd.print("TMax:");
      lcd.print(maxTemp);
      lcd.print("                    ");
    } else {
      lcd.print("                    "); // Limpiar cualquier carácter anterior
    }
  }

  delay(100); // Ajusta la frecuencia de actualización
}

void displayModeChange(const char* device, bool mode) {
  lcd.setCursor(0, 1);
  lcd.print(device);
  lcd.print(":");
  lcd.print(mode ? "Manual" : "Auto");
  lcd.print("                           "); // Limpiar cualquier carácter anterior
  delay(2000);
  // Limpiar el mensaje y volver a mostrar la velocidad
  lcd.setCursor(0, 1);
  lcd.print("Vel: ");
  lcd.print(motorSpeed);
  lcd.print("                           "); // Limpiar cualquier carácter anterior
}
