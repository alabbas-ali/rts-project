#include "Lines.h"
#include "Stations.h"
#include "Swiches.h"
#include <stdio.h>

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect.
    // Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  Serial.println(F("Application is ready"));
  
  // create application Semaphores
  //createSerialSemaphores();
  createStationSemaphores();
  createLinesSemaphores();
  createSwitchSemaphores();

  Serial.println(F("Start Task Creation"));

  Train1();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void Train1() {
  
  int const train_Numner = 1;
  int direction = 1;
  char printstring[100];

 
  Serial.println(F("Train 1 Start Runing"));
   
  for (;;) {

    delay(1);
    
    direction = 1;
    // Going From 21 to 1 : Line: 0, 
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 21, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[0]);
   

    // Entering station 1 direction 1 : Station: 0
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);
        Serial.flush();
     
      delay(stationDelay);
 

    // Going From 1 to 2 :  Line: 1
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 2, direction);
        Serial.println(printstring);
        Serial.flush();
     
      delay(linesDelay[1]);


    // Entering station 2 direction 1: Station: 1
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
     
    // Going From 2 to 32:  Line: 2
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 32, direction);
        Serial.println(printstring);
        Serial.flush();
     
      delay(linesDelay[2]);


    // Changing Switch 32 to state 1
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 32, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(switchDelay);

   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 32, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[3]);

    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);

    
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 33, direction);
        Serial.println(printstring);
        Serial.flush();
        
      delay(linesDelay[4]);
    

    // Changing Switch 33 to state 1
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(switchDelay);

    // Going From 33 to 4:   Line: 5
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);
        Serial.flush();
        
      delay(linesDelay[5]);

    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);
        Serial.flush();
        
      delay(stationDelay);
    
  
  
  // Going From 4 to 5 :  Line: 6
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[6]);
    
  
  // Entering station 5 direction 1 : Station: 4
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      delay(stationDelay);
    

    // Going From 5 to 34:  Line: 7
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[6]);
    

    // Changing Switch 34 to state 1
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(switchDelay);
    

    // Going From 34 to 6 : Line: 8
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[8]);

    
    // Entering station 6 direction 1
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    

        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[9]);
    

    // Entering station 7 direction 1
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    

    // Going From 7 to 25 L Line : 10
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 25, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[10]);
    
  
  direction = 2;
  
 
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 25, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[10]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);
        Serial.flush();
     
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[9]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 35, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[8]);
    
  
  // Changing Switch 34 to state 1
   
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);
        Serial.flush();
     
      delay(switchDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[7]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[6]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[5]);
    
  
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(switchDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[4]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 37, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[3]);
    
  
 
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 37, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(switchDelay);
    
  
  
     
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 37, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[2]);
    
  
 
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[1]);
    
  
  
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(stationDelay);
    
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 21, direction);
        Serial.println(printstring);
        Serial.flush();
      
      delay(linesDelay[0]);
    
  }
}
