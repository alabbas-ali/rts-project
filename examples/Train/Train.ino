#include "Arduino_FreeRTOS.h"

#include "semphr.h"
#include "stdio.h"
#include "Stations.h"
#include "Lines.h"
#include "Swiches.h"

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialSemaphore;

// define two tasks for Blink & AnalogRead
void Train1( void *pvParameters );
void Train2( void *pvParameters );
void Train3( void *pvParameters );
//void readFromSerial( void *pvParameters );

void createSerialSemaphores(){
  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL ) {
    xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
    if ( ( xSerialSemaphore ) != NULL )
      xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
  }

  Serial.println(F("Serial Semaphore is Created"));
}

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect.
    // Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  Serial.println(F("Application is ready"));
  
  // create application Semaphores
  createSerialSemaphores();
  createStationSemaphores();
  createLinesSemaphores();
  createSwitchSemaphores();

  Serial.println(F("Start Task Creation"));
  
  // Now set up two tasks to run independently.
  xTaskCreate(Train1, (const portCHAR *) "Train 1", 128, NULL, 3, NULL);
  xTaskCreate(Train2, (const portCHAR *) "Train 2", 128, NULL, 2, NULL);
  xTaskCreate(Train3, (const portCHAR *) "Train 3", 128, NULL, 1, NULL);

  Serial.println(F("Start Schedular"));
  
  // Start the scheduler so the created tasks start executing. */
  //vTaskStartScheduler();
}

