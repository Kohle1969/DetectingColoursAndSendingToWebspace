// Libraries einbinden
#include "Servo.h"
#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); //Hier wird festgelegt um was für einen Display es sich handelt 
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

// Servo-Positionen in Grad für Farben
const int rot = 160;
const int orange = 130;
const int gelb = 100;
const int gruen = 75;
const int blau = 50;
const int nix = 20; // Kein Objekt erkannt
 
// Servo initialisieren
Servo myservo;
 
// Color Sensor initialisieren
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
 
// setup() wird einmal beim Start des Arduino ausgeführt
void setup() {
  lcd.init();
  lcd.backlight(); // LCD Display initiaisieren und aktivieren
  
  mySwitch.enableTransmit(10);  // Der Sender wird an Pin 10 angeschlossen
 
 // Serielle Kommunikation zur Ausgabe der Wert im seriellen Monitor
 Serial.begin(9600);
 
 // Überprüfen, ob Color Sensor sich auch zurückmeldet
 if (tcs.begin()) {
 // Alles OK
 Serial.println("Sensor gefunden");
 } else {
 // Kein Sensor gefunden. Programm an dieser Stelle einfrieren
 Serial.println("TCS34725 nicht gefunden ... Ablauf gestoppt!");
 lcd.setCursor(0, 0); 
 lcd.print("Sensor nicht");
 lcd.setCursor(0, 1); 
 lcd.print("gefunden."); //Fehlermeldung im LCD Display, falls Sensor einen Wackelkontakt hat
 while (1); // Halt!
 } 
 
 // Der Servo hängt am Pin 3 
 myservo.attach(3);
 // Servo in Grundstellung fahren
 myservo.write(nix);
 delay(1000);
 
}
 
// loop() wird wiederholt, solange der Arduino läuft
void loop() {
 lcd.setCursor(0, 0); 
  lcd.print("Die Farbe ist:"); // Grundausgabe für das LCD Display
 // Der Sensor liefert Werte für R, G, B und einen Clear-Wert zurück
 uint16_t clearcol, red, green, blue;
 float average, r, g, b;
 delay(100); // Farbmessung dauert c. 50ms 
 tcs.getRawData(&red, &green, &blue, &clearcol); //Rohdaten werden in "red,green,blue und clearcol" abgelegt

//-------------------------Farbbestimmung--------------------------------- 

 //Farbbestimmung für die Smarties-Farben Rot, Grün, Blau, Orange und Gelb
 // Durchschnitt von RGB ermitteln
 
 average = (red+green+blue)/3;
 
 //Farbwerte durch Durchschnitt alle Werte teien um verwendbare Werte zu erhalten
 
 r = red/average;
 g = green/average;
 b = blue/average;
 
 // Clear-Wert und r,g,b ausgeben zur Kontrolle
 Serial.print("\tClear:"); Serial.print(clearcol);
 Serial.print("\tRed:"); Serial.print(r);
 Serial.print("\tGreen:"); Serial.print(g);
 Serial.print("\tBlue:"); Serial.print(b);
 
 // Versuch der Farbfeststellung anhand der r,g,b-Werte.
 // Am besten mit Rot, Grün, Blau anfanen, sind einfacher als Gelb und Orange
 // serielle Ausgabe und versenden der passenden Dezimalzahl zu den Farben
 
 if ((r > 1.25) && (g < 0.85) && (b < 0.9)) { //Versuch der Grenzwertbestimmung
 Serial.print("\tROT");
 myservo.write(rot); //Rote Position wird vom Servo ausgegeben 
 lcd.setCursor(0, 1); //LCD wählt die erste Zeile und Spalte aus
 lcd.print("ROT     "); //"Rot" wird in das LCD geschrieben
 mySwitch.send(1, 24); // Der 433mhz Sender versendet die entsprechende Dezimalzahl
 }
 else if ((r < 0.95) && (g > 1.4) && (b < 0.8)) { //Die folgenden else if Schleifen verlaufen analog zur Ersten
 Serial.print("\tGRUEN");
 myservo.write(gruen);
 lcd.setCursor(0, 1); 
 lcd.print("GRUEN     ");
 mySwitch.send(2, 24); 
 }
 else if ((r < 0.65) && (g < 1.25) && (b > 1.15)) {
 Serial.print("\tBLAU");
 myservo.write(blau);
 lcd.setCursor(0, 1); 
 lcd.print("BLAU     ");
 mySwitch.send(3, 24); 
 }
 // Gelb und Orange sind etwas schwieriger
 else if ((r < 1.2) && (g < 1.4) && (b < 0.7)) {
 Serial.print("\tGELB");
 myservo.write(gelb);
 lcd.setCursor(0, 1); 
 lcd.print("GELB     ");
 mySwitch.send(4, 24); 
 }
 else if ((r > 1.4) && (g > 0.9) && (b < 0.7)) {
 Serial.print("\tORANGE");
 myservo.write(orange);
 lcd.setCursor(0, 1); 
 lcd.print("ORANGE     ");
 mySwitch.send(5, 24); 
 } 
 // Wenn keine Regel greift, dann keine Ausgabe
 else {
 Serial.print("\tNICHT ERKANNT"); 
 myservo.write(nix);
 lcd.setCursor(0, 1); 
 lcd.print("             ");
 mySwitch.send(6, 24);  
 }
 
 
 // Zeilenwechsel ausgeben
 Serial.println("");
 
 // Wartezeit anpassen für serielles Debugging
 delay(100);
 
}
