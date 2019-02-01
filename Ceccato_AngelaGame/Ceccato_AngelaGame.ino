bool turn = true; //true = Player 1, false = Player 2
bool zero = true; //Diviene "false" dopo il primo turno.
int meta = 0; //La meta.
int current = 0; //Il numero di punti attuale.
int previous = -1; //Il turno precedente.

int counterP1 = 0; //Numero di vittorie di P1 e P2.
int counterP2 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("==========Benvenuti in Angela Game!============");
  Serial.println("Un gioco di P.Perotto, porting by F.Ceccato");
  Serial.println("Le istruzioni del gioco sono nel file 'AngelaReadme.txt' presente nella cartella del gioco.");
  Serial.println("");
}

void loop() {
   setMeta(); //Impostazione della meta.
   while(current<meta){ //Il gioco continua finchè la meta non è stata raggiunta o superata.
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
  while(!done){ //Il metodo continuerà a chiedere il numero fintantoché questo non sarà accettabile.     
      Serial.println("- Si digiti un numero intero compreso fra 30 e 99!");
      while (Serial.available() == 0);//Attendi la disponibilità di byte da leggere in console.
      Serial.println(String(i = Serial.parseInt())); //Prende il numero, lo attribuisce ad i e poi lo stampa.
      done = (i>=30 && i<100); //La definizione di "accettabile".
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
  Serial.println("");
  Serial.println("- Il vincitore è " + String(b ? "P1" : "P2") + "! " + scoreCompare());
  Serial.println("");
  Serial.println(winCounter(b));
  Serial.println("");
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
  turn = true;
  zero = true;
  meta = 0;
  current = 0;
  previous = -1;
}

void aftermath() {
  bool done = false;
  while(!done) {
    Serial.println("- Inserisci un carattere e premi [Invia] per rigiocare!");
    Serial.println("- Oppure inserisci 'x' per cancellare i punteggi.");
    while (Serial.available() == 0);
    String s = Serial.readString();
    if(s=="x"){
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
  if(b) counterP1++; else counterP2++;
  return "P1 :     " + String(counterP1) + " | " + String(counterP2) + "     : P2"; 
}

