#include <DHT.h>
#include <Wire.h> // Necesario para I2C
#include <LiquidCrystal_I2C.h>

// Crear una instancia del LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definir los pines del sensor DHT11, del LED y del potenciómetro
const int dhtPin = 2;  // Pin del sensor DHT11
const int ledPin = 3;  // Pin del LED
const int potPin = A0; // Pin del potenciómetro

// Definir el tipo de sensor DHT11
#define DHTTYPE DHT11

// Crear una instancia del sensor DHT
DHT dht(dhtPin, DHTTYPE);

void setup() {
  lcd.init();  // Inicialización del LCD
  lcd.backlight();  // Encender la luz de fondo del LCD

  // Inicializar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  // Inicializar el sensor DHT
  dht.begin();
}

void loop() {
  // Leer la temperatura del sensor DHT11
  float temperature = dht.readTemperature();
  // Leer el valor del potenciómetro
  int potValue = analogRead(potPin);
  // Mapear el valor del potenciómetro a un rango de temperatura (ej. 15 a 30 grados Celsius)
  float tempThreshold = map(potValue, 0, 1023, 0, 60);

  // Limpiar la pantalla LCD
  lcd.clear();
  
  // Mostrar la temperatura en el LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  // Mostrar la escala de temperatura en el LCD
  lcd.setCursor(0, 1);
  lcd.print("Fijar Temp:");
  lcd.print(tempThreshold);
  lcd.print(" C");

  // Verificar si la lectura es válida
  if (isnan(temperature)) {
    Serial.println("Error al leer la temperatura!");
  } else {
    // Verificar si la temperatura es mayor o igual al umbral
    if (temperature <= tempThreshold) {
      // Encender el LED
      digitalWrite(ledPin, HIGH);
    } else {
      // Apagar el LED
      digitalWrite(ledPin, LOW);
    }
  }

  delay(1000);  // Esperar 1 segundo antes de leer la temperatura nuevamente
}

