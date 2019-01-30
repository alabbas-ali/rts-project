#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>
#include <time.h>
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

void createSerialSemaphores() {
  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  // Semaphores should only be used whilst the scheduler is running, but we can set it up here.
  if ( xSerialSemaphore == NULL ) {
    xSerialSemaphore = xSemaphoreCreateBinary();  // Create a mutex semaphore we will use to manage the Serial Port
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

  //setup for adding randonmess to the task delays
  srand(time(NULL));

  // create application Semaphores
  createSerialSemaphores();
  createStationSemaphores();
  createLinesSemaphores();
  createSwitchSemaphores();

  Serial.println(F("Start Task Creation"));

  // Now set up four tasks to run independently.
  xTaskCreate(Train1, (const portCHAR *) "Train 1", 512, NULL, 4, NULL);
  xTaskCreate(Train2, (const portCHAR *) "Train 2", 512, NULL, 3, NULL);
  xTaskCreate(Train3, (const portCHAR *) "Train 3", 512, NULL, 2, NULL);
  xTaskCreate(Train4, (const portCHAR *) "Train 4", 512, NULL, 1, NULL);

  Serial.println(F("Start Schedular"));

  // Start the scheduler so the created tasks start executing. */
  vTaskStartScheduler();
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
  char printstring[80];

  xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
  {
    Serial.println(F("Train 1 Start Runing"));
  }
  xSemaphoreGive(xSerialSemaphore);

  for (;;) {

    vTaskDelay(15 + (rand() % 50));

    direction = 1;
    // Going From 21 to 1 : Line: 0,
    xSemaphoreTake(lines[0][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 21, 1, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[0][1]);
      vTaskDelay(linesDelay[0]);
    }

    // Entering station 1 direction 1 : Station: 0
    xSemaphoreTake(stations[0][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[0][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 1 to 2 :  Line: 1
    xSemaphoreTake(lines[1][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 2, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[0][0]);
      vTaskDelay(linesDelay[1]);
    }

    // Entering station 2 direction 1: Station: 1
    xSemaphoreTake(stations[1][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[1][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[2][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 32, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[1][0]);
      vTaskDelay(linesDelay[1]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 32, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[0]);
      vTaskDelay(switchDelay);
    }

    // Going From 32 to 3:  Line: 3
    xSemaphoreTake(lines[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 32, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[2][0]);
      vTaskDelay(linesDelay[3]);
    }

    // Entering station 3 direction 1: Station: 2
    xSemaphoreTake(stations[2][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 3 to 33:   Line: 4
    xSemaphoreTake(lines[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][0]);
      vTaskDelay(linesDelay[4]);
    }

    // Changing Switch 33 to state 1
    xSemaphoreTake(switchs[1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[1]);
      vTaskDelay(switchDelay);
    }

    // Going From 33 to 4:   Line: 5
    xSemaphoreTake(lines[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][0]);
      vTaskDelay(linesDelay[5]);
    }

    // Entering station 4 direction 1 : Station: 3
    xSemaphoreTake(stations[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 4 to 5 :  Line: 6
    xSemaphoreTake(lines[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][0]);
      vTaskDelay(linesDelay[6]);
    }

    // Entering station 5 direction 1 : Station: 4
    xSemaphoreTake(stations[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 5 to 34:  Line: 7
    xSemaphoreTake(lines[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][0]);
      vTaskDelay(linesDelay[7]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[2], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[2]);
      vTaskDelay(switchDelay);
    }

    // Going From 34 to 6 : Line: 8
    xSemaphoreTake(lines[8][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][0]);
      vTaskDelay(linesDelay[8]);
    }

    // Entering station 6 direction 1
    xSemaphoreTake(stations[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[8][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 6 to 7: Line: 9
    xSemaphoreTake(lines[9][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[5][0]);
      vTaskDelay(linesDelay[9]);
    }

    // Entering station 7 direction 1
    xSemaphoreTake(stations[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[9][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 7 to 25 L Line : 10
    xSemaphoreTake(lines[10][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 25, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[6][0]);
      vTaskDelay(linesDelay[10]);
    }

    direction = 2;

    xSemaphoreTake(lines[10][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 25, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[10][0]);
      vTaskDelay(linesDelay[10]);
    }

    xSemaphoreTake(stations[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[10][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[9][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[6][1]);
      vTaskDelay(linesDelay[9]);
    }

    xSemaphoreTake(stations[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[9][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[8][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[5][1]);
      vTaskDelay(linesDelay[8]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[3], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[3]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[8][1]);
      vTaskDelay(linesDelay[7]);
    }

    xSemaphoreTake(stations[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][1]);
      vTaskDelay(linesDelay[6]);
    }

    xSemaphoreTake(stations[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][1]);
      vTaskDelay(linesDelay[5]);
    }

    xSemaphoreTake(switchs[4], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[4]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][1]);
      vTaskDelay(linesDelay[4]);
    }

    xSemaphoreTake(stations[2][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][1]);
      vTaskDelay(linesDelay[3]);
    }

    xSemaphoreTake(switchs[6], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[6]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[2][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 37, 2, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][1]);
      vTaskDelay(linesDelay[2]);
    }

    xSemaphoreTake(stations[1][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 2, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[2][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[1][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 2, 1, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[1][1]);
      vTaskDelay(linesDelay[1]);
    }

    xSemaphoreTake(stations[0][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 1, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[1][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[0][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 1, 21, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[0][1]);
      vTaskDelay(linesDelay[0]);
    }
    Serial.flush();
  }
}

void Train2(void *pvParameters) {
  (void) pvParameters;
  int const train_Numner = 2;
  int direction = 1;
  char printstring[80];


  xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
  {
    Serial.println("Train 2 Start Runing");
  }
  xSemaphoreGive(xSerialSemaphore);

  for (;;) {
    vTaskDelay(15 + (rand() % 50));

    direction = 1;

    xSemaphoreTake(lines[20][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 22, 13, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[20][1]);
      vTaskDelay(linesDelay[20]);
    }

    // Entering station 1 direction 1 : Station: 0
    xSemaphoreTake(stations[12][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 13, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[20][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 1 to 2 :  Line: 1
    xSemaphoreTake(lines[19][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 13, 31, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[12][0]);
      vTaskDelay(linesDelay[19]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[7], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 31, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[7]);
      vTaskDelay(switchDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[18][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 31, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[19][0]);
      vTaskDelay(linesDelay[18]);
    }

    // Entering station 2 direction 1: Station: 1
    xSemaphoreTake(stations[11][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[18][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[17][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 12, 32, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[11][0]);
      vTaskDelay(linesDelay[17]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 32, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[0]);
      vTaskDelay(switchDelay);
    }

    // Going From 32 to 3:  Line: 3
    xSemaphoreTake(lines[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 32, 3, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[17][0]);
      vTaskDelay(linesDelay[3]);
    }

    // Entering station 3 direction 1: Station: 2
    xSemaphoreTake(stations[2][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 3 to 33:   Line: 4
    xSemaphoreTake(lines[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][0]);
      vTaskDelay(linesDelay[4]);
    }

    // Changing Switch 33 to state 1
    xSemaphoreTake(switchs[1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[1]);
      vTaskDelay(switchDelay);
    }

    // Going From 33 to 4:   Line: 5
    xSemaphoreTake(lines[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][0]);
      vTaskDelay(linesDelay[5]);
    }

    // Entering station 4 direction 1 : Station: 3
    xSemaphoreTake(stations[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 4 to 5 :  Line: 6
    xSemaphoreTake(lines[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][0]);
      vTaskDelay(linesDelay[6]);
    }

    // Entering station 5 direction 1 : Station: 4
    xSemaphoreTake(stations[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 5 to 34:  Line: 7
    xSemaphoreTake(lines[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][0]);
      vTaskDelay(linesDelay[7]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[2], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[2]);
      vTaskDelay(switchDelay);
    }

    // Going From 34 to 6 : Line: 8
    xSemaphoreTake(lines[8][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][0]);
      vTaskDelay(linesDelay[8]);
    }

    // Entering station 6 direction 1
    xSemaphoreTake(stations[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[8][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 6 to 7: Line: 9
    xSemaphoreTake(lines[9][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[5][0]);
      vTaskDelay(linesDelay[9]);
    }

    // Entering station 7 direction 1
    xSemaphoreTake(stations[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[9][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 7 to 25 L Line : 10
    xSemaphoreTake(lines[10][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 25, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[6][0]);
      vTaskDelay(linesDelay[10]);
    }

    direction = 2;

    xSemaphoreTake(lines[10][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 25, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[10][0]);
      vTaskDelay(linesDelay[10]);
    }

    xSemaphoreTake(stations[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 7, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[10][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[9][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 7, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[6][1]);
      vTaskDelay(linesDelay[9]);
    }

    xSemaphoreTake(stations[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 6, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[9][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[8][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 6, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[5][1]);
      vTaskDelay(linesDelay[8]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[3], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[3]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[8][1]);
      vTaskDelay(linesDelay[7]);
    }

    xSemaphoreTake(stations[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][1]);
      vTaskDelay(linesDelay[6]);
    }

    xSemaphoreTake(stations[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][1]);
      vTaskDelay(linesDelay[5]);
    }

    xSemaphoreTake(switchs[4], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[4]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][1]);
      vTaskDelay(linesDelay[4]);
    }

    xSemaphoreTake(stations[2][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][1]);
      vTaskDelay(linesDelay[3]);
    }

    xSemaphoreTake(switchs[6], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[6]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[17][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 37, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][1]);
      vTaskDelay(linesDelay[17]);
    }

    xSemaphoreTake(stations[11][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[17][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[18][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 12, 38, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[11][1]);
      vTaskDelay(linesDelay[18]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[6], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 38, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[6]);
      vTaskDelay(switchDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[19][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 38, 13, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[18][1]);
      vTaskDelay(linesDelay[19]);
    }

    xSemaphoreTake(stations[12][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 13, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[19][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[20][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 13, 22, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[12][1]);
      vTaskDelay(linesDelay[20]);
    }
    Serial.flush();
  }
}

void Train3(void *pvParameters) {
  (void) pvParameters;
  int const train_Numner = 3;
  int direction = 1;
  char printstring[80];

  xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
  {
    Serial.println("Train 3 Start Runing");
  }
  xSemaphoreGive(xSerialSemaphore);

  for (;;) {
    vTaskDelay(15 + (rand() % 50));

    direction = 1;

    xSemaphoreTake(lines[22][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 23, 14, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[22][1]);
      vTaskDelay(linesDelay[22]);
    }

    // Entering station 1 direction 1 : Station: 0
    xSemaphoreTake(stations[13][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 14, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[22][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 1 to 2 :  Line: 1
    xSemaphoreTake(lines[21][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 14, 31, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[13][0]);
      vTaskDelay(linesDelay[21]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[7], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 31, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[7]);
      vTaskDelay(switchDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[18][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 31, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[21][0]);
      vTaskDelay(linesDelay[18]);
    }

    // Entering station 2 direction 1: Station: 1
    xSemaphoreTake(stations[11][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[18][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[17][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 12, 32, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[11][0]);
      vTaskDelay(linesDelay[17]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 32, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[0]);
      vTaskDelay(switchDelay);
    }

    // Going From 32 to 3:  Line: 3
    xSemaphoreTake(lines[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 32, 3, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[17][0]);
      vTaskDelay(linesDelay[3]);
    }

    // Entering station 3 direction 1: Station: 2
    xSemaphoreTake(stations[2][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 3 to 33:   Line: 4
    xSemaphoreTake(lines[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][0]);
      vTaskDelay(linesDelay[4]);
    }

    // Changing Switch 33 to state 1
    xSemaphoreTake(switchs[1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[1]);
      vTaskDelay(switchDelay);
    }

    // Going From 33 to 4:   Line: 5
    xSemaphoreTake(lines[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][0]);
      vTaskDelay(linesDelay[5]);
    }

    // Entering station 4 direction 1 : Station: 3
    xSemaphoreTake(stations[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 4 to 5 :  Line: 6
    xSemaphoreTake(lines[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][0]);
      vTaskDelay(linesDelay[6]);
    }

    // Entering station 5 direction 1 : Station: 4
    xSemaphoreTake(stations[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 5 to 34:  Line: 7
    xSemaphoreTake(lines[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][0]);
      vTaskDelay(linesDelay[7]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[2], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[2]);
      vTaskDelay(switchDelay);
    }

    // Going From 34 to 6 : Line: 8
    xSemaphoreTake(lines[11][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][0]);
      vTaskDelay(linesDelay[11]);
    }

    // Entering station 6 direction 1
    xSemaphoreTake(stations[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[11][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 6 to 7: Line: 9
    xSemaphoreTake(lines[12][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 8, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[7][0]);
      vTaskDelay(linesDelay[12]);
    }

    // Entering station 7 direction 1
    xSemaphoreTake(stations[8][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[12][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 7 to 25 L Line : 10
    xSemaphoreTake(lines[13][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 9, 26, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[8][0]);
      vTaskDelay(linesDelay[13]);
    }

    direction = 2;

    xSemaphoreTake(lines[13][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 26, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[13][0]);
      vTaskDelay(linesDelay[13]);
    }

    xSemaphoreTake(stations[8][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[13][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[12][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 9, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[8][1]);
      vTaskDelay(linesDelay[12]);
    }

    xSemaphoreTake(stations[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[12][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[11][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 8, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[7][1]);
      vTaskDelay(linesDelay[11]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[3], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[3]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[11][1]);
      vTaskDelay(linesDelay[7]);
    }

    xSemaphoreTake(stations[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][1]);
      vTaskDelay(linesDelay[6]);
    }

    xSemaphoreTake(stations[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][1]);
      vTaskDelay(linesDelay[5]);
    }

    xSemaphoreTake(switchs[4], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[4]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][1]);
      vTaskDelay(linesDelay[4]);
    }

    xSemaphoreTake(stations[2][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 3, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[4][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 3, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[2][1]);
      vTaskDelay(linesDelay[3]);
    }

    xSemaphoreTake(switchs[6], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 37, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[6]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[17][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 37, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[3][1]);
      vTaskDelay(linesDelay[17]);
    }

    xSemaphoreTake(stations[11][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 12, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[17][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[18][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 12, 38, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[11][1]);
      vTaskDelay(linesDelay[18]);
    }

    // Changing Switch 32 to state 1
    xSemaphoreTake(switchs[6], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 38, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[6]);
      vTaskDelay(switchDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[21][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 38, 14, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[18][1]);
      vTaskDelay(linesDelay[21]);
    }

    xSemaphoreTake(stations[13][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 14, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[21][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[22][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 14, 23, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[13][1]);
      vTaskDelay(linesDelay[22]);
    }
    Serial.flush();
  }
}

void Train4(void *pvParameters) {
  (void) pvParameters;
  int const train_Numner = 4;
  int direction = 1;
  char printstring[80];

  xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
  {
    Serial.println(F("Train 4 Start Runing"));
  }
  xSemaphoreGive(xSerialSemaphore);

  for (;;) {

    vTaskDelay(15 + (rand() % 50));

    direction = 1;
    // Going From 21 to 1 : Line: 0,
    xSemaphoreTake(lines[16][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i , 'direction': %i}" , "InterLine" , train_Numner, 24, 11, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[16][1]);
      vTaskDelay(linesDelay[16]);
    }

    // Entering station 1 direction 1 : Station: 0
    xSemaphoreTake(stations[10][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 11, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[16][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 1 to 2 :  Line: 1
    xSemaphoreTake(lines[15][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 11, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[10][0]);
      vTaskDelay(linesDelay[15]);
    }

    // Entering station 2 direction 1: Station: 1
    xSemaphoreTake(stations[9][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 10, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[15][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 2 to 32:  Line: 2
    xSemaphoreTake(lines[14][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 10, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[9][0]);
      vTaskDelay(linesDelay[14]);
    }

    // Changing Switch 33 to state 1
    xSemaphoreTake(switchs[1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 33, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[1]);
      vTaskDelay(switchDelay);
    }

    // Going From 33 to 4:   Line: 5
    xSemaphoreTake(lines[5][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 33, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[14][0]);
      vTaskDelay(linesDelay[5]);
    }

    // Entering station 4 direction 1 : Station: 3
    xSemaphoreTake(stations[3][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 4 to 5 :  Line: 6
    xSemaphoreTake(lines[6][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][0]);
      vTaskDelay(linesDelay[6]);
    }

    // Entering station 5 direction 1 : Station: 4
    xSemaphoreTake(stations[4][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 5 to 34:  Line: 7
    xSemaphoreTake(lines[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][0]);
      vTaskDelay(linesDelay[7]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[2], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 34, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[2]);
      vTaskDelay(switchDelay);
    }

    // Going From 34 to 6 : Line: 8
    xSemaphoreTake(lines[11][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 34, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][0]);
      vTaskDelay(linesDelay[8]);
    }

    // Entering station 6 direction 1
    xSemaphoreTake(stations[7][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[11][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 6 to 7: Line: 9
    xSemaphoreTake(lines[12][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 8, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[7][0]);
      vTaskDelay(linesDelay[12]);
    }

    // Entering station 7 direction 1
    xSemaphoreTake(stations[8][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[12][0]);
      vTaskDelay(stationDelay);
    }

    // Going From 7 to 25 L Line : 10
    xSemaphoreTake(lines[13][0], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 9, 26, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[8][0]);
      vTaskDelay(linesDelay[13]);
    }

    direction = 2;

    xSemaphoreTake(lines[13][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 26, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[13][0]);
      vTaskDelay(linesDelay[13]);
    }

    xSemaphoreTake(stations[8][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 9, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[13][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[12][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 9, 8, direction);
        Serial.println(printstring);
      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[8][1]);
      vTaskDelay(linesDelay[12]);
    }

    xSemaphoreTake(stations[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 8, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[12][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[11][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 8, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[7][1]);
      vTaskDelay(linesDelay[11]);
    }

    // Changing Switch 34 to state 1
    xSemaphoreTake(switchs[3], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 35, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[3]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[7][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 35, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[11][1]);
      vTaskDelay(linesDelay[7]);
    }

    xSemaphoreTake(stations[4][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 5, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[7][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[6][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 5, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[4][1]);
      vTaskDelay(linesDelay[6]);
    }

    xSemaphoreTake(stations[3][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 4, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[6][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[5][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 4, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[3][1]);
      vTaskDelay(linesDelay[5]);
    }

    xSemaphoreTake(switchs[4], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'switch': %i, 'direction': %i}" , "ChangeSwitch" , train_Numner, 36, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(switchs[4]);
      vTaskDelay(switchDelay);
    }

    xSemaphoreTake(lines[14][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 36, 10, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[5][1]);
      vTaskDelay(linesDelay[14]);
    }

    xSemaphoreTake(stations[9][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 10, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[14][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[15][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 10, 11, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[9][1]);
      vTaskDelay(linesDelay[15]);
    }

    xSemaphoreTake(stations[10][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'station': %i, 'direction': %i}" , "InterStation" , train_Numner, 11, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(lines[15][1]);
      vTaskDelay(stationDelay);
    }

    xSemaphoreTake(lines[16][1], portMAX_DELAY);
    {
      xSemaphoreTake(xSerialSemaphore, portMAX_DELAY);
      {
        sprintf(printstring, "{'command': %s, 'train': %i, 'from': %i, 'to': %i, 'direction': %i}" , "InterLine" , train_Numner, 11, 24, direction);
        Serial.println(printstring);

      }
      xSemaphoreGive(xSerialSemaphore);
      xSemaphoreGive(stations[11][1]);
      vTaskDelay(linesDelay[16]);
    }
    Serial.flush();
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
//
//      }
//      xSemaphoreGive( xSerialSemaphore );
//    }
//    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
//  }
//}
