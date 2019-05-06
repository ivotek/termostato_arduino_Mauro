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
// Pulsante di start 7
// Pulsante di stop 8

//Libreria per LCD
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Va alla temepratura passata come parametro
void GoTemp(int);
//Mantiene la temperatura per il tempo passato
void KeepTime(int);

//Prima temperatura
int firstTemp = 70 ;
//Seconda temperatura
int secondTemp = 100;
//Primo tempo
int firstTime = 30;
//Secondo tempo
int secondTime = 30;
//Pulsante di start
int Start = 7;
//Pulsante id stop
int Stop = 8;

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
  pinMode(INPUT, Start);
  pinMode(INPUT,Stop);

}

void loop() {
  //Saluto iniziale
  HelloInit();
  
  //Porta la temperatura a 70 gradi
  GoTemp(firstTemp);

  //Mantiene la temepratura per 30 minuti
  KeepTime(firstTime);
  
  //Porta la temperatura a 100 gradi
  GoTemp(secondTemp);

  //Mantiene la temperatura
  KeepTime(secondTime);
  
  //Spegne tutto
  StopF();

}

void HelloInit(){
  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Ready");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0,2);
  //Invio il messaggio
  lcd.print("Press botton");

  delay(3000);
}

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

  delay(3000);
}

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

  delay(3000);
  
}

void StopF (){
  //Cancello lo schermo
  lcd.clear();
  //Invio il messaggio
  lcd.print("Arrresto del");
  //Mi sposto alla colonna 0 riga 2
  lcd.setCursor(0,2);
  //Invio il messaggio
  lcd.print("sistema");

  delay(3000);  
}
