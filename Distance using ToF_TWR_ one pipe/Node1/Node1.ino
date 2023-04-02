#include <SPI.h>
#include <RF24.h>

#define pinCE   7             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  8             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel  "PIPE0"       // On définit le "nom de tunnel" (5 caractères) à travers lequel on va recevoir les données de l'émetteur
#define Speed_OF_Light  299792458 

RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01

const byte adresse[6] = tunnel;       // Mise au format "byte array" du nom du tunnel
const char* message;                     // Avec cette librairie, on est "limité" à 32 caractères par message
unsigned long sendTime;
unsigned long recievedTime;
unsigned long Distance;
unsigned long ToF2;

void setup() {
  // Initialisation du port série (pour afficher les infos reçues, sur le "Moniteur Série" de l'IDE Arduino)
  Serial.begin(9600);
  Serial.println("Node1");
  Serial.println("");

  // Partie NRF24
  radio.begin();                      // Initialisation du module NRF24
  radio.setChannel(100);               // Sélection du channal 100 correspond à la fréquence 2.410 Ghz (pour éviter les interferance avec BLE and WIFI)
  radio.openWritingPipe(adresse);     // Ouverture du tunnel en ECRITURE
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.setPayloadSize(32);           // In order to provide a Full-Duplex communication in PIPE1(set payload to 32)
  radio.stopListening();             // Démarrage de l'envoi du NRF24 (signifiant qu'on va émettre, et non recevoir quoi que ce soit, ici)
}


void loop() {
  message="Hello Node2!";
  sendTime=micros();// Envoyer à la Noeud2 un message // Timer Starter
  radio.write(&message,sizeof(message));
  Serial.print("From Node1 to Node2 : ");Serial.println(message);
  radio.openReadingPipe(0, adresse);  // Ouverture du tunnel en LECTURE, avec le "nom" qu'on lui a donné
  // Basculer en mode lecture
  radio.startListening(); 
  if (radio.available()) {                      
    // Recevoir de la Noeud2 un message pour acquisé la reception // Timer Stop
    sendTime=micros(); 
    radio.read(&message,sizeof(message)); // Récuperer le message
    Serial.print("From Node2 to Node1 : ");Serial.println(message);
  }
  ToF2=recievedTime-sendTime;
  Distance=(ToF2*0.0000001*Speed_OF_Light)/2; // Dividing by two becaude ToF2 corresponds to time needed for the two way data exchage
  Serial.print("The distance between the tow nodes is :");Serial.print(String(Distance));Serial.println(" m");
  delay(1000);
  radio.stopListening();   
}