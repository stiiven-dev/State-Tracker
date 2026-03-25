#ifndef HEART_H
#define HEART_H

#include <Arduino.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"

// =========================
// Global sensor object
// =========================
MAX30102 heartSensor;

// =========================
// Config
// =========================
const int HEART_LED_BRIGHTNESS = 60;
const int HEART_SAMPLE_AVERAGE = 4;
const int HEART_LED_MODE = 2;      // Red + IR
const int HEART_SAMPLE_RATE = 100;
const int HEART_PULSE_WIDTH = 411;
const int HEART_ADC_RANGE = 4096;

const long FINGER_THRESHOLD = 5000;

int MIN_HEART_RATE = 50;
int MAX_HEART_RATE = 120;
int MIN_SPO2 = 92;

unsigned long HEART_SEND_INTERVAL = 5000;

// =========================
// State variables
// =========================
int currentHeartRate = -1;
int currentSpO2 = -1;

bool heartRateValid = false;
bool spo2Valid = false;
bool fingerDetected = false;

long lastIR = 0;
long lastRed = 0;

unsigned long lastHeartSendTime = 0;
unsigned long lastHeartComputeTime = 0;

// Beat detection
byte rates[4];
byte rateSpot = 0;
long lastBeat = 0;
int beatAverage = 0;

// SpO2 buffers
const int HEART_BUFFER_SIZE = 100;
uint32_t irBuffer[HEART_BUFFER_SIZE];
uint32_t redBuffer[HEART_BUFFER_SIZE];
int heartBufferIndex = 0;
bool heartBufferReady = false;

// =========================
// Function declarations
// =========================
bool heart_begin(TwoWire &wirePort = Wire);
void heart_update(bool isOnline);
void heart_store_sample(uint32_t red, uint32_t ir);
void heart_detect_rate(long irValue);
void heart_compute_spo2();
void heart_handle_logic(bool isOnline);
void heart_send_to_cloud(int hr, int spo2);
void heart_alert(String message);
void heart_reset_buffers();

// =========================
// Function definitions
// =========================

bool heart_begin(TwoWire &wirePort) {
    if (!heartSensor.begin(wirePort, I2C_SPEED_FAST)) {
        return false;
    }

    heartSensor.setup(
        HEART_LED_BRIGHTNESS,
        HEART_SAMPLE_AVERAGE,
        HEART_LED_MODE,
        HEART_SAMPLE_RATE,
        HEART_PULSE_WIDTH,
        HEART_ADC_RANGE
    );

    heartSensor.setPulseAmplitudeRed(HEART_LED_BRIGHTNESS);
    heartSensor.setPulseAmplitudeIR(HEART_LED_BRIGHTNESS);

    heart_reset_buffers();

    currentHeartRate = -1;
    currentSpO2 = -1;
    heartRateValid = false;
    spo2Valid = false;
    fingerDetected = false;
    lastIR = 0;
    lastRed = 0;
    lastHeartSendTime = 0;
    lastHeartComputeTime = 0;
    rateSpot = 0;
    lastBeat = 0;
    beatAverage = 0;

    for (int i = 0; i < 4; i++) {
        rates[i] = 90;  // Start with neutral ~90 BPM
    }

    return true;
}

void heart_update(bool isOnline) {
    heartSensor.check();

    while (heartSensor.available()) {
        lastIR = heartSensor.getIR();
        lastRed = heartSensor.getRed();

        fingerDetected = (lastIR > FINGER_THRESHOLD);

        if (fingerDetected) {
            heart_detect_rate(lastIR);
            heart_store_sample(lastRed, lastIR);
        } else {
            currentHeartRate = -1;
            currentSpO2 = -1;
            heartRateValid = false;
            spo2Valid = false;
        }

        heartSensor.nextSample();
    }

    if (millis() - lastHeartComputeTime >= 1000) {
        lastHeartComputeTime = millis();

        if (fingerDetected) {
            heart_compute_spo2();
            heart_handle_logic(isOnline);
        } else {
            heart_alert("No finger detected");
        }
    }
}

void heart_store_sample(uint32_t red, uint32_t ir) {
    redBuffer[heartBufferIndex] = red;
    irBuffer[heartBufferIndex] = ir;

    heartBufferIndex++;

    if (heartBufferIndex >= HEART_BUFFER_SIZE) {
        heartBufferIndex = 0;
        heartBufferReady = true;
    }
}

void heart_detect_rate(long irValue) {
    if (checkForBeat(irValue)) {
        long delta = millis() - lastBeat;
        lastBeat = millis();

        float bpm = 60.0 / (delta / 1000.0);

        if (bpm > 20 && bpm < 255 && delta > 200) {  // Reject if beat too fast
            rates[rateSpot] = (byte)bpm;
            rateSpot++;
            rateSpot %= 4;

            int sum = 0;
            for (int i = 0; i < 4; i++) {
                sum += rates[i];
            }

            beatAverage = sum / 4;
            currentHeartRate = beatAverage;
            heartRateValid = true;
        }
    }
}

void heart_compute_spo2() {
    if (!heartBufferReady) return;

    int32_t spo2Value;
    int8_t spo2IsValid;
    int32_t heartRateValue;
    int8_t hrIsValid;

    maxim_heart_rate_and_oxygen_saturation(
        irBuffer,
        HEART_BUFFER_SIZE,
        redBuffer,
        &spo2Value,
        &spo2IsValid,
        &heartRateValue,
        &hrIsValid
    );

    if (spo2IsValid) {
        currentSpO2 = spo2Value;
        spo2Valid = true;
    } else {
        currentSpO2 = -1;
        spo2Valid = false;
    }

    if (hrIsValid) {
        currentHeartRate = heartRateValue;
        heartRateValid = true;
    }
}

void heart_handle_logic(bool isOnline) {
    if (isOnline) {
        if (millis() - lastHeartSendTime >= HEART_SEND_INTERVAL) {
            lastHeartSendTime = millis();
            heart_send_to_cloud(currentHeartRate, currentSpO2);
        }
    } else {
        if (spo2Valid && currentSpO2 < MIN_SPO2) {
            heart_alert("Low SpO2 detected");
        }

        if (heartRateValid && currentHeartRate < MIN_HEART_RATE) {
            heart_alert("Heart rate too low");
        }

        if (heartRateValid && currentHeartRate > MAX_HEART_RATE) {
            heart_alert("Heart rate too high");
        }
    }
}

void heart_send_to_cloud(int hr, int spo2) {
    if (hr < 0 || spo2 < 0) {
        Serial.println("Invalid readings - skipping cloud send");
        return;
    }
    Serial.println("Sending heart data to cloud...");
    Serial.print("Heart Rate: ");
    Serial.println(hr);
    Serial.print("SpO2: ");
    Serial.println(spo2);

    // Put your HTTP or MQTT code here
}

void heart_alert(String message) {
    Serial.print("ALERT: ");
    Serial.println(message);

    // Put buzzer / LED / notification code here
}

void heart_reset_buffers() {
    for (int i = 0; i < HEART_BUFFER_SIZE; i++) {
        irBuffer[i] = 0;
        redBuffer[i] = 0;
    }

    heartBufferIndex = 0;
    heartBufferReady = false;
}

#endif