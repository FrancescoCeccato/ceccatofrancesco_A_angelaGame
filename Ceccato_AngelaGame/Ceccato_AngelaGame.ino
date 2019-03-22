#include <LiquidCrystal.h>
#define DELAYSHORT 60
#define DELAYBTN 300
#define DELAYPHASE 1000
#define BTN1 A0
#define BTN2 A1
#define BTN3 A2

const int rs = 9, en = 8, d4 = 5, d5 = 4 , d6 = 3, d7 = 2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

bool firstRun = true;
bool turn,zero;
int meta,current,previous;
int counterP1,counterP2;
String nameP1,nameP2;

void setup() {
  if(firstRun){
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  
  lcd.begin(16,2);
  angelaIntro();
  setNames();
    
  firstRun = false;
  }

  turn = zero = true;
  meta = current = 0;
  previous = -1;
}

void loop() {
   setMeta();
   while(current<meta){
    execTurn();
   }
   gameOver();
}

void setNames(){
  print_bar("LETTERS");
  setName_for("P1");
  setName_for("P2");
}

void setName_for(String player){ //Imposta il nome (3 caratteri) del giocatore P1 o P2.
  lcd.setCursor(0,0); lcd.print(player+">Name:  ___");
  String output = "";
  String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
  int cur = 10, chr = 0;
  while(cur<13){
    lcd.setCursor(cur,0); lcd.print(chars[chr]);
    if(digitalRead(BTN1)==LOW && chr>0){
      hold_delay(BTN1);
      chr--;
    }
    if(digitalRead(BTN3)==LOW && chr<35){
      hold_delay(BTN3);
      chr++;
    }
    if(digitalRead(BTN2)==LOW){
      hold_lock(BTN2);
      output+=chars[chr];
      chr=0; cur++;
    }
  }

  if(player == "P1"){ 
    nameP1 = output;
  } else if (player == "P2") {
    nameP2 = output;
  }

}

void setMeta(){
  setMeta_phrasing();
  setMeta_digits();
  
  scroll_double(" LET'S","PLAY! ");
  delay(DELAYPHASE);
}

void setMeta_phrasing(){
  print_bar("NUMBERS");
  lcd.setCursor(3,0); lcd.print("Goal:");
}

void setMeta_digits(){
  meta = 30;
  while(digitalRead(BTN2) == HIGH){  
    if(digitalRead(BTN1) == LOW && meta>30){
      hold_delay(BTN1);
      meta--; 
    }
    if(digitalRead(BTN3) == LOW && meta<99){
      hold_delay(BTN3);
      meta++; 
    }
   lcd.setCursor(10,0); lcd.print(meta); 
  }
}

void execTurn(){
  print_bar("NUMBERS");
  
  bool done = false, foul = false; 
  int pick = 0;
  print_turn();
  
  while(!done){
    if(digitalRead(BTN1)==LOW || digitalRead(BTN3)==LOW) {
      if(digitalRead(BTN1)==LOW && pick>0){
        hold_delay(BTN1);
        pick--;
      }
      if(digitalRead(BTN3)==LOW && pick<6){
        hold_delay(BTN3);
        pick++;
      }
    }
    if(digitalRead(BTN2)==LOW){
      hold_lock(BTN2);
      done = !foul;
    }
    foul = pick==previous || pick==7-previous || (pick==0 && !zero);
    print_selektr(pick, foul); 
  }
  ready4NextTurn(pick);
}

void gameOver(){
  bool b = (turn == (current>meta));

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
   
  print_winner(b);
  
  setup();
}

void ready4NextTurn(int i){
  previous = i;
  current += i; 

  animate();
  
  turn = !turn;
  zero = false;  
}

void angelaIntro(){
  scroll_double("ANGELA", " Game ");
  int i1=5, i2=10;
  while(i1>=0){
     char c = i1 == 0 ? '|' : '.';
     lcd.setCursor(i1,1); lcd.print(c);
     lcd.setCursor(i2,1); lcd.print(c);
     if(i1!=5){
      lcd.setCursor(i1,0); lcd.print(c);
      lcd.setCursor(i2,0); lcd.print(c);
     }
      delay(DELAYSHORT);
      i1--; i2++;
  }
 delay(DELAYPHASE);
}

void scroll_double(String s1, String s2){
  lcd.clear();
  int c1 = -5, c2 = 15;
  int i1 = c1, i2 = c2;
  while(c1<6){
      delay(DELAYSHORT);
      lcd.setCursor(c1-1,0); lcd.print(' ');
      lcd.setCursor(c2+1,0); lcd.print(' ');
      i1=c1; i2=c2;
      while(i1<c1+6){
        lcd.setCursor(i1,0); lcd.print(s1[i1-c1]);
        lcd.setCursor(i2,1); lcd.print(s2[i2-c2]);
        i1++; i2++;
      }
      c1++; c2--;
  }     
  delay(DELAYSHORT);
}


void print_bar(String type){
  lcd.clear();
  lcd.setCursor(6,1); lcd.print("[OK]");
  if(type == "NUMBERS") {
   lcd.setCursor(0,1); lcd.print("[-]");
   lcd.setCursor(13,1); lcd.print("[+]");
  }
  else if (type == "LETTERS") {
   lcd.setCursor(0,1); lcd.print("[<]");
   lcd.setCursor(13,1); lcd.print("[>]"); 
  }
}

void print_turn(){
  lcd.setCursor(0,0); lcd.print(turn ? nameP1 : nameP2);
  lcd.setCursor(3,0); lcd.print('>');
  String curr1 = String(current);
  String curr2 = current<10 ? "0"+curr1 : curr1; 
  lcd.setCursor(11,0); lcd.print(curr2+"/"+String(meta));
}

void print_selektr(int pick, bool foul){
  lcd.setCursor(6,0); 
  String s1 = pick == 0 ? " " : "<";
  String s2 = foul ? "x" :String(pick);
  String s3 = pick == 6 ? " " : ">";
  lcd.print(s1+s2+s3);    
}

void print_winner(bool w){
  String wName = " " + (w ? nameP1 : nameP2) + "  ";
  scroll_double(wName, "WINS!  ");
  delay(100);
  w ?  counterP1++ : counterP2++;
  delay(DELAYPHASE);
  print_increase(w);
 }

void print_increase(bool w) {
  lcd.clear();
  lcd.setCursor(3,0); lcd.print(nameP1);
  lcd.setCursor(10,0); lcd.print(nameP2);
   
  int wp = w ? 4 : 11; 
  int wn = w? counterP1 : counterP2;
  int lp = 15 - wp; 
  int ln = w? counterP2 : counterP1;
    
  lcd.setCursor(wp,1); lcd.print(String(wn-1));
  lcd.setCursor(lp,1); lcd.print(String(ln));
  delay(DELAYPHASE);
  lcd.setCursor(wp,1); lcd.print(String(wn));
  delay(DELAYPHASE);
}

void animate(){
  lcd.setCursor(0,1);
  lcd.print("                ");
  for(int i = 6; i<11; i++) {
    delay(DELAYSHORT);
    lcd.setCursor(i,0);
    lcd.print(" "+String(previous));   
  }
  print_turn();
  delay(DELAYPHASE);
}

void hold_lock(int i) {
  while(digitalRead(i) == LOW);
}

void hold_delay(int i)
{
  delay(DELAYBTN);
}

  
 
 
