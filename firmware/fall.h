#ifndef FALL_H
#define FALL_H

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>

class FallDetector {
  public:
    FallDetector() {
      filteredAcc = 1.0;
      alpha = 0.7;
      freeFall = false;
      impactDetected = false;
      fallDetected = false;
    }

    void begin() {
      Wire.begin();
      mpu.initialize();
    }

    void update() {
      int16_t rawAx, rawAy, rawAz;
      mpu.getAcceleration(&rawAx, &rawAy, &rawAz);

      // Convert to g
      ax = rawAx / 16384.0;
      ay = rawAy / 16384.0;
      az = rawAz / 16384.0;

      float acc = sqrt(ax * ax + ay * ay + az * az);

      // Low-pass filter
      filteredAcc = alpha * filteredAcc + (1 - alpha) * acc;

      unsigned long now = millis();

      // 1. Free fall
      if (filteredAcc < 0.7 && !freeFall) {
        freeFall = true;
        freeFallTime = now;
      }

      // 2. Impact
      if (freeFall && filteredAcc > 1.3 && (now - freeFallTime < 1000)) {
        impactDetected = true;
        impactTime = now;
      }

      // 3. Orientation
      float angle = atan2(ax, sqrt(ay * ay + az * az)) * 180 / PI;

      // 4. Confirm fall
      if (impactDetected && abs(angle) > 30 && (now - impactTime < 1000)) {
        fallDetected = true;

        freeFall = false;
        impactDetected = false;
      }

      // Reset if timeout
      if (freeFall && (now - freeFallTime > 1500)) {
        freeFall = false;
        impactDetected = false;
      }
    }

    bool isFallDetected() {
      if (fallDetected) {
        fallDetected = false;
        return true;
      }
      return false;
    }

    float getAcceleration() {
      return filteredAcc;
    }

    float getAngle() {
      return atan2(ax, sqrt(ay * ay + az * az)) * 180 / PI;
    }

  private:
    MPU6050 mpu;

    float ax, ay, az;
    float filteredAcc;
    float alpha;

    bool freeFall;
    bool impactDetected;
    bool fallDetected;

    unsigned long freeFallTime;
    unsigned long impactTime;
};

#endif
//example of main:
/*
   #include "fall.h"

FallDetector fall;

void setup() {
  Serial.begin(9600);
  fall.begin();
}

void loop() {
  fall.update();

  Serial.print("Acc: ");
  Serial.print(fall.getAcceleration());
  Serial.print(" | Angle: ");
  Serial.println(fall.getAngle());

  if (fall.isFallDetected()) {
    Serial.println("🚨 FALL DETECTED!");
    delay(2000);
  }

  delay(20);
}
*/