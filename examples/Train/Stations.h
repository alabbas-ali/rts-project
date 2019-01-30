#include <stdlib.h>

SemaphoreHandle_t stations[14][2];

// Minimual Time a Train can wait in a Station
int stationDelay = 1500 + (rand() % 750);

void createStationSemaphores() {
  for (int i = 0; i < 14; i++) {
    if ( stations[i][0] == NULL ) {
      stations[i][0] = xSemaphoreCreateBinary();
      if ( ( stations[i][0] ) != NULL )
        xSemaphoreGive( ( stations[i][0] ) );
    }
    if ( stations[i][1] == NULL ) {
      stations[i][1] = xSemaphoreCreateBinary();
      if ( ( stations[i][1] ) != NULL )
        xSemaphoreGive( ( stations[i][1] ) );
    }
  }
  Serial.println(F("Station Semaphores is Created"));
}
