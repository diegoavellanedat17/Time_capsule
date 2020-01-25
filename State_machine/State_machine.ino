#include <SimpleTimer.h>
#include "src/S1_STATE/S1_STATE.h"
#include "RTClib.h"

RTC_DS1307 rtc;
SimpleTimer timer;

int state; // el estado en el que se encuentra la capsula, esto se definira tambien por la EEPROM
int buttonPin;
const int next_button = 8;// el pin al que conectaremos un pin que será como una interrupcion posteriormente
const int enter_button = 9;

boolean next_buttonState = LOW;
boolean enter_buttonState = LOW; 

const int REED_SWITCH=10;


// Variables para el estado de configuración
int possible_years[]={2020,2021,2022,2023,2024,2025};
String possible_months[]={"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
uint8_t possible_months_integer[]={1,2,3,4,5,6,7,8,9,10,11,12};
uint8_t possible_days[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
  //Posibles horas, y posibles minutos
int possible_hours[25];
int possible_minutes[61]; 
int timer_sleep;
uint8_t pressed=0;
uint8_t part=0;
int final_year;
String final_month;
uint8_t final_month_integer;
uint8_t final_day;
uint8_t final_minute;
uint8_t final_hour;
uint8_t current_length;

int current_year;
uint8_t current_month;
uint8_t current_day;
uint8_t current_hour;
uint8_t current_minute;

uint8_t SHUT_DOWN_ME=3;
uint8_t SHAKE_SENSOR=2;

void TimerTask(){
  Serial.println("10 seconds after button was pressed");
  timer.disable(timer_sleep);
  pressed=0;
  part=0;
  digitalWrite(SHUT_DOWN_ME,HIGH);
}



void setup() {
  
  rtc.begin();
  Serial.begin(9600);
  // Entradas botones de configuración 
  pinMode(next_button, INPUT);
  pinMode(enter_button, INPUT);
  // Entrada el Shake_Sensor para consultar o configurar
  pinMode(SHAKE_SENSOR,INPUT);
  // Salida para bajo consumo
  pinMode(SHUT_DOWN_ME,OUTPUT);

  pinMode(REED_SWITCH,INPUT);

  // Entrar en el estado correspondiente de acuerdo con los sensores 
  int OPEN_SENSOR= analogRead(A0);
  int CLOSE_SENSOR= analogRead(A1);


  for (int i=0; i<24;i++){
    possible_hours[i]=i;
  }
  for (int i=0; i<60;i++){
    possible_minutes[i]=i;
  }
  
  if(OPEN_SENSOR<500){
    Serial.println("Encendido en estado 0");
    state=0;
  }
  else{
    Serial.println("Encendido en estado 2"); 
    state=2;
  }
  // The timeout when any button is pressed
  timer_sleep =timer.setInterval(10000, TimerTask);

   if (! rtc.isrunning()) {
     Serial.println("Onfigurando con la fecha de compilación !");
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2018, 12, 7, 11, 0, 0));
    }
    else {
      Serial.println("La fecha actual es : ");
      
   }

}

