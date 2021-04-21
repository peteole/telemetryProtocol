# Telemetry Protocol

This is a library for arduino/platformio to enable extremely efficient serial communication of sensor values while still being convenient to use.

## Concept
You want to avoid sending binary data like numbers in a text format for efficiency reasons. However it is annoying to just send floats in a binary format, or at least to read them on the other side.
The basic concept of this library is to once define a message dataschema, send that definition and then only send binary data encoded in that dataschema.

To do so, the following classes are used:

- SensorValue: A value which may change over time. It has a name, a current value, a method to encode that value to a binary array and a method to update its value by parsing a binary array ending its new value.
- Basic sensorvalue: A sensorvalue directly associated to a physical sensorvalue
- List sensorvalue: A sensorvalue which is composed of a list of other sensorvalues. For example you may want to compose a list sensorvalue of the GPS longitude, lattitude, speed and time in a list sensorvalue.
- Message: A message has a single sensorvalue (which may be a list sensorvalue) and is able to encode and send that sensorvalue over a medium. It can parse an encoded incoming new value for that sensorvalue as well. A message has a unique id which is used to identify which type of message is incoming when new pieces of data arrive. When sending a message, the sensorvalue is encoded. However, a start and end byte as well as the idea must be injected into the message.
- MessageRegistry: This class handles all the messages involved in a program. You can feed it input data and it will decide which of the messages it handles must consume the input.

## String messages

For convenience each registry is initialized with a special message called its "stream". It is a special type of message which can send string messages like the arduino Serial object.
## Example

``` C++
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
```

## Client

The other side must be able to parse and send messages. This must currently be done using the javascript/typescript client:
https://github.com/peteole/telemetryProtocolClient