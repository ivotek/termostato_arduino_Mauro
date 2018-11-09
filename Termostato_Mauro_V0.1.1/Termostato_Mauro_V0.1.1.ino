//*****************************************************
//
// Data inizio 8/11/2018
// Data ultima modfica 9/11/2018
// Realizzato da Daniele loddo alias Ivotek
// Contatti www.ivotek.it ivotek@gmail.com
//
// Credits:
//
// Librerie necessarie presenti nel gestore di librerie di arduino 1.8.7
// LCD LiquidCrystal
// Comunicazione OneWire
// Decodifica protocollo DallasTemperature
//
// Display utilizzato Hitachi HD44780 1602 parallelo
// Piedinatura:
// LCD RS pin to digital pin 12
// LCD Enable pin to digital pin 11
// LCD D4 pin to digital pin 5
// LCD D5 pin to digital pin 4
// LCD D6 pin to digital pin 3
// LCD D7 pin to digital pin 2
// LCD R/W pin to ground
// LCD VSS pin to ground
// LCD VCC pin to 5V
// 10K resistor: ends to +5V and ground
// wiper to LCD VO pin (pin 3)
//
// Pulsante di start pin 7 NA collegato a massa
// Pulsante di stop pin 8  NA collegato a massa
//
// Relè attivazione resistenza di riscaldamenti  pin 6 HIGH=attivo LOW=disattivo
//
// Sensore temperatura pin 9 modello DS18B20
//
//*******************************************************************


//Libreria per LCD
#include <LiquidCrystal.h>
//Libreria per la comunicazone
#include <OneWire.h>
//Libreria per la decodifica
#include <DallasTemperature.h>

//Associazione dei piedini con le funzioni del display LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Va alla temperatura passata come parametro
void GoTemp(int);
//Mantiene la temperatura per il tempo passato
void KeepTime(int);
//Attende la pressione del pulsante start
void WaitStart();
//Termina il programma
void Stop ();


//Prima temperatura
const int firstTemp = 70 ;
//Seconda temperatura
const int secondTemp = 100;
//Primo tempo
const int firstTime = 30;
//Secondo tempo
const int secondTime = 30;
//Relè per attivare la resistenza
const int rele = 6;
//Pulsante di start
const int pulsStart = 7;
//Pulsante id stop
const int pulsStop = 8;
//Sensore di temperatura
const int DS18B20_Pin = 9;

//Assegno il piedino per la comuncicazione one wire
OneWire oneWire(DS18B20_Pin); 

//Specifico quale codifica deve essere fatta
DallasTemperature sensors(&oneWire);

void setup() {
  //Settaggi iniziali display

  //DEBUG
  Serial.begin(9600);
  
  //Setto il numero di linne e colonne:
  lcd.begin(16, 2);
  // Stampo la scritta iniziliale
  lcd.print("ATTENDERE");
  lcd.setCursor(0,2);
  lcd.print("AVVIO");
  delay(1000);

  //Settaggi porte
  //Pulsanti
  pinMode(pulsStart,INPUT_PULLUP);
  pinMode(pulsStop,INPUT_PULLUP);

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio Setup");
  Serial.print("INFORMAZIONI: Prima temperatura impostata: ");
  Serial.println(firstTemp);
  Serial.print("INFORMAZIONI: Primo tempo impostato: ");
  Serial.println(firstTime);
  Serial.print("INFORMAZIONI: Seconda temperatura impostata: ");
  Serial.println(secondTemp);
  Serial.print("INFORMAZIONI: Secondo tempo impostato: ");
  Serial.println(secondTime);

}

void loop() {
  //Saluto iniziale
  HelloInit();

  //Attendo la pressione del pulsante start
  WaitStart();
  
  //Porta la temperatura a 70 gradi
  GoTemp(firstTemp);

  //Mantiene la temepratura per 30 minuti
  KeepTime(firstTime);
  
  //Porta la temperatura a 100 gradi
  GoTemp(secondTemp);

  //Mantiene la temperatura
  KeepTime(secondTime);
  
  //Spegne tutto
  Stop();

}

//Inizializzazione del LCD e messaggio di partenza 
void HelloInit(){

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio HelloInit");
  
  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Pronto.");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0,2);
  //Invio il messaggio
  lcd.print("Premere start");

  //FIXME DA ELIMINARE
  delay(3000);
}

//Attesa della pressione del pulsante start
void WaitStart(){

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio WaitStart");
  
  //Se il pulsante non è premuto rimango nel ciclo
  while (digitalRead(pulsStart)==HIGH){};
  
}

//Vado alla temepratura passata come parametro
void GoTemp(int temp){

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio GoTemp");

  //Ciclo per il controllo della temperatura

  //Sino a quando la temepratura è bassa
  //effettuo il ciclo
  //ATTENZIONE!! Le due variabili sono differenti 
  // GetTemp è float temp è int
  while(GetTemp()<temp){
    //Cancello lo schermo
    lcd.clear();
    //Invio il messaggio
    lcd.print("Inizio aumento");
    //Mi sposto alla colonna 0 riga 2
    lcd.setCursor(0,2);
    //Invio il messaggio
    lcd.print("temperatura: ");
    lcd.print(temp);
    lcd.print((char)223);

    //Attivo il relè
    digitalWrite(rele,HIGH);
    //DEBUG
    Serial.println("INFORMAZIONI: Relè attivo");
  }

    //Finito il ciclo disattivo il relè
    digitalWrite(rele,LOW);
    //DEBUG
    Serial.println("INFORMAZIONI: Relè disattivo");
  

  //FIXME DA ELIMINARE
  delay(3000);
}

//Tengo la temperatura per il tempo passato come parametro
void KeepTime(int ktime){

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio KeepTime");
  
  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Mantenimento");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0,2);
  //Invio il messaggio
  lcd.print("temperatura: ");
  lcd.print(ktime);
  lcd.print("sec");

  //Ciclo per il controllo del tempo
    //Ciclo per il mantenimento della temepratura
    
  //FIXME DA ELIMINARE
  delay(3000);
  
}

//Fermo i processi
void Stop (){

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio Stop");
  
  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Arrresto del");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0,2);
  //Invio il messaggio
  lcd.print("sistema");

  //Controllo il perchè dello stop
  //Se è un "emergenza" setto i flag
  //Sistemo le uscite
  
  //FIXME DA ELIMINARE
  delay(3000);  
}

//Leggo e ritorno la temperatura
float GetTemp(){
  
  //DEBUG
  Serial.println("INFORMAZIONI: Avvio GetTemp");
  
  //Richiedo la temperatura al sensore
  sensors.requestTemperatures();

  //DEBUG
  Serial.print("INFORMAZIONI: Temepratura misurata = ");
  Serial.println(sensors.getTempCByIndex(0));
  
  //Ritorno la temperatura corrente
  return sensors.getTempCByIndex(0);
}
