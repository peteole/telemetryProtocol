#ifndef DATSCHEMA_H
#define DATASCHEMA_H
#include "Arduino.h"
struct SensorValue
{
    String name;
    uint16_t size;

public:
    virtual String toJSON()=0;
};
struct BasicSensorValue : public SensorValue
{
    String type;
    String toJSON() override;
};
struct SensorValueList : public SensorValue
{
public:
    SensorValue *Sensorvalues;
    uint16_t length;
    String toJSON() override;
};


#endif