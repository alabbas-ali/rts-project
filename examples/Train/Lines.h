#include <stdlib.h>

SemaphoreHandle_t lines[23][2];

int linesDelay[23];

// Time is a scaled of the real word time be each 1 min to 1 second
// This is the minimum time any train required to base on the line
// This time could be extended in case the next resource required
// to release the line is used .
//
// Line # ||  From -> To    ||  Time (ms) ||
// 0      ||  21 -> 1       ||  1000      || lines[0][0] //  lines[0][1]
// 1      ||  1  -> 2       ||  3000      || lines[1][0] //  lines[1][1]
// 2      ||  2  -> 32/37   ||  3500      || lines[2][0] //  lines[2][1]
// 3      ||  32 -> 3       ||  500       || lines[3][0] //  lines[3][1]
// 4      ||  3  -> 33/36   ||  2500      || lines[4][0] //  lines[4][1]
// 5      ||  33/36 -> 4    ||  500       || lines[5][0] //  lines[5][1]
// 6      ||  4  -> 5       ||  2500      || lines[6][0] //  lines[6][1]
// 7      ||  5  -> 34/35   ||  500       || lines[7][0] //  lines[7][1]
// 8      ||  34/35  -> 6   ||  3500      || lines[8][0] //  lines[8][1]
// 9      ||  34/35  -> 8   ||  3000      || lines[9][0] //  lines[8][1]
// 10     ||  6  -> 7       ||  4000      || lines[10][0] //  lines[10][1]
// 11     ||  7  -> 25      ||  1000      || lines[11][0] //  lines[11][1]
// 12     ||  8  -> 9       ||  3000      || lines[12][0] //  lines[12][1]
// 13     ||  9  -> 26      ||  1000      || lines[13][0] //  lines[13][1]
// 14     ||  33/36  -> 10  ||  3500      || lines[14][0] //  lines[14][1]
// 15     ||  10  -> 11     ||  3000      || lines[15][0] //  lines[15][1]
// 16     ||  11  -> 24     ||  1000      || lines[16][0] //  lines[16][1]
// 17     ||  32/37  -> 12  ||  4000      || lines[17][0] //  lines[17][1]
// 18     ||  12  -> 31/38  ||  500       || lines[18][0] //  lines[18][1]
// 19     ||  31/38  -> 13  ||  2500      || lines[19][0] //  lines[19][1]
// 20     ||  31/38  -> 14  ||  3000      || lines[21][0] //  lines[20][1]
// 21     ||  13  -> 22     ||  1000      || lines[20][0] //  lines[21][1]
// 22     ||  14  -> 23     ||  1000      || lines[22][0] //  lines[22][1]

void createLinesSemaphores() {
  for (int i = 0; i < 23; i++) {
    if ( lines[i][0] == NULL ) {
      lines[i][0] = xSemaphoreCreateMutex();
      if ( ( lines[i][0] ) != NULL )
        xSemaphoreGive( ( lines[i][0] ) );
    }
    if ( lines[i][1] == NULL ) {
      lines[i][1] = xSemaphoreCreateMutex();
      if ( ( lines[i][1] ) != NULL )
        xSemaphoreGive( ( lines[i][1] ) );
    }
  }
  linesDelay[0] = 500;
  linesDelay[1] = 1500;
  linesDelay[2] = 1750;
  linesDelay[3] = 250;
  linesDelay[4] = 1250;
  linesDelay[5] = 250;
  linesDelay[6] = 1250;
  linesDelay[7] = 250;
  linesDelay[8] = 1750;
  linesDelay[9] = 1500;
  linesDelay[10] = 2000;
  linesDelay[11] = 500;
  linesDelay[12] = 1500;
  linesDelay[13] = 500;
  linesDelay[14] = 1750;
  linesDelay[15] = 1500;
  linesDelay[16] = 500;
  linesDelay[17] = 2000;
  linesDelay[18] = 250;
  linesDelay[19] = 1250;
  linesDelay[20] = 500;
  linesDelay[21] = 1500;
  linesDelay[22] = 500;

  // Serial.println(F("Lines Semaphores is Created"));
}
