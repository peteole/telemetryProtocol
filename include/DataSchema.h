#ifndef DATSCHEMA_H
#define DATASCHEMA_H
#include "Arduino.h"
struct SensorValue
{
    String name;
    uint16_t size;

public:
    virtual String toJSON() = 0;
    virtual char *getMessage() = 0;
};
struct BasicSensorValue : public SensorValue
{
    String type;
    String toJSON() override;
};

struct SensorValueList : public SensorValue
{
private:
    char *messageBuffer;

public:
    SensorValue **sensorvalues;
    uint16_t length;
    String toJSON() override;
    char *getMessage() override;
    SensorValueList(SensorValue **sensorvalues, uint16_t length);
};

// A message type which allows setting sensor values and converting them to a sendable format
class Message
{
public:
    // uinique ID for this kind of message. Is sent at the beginning of each message.
    uint8_t id;
    SensorValue *value;
    Message(SensorValue *value, uint8_t id);
    void send(Stream *medium);
};

template <typename t, const char *type>
struct BasicSensorValueTemplate : public BasicSensorValue
{
    t value;
    char *messageBuffer;

public:
    BasicSensorValueTemplate(String name);
    char *getMessage();
};

extern const char intText[] = "int";
typedef BasicSensorValueTemplate<int, intText> IntSensorValue;

extern const char floatText[] = "float";
typedef BasicSensorValueTemplate<float, floatText> FloatSensorValue;

extern const char stringText[] = "String";
typedef BasicSensorValueTemplate<String, stringText> StringSensorValue;
#endif