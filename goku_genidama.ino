#include <LiquidCrystal.h>
//------------------------------------------------------------
// Idees:
// Format de l'heure en H24 (22:30)
// Format de l'heure en AM/PM (02:24 PM)
//------------------------------------------------------------

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //initialisation communication's pins with LCD screen


const int L1 = 2;               // pin 2 correspond to led1
const int L2 = 3;               // pin 3 correspond to led2
const int L3 = 4;               // pin 4 correspond to led3
const int bp1 = 0;              // pin 6 correspond to button 1
const int bp2 = 1;              // pin 5 correspond to button 2
const int bp3 = 2;              // pin 13 correspond to button 3
int i = 0;                      // i is used to false timeclock 
int j = 0;
int heure = 23;                 // init's hours
int minute = 57;                // init's minutes  
int h_reveil = 0;               // alarm clock's hours
int m_reveil = 0;               // alarm clock's minutes
String s = "ALARM ON";          // s is used to alarm clock's detection 
String mode = "Mode Auto";      // mode is used to detection's mode (auto or manual)
int petit_delay = 50;           // small delay de 50ms
int long_delay = 700;           // long delay de 700ms
int moy_delay = 300;            // middle delay de 300ms
int etat_bp1_int;               // state button 1
int etat_bp2_int;               // state button 2
int etat_bp3_int;               // state button 3
boolean etat_bp1;               // state button 1
boolean etat_bp2;               // state button 2
boolean etat_bp3;               // state button 3
boolean etat_reveil=0;          // alarm clock on = 1 , off = 0;
boolean dernier_etat_bp1 = LOW; // memory to last action to button 1
boolean dernier_etat_bp2 = LOW; // memory to last action to button 2
boolean dernier_etat_bp3 = LOW; // memory to last action to button 3


    
void setup() {
    
    
    Serial.begin(9600); // serial init
    lcd.begin(8,2);     // init to LCD screen in 8 characters x 2 lines
    lcd.print("INITIALISATION");
    delay(long_delay);

    pinMode(L1, OUTPUT); // L1 is a output pin
    pinMode(L2, OUTPUT); // L2 is a output pin
    pinMode(L3, OUTPUT); // L3 is a output pin
    
    etat_bp1=analogRead(bp1); // read value to bp1 and affect it in variable 'etat_bp1'
    etat_bp2=analogRead(bp2); // read value to bp2 and affect it in variable 'etat_bp2'
    etat_bp3=analogRead(bp2); // read value to bp3 and affect it in variable 'etat_bp3'

    
}

void loop() {

      delay(moy_delay);
    do{
        lcd.clear();
        affiche_heure();
        dernier_etat_bp1 = etat_bp1;
        dernier_etat_bp2 = etat_bp2;
        dernier_etat_bp3 = etat_bp3;
        
        lecture_bp();
      if((heure==h_reveil) && (minute==m_reveil) && (s=="ALARM ON")){
        etat_reveil=1;
        }
      delay(petit_delay);
      //Serial.println(etat_bp1);
    }while((etat_bp1 !=1 ) && (etat_bp2 !=1 ) && (etat_bp3 !=1 ) && (etat_reveil != 1));


    if(etat_reveil==1){
      Serial.println(etat_reveil);
      alarme_activated();
    }
    if((etat_bp3 == 1 ) && (etat_bp1==0) && (etat_bp2==0) && (dernier_etat_bp3 == 0)){
          alarme_menu();
    }
    
    
    if((etat_bp1==1) && (etat_bp2==0) && (etat_bp3 == 0) && (dernier_etat_bp1 == 0)){ // bp1 id used to actived/disactived alarm clock
          alarme_mode();
    }
    
    if((etat_bp2==1) && (etat_bp1==0) && (etat_bp3 == 0) && (dernier_etat_bp2 == 0)){ // bp2 is used to change manual mode and auto mode
          j++;
          led_modes();    
    }
    lcd.clear();
    affiche_heure();
    delay(petit_delay);
    i++;      
}


void lecture_bp(){
      etat_bp1_int = analogRead(bp1);
      //Serial.println(etat_bp1_int);
      if(etat_bp1_int > 650)
      {
        etat_bp1=1;
      }
      else if(etat_bp1_int < 650){
        etat_bp1=0;
      }
      etat_bp2_int = analogRead(bp2);
      if(etat_bp2_int > 650)
      {
        etat_bp2=1;
      }
      else if(etat_bp2_int < 650){
        etat_bp2=0;
      }
      etat_bp3_int = analogRead(bp3);
      if(etat_bp3_int > 650)
      {
        etat_bp3=1;
      }
      else if(etat_bp3_int < 650){
        etat_bp3=0;
      }
}
void alarme_activated(){
  led_off();
  led_on_delay();
  etat_reveil=0;
  delay(long_delay);
}

