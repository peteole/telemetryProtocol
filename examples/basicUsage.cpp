#include <Arduino.h>
#include "DataSchema.h"
#include "Message.h"
#include "MessageRegistry.h"

//construct basic sensorvalues
FloatSensorValue altitude("altitude");
IntSensorValue heading("heading");

//construct composed sensor value with length 2
SensorValue *list[] = {&altitude, &heading};
SensorValueList package(list, 2);

//construct message from composed sensorvalue
Message message(&package, 17);
MessageRegistry registry;
void setup()
{
    Serial.begin(9600);
    registry.addMessage(&message);
    delay(1000);
    registry.sendDataSchema(&Serial);
}

void loop()
{
    delay(1000);
    //modify fake sensor values
    altitude.value++;
    heading.value++;
    heading.value %= 360;
    //resend message with new values
    message.send(&Serial);

    //send string message
    registry.stream->addMessage("Hello world from new library!");
    registry.stream->send(&Serial);
    // listen for incoming string messages
    registry.readDataInStream(&Serial);
    while (registry.stream->available())
    {
        //repeat them and send them back
        registry.stream->addMessage("revieved: " + registry.stream->readMessage());
    }
}