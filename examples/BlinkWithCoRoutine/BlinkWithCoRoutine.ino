#include <Arduino_FreeRTOS.h>

// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogRead( void *pvParameters );

// the setup function runs once when 
// you press reset or power the board
void setup() {
  
  // initialize serial communication 
  // at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. 
    // Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(TaskBlink, (const portCHAR *) "Blink", 128, NULL, 2, NULL);   
  // "Blink" A name just for humans
  // 128 This stack size can be checked & adjusted by reading the Stack Highwater
  // 2 Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.

  xTaskCreate(TaskAnalogRead, (const portCHAR *) "AnalogRead", 128, NULL, 1, NULL);
  // Now the task scheduler, which takes over control
  // of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*--------------------- Routins --------------------*/
/*--------------------------------------------------*/


void vFlashCoRoutine(CoRoutineHandle_t xHandle, UBaseType_t uxIndex) {
   // Co-routines must start with a call to crSTART().
   crSTART( xHandle );
   for( ;; ) {
      // Delay for a fixed period.
      crDELAY( xHandle, 10 );
      // Flash an LED.
      vParTestToggleLED( 0 );
   }
   // Co-routines must end with a call to crEND().
   crEND();
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void *pvParameters) {
  (void) pvParameters;
  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // A Task shall never return or exit.
  for (;;) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskAnalogRead(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    // read the input on analog pin 0:
    int sensorValue = analogRead(A0);
    // print out the value you read:
    Serial.println(sensorValue);
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}
