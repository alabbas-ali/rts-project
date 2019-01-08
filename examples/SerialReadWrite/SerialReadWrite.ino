#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Declare a mutex Semaphore Handle which we will use to manage the Serial Port.
// It will be used to ensure only only one Task is accessing this resource at any time.
SemaphoreHandle_t xSerialMutex;

/* The function that uses a mutex to control access to standard out. */
static void writeToSerial( const char *pcString );

// define two tasks for Blink & AnalogRead
void Task1( void *pvParameters );
void Task2( void *pvParameters );

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
  
  // Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
  // because it is sharing a resource, such as the Serial port.
  xSerialMutex = xSemaphoreCreateMutex();
    
  // Check to confirm that the Serial Semaphore has not already been created.
  if ( xSerialMutex != NULL ) {

    char const *t = "Application is ready";
    Serial.println(t);
    
    // Now set up two tasks to run independently.
    xTaskCreate(Task1, (const portCHAR *) "Blink", 128, NULL, 2, NULL);   
    // "Blink" A name just for humans
    // 128 This stack size can be checked & adjusted by reading the Stack Highwater
    // 2 Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  
    xTaskCreate(Task2, (const portCHAR *) "AnalogRead", 128, NULL, 1, NULL);
    // Now the task scheduler, which takes over control
    // of scheduling individual tasks, is automatically started.

    // Start the scheduler so the created tasks start executing. */
    vTaskStartScheduler();
  }

  // If all is well we will never reach here as the scheduler will now be
  // running the tasks.  If we do reach here then it is likely that there was
  // insufficient heap memory available for a resource to be created.
  for( ;; );
  //return 0;
}

void loop() {
  // Empty. Things are done in Tasks.
}

void writeToSerial(const char *pcString) {
    // See if we can obtain or "Take" the Serial Semaphore.
    // If the semaphore is not available, wait 5 ticks of the Scheduler to see if it becomes free.
    xSemaphoreTake( xSerialMutex, portMAX_DELAY );
    {
      Serial.println(pcString);
      Serial.flush();
    }
    xSemaphoreGive( xSerialMutex );
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
void Task1(void *pvParameters) {
  (void) pvParameters;
  char const *pcStringToPrint = "This is Task One";
  for (;;) {
    vTaskDelay(1000); // wait for one second
    writeToSerial( pcStringToPrint );
  }
}

void Task2(void *pvParameters) {
  (void) pvParameters;
  char const *pcStringToPrint = "This is Task Tow";
  for (;;) {
    vTaskDelay(2000); 
    writeToSerial( pcStringToPrint );
  }
}
