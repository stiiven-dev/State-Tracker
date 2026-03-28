#include <SD.h>  

// Define the data structure for storing health and location data  
struct HealthData {  
    int heartRate;  
    int spO2;  
    float longitude;  
    float latitude;  
    String status;  
};  

// Function to store health data to SD card  
void storeHealthData(HealthData data) {  
    File dataFile = SD.open("health_data.txt", FILE_WRITE);  
    if (dataFile) {  
        // Write the data to the file  
        dataFile.print("Heart Rate: ");  
        dataFile.println(data.heartRate);  
        dataFile.print("SpO2: ");  
        dataFile.println(data.spO2);  
        dataFile.print("Longitude: ");  
        dataFile.println(data.longitude);  
        dataFile.print("Latitude: ");  
        dataFile.println(data.latitude);  
        dataFile.print("Status: ");  
        dataFile.println(data.status);  
        dataFile.close();  
        Serial.println("Data written to health_data.txt");  
    } else {  
        Serial.println("Failed to open health_data.txt");  
    }  
}  
