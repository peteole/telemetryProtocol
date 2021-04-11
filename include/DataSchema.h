#ifndef DATASCHEMA_H
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
    SensorValueList(SensorValue **sensorvalues, uint16_t length, String name = "list");
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

template <typename intType>
class IntSensorValueTemplate : public BasicSensorValueTemplate<intType>
{
public:
    IntSensorValueTemplate(String name) : BasicSensorValueTemplate("int", name) {}
};

template <typename uintType>
class UIntSensorValueTemplate : public BasicSensorValueTemplate<uintType>
{
public:
    UIntSensorValueTemplate(String name) : BasicSensorValueTemplate("uint", name) {}
};

template <typename floatType>
class FloatSensorValueTemplate : public BasicSensorValueTemplate<floatType>
{
public:
    FloatSensorValueTemplate(String name) : BasicSensorValueTemplate("float", name) {}
};
typedef IntSensorValueTemplate<int> IntSensorValue;
typedef IntSensorValueTemplate<int32_t> Int32SensorValue;
typedef IntSensorValueTemplate<int16_t> Int16SensorValue;
typedef IntSensorValueTemplate<int8_t> Int8SensorValue;

typedef UIntSensorValueTemplate<uint> UIntSensorValue;
typedef UIntSensorValueTemplate<uint32_t> UInt32SensorValue;
typedef UIntSensorValueTemplate<uint16_t> UInt16SensorValue;
typedef UIntSensorValueTemplate<uint8_t> UInt8SensorValue;

typedef FloatSensorValueTemplate<float> FloatSensorValue;
typedef FloatSensorValueTemplate<double_t> DoubleSensorValue;

class StringSensorValue : public SensorValue
{
public:
    StringSensorValue(String name, int maxSize);
    String toJSON();
    char *getMessage();
    void parse(char *message);
    String value;

private:
    String name;
};

#endif