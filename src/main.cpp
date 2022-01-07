#include <MQ131-V2.h>

void setup() {
  Serial.begin(115200);

  // Init the sensor
  // - Heater control on pin 2
  // - Sensor analog read on pin A0
  // - Model LOW_CONCENTRATION
  // - Load resistance RL of 1MOhms (1000000 Ohms)
  MQ131.begin(2,A0, LOW_CONCENTRATION, 1000000);  

  Serial.println("Calibration in progress...");
  
  MQ131.calibrate();
  
  Serial.println("Calibration done!");
  Serial.print("R0 = ");
  Serial.print(MQ131.getR0());
  Serial.println(" Ohms");
  Serial.print("Time to heat = ");
  Serial.print(MQ131.getTimeToRead());
  Serial.println(" s");
}

#define PRINT_INTERVAL_MS 1000
unsigned long lastPrintTime = 0; 
float lastOzoneValue = 0;

void print_current_value() {
    Serial.print(lastOzoneValue);
    Serial.println(" ppm");   
}

/* function called by the MQ131-V2 library when we are twiddling our thumbs */
void sensor_waiting_func() {
    // check if 1 second has passed and we need to print it again
    //this is the blink without delay concept, non-blocking check.
    if(millis() - lastPrintTime >= PRINT_INTERVAL_MS) {
        print_current_value();
        lastPrintTime = millis();
    }
}

void loop() {
  Serial.println("Sampling...");
  // request a sample, but while we are waiting, print the last known value 
  // in one second intervals
  MQ131.sample(&sensor_waiting_func);
  lastOzoneValue = MQ131.getO3(PPM);
}