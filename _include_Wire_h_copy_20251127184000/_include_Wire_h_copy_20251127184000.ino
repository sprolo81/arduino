#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección I2C típica del módulo: 0x27 o 0x3F.
// Si no te funciona con 0x27, probá 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SDA_PIN  8
#define SCL_PIN  9

void setup() {
  Wire.begin(SDA_PIN,SCL_PIN);

  lcd.init();          // inicializa display
  lcd.backlight();     // enciende luz de fondo

}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Hola Venados!");

  lcd.setCursor(0, 1);
  lcd.print("Asi esta el mundo amigos");
   lcd.scrollDisplayLeft();  // desplaza ambos renglones hacia la izquierda
  delay(250); 
}