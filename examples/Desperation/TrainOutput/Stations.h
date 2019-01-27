
//SemaphoreHandle_t stations[13][2];

// Minimual Time a Train can wait in a Station
int stationDelay = 1500;

void createStationSemaphores(){
//  for(int i = 0; i < 13; i++){
//    if ( stations[i][0] == NULL ) {
//      stations[i][0] = xSemaphoreCreateBinary();
//      if ( ( stations[i][0] ) != NULL )
//        xSemaphoreGive( ( stations[i][0] ) );  
//    }
//    if ( stations[i][1] == NULL ) {
//      stations[i][1] = xSemaphoreCreateBinary();
//      if ( ( stations[i][1] ) != NULL )
//        xSemaphoreGive( ( stations[i][1] ) );  
//    } 
//  }
  Serial.println("Station Semaphores is Created");
}
