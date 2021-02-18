#include "DataSchema.h"
String BasicSensorValue::toJSON()
{
    return "{name:" + this->name + ", size: " + this->size + ", type: " + this->type;
}
char *SensorValueList::getMessage()
{
}
String SensorValueList::toJSON()
{
    String JSON = "{name:" + this->name + ", size: " + this->size + ", values:[\n";
    for (int i = 0; i < this->length; i++)
    {
        JSON += this->sensorvalues[i].toJSON() + ",\n";
    }
    JSON += "]\n}";
    return JSON;
}

SensorValueList::SensorValueList(SensorValue *sensorvalues, uint16_t length)
{
    this->sensorvalues = sensorvalues;
    this->length = length;
    this->size = 0;
    for (int i = 0; i < this->length; i++)
    {
        this->size += this->sensorvalues[i].size;
    }
    this->messageBuffer = new char[this->size];
}