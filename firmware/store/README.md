# Store Module

## Overview
The Store module is responsible for storing health and location data from the State-Tracker wearable device to an SD card. It captures vital signs (heart rate and blood oxygen saturation) along with GPS coordinates and system status information.

## Features
- **Data Logging**: Store vital signs and location data persistently on SD card
- **Health Monitoring**: Capture heart rate and SpO2 measurements
- **GPS Integration**: Record longitude and latitude coordinates
- **Status Tracking**: Log system status and alert information
- **CSV Format**: Data stored in easily readable format

## Files
- `store.h` - Header file containing function declarations

## Function Reference

### storeData()
Stores health data and location information to the SD card.

