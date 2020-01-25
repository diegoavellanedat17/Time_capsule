/*
 * myFirstLibrary.h - An introduction to library setup
 * Created by Christian @ Core Electronics on 1/06/18
 * Revision #5 - See readMe 
 */

//	The #ifndef statement checks to see if the myFirstLibrary.h
//	file isn't already defined. This is to stop double declarations
//	of any identifiers within the library. It is paired with a
//	#endif at the bottom of the header and this setup is known as 
//	an 'Include Guard'. 
#ifndef S1_STATE_h

//	The #define statement defines this file as the myFirstLibrary
//	Header File so that it can be included within the source file.                                           
#define S1_STATE_h

//	The #include of Arduino.h gives this library access to the standard
//	Arduino types and constants (HIGH, digitalWrite, etc.). It's 
//	unneccesary for sketches but required for libraries as they're not
//	.ino (Arduino) files.

#include "Arduino.h"
	
	int validDate(int day,int month,int year);;
	boolean debounceButton(boolean state,int buttonPin);
	boolean lessThanMonth(int final_year, uint8_t final_month, int current_year, uint8_t current_month);
	boolean compareDates(int final_year, uint8_t final_month, uint8_t final_day, uint8_t final_hour, uint8_t final_minute, int current_year, uint8_t current_month, uint8_t current_day, uint8_t current_hour, uint8_t current_minute);
	void countdown_date(uint8_t final_day, uint8_t final_hour, uint8_t final_minute, uint8_t current_day,uint8_t current_hour, uint8_t current_minute);


#endif