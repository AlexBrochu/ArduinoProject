#include <Arduino.h>

const int greenLEDPin = 9;
const int redLEDPin = 10;
const int blueLEDPin = 11;

int greenLEDValue = 0;
int redLEDValue = 0;
int blueLEDValue = 0;

const int sensorPin = A0;
const float baselineTemp =19.0;

void setup() {
    Serial.begin(9600);

    for(int pinNumber = 2; pinNumber <5; pinNumber++){
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, HIGH);
    }
    pinMode(greenLEDPin, OUTPUT);
    pinMode(redLEDValue, OUTPUT);
    pinMode(blueLEDValue, OUTPUT);
    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
    int sensorVal = analogRead(sensorPin);
    Serial.print("Sensor Value: ");
    Serial.print(sensorVal);

    float voltage = (sensorVal/1024.0) *5.0;
    Serial.print(", Volt: ");
    Serial.print(voltage);

    Serial.print(", degrees C: ");
    float temperature = (voltage - 0.5) *100;
    Serial.println(temperature);

    if(temperature < baselineTemp){
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
    } else if(temperature >= baselineTemp && temperature < baselineTemp+2){
        digitalWrite(2, HIGH);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
    } else if(temperature >= baselineTemp + 2 && temperature < baselineTemp+4){
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
    }else if(temperature >= baselineTemp + 4){
        digitalWrite(2, HIGH);
        digitalWrite(3, HIGH);
        digitalWrite(4, HIGH);
    }
    delay(1);

    greenLEDValue = rand() % 256;
    redLEDValue = rand() % 256;
    blueLEDValue = rand() % 256;

    Serial.print("Green : ");
    Serial.print(greenLEDValue);
    Serial.print(", Red : ");
    Serial.print(redLEDValue);
    Serial.print(", Blue : ");
    Serial.println(blueLEDValue);

    analogWrite(greenLEDPin, greenLEDValue);
    analogWrite(redLEDPin, redLEDValue);
    analogWrite(blueLEDPin, blueLEDValue);

    delay(500);
}
