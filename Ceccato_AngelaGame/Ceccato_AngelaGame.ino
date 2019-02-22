#include <LiquidCrystal.h>
#define DELAYINTRO 50
#define DELAYNEXTFUNC 500
#define B1 12
#define B2 11
#define B3 10

const int rs = 9, en = 8, d4 = 5, d5 = 4 , d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

bool turn;
bool zero;
int meta;
int current;
int previous;

int counterP1 = 0; //Numero di vittorie di P1 e P2.
int counterP2 = 0;
bool firstGame = true;

void setup() {
  turn = true; //true = Player 1, false = Player 2
  zero = true; //Diviene "false" dopo il primo turno.
  meta = 0; //La meta.
  current = 0; //Il numero di punti attuale.
  previous = -1; //Il turno precedente.
  
  if(firstGame){
    lcd.begin(16,2);
    pinMode(B1, INPUT_PULLUP);
    pinMode(B2, INPUT_PULLUP);
    pinMode(B3, INPUT_PULLUP);
    
    arduino_Intro();
    firstGame = false;
  }
}

void loop() {
//   set_Meta(); //Impostazione della meta.
   while(current<meta){ //Il gioco continua finchè la meta non è stata raggiunta o superata.
    execTurn();
   }
   gameOver();
}

/*void getMeta(){
  int i;
  while(i>=30 && i<100){ //Il metodo continuerà a chiedere il numero fintantoché questo non sarà accettabile.     
      Arduino_Display_Meta();
      i = Arduino_SetMeta; //Prende il numero, lo attribuisce ad i e poi lo stampa.
  }
  meta = i;
}*/

void printMeta(){
   String s = "- La meta scelta ammonta a " + String(meta); 
   Serial.println(s);
}

void execTurn(){
  int i;
  bool done = false;
  Serial.println("- Turno di P"  + String(turn? "1" : "2") + " - Punti: " + scoreCompare());
  while(!done){  
    while (Serial.available() == 0);
    Serial.println(String(i = Serial.parseInt()));
    done = evaluateTurn(i);
  } 
  ready4NextTurn(i);
}

bool evaluateTurn(int i){ //L'ultima condizione sottintende che 0 sarà valido al primo turno. Questo perché "zero" diviene false dopo il primo turno.
  if(i>6 || i<0){
    Serial.println("- Inserisci un numero valido!");
    return false;
  }
  else if((i == 0 && !zero)|| i == previous || i == 7-previous){ 
    Serial.println("Non barare!");
    return false;
  }
  else return true;
}

void gameOver(){
  bool b = (turn == (current>meta));
  Serial.println("\n- Il vincitore è " + String(b ? "P1" : "P2") + "! " + scoreCompare()+"\n");
  Serial.println(winCounter(b)+"\n");
  resetValues();
  aftermath();

  /*La condizione "boolA == boolB" equivale all'operatore logico A XNOR B.
   *TURN(A)|current>meta(B)| A XNOR B  | WINNER
   *TRUE   |     TRUE      |   TRUE    |   P1
   *TRUE   |     FALSE     |   FALSE   |   P2
   *FALSE  |     TRUE      |   FALSE   |   P2
   *FALSE  |     FALSE     |   TRUE    |   P1
   *
   *Si osservi la tavola della verità. Il valore di TURN è quello del giocatore che normalmente inizierebbe il turno:
   *dovesse P1 iniziare il turno con current > di meta, allora P2 ha perduto, e viceversa.
   *Questo metodo di valutazione si interpreta con fatica ad occhio, ma risparmia delle righe di codice.
   */
}

void ready4NextTurn(int i)//Modifica i valori per il turno successivo.
{
  zero = false;
  previous = i;
  current += i;
  turn = !turn;
}

void resetValues() {//Modifica i valori per la partita successiva.
  turn = zero = true;
  meta = current = 0;
  previous = -1;
}

void aftermath() {
  bool done = false;
  while(!done) {
    Serial.println("- Inserisci un carattere casuale e premi [Invia] per rigiocare!");
    Serial.println("- Oppure inserisci 'x' per cancellare i punteggi.");
    while (Serial.available() == 0);
    if(Serial.readString()=="x"){
      counterP1 = counterP2 = 0;
      Serial.println("- I punteggi sono stati cancellati.");
    } else done = true;
  }
}

String scoreCompare(){
  return String(current)+"/"+String(meta);
}

String winCounter(bool b)
{
  b? counterP1++ : counterP2++;
  return "P1 :     " + String(counterP1) + " | " + String(counterP2) + "     : P2"; 
}

void arduino_Intro(){
  String s1 = "ANGELA", s2 = " Game ";
  
  int c1 = -5, c2 = 15; 
  while(c1<6)
  {
    lcd.setCursor(c1-1,0); lcd.print(' ');
    int i1 = c1, i2 = c2;
    while(i1<c1+6)
    {
       lcd.setCursor(i1,0); lcd.print(s1[i1-c1]);
       lcd.setCursor(i2,1); lcd.print(s2[i2-c2]);
       i1++; i2++;
    }
    delay(DELAYINTRO);  
    c1++; c2--;
  }

  int i1=5, i2=10;
  while(i1>=0)
  {
     char c = i1 == 0 ? '|' : '.';
     lcd.setCursor(i1,1); lcd.print(c);
     lcd.setCursor(i2,1); lcd.print(c);
     if(i1!=5)
     {
      lcd.setCursor(i1,0); lcd.print(c);
      lcd.setCursor(i2,0); lcd.print(c);
     }
     delay(DELAYINTRO);
     i1--; 
     i2++;
  }
 
  delay(DELAYNEXTFUNC); 
  lcd.clear();
}

void arduino_MetaState(){
  lcd.setCursor(0,1); lcd.print("[-]");
  lcd.setCursor(6,1); lcd.print("[OK]");
  lcd.setCursor(13,1); lcd.print("[+]");

  lcd.setCursor(4,0); lcd.print("Meta:");
  //while(digitalRead(
}

  

  
 
 
