//*****************************************************
//
// Data inizio 8/10/2018
// Data ultima modfica 
// Realizzato da Daniele loddo alias Ivotek
// Contatti www.ivotek.it ivotek@gmail.com
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
// 10K resistor:
// ends to +5V and ground
// wiper to LCD VO pin (pin 3)
//
// Pulsante di start pin 7 NA collegato a massa
// Pulsante di stop pin 8  NA collegato a massa
//
// Relè attivazione resistenza di riscaldamenti  pin 6 HIGH=attivo LOW=disattivo
//
// Sensore temperatura pin A1 modello
//
//*******************************************************************


//Libreria per LCD
#include <LiquidCrystal.h>

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
int firstTemp = 70 ;
//Seconda temperatura
int secondTemp = 100;
//Primo tempo
int firstTime = 30;
//Secondo tempo
int secondTime = 30;
//Pulsante di start
int pulsStart = 7;
//Pulsante id stop
int pulsStop = 8;

void setup() {
  //Settaggi iniziali display
  
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
  //Se il pulsante non è premuto rimango nel ciclo
  while (digitalRead(pulsStart)==HIGH){};
  
}

//Vado alla temepratura passata come parametro
void GoTemp(int temp){
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

  //Ciclo per il controllo della temperatura
    //Attivazione del relè

  //FIXME DA ELIMINARE
  delay(3000);
}

//Tengo la temperatura per il tempo passato come parametro
void KeepTime(int ktime){
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
