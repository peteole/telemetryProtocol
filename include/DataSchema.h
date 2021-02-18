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

template <typename t>
struct BasicSensorValueTemplate : public BasicSensorValue
{
public:
    t value;
    char *messageBuffer;

    BasicSensorValueTemplate(String type, String name)
    {
        this->type = type;
        this->size = sizeof(t);
        this->messageBuffer = new char[this->size];
        this->name = name;
    }
    char *getMessage()
    {
        return reinterpret_cast<char *>(&(this->value));
    }
};

class IntSensorValue : public BasicSensorValueTemplate<int>
{
public:
    IntSensorValue(String name) : BasicSensorValueTemplate("int", name) {}
};

class FloatSensorValue : public BasicSensorValueTemplate<float>
{
public:
    FloatSensorValue(String name) : BasicSensorValueTemplate("float", name) {}
};

class StringSensorValue : public BasicSensorValueTemplate<String>
{
public:
    StringSensorValue(String name) : BasicSensorValueTemplate("string", name) {}
};

#endif