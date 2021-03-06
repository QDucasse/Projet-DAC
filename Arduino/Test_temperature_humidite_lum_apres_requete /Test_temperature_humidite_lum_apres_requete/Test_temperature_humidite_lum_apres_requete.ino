#include <OneWire.h>
#include "DHT.h"


/* DHT22 */
const int dhtpin = 9;
DHT dht(dhtpin, DHT22);

/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 7;
const int out =8;
String temp="";

/* Photoresistannce */
const int photoresistance =0;

/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};

OneWire ds(BROCHE_ONEWIRE);

/**
 * Fonction de lecture de la température via un capteur DS18B20.
 */
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté
  
  /* Reset le bus 1-Wire ci nécessaire (requis pour la lecture du premier capteur) */
  if (reset_search) {
    ds.reset_search();
  }
 
  /* Recherche le prochain capteur 1-Wire disponible */
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  
  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
 
  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
 
  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(addr);
  
  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
  delay(800);
  
  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
 
 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
   
  /* Calcul de la température en degré Celsius */
  *temperature = (int16_t) ((data[1] << 8) | data[0]) * 0.0625; 
  
  // Pas d'erreur
  return READ_OK;
}

void allumer_led(int n, float t)
{
  digitalWrite(out,HIGH);
  delay(t);
  digitalWrite(out,LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(out,OUTPUT);
  digitalWrite(out,LOW);
}


void loop() {
  
    // LECTURE PORT SERIE ----------------------------------------------------
    String requete= "";
    char carlu = 0;

    // On lit les caractères tant qu'il y en a qui sont envoyé dans le port Série.
    if(Serial.available()) // tant qu'il y a des caractères à lire
    {
        carlu = Serial.read(); // on lit le caractère
        if (carlu!='\n')
        {
        temp=temp+carlu; // Si ce n'est pas un retour charriot alors on le concatène dans le String "temp"
        }
        if (carlu=='$')//Si le caractère est un '$' il s'agit du caractère de fin de commande
        {
          requete=temp; //La requete devient le "temp"
          temp=""; //On vide le "temp"
        }
    }

    if (requete!="")
    {
          if (requete=="T1$")//Si la requete est d'accéder à la température alors on allume la LED 0,5s et on renvoit la température
          {
              
            float temperature;
             
            /* Lit la température ambiante à ~1Hz */
            if (getTemperature(&temperature, true) != READ_OK) {
              Serial.println(F("Erreur de lecture du capteur"));
            }
            else
            {
              allumer_led(out,500);
              Serial.println(temperature, 2);
             }
          }
          else if (requete=="Hum$")
          {
            float hum=dht.readHumidity();
            if (isnan(hum))
            {
              Serial.println("Erreur de lecture du capteur");
            }
            else
            {
              Serial.println(hum);
            }
          }
          else if (requete=="T2$")
          {
            float temp=dht.readTemperature();
            if (isnan(temp))
            {
              Serial.println("Erreur de lecture du capteur");
            }
            else
            {
              Serial.println(temp);
            }
          }
          else if (requete=="Lum$")
          {
            int lum=analogRead(photoresistance);
            Serial.println(lum);
          }
          else 
          {
            Serial.println("Requete inconnue");
          }
          requete ="";
          
     }
}
