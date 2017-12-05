// This #include statement was automatically added by the Particle IDE.
#include <LedControl-MAX7219-MAX7221.h>
// https://github.com/chrisloris/LedControl-MAX7219-MAX7221/blob/master/firmware/examples/matrixpaneldemo.ino

LedControl *display;

int data = A0;
int load = A1;
int myclock = A2;
String token;
int rotation = 0;
int noDisplays = 4;

int haveData = 0;

void myHandler(const char *event, const char *data) {
/*    {
paid_value: "22.7",
paid_units: "M",
programs: 942,
resolved: 59249,
recognized: 6197,
submitted: 19584

22.7~M~942~59265~6197~19586
} */

    String test = String(data);
    if (test.length() > 0) {
        haveData = 1;
        
        String bounty = getValue(data,'~',0);
        String programs = getValue(data,'~',2);
        String resolved = getValue(data,'~',3);
        String recognised = getValue(data,'~',4);
        String submitted = getValue(data,'~',5);

    
        for (int i = 0; i < noDisplays; i++) {
            clearDisplay(i);
        }
        
        setDisplay(bounty, 0);
        setDisplay(programs, 1);
        setDisplay(resolved, 2);
        setDisplay(recognised, 3);
        setDisplay(submitted, 4);
    }
    
}

// String to display, index of the MAX chip
void setDisplay(String str, int index) {
    
    /*
    
    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
    |   |   |   |   |   |   |   |   |
    | _ | _ | _ | _ | 2 | 2 | . | 8 |
    
    length = 8
    str.length = 4
    */
    // we need to fill it in backwards
    int nextDigit = 7;
    for (int i = 0; i < str.length(); i++) {
            display -> setChar(index, nextDigit, str.charAt(i), false);
            nextDigit--;
    }
}

void clearDisplay(int index) {
    for (int i = 0; i < 8; i++) {
        display -> setChar(index, i, ' ', false);
    }  
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
    // LedControl mydisplay = LedControl(DIN_pin, CLK_pin, LOAD_pin, number_of_chips);
     display = new LedControl(data,myclock,load,noDisplays); //DIN,CLK,CS,HowManyDisplays
     // mydisplay.shutdown(chip, false); - initialise all chips
     for (int i = 0; i < noDisplays; i++) {
        display -> shutdown(i, false);
        display -> setIntensity(i, 5);
        clearDisplay(i);
     }
     
     /*for (int y = (noDisplays-1); y >= 0; y--) {
        //mydisplay.setDigit(chip, digit_position, number, dot);
        // Sets a 7 segment display to "number". The chip and position of the digit on that chip must be given. "dot" is usually false. Using "true" will turn on the dot associated with that digit.
          display -> setDigit(y, 0, 0, false);
          display -> setDigit(y, 1, 0, false);
          display -> setDigit(y, 2, 0, false);
          display -> setDigit(y, 3, 0, false);
          display -> setDigit(y, 4, 0, true);
          display -> setDigit(y, 5, 0, false);
          display -> setDigit(y, 6, 0, false);
          display -> setDigit(y, 7, 0, false);
     }*/
     
     
       // Subscribe to the integration response event
    Particle.subscribe("hook-response/H1Metrics", myHandler, MY_DEVICES);
    
    

}

void loop() {
    
    Particle.publish("H1Metrics", PRIVATE);
    if (haveData == 0) {
        animateDisplay(0);
        animateDisplay(1);
        animateDisplay(2);
        animateDisplay(3);
        delay(300);
    }
    else {
        // there is data - no rush to repeat
        delay(60000);
    }
}

// animation
void animateDisplay(int index) {
    if (rotation == 0) {
        display -> setChar(index, 0, '-', false);
        rotation = 1;
    }
    else {
        display -> setChar(index, 0, ' ', true);
        rotation = 0;
    }
}
