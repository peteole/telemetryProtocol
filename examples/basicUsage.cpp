#include "DataSchema.h"
#include "Message.h"
#include "MessageRegistry.h"
#include <Arduino.h>
FloatSensorValue altitude("altitude");
IntSensorValue heading("heading");
SensorValue *list[] = {&altitude, &heading};
SensorValueList package(list, 2);
Message message(&package, 17);
MessageRegistry registry;
void setup()
{
    Serial.begin(9600);
    registry.addMessage(&message);
    registry.sendDataSchema(&Serial);
}

void loop()
{
    altitude.value++;
    heading.value++;
    heading.value %= 360;
    message.send(&Serial);
}