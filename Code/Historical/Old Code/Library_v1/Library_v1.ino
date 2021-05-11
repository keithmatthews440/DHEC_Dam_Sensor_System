#include <ATSimple.h>

ATSimple AT();

void setup() {
  //AT.begin();// put your setup code here, to run once:
}

void loop() {
  //AT.check();// put your main code here, to run repeatedly:
  AT.doSomething();
  delay(100);
}
