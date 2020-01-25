
#include "RTClib.h"

RTC_DS1307 rtc;


void setup() {
  Serial.begin(9600);
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2018, 12, 7, 11, 0, 0));
  }
  else {
    Serial.println("RTC is NOW running!");
  }

}

void loop() {
  DateTime time = rtc.now();

 //Full Timestamp
 Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+time.timestamp(DateTime::TIMESTAMP_FULL));

 Serial.println("\n");
  delay(5000);
}
