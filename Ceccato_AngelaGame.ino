int turn = 0;
bool zero = false;
int meta = 5;
int current = 0;
int previousMove;

void setup() {
  Serial.begin(9600);
}

void loop() {
      setMeta();

      while(true);
}

int setMeta()
{
  getMeta();
  printMeta();
}

int getMeta()
{
  int i;
  bool done = false;
  while(!done)
  {      
      Serial.println("Digita un numero compreso fra 1 e 99!");
      while (Serial.available() == 0);
      i = Serial.parseInt();
      done = (i>0 && i<100) ? true : false;
  }
  meta = i;
}

int printMeta()
{
      String s = "La meta scelta ammonta a " + meta; 
      Serial.println(s);
}

