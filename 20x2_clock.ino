
#include <LiquidCrystal.h>
#include <BigNumbers.h>
#include <RTClib.h>

//GPIO's
const int rs = 12, en = 13, d4 = 9, d5 = 10, d6 = 3, d7 = 2;
const int pwm = 16;

const int spreadersPlaces[] = {6, 0, 13, 0, 6, 1, 13, 1}; 
const int backlightDimStartHour = 22;
const int backlightDimEndHour = 6;
const int backlightDayLevel = 200;
const int backlightNightLevel = 20;
const int pwmFreq = 40000;
int cell;

RTC_DS1307 rtc;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
BigNumbers bigNum(&lcd);


void setup() {
  lcd.begin(20, 2);
  bigNum.begin();
  rtc.begin();
  analogWriteFreq(pwmFreq);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop() {
  DateTime now = rtc.now();
  displayUnit(now.hour(), 0);
  displayUnit(now.minute(), 7);
  displayUnit(now.second(), 14);

  backlightControl(now.hour());
  spreaderBlink();
}


void displayUnit(int unit, int column){
  if (unit < 10){
    bigNum.displayLargeInt(0, column, 1, false);
    bigNum.displayLargeInt(unit, column + 3, 1, false);
  }
  else{
    bigNum.displayLargeInt(unit, column, 2, false);
  }
}


void spreaderSet(char symbol) {
  delay(500);
  for (cell = 0; cell < 7; cell = cell + 2)
  {
    lcd.setCursor(spreadersPlaces[cell], spreadersPlaces[cell+1]);
    lcd.print(symbol);
  }
}


void spreaderBlink(){
  spreaderSet('*');
  spreaderSet(' ');
}


void backlightControl(int currentHour){
  if (currentHour <= backlightDimEndHour || currentHour >= backlightDimStartHour){
    analogWrite(pwm, backlightNightLevel);
  }
  else {
    analogWrite(pwm, backlightDayLevel);
  }
}
