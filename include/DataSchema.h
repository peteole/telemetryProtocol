#ifndef DATSCHEMA_H
#define DATASCHEMA_H
#include "Arduino.h"
struct SensorValue
{
    // This must be unique for this sensor value
    String name;
    uint16_t size;

public:
    virtual String toJSON() = 0;
    virtual char *getMessage() = 0;
    virtual void parse(char *message) = 0;
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
    void parse(char *message) override;
    SensorValueList(SensorValue **sensorvalues, uint16_t length);
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

    void parse(char *message) override
    {
        this->value = *reinterpret_cast<t *>(message);
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