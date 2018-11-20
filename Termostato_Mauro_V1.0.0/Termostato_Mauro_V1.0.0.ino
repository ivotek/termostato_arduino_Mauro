//*****************************************************
//
// Data inizio 8/11/2018
// Data ultima modfica 10/11/2018
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
// Pulsante di start pin 10 NA collegato a massa
// Pulsante di stop pin 13  NA collegato a massa
//
// Relè attivazione resistenza di riscaldamenti  pin 6 HIGH=attivo LOW=disattivo
//
// Sensore temperatura pin 8 modello DS18B20
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
const float firstTemp = 70 ;
//Seconda temperatura
const float secondTemp = 100;
//Primo tempo
const long firstTime = 30;
//Secondo tempo
const long secondTime = 30;
//Relè per attivare la resistenza
const int rele = 6;
//Pulsante di start
const int pulsStart = 10;
//Pulsante id stop
const int pulsStop = 13;
//Sensore di temperatura
const int DS18B20_Pin = 8;
//Variabile booleana per segnalare lo stop non regolare
boolean flgStop = false;
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
  lcd.setCursor(0, 2);
  lcd.print("AVVIO");
  delay(1000);

  //Settaggi porte
  //Pulsanti
  pinMode(pulsStart, INPUT_PULLUP);
  pinMode(pulsStop, INPUT_PULLUP);

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
  KeepTime(firstTime, firstTemp);

  //Porta la temperatura a 100 gradi
  GoTemp(secondTemp);

  //Mantiene la temperatura
  KeepTime(secondTime, secondTemp);

  //Spegne tutto
  Stop();

}

//Inizializzazione del LCD e messaggio di partenza
void HelloInit() {

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio HelloInit");

  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Pronto.");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0, 2);
  //Invio il messaggio
  lcd.print("Premere start");
}

//Attesa della pressione del pulsante start
void WaitStart() {

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio WaitStart");

  //Se il pulsante non è premuto rimango nel ciclo
  //A meno chè non venga premuto il pulsante di stop
  while (1) {
    //Pulsante start premuto, esco dal ciclo
    if (digitalRead(pulsStart) == LOW) return;
    //Pulsante stop premuto, esco dal ciclo e setto la variabile come true
    if (digitalRead(pulsStop) == LOW ) {
      //Setto la variabile
      flgStop = true;
      //Esco dal ciclo
      return;
    }
  };
}

//Vado alla temepratura passata come parametro
void GoTemp(int temp) {

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio GoTemp");

  //Ciclo per il controllo della temperatura

  //Sino a quando la temperatura è bassa effettuo il ciclo
  //Salto il cicl ose la variabileflgStop è settata su true
  while ((GetTemp() < temp) &&(flgStop==false) ) {
    //Cancello lo schermo
    lcd.clear();
    //Invio il messaggio
    lcd.print("Inizio aumento");
    //Mi sposto alla colonna 0 riga 2
    lcd.setCursor(0, 2);
    //Invio il messaggio
    lcd.print("temperatura: ");
    lcd.print(temp);
    lcd.print((char)223);

    //Attivo il relè
    digitalWrite(rele, HIGH);
    //DEBUG
    Serial.println("INFORMAZIONI: Relè attivo");

    //Controllo se ha premuto il pulsante di stop
    if (digitalRead(pulsStop) == LOW ){
      //Setto la variabilea true
      flgStop = true;
      //Esco dalla funzione
      return;
    }
  }

  //Finito il ciclo disattivo il relè
  digitalWrite(rele, LOW);
  //DEBUG
  Serial.println("INFORMAZIONI: Relè disattivo");

}

//Tengo la temperatura per il tempo passato come parametro
void KeepTime(int ktime, float temp) {

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio KeepTime");

  //Creo un ciclo per il tempo passato come parametro
  //Prendo il tempo
  long timenow = millis();
  //Converto il ktime da minuti a millisecondi
  ktime = ktime * 1000 * 60;
  while ((millis() - timenow ) < ktime && (flgStop == false)) {
    //Il tempo è minore quindi controllo la temperatura
    if (GetTemp() < temp) {
      //La temperatura è minore
      //Attivo il relè
      digitalWrite(rele, HIGH);

      //Aggiorno il display
      //Cancello lo schermo
      lcd.clear();
      //Invio il messaggio
      lcd.print("Mantenimento");
      //Mi sposto alla colonna 0 riga 2
      lcd.setCursor(0, 2);
      //Invio il messaggio
      lcd.print("temperatura: ");
      lcd.print(ktime);
      lcd.print("sec");
    }
    else
    {
      //La temperatura è paggiore
      //Disattivo il relè
      digitalWrite(rele, LOW);
      //Aggiorno il display
      //Cancello lo schermo
      lcd.clear();
      //Invio il messaggio
      lcd.print("Mantenimento");
      //Mi sposto alla colonna 0 riga 2
      lcd.setCursor(0, 2);
      //Invio il messaggio
      lcd.print("temperatura: ");
      lcd.print(ktime);
      lcd.print("sec");
    }
  }

  //Finito il ciclo disattivo il relè
  digitalWrite(rele, LOW);

  //Controllo se ha premuto il tasto stop
  if (digitalRead(pulsStop) == LOW ){
      //Setto la variabilea true
      flgStop = true;
      //Esco dalla funzione
      return;
    } 
}

//Fermo i processi
void Stop () {

  //DEBUG
  Serial.println("INFORMAZIONI: Avvio Stop");

  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Arrresto del");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0, 2);
  //Invio il messaggio
  lcd.print("sistema");

  //Controllo il perchè dello stop
  //Se è un "emergenza" setto i flag
  //Sistemo le uscite

  if (flgStop == true){
    //Apro tutti i relè
    digitalWrite(rele,LOW); 
    //Resetto la variabile d'emergenza
    flgStop = false;
  }
}

//Leggo e ritorno la temperatura
float GetTemp() {

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