void alarme_menu(){
  lcd.clear();
  lcd.print("REGLAGE");
  delay(long_delay);
  do{
          reglage_alarme();
          affiche_alarme();
          lecture_bp();
          dernier_etat_bp1 = etat_bp1;
          dernier_etat_bp2 = etat_bp2;
          delay(petit_delay);
      }while((etat_bp3 != 1));
}

void reglage_alarme(){
    lecture_bp();
    Serial.println(etat_bp1);
    Serial.println(etat_bp2);
  if((etat_bp1==1) && (dernier_etat_bp1 == 0)){
    h_reveil++;
  }
  if((etat_bp2==1) && (dernier_etat_bp2 ==0)){
    m_reveil++;
  }
}

void affiche_alarme(){
   if(m_reveil>59){
    m_reveil=0;
    heure++;
   }
   if(h_reveil>23){
    h_reveil=0;
   }
   
   if((h_reveil<=9) && (m_reveil<=9)){ // both inferiors to 10
    lcd.clear();
    lcd.print("0");
    lcd.print(h_reveil);
    lcd.print(":0");
    lcd.print(m_reveil);
  }
  if((h_reveil<=9) && (m_reveil>=10)){ // hour inferior to 10 et minute superior
    lcd.clear();
    lcd.print("0");
    lcd.print(h_reveil);
    lcd.print(":");
    lcd.print(m_reveil);
  }
  if((h_reveil>=10) && (m_reveil<=9)){ // minute inferior to 10 and hour superior
    lcd.clear();
    lcd.print(h_reveil);
    lcd.print(":0");
    lcd.print(m_reveil);
  }
  if((h_reveil>=10) && (m_reveil>=10)){ // both superiors to 10
    lcd.clear();
    lcd.print(h_reveil);
    lcd.print(":");
    lcd.print(m_reveil);
  }
}

void affiche_heure(){
  if(i>10){
        minute++;
        i=0;
      }
      
   if(minute>59){
    minute=0;
    heure++;
   }
   if(heure>23){
    heure=0;
   }
   
   if((heure<=9) && (minute<=9)){ // both inferiors to 10
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("0");
    lcd.print(heure);
    lcd.print(":0");
    lcd.print(minute);
  }
  if((heure<=9) && (minute>=10)){ // hour inferior to 10 and minute superior
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("0");
    lcd.print(heure);
    lcd.print(":");
    lcd.print(minute);
  }
  if((heure>=10) && (minute<=9)){ // minute inferior to 10 and hour superior
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(heure);
    lcd.print(":0");
    lcd.print(minute);
  }
  if((heure>=10) && (minute>=10)){ // both superiors to 10
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(heure);
    lcd.print(":");
    lcd.print(minute);
  }
  i++;
}
void alarme_mode(){
        delay(long_delay);
        s = etat_actuel(s);
        lcd.clear();
        lcd.print(s);
        delay(long_delay);
        if(s=="ALARMOFF"){
              led_off();
        }
        delay(long_delay);
        lcd.clear();
}
void led_modes(){
      delay(petit_delay);
      mode = etat_mode(mode);
      lcd.clear();
      if(mode=="Mode Manu"){
        lcd.clear();
        lcd.print("Mode Manu");
        if(j == 1){
          led_on1();
        }
        else if(j == 2){
          led_on2();
        }
        else if(j == 3){
          led_on();
        }
        
        delay(long_delay);
        affiche_heure();
        
      }
      if(mode=="Mode Auto"){
        lcd.clear();
        lcd.print("Mode Auto");
        delay(long_delay);
        j=0;
        led_off(); 
      }       
}


void led_off(){
  digitalWrite(L1, LOW); // L1 off
  digitalWrite(L2, LOW); // L2 off
  digitalWrite(L3, LOW); // L3 off
}

void led_on(){
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, HIGH); // L2 on
  digitalWrite(L3, HIGH); // L3 on
}

void led_on1(){
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, LOW);  // L2 off
  digitalWrite(L3, LOW);  // L3 off
}

void led_on2(){
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, HIGH); // L2 on
  digitalWrite(L3, LOW);  // L3 off
}

void led_on_delay(){
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, LOW); // L2 off
  digitalWrite(L3, LOW); // L3 off
  delay(3000);
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, HIGH); // L2 on
  digitalWrite(L3, LOW); // L3 off
  delay(3000);
  digitalWrite(L1, HIGH); // L1 on
  digitalWrite(L2, HIGH); // L2 on
  digitalWrite(L3, HIGH); // L3 on
  delay(3000);
}

String etat_actuel(String s){
        if(s=="ALARMOFF"){
          return "ALARM ON";
        }
        else if(s=="ALARM ON"){
          return "ALARMOFF";
          
        }
}
String etat_mode(String mode){
        if(mode=="Mode Auto"){
          return "Mode Manu";
        }
        else if((mode=="Mode Manu") && (j <= 3)){
          return "Mode Manu";
        }
        else if((mode=="Mode Manu") && (j == 4)){
          return "Mode Auto";
          
        }
}