void loop() {
  DateTime time = rtc.now();
  // Timer start 
  timer.run();
  // En el estado 0 si se recibe un SS 
  
  if(state==0){
    
   Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));
   Serial.println("\n");
   
    if(digitalRead(SHAKE_SENSOR)==HIGH){
      Serial.print("Entramos en estado de configuración");
      state=1;
    }
  else{
    Serial.println("Falsa alarma apaguemos"); 
  }
    
  }

  if(state==1){

    if(debounceButton(next_buttonState,next_button) == HIGH && next_buttonState == LOW){
      //Set del timmer por si uno deja de presionar el boton 
      timer.enable(timer_sleep);
      timer.restartTimer(timer_sleep);
      Serial.println("Timer start");
      next_buttonState = HIGH;
      pressed++;
    
      if(pressed>=current_length){
        pressed=0;
      }
    
      if (part==0){
        Serial.println(possible_years[pressed]);
        current_length= (sizeof(possible_years) / sizeof(possible_years[0]));
      }
      if(part==1){
        Serial.println(possible_months[pressed]);
        current_length= (sizeof(possible_months) / sizeof(possible_months[0]));
      }

      if(part==2){
        Serial.println(possible_days[pressed]);
        current_length= (sizeof(possible_days) / sizeof(possible_days[0]));
      }
      if(part==3){
        Serial.println(possible_hours[pressed]);
        current_length= (sizeof(possible_hours)/sizeof(possible_hours[0]));
      }

      if(part==4){
        Serial.println(possible_minutes[pressed]);
        current_length= (sizeof(possible_minutes)/sizeof(possible_minutes[0]));
      }
    }
    else if(debounceButton(next_buttonState,next_button) == LOW && next_buttonState == HIGH)
    {
       next_buttonState = LOW;
    }
    // Enter button
    if(debounceButton(enter_buttonState,enter_button) == HIGH && enter_buttonState == LOW)
    {
    timer.enable(timer_sleep);
    timer.restartTimer(timer_sleep);
    enter_buttonState = HIGH;
    
    if(part==0){
      final_year=possible_years[pressed];
    }

    if(part==1){
      final_month=possible_months[pressed];
      final_month_integer= possible_months_integer[pressed];
    }

    if(part==2){
      final_day=possible_days[pressed];
    }

    if(part==3){
      final_hour=possible_hours[pressed];
    }

    if(part==4){
      final_minute=possible_minutes[pressed];
    }
    
    pressed=0;
    part++;
    
    Serial.print("part :");
    Serial.println(part);

    if(part==5){
      part=0;
      
      if(validDate(final_day,final_month_integer,final_year) != 999){
        Serial.print("La capsula se abrira el : ");
        Serial.print(final_day);
        Serial.print(" ");
        Serial.print(final_month);
        Serial.print(" ");
        Serial.print(final_year);
        Serial.print(" a las ");
        Serial.print(final_hour);
        Serial.print(":");
        Serial.println(final_minute);
        state=5;
        Serial.println("Deseas cerrar la capsula?");
     }

     else{
        Serial.println("No es posible configurar para ese día");
     } 
     
    }

  }
  else if(debounceButton(enter_buttonState,enter_button) == LOW && enter_buttonState == HIGH)
  {
       enter_buttonState = LOW;
  }
 }

  if(state==2){
    // Cuando la Capusla se cierra, caerá en este estado, siempre, aqui comparará 
    // el año, el mes, el dia, la hora y el minuto, si es igual envía a otro estado 
    // si es diferente, envía señal de apagar el dispositivo....
    //Leer los valores de final todo desde la EEPROM
    char current_year_char[]="YYYY";
    char current_month_char[]="MM";
    char current_day_char[]="DD";
    char current_hour_char[]="hh";
    char current_minute_char[]="mm"; 

    current_year=atoi(time.toString(current_year_char));
    current_month=atoi(time.toString(current_month_char));
    current_day=atoi(time.toString(current_day_char));
    current_hour=atoi(time.toString(current_hour_char));
    current_minute=atoi(time.toString(current_minute_char));
    //Serial.println(time.toString(current_year_char));
    
    //Envía true si está en el mes de apertura de la cápsula. 
    boolean ltm=lessThanMonth(final_year,final_month_integer,current_year,current_month);
    // compara si la fecha es exacta para abrir la cápsula.
    boolean cmpDates=compareDates(final_year,final_month_integer,final_day,final_hour,final_minute,current_year,current_month,current_day,current_hour,current_minute);
    Serial.println(cmpDates);
    if(ltm==1){
      // Mostraremos el conteo regresivo
      countdown_date(final_day,final_hour,final_minute,current_day,current_hour,current_minute);
    }
    else{
        Serial.print("La capsula se abrira el : ");
        Serial.print(final_day);
        Serial.print(" ");
        Serial.print(final_month);
        Serial.print(" ");
        Serial.print(final_year);
        Serial.print(" a las ");
        Serial.print(final_hour);
        Serial.print(":");
        Serial.println(final_minute);
    }

    if(cmpDates==1){
      Serial.println("Abrir la cápsula"); 
    }
    
    
    delay(1000);
  }

  
 if(state==5){
  
    if(debounceButton(enter_buttonState,enter_button) == HIGH && enter_buttonState == LOW)
    {
      timer.enable(timer_sleep);
      timer.restartTimer(timer_sleep);
      enter_buttonState = HIGH;
     Serial.println("Timer start");
     
    if(digitalRead(REED_SWITCH)==HIGH){
      state=2;
      Serial.println("Cerrando Cápsula");
      // Guardar en la EEPROM la fecha
    }
    else{
      Serial.println("Asegurate que la capsula este cierrada");
    }
  }
    else if(debounceButton(enter_buttonState,enter_button) == LOW && enter_buttonState == HIGH)
  {
       enter_buttonState = LOW;
  }

  
  
 }
  

  

}
