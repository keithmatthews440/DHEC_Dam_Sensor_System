#ifdef AT_SHORTS_H
#define AT_SHORTS_H

#include "Arduino.h"


class ATShorts{
  public:
    ATShorts();
    String read();
    void print();
    void begin();
    void printHex();
  private:
  
}
#endif
