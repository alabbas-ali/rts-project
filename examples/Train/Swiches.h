
SemaphoreHandle_t switchs[8];

int switchDelay = 70;

void createSwitchSemaphores(){
  for(int i = 0; i < 8; i++){
    if ( switchs[i] == NULL ) {
      switchs[i] = xSemaphoreCreateMutex();  
      if ( ( switchs[i] ) != NULL )
        xSemaphoreGive( ( switchs[i] ) );  
    }
  }

  // Serial.println(F("Switch Semaphores is Created"));
}
