#include <LiquidCrystal.h>
//------------------------------------------------------------
// Idees:
// Format de l'heure en H24 (22:30)
// Format de l'heure en AM/PM (02:24 PM)
//------------------------------------------------------------

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); //Initialisation des pins de communication avec l'afficheur LCD


const int L1 = 2;     // La broche 2 de l'arduino correspond à la led1
const int L2 = 3;     // La broche 3 de l'arduino correspond à la led2
const int L3 = 4;     // La broche 4 de l'arduino correspond à la led3
const int bp1 = 0;  // La broche 6 de l'arduino correspond au boutton poussoir 1
const int bp2 = 1;  // La broche 5 de l'arduino correspond au boutton poussoir 2
const int bp3 = 2; // La broche 13 de l'arduino correspond au boutton poussoir 3
int i = 0;              // I va servir au faux compteur de temps 
int j = 0;
int heure = 23;       // Heure d'initialisation
int minute = 57;      // Minute d'init
int h_reveil = 0;     // Heure reveil
int m_reveil = 0;     // Minute reveil
String s = "ALARM ON";  // s sert à la detection du reveil
String mode = "Mode Auto"; // mode sert à la detection du mode
int petit_delay = 50; // petit delay de 50ms
int long_delay = 700; // long delay de 700ms
int moy_delay = 300;  // moyen delay de 300ms
int etat_bp1_int;     // etat du bouton poussoir 1
int etat_bp2_int;     // etat du bouton poussoir 2
int etat_bp3_int;     // etat du bouton poussoir 3
boolean etat_bp1;     // etat du bouton poussoir 1
boolean etat_bp2;     // etat du bouton poussoir 2
boolean etat_bp3;     // etat du bouton poussoir 3
boolean etat_reveil=0;// reveil actif = 1 , inactif = 0;
boolean dernier_etat_bp1 = LOW; // mémoire de la dernière action du boutton 1
boolean dernier_etat_bp2 = LOW; // mémoire de la dernière action du boutton 2
boolean dernier_etat_bp3 = LOW; // mémoire de la dernière action du boutton 3


    
void setup() {
    
    
    Serial.begin(9600); // Init du serial 
    lcd.begin(8,2);     // Init de l'ecran lcd en 8 charactères x 2 lignes
    lcd.print("INITIALISATION");  
    delay(long_delay);

    pinMode(L1, OUTPUT); //L1 est une broche de sortie
    pinMode(L2, OUTPUT); //L2 est une broche de sortie
    pinMode(L3, OUTPUT); //L3 est une broche de sortie
    
    etat_bp1=analogRead(bp1); //  on lit la valeur du bp1 et on l’affecte à la variable "etat_bp1"
    etat_bp2=analogRead(bp2); //  on lit la valeur du bp2 et on l’affecte à la variable "etat_bp2"
    etat_bp3=analogRead(bp2); //  on lit la valeur du bp3 et on l’affecte à la variable "etat_bp3"

    
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
    
    
    if((etat_bp1==1) && (etat_bp2==0) && (etat_bp3 == 0) && (dernier_etat_bp1 == 0)){ // bp1 sert à activer/desactiver l'alarme
          alarme_mode();
    }
    
    if((etat_bp2==1) && (etat_bp1==0) && (etat_bp3 == 0) && (dernier_etat_bp2 == 0)){ //bp2 sert a changer du mode auto au mode manu
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
   
   if((h_reveil<=9) && (m_reveil<=9)){ //Les deux inferieurs a 10
    lcd.clear();
    lcd.print("0");
    lcd.print(h_reveil);
    lcd.print(":0");
    lcd.print(m_reveil);
  }
  if((h_reveil<=9) && (m_reveil>=10)){ //Heure inferieur a 10 et minute superieur
    lcd.clear();
    lcd.print("0");
    lcd.print(h_reveil);
    lcd.print(":");
    lcd.print(m_reveil);
  }
  if((h_reveil>=10) && (m_reveil<=9)){ //Minute inferieur a 10 et heure superieur
    lcd.clear();
    lcd.print(h_reveil);
    lcd.print(":0");
    lcd.print(m_reveil);
  }
  if((h_reveil>=10) && (m_reveil>=10)){ //Les deux superieur
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
   
   if((heure<=9) && (minute<=9)){ //Les deux inferieurs a 10
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("0");
    lcd.print(heure);
    lcd.print(":0");
    lcd.print(minute);
  }
  if((heure<=9) && (minute>=10)){ //Heure inferieur a 10 et minute superieur
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("0");
    lcd.print(heure);
    lcd.print(":");
    lcd.print(minute);
  }
  if((heure>=10) && (minute<=9)){ //Minute inferieur a 10 et heure superieur
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(heure);
    lcd.print(":0");
    lcd.print(minute);
  }
  if((heure>=10) && (minute>=10)){ //Les deux superieur
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
  digitalWrite(L1, LOW); //eteindre L1
  digitalWrite(L2, LOW); //eteindre L2
  digitalWrite(L3, LOW); //eteindre L3
}

void led_on(){
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, HIGH); //allumer L2
  digitalWrite(L3, HIGH); //allumer L3
}

void led_on1(){
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, LOW); //allumer L2
  digitalWrite(L3, LOW); //allumer L3
}

void led_on2(){
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, HIGH); //allumer L2
  digitalWrite(L3, LOW); //allumer L3
}

void led_on_delay(){
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, LOW); //allumer L2
  digitalWrite(L3, LOW); //allumer L3
  delay(3000);
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, HIGH); //allumer L2
  digitalWrite(L3, LOW); //allumer L3
  delay(3000);
  digitalWrite(L1, HIGH); //allumer L1
  digitalWrite(L2, HIGH); //allumer L2
  digitalWrite(L3, HIGH); //allumer L3
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
