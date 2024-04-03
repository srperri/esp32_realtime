#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
//para el malloc
#include <stdlib.h>
#include <malloc.h>
//para el bzero
#include <string.h>
//para el write
#include<unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#define HEADER_SIZE 16


void error(char *msg)
{
    Serial.println(msg);
    exit(1);
}

/* Time in seconds from some point in the past */
double dwalltime();

static int n = 60;



void setup()
{
  int i;
  Serial.begin(115200); 
  Serial.println("setup inicia");
  srand(time(NULL));



  Serial.println("setup");
  xTaskCreatePinnedToCore (
        task1,     // Function to implement the task
        "task1",   // Name of the task
        10000,      // Stack size in bytes
        NULL,      // Task input parameter
        10,         // Priority of the task
        NULL,      // Task handle.
        0          // Core where the task should run
      );
  
  xTaskCreatePinnedToCore (
        task2,     // Function to implement the task
        "task2",   // Name of the task
        10000,      // Stack size in bytes
        NULL,      // Task input parameter
        10,         // Priority of the task
        NULL,      // Task handle.
        1          // Core where the task should run
      );
  delay(500);
}


//el medidor de tiempo
double dwalltime() {
  double sec;
  struct timeval tv;

  gettimeofday( & tv, NULL);
  sec = tv.tv_sec + tv.tv_usec / 1000000.0;
  return sec;
}

void loop() {
  vTaskDelay(1);
}
      
void task1(void* parameter) {

  /*El procesamiento en si*/
  int i, j, k, h;
  char midS[8];
  char promedioS[20];
  char tiempo[12];
  double totalTime,promedio;
  double workTime2,timetick2;
  
  
  Serial.println("task1 inicia...");
  float *A, *B, *C;
  /* Getting memory */
  A = (float * ) malloc(n*n*sizeof(float));
  B = (float * ) malloc(n*n*sizeof(float));
  C = (float * ) malloc(n*n* sizeof(float));
  /* Initialization */
  for (i = 0; i < n*n; i++) {
    A[i] = (float)rand()/(float)(RAND_MAX/10)+1;
    B[i] = (float)rand()/(float)(RAND_MAX/10)+1;
  }
  for(;;){
      totalTime=0;
      for(h=1;h<=20;h++){
        timetick2 = dwalltime();
          for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
              C[i*n+j]=0;
              for (k = 0; k < n; k++) {
                C[i*n+j] += A[i*n+k] * B[j*n+k];
              }
            }
          }
        workTime2 = dwalltime() - timetick2;
        totalTime+=workTime2;
      }

    promedio=totalTime/20;
    sprintf(promedioS,"Avg1 20: %0.4lf",promedio*1000);
    //Serial.println(promedioS);
    vTaskDelay(1);
  };
}

void task2(void* parameter) {
  int i, j, k, h;
  char midS[8];
  char promedioS[20];
  char tiempo[12];
  double totalTime,promedio;
  double workTime2,timetick2;
  
  float *D, *E, *F;
  /* Getting memory */
  D = (float * ) malloc(n*n*sizeof(float));
  E = (float * ) malloc(n*n*sizeof(float));
  F = (float * ) malloc(n*n*sizeof(float));
  /* Initialization */
  for (i = 0; i < n*n; i++) {
    D[i] = (float)rand()/(float)(RAND_MAX/10)+1;
    E[i] = (float)rand()/(float)(RAND_MAX/10)+1;
  }
  
  Serial.println("task2 inicia...");
  for(;;){
      totalTime=0;
      for(h=1;h<=20;h++){
        timetick2 = dwalltime();
          for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
              F[i*n+j]=0;
              for (k = 0; k < n; k++) {
                F[i*n+j] += D[i*n+k] * E[j*n+k];
              }
            }
          }
        workTime2 = dwalltime() - timetick2;
        totalTime+=workTime2;
      }

    promedio=totalTime/20;
    sprintf(promedioS,"Avg2 20: %0.4lf",promedio*1000);
    Serial.println(promedioS);
    vTaskDelay(1);
  };
}
