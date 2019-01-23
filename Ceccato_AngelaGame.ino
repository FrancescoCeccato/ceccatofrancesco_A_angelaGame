bool turn = true;
bool zero = true;
int meta = 0;
int current = -1;
int previous;

void setup() {
  Serial.begin(9600);
}

void loop() {
   setMeta();
   while(current<meta)
   {
    execTurn();
   }
}

int setMeta(){
   getMeta();
   printMeta();
}

void getMeta(){
  int i;
  bool done = false;
  while(!done){      
      Serial.println("- Digita un numero intero compreso fra 1 e 99!");
      while (Serial.available() == 0);
      i = Serial.parseInt();
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
  Serial.println("- Turno di P"  + String(turn? "1" : "2") + " - Punti: " + String(current)+"/"+String(meta));
  while(!done){  
    while (Serial.available() == 0);
    i = Serial.parseInt();
    done = evaluateTurn(i);
  } 
  previous = i;
  current += i;
  turn = !turn;
}

bool evaluateTurn(int i){
  if(i == previous || i == 7-previous || (i==0 && !zero)){ 
    Serial.println("Non barare!");
    return false;
  }
  else if(i == 0 && zero){
    zero = false;
    return true;
  }
  else if(i>6)
  return false;
  else return true;
}
