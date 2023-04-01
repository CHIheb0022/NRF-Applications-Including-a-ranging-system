#include <SPI.h>
#include <RF24.h>

#define pinCE   7             // On associe la broche "CE" du NRF24L01 à la sortie digitale D7 de l'arduino
#define pinCSN  8             // On associe la broche "CSN" du NRF24L01 à la sortie digitale D8 de l'arduino
#define tunnel  "PIPE0"       // On définit le "nom de tunnel" (5 caractères) à travers lequel on va recevoir les données de l'émetteur
#define Speed_OF_Light  299792458 

RF24 radio(pinCE, pinCSN);    // Instanciation du NRF24L01

const byte adresse[6] = tunnel;       // Mise au format "byte array" du nom du tunnel
char message[32];                     // Avec cette librairie, on est "limité" à 32 caractères par message
unsigned long sendTime;
unsigned long recievedTime;
unsigned long Distance;
unsigned long ToF2;

void setup() {
  // Initialisation du port série (pour afficher les infos reçues, sur le "Moniteur Série" de l'IDE Arduino)
  Serial.begin(9600);
  Serial.println("Node2");
  Serial.println("");

  // Partie NRF24
  radio.begin();                      // Initialisation du module NRF24
  radio.setChannel(100);               // Sélection du channal 100 correspond à la fréquence 2.410 Ghz (pour éviter les interferance avec BLE and WIFI)
  radio.openReadingPipe(0, adresse);  // Ouverture du tunnel en LECTURE, avec le "nom" qu'on lui a donné
  radio.openWritingPipe(adresse);     // Ouverture du tunnel en ECRITURE
  radio.setPALevel(RF24_PA_MIN);      // Sélection d'un niveau "MINIMAL" pour communiquer (pas besoin d'une forte puissance, pour nos essais)
  radio.setPayloadSize(32);           // In order to provide a Full-Duplex communication in PIPE1(set payload to 32)
  radio.startListening();             // Démarrage de l'écoute du NRF24 (signifiant qu'on va recevoir, et non émettre quoi que ce soit, ici)
}


void loop() {
  // Recevoir à partir de la Noeud1 
  if (radio.available()){
    radio.read(&message,sizeof(message));
  }
  Serial.println("Message Recieved From Nod1");
  // Basculer en mode écriture
  radio.stopListening();  
  // Il faut extraire le retard resultant de cette instruction
  radio.write(&message=,sizeof(message));   
  Serial.println("Acknologment sent to node 1 saying " + message); // Pour acqusé la reception
}