void loop() {
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
void Train1(void *pvParameters) {
	
  (void) pvParameters;
  int const train_Numner = 1;
  int direction = 1;
  char printstring[100];

  xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
  {   
    Serial.println(F("Train 1 Start Runing"));
  }
  xSemaphoreGive(xSerialSemaphore);
   
  for (;;) {

    delay(1);
    
	  direction = 1;
    // Going From 21 to 1 : Line: 0, 
    xSemaphoreTake(lines[0][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 21, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(linesDelay[0]);
    }

    // Entering station 1 direction 1 : Station: 0
    xSemaphoreTake(stations[0][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	    xSemaphoreGive(lines[0][0]);
      delay(stationDelay);
    }

    // Going From 1 to 2 :  Line: 1
    xSemaphoreTake(lines[1][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      }
	  xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[0][0]);
      delay(linesDelay[1]);
    }

    // Entering station 2 direction 1: Station: 1
    xSemaphoreTake(stations[1][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[1][0]);
      delay(stationDelay);
    }
     
    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[2][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 32, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[1][0]);
      delay(linesDelay[2]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 32, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }

    // Going From 32 to 3:  Line: 3
    xSemaphoreTake(lines[3][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 32, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[2][0]);
      delay(linesDelay[3]);
    }

    // Entering station 3 direction 1: Station: 2
    xSemaphoreTake(stations[2][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[3][0]);
      delay(stationDelay);
    }

    // Going From 3 to 33:   Line: 4
    xSemaphoreTake(lines[4][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 33, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[2][0]);
      delay(linesDelay[4]);
    }

    // Changing Switch 33 to state 1
    xSemaphoreTake(switchs[1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }

    // Going From 33 to 4:   Line: 5
    xSemaphoreTake(lines[5][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[4][0]);
      delay(linesDelay[5]);
    }

    // Entering station 4 direction 1 : Station: 3
    xSemaphoreTake(stations[3][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[5][0]);
      delay(stationDelay);
    }
	
	
	// Going From 4 to 5 :  Line: 6
    xSemaphoreTake(lines[6][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[3][0]);
      delay(linesDelay[6]);
    }
	
	// Entering station 5 direction 1 : Station: 4
    xSemaphoreTake(stations[4][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[6][0]);
      delay(stationDelay);
    }

    // Going From 5 to 34:  Line: 7
    xSemaphoreTake(lines[7][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[4][0]);
      delay(linesDelay[6]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[2], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }

    // Going From 34 to 6 : Line: 8
    xSemaphoreTake(lines[8][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[7][0]);
      delay(linesDelay[8]);
    }

    // Entering station 6 direction 1
    xSemaphoreTake(stations[5][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[8][0]);
      delay(stationDelay);
    }

    // Going From 6 to 7: Line: 9
    xSemaphoreTake(lines[9][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[5][0]);
      delay(linesDelay[9]);
    }

    // Entering station 7 direction 1
    xSemaphoreTake(stations[6][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[9][0]);
      delay(stationDelay);
    }

    // Going From 7 to 25 L Line : 10
    xSemaphoreTake(lines[10][0], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 25, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[6][0]);
      delay(linesDelay[10]);
    }
	
	direction = 2;
	
	xSemaphoreTake(lines[10][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 25, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(linesDelay[10]);
    }
	
	xSemaphoreTake(stations[6][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[10][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[9][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[6][1]);
      delay(linesDelay[9]);
    }
	
	xSemaphoreTake(stations[5][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[9][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[8][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 35, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[5][1]);
      delay(linesDelay[8]);
    }
	
	// Changing Switch 34 to state 1
    xSemaphoreTake(switchs[3], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }
	
	xSemaphoreTake(lines[7][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[8][1]);
      delay(linesDelay[7]);
    }
	
	xSemaphoreTake(stations[4][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[7][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[6][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[4][1]);
      delay(linesDelay[6]);
    }
	
	xSemaphoreTake(stations[3][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[6][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[5][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[3][1]);
      delay(linesDelay[5]);
    }
	
    xSemaphoreTake(switchs[4], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }
	
	xSemaphoreTake(lines[4][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[5][1]);
      delay(linesDelay[4]);
    }
	
	xSemaphoreTake(stations[2][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[4][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[3][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 37, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[2][1]);
      delay(linesDelay[3]);
    }
	
	xSemaphoreTake(switchs[5], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'witch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 37, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
      delay(switchDelay);
    }
	
	xSemaphoreTake(lines[2][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 37, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[3][1]);
      delay(linesDelay[2]);
    }
	
	xSemaphoreTake(stations[1][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[2][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[1][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[1][1]);
      delay(linesDelay[1]);
    }
	
	xSemaphoreTake(stations[0][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(lines[1][1]);
      delay(stationDelay);
    }
	
	xSemaphoreTake(lines[0][1], ( TickType_t ) 1000);
    {
      xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 21, direction);
        Serial.println(printstring);
        Serial.flush();
      }
      xSemaphoreGive(xSerialSemaphore);
	  xSemaphoreGive(stations[0][1]);
      delay(linesDelay[0]);
    }
  }
}




void Train2(void *pvParameters) {
  (void) pvParameters;
  char const *Train1Name = "Train 2";

  xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
  {   
    Serial.println("Train 2 Start Runing");
  }
  xSemaphoreGive(xSerialSemaphore);
  
  for (;;) {
    delay(2000);
    xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
    {
      Serial.println( Train1Name );
    }
    xSemaphoreGive(xSerialSemaphore);
  }
}

void Train3(void *pvParameters) {
  (void) pvParameters;
  char const *Train1Name = "Train 3";

  xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
  {   
    Serial.println("Train 3 Start Runing");
  }
  xSemaphoreGive(xSerialSemaphore);
  for (;;) {
    delay(2000);
    xSemaphoreTake(xSerialSemaphore, ( TickType_t ) 1000);
    {
      Serial.println( Train1Name );
    }
    xSemaphoreGive(xSerialSemaphore);
  }
}


/*--------------------------------------------------*/
/*------------- Read Config From Serial  -----------*/
/*--------------------------------------------------*/

//void readFromSerial( void *pvParameters __attribute__((unused)) ) {
//  char string[] = "";   // for incoming serial string data
//  for (;;) {
//    // See if we can obtain or "Take" the Serial Semaphore.
//    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
//    if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 5 ) == pdTRUE ) {
//      if (Serial.available() > 0) {
//        int byteAvailable = Serial.available();
//        for (int i = 0; i < byteAvailable; i++) {
//          string[i] = Serial.read();
//        }
//        Serial.print("Chare is readed : ");
//        Serial.println(string);
//        Serial.flush();
//      }
//      xSemaphoreGive( xSerialSemaphore );
//    }
//    delay(1);  // one tick delay (15ms) in between reads for stability
//  }
//}
