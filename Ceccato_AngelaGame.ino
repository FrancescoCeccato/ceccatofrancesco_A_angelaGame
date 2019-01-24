bool turn = true;
bool zero = true;
int meta = 0;
int current = 0;
int previous;

void setup() {
  Serial.begin(9600);
  Serial.println("Benvenuti in Angela Game!");
  Serial.println("Un gioco di P.Perotto, porting by F.Ceccato");
}

void loop() {
   setMeta();
   while(current<meta){
    execTurn();
   }
   gameOver();
}

int setMeta(){
   getMeta();
   printMeta();
}

void getMeta(){
  int i;
  bool done = false;
  while(!done){      
      Serial.println("- Si digiti un numero intero compreso fra 30 e 99!");
      while (Serial.available() == 0);
      Serial.println(String(i = Serial.parseInt()));
      done = (i>=30 && i<100);
  }
  meta = i;
}

void printMeta(){
   String s = "- La meta scelta ammonta a " + String(meta); 
   Serial.println(s);
}

void execTurn(){
  int i;
  bool done = false;
  Serial.println("- Turno di P"  + String(turn? "1" : "2") + " - Punti: " + counter());
  while(!done){  
    while (Serial.available() == 0);
    Serial.println(String(i = Serial.parseInt()));
    done = evaluateTurn(i);
  } 
  previous = i;
  current += i;
  turn = !turn;
}

bool evaluateTurn(int i){
  if(i==0 && zero){
    return true;
  }
  else if(i>6 || i<0){
    Serial.println("- Inserisci un numero valido!");
    return false;
  }
  else if((i == 0 && !zero)|| i == previous || i == 7-previous){ 
    Serial.println("Non barare!");
    return false;
  }
  else
  {
    zero = false;
    return true;
  }
}

void gameOver()
{
  Serial.println("Il vincitore è " + String((turn == (current>meta)) ? "P1" : "P2") + "! " + counter());
  resetValues();
  Serial.println("Premi [Invia] per rigiocare!");
  while (Serial.available() == 0);

  /*La condizione "bool == bool" equivale all'operatore logico A XNOR B.
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

void resetValues()
{
  turn = true;
  zero = true;
  meta = 0;
  current = 0;
  previous = 0;
}

String counter(){
  return String(current)+"/"+String(meta);
}
