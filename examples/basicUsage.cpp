#include "DataSchema.h"
#include <Arduino.h>
FloatSensorValue altitude("altitude");
IntSensorValue heading("heading");
SensorValue *list[] = {&altitude, &heading};
SensorValueList package(list, 2);
Message message(&package, 17);

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    altitude.value++;
    heading.value++;
    heading.value %= 360;
    message.send(Serial);
}