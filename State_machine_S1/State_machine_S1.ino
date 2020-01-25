#include <SimpleTimer.h>

SimpleTimer timer;

uint8_t  state=0; // el estado en el que se encuentra la capsula, esto se definira tambien por la EEPROM
int buttonPin;
const int next_button = 8;// el pin al que conectaremos un pin que será como una interrupcion posteriormente
const int enter_button = 9;
boolean next_buttonState = LOW;
boolean enter_buttonState = LOW; 
int possible_years[]={2020,2021,2022,2023,2024,2025};
String possible_months[]={"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};
int possible_months_integer[]={1,2,3,4,5,6,7,8,9,10,11,12};
int possible_days[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
int timer_sleep;
uint8_t pressed=0;
uint8_t part=0;
int final_year;
String final_month;
int final_month_integer;
int final_day;
int current_length; 

void TimerTask(){
  Serial.println("10 seconds after button was pressed");
  timer.disable(timer_sleep);
  pressed=0;
  part=0;
}

void setup() {
  Serial.begin(9600);
  pinMode(next_button, INPUT);
  pinMode(enter_button, INPUT);
  timer_sleep =timer.setInterval(5000, TimerTask);
  
}

void loop() {
  
  timer.run();
  
  if(debounceButton(next_buttonState,next_button) == HIGH && next_buttonState == LOW)
  {
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
    pressed=0;
    part++;
    Serial.print("part :");
    Serial.println(part);

    if(part==3){
      part=0;
      if(calculateDayOfYear(final_day,final_month_integer,final_year) != 999){

      Serial.print(final_day);
      Serial.print(" ");
      Serial.print(final_month);
      Serial.print(" ");
      Serial.println(final_year);
      }

      else{
        Serial.println("ese día no existe");
      }

      
    }

  }
  else if(debounceButton(enter_buttonState,enter_button) == LOW && enter_buttonState == HIGH)
  {
       enter_buttonState = LOW;
  }
  

}

boolean debounceButton(boolean state,int buttonPin)
{
  boolean stateNow = digitalRead(buttonPin);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin);
  }
  return stateNow;

}

int calculateDayOfYear(int day, int month, int year) {
  
  // Given a day, month, and year (4 digit), returns 
  // the day of year. Errors return 999.
  
  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  // Verify we got a 4-digit year
  if (year < 1000) {
    return 999;
  }
  
  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (year%4  == 0) {
    if (year%100 != 0) {
      daysInMonth[1] = 29;
    }
    else {
      if (year%400 == 0) {
        daysInMonth[1] = 29;
      }
    }
   }

  // Make sure we are on a valid day of the month
  if (day < 1) 
  {
    return 999;
  } else if (day > daysInMonth[month-1]) {
    return 999;
  }
  
  int doy = 0;
  for (int i = 0; i < month - 1; i++) {
    doy += daysInMonth[i];
  }
  
  doy += day;
  return doy;
}
