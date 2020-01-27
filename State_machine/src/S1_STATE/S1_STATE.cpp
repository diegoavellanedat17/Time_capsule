/*
 * S1_STATE.pp- Library created by Dover Jan 20 /20
 */

//	The #include of Arduino.h gives this library access to the standard
//	Arduino types and constants (HIGH, digitalWrite, etc.). It's 
//	unneccesary for sketches but required for libraries as they're not
//	.ino (Arduino) files. 
#include "Arduino.h"

//	This will include the Header File so that the Source File has access
//	to the function definitions in the S1_STATE library.
#include "S1_STATE.h" 


int validDate(int day, int month, int year) {
  
  // Given a day, month, and year (4 digit), returns 
  // the day of year. Errors return 999.
  year=year+2000;
  
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
  else {
    return 1;
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

boolean compareDates(int final_year, uint8_t final_month, uint8_t final_day, uint8_t final_hour, uint8_t final_minute, int current_year, uint8_t current_month, uint8_t current_day, uint8_t current_hour, uint8_t current_minute){

  if(final_year == current_year){

    if(final_month==current_month){

      if(final_day==current_day){

        if(final_hour== current_hour){

          if(final_minute==current_minute){

            return true;
          }

          else {
            return false;
          }

        }

        else{
          return false;
        }

      }

      else{
        return false;
      }


    }
    else {
      return false;
    }
  }

  else{
    return false;
  }

}

// Esta funcion retorna True si falta un mes o menos 
boolean lessThanMonth(int final_year, uint8_t final_month, int current_year, uint8_t current_month){

  if(final_year==current_year){

    if(final_month== current_month){

      return true;

    }

    else{

      return false;

    }

  }

  else{

    return false;
  }

}

void countdown_date(uint8_t final_day, uint8_t final_hour, uint8_t final_minute, uint8_t current_day,uint8_t current_hour, uint8_t current_minute){
int number_days=final_day - current_day;
int number_hours= final_hour - current_hour;
int number_minutes= final_minute - current_minute;

Serial.print("Faltan ");
Serial.print(number_days);
Serial.print(" días");
Serial.print(number_hours);
Serial.print(" horas y ");
Serial.print(number_minutes);
Serial.print(" minutos... ");

}

