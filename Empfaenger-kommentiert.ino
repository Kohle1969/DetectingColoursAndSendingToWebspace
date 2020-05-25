#include <RCSwitch.h> //Einbinden der Librarys
RCSwitch mySwitch = RCSwitch();
#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Festlegen den MAC Adresse des Ethernet Shields

//-------------------Internet Einstellungen---------------------------

char server[] = "jonaskohlenberger.de"; //Website für den Zugriff und die Übertragung
IPAddress ip(192,168,200,55); //IP-Adresse des Arduinos
EthernetClient client; 

int ra = 0; //Definition der Variablen für das PHP Script
int ga = 0;
int ba = 0;
int gea = 0;
int oa = 0;
int an = 0;

void setup() {
Serial.begin(9600);
mySwitch.enableReceive(0);  // Empfänger ist am Pin2 angeschlossen
  
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to connect to Ethernet");
  }
  else{Ethernet.begin(mac, ip);} //Falls eine Verbindung zum Internet aufgebaut werden kann, wird diese ausgeführrt. Wenn nicht wird ein Fehlercode ausgegeben.
  
  delay(1000);
}

void loop() {   

if(client.connect(server, 80)) 
{Serial.println("connected");} 
else{Serial.println("connection failed");} //Die Verbindung zum Server wird über den Port 80 aufgebaut, falls nicht, wird ein Fehlercode ausgegeben.

//---------------------------Interpretation der empfangenen Dezimalzahlen------------------------------------
  
  if (mySwitch.available()) // Wenn ein Code Empfangen wird...
  {
    int value = mySwitch.getReceivedValue(); // Empfangene Daten werden unter der Variable "value" gespeichert.
  
    if (value == 6) // Wenn die Empfangenen Daten "6" sind, wird "Keine Farbe erkannt" angezeigt.
    {
      Serial.print("Keine Farbe erkannt");
    } 
    else if (value == 1)// Wenn der Empfangene Code brauchbar ist, wird er hier in den enstprechenden Variablen gespeichert und jedesmal hochgezählt
    {                   // wenn die entsprechende Farbe erneut auftaucht, um die gesamte Anzahl der Farben zu erhalten. an(Anzahl) wird jedesmal hochgezählt, um die gesamte Anzahl zu erhalten. 
      Serial.print("Die Farbe ist: ROT");
      ra = ra + 1;
      an = an + 1;
      Sending_To_phpmyadmindatabase(); //Festgelegte Funktion wird ausgeführt
    }
    else if (value == 2) //Die restlichen else if Schleifen verlaufen analog zur Ersten
    {
      Serial.print("Die Farbe ist: GRUEN");
      ga = ga + 1;
      an = an + 1;
      Sending_To_phpmyadmindatabase(); 
    }
    else if (value == 3)
    {
      Serial.print("Die Farbe ist: BLAU");
      ba = ba + 1;
      an = an + 1;
      Sending_To_phpmyadmindatabase();
    }
    else if (value == 4)
    {
      Serial.print("Die Farbe ist: GELB");
      gea = gea + 1;
      an = an + 1;
      Sending_To_phpmyadmindatabase(); 
    }
    else if (value == 5)
    {
      Serial.print("Die Farbe ist: ORANGE");
      oa = oa + 1;
      an = an + 1;
      Sending_To_phpmyadmindatabase(); 
    }
    }
    else{ Serial.print("Kein Signal"); } //Wenn nichts empfangen wird, wird dies in den seriellen Monitor geschrieben

mySwitch.resetAvailable(); // Hier wird der Empfänger "resettet"



Serial.println("");

delay(700);
}

//----------------------Funktionen-----------------------------------------

void Sending_To_phpmyadmindatabase(){   //Mithilfe des PHP-Scripts werden die Variablen in die Adresszeile eines Browsers geschrieben  
    client.print("GET /Sendescript.php?r=");  //und so auf die Datenbank übertragen
    client.print(ra); 
    client.print("&g=");
    client.print(ga);
    client.print("&b=");
    client.print(ba);
    client.print("&ge=");
    client.print(gea);
    client.print("&o=");
    client.print(oa);
    client.print("&a=");
    client.print(an);
    client.print(" ");      //Einen Platz freilassen HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: jonaskohlenberger.de");
    client.println("Connection: close");
    client.println();}  
