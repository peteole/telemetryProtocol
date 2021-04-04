#include "DataSchema.h"
String BasicSensorValue::toJSON()
{
    return "{\"name\": \"" + this->name + "\", \"size\": " + this->size + ", \"type\": \"" + this->type + "\"}";
}
char *SensorValueList::getMessage()
{
    uint16_t position = 0;
    for (int i = 0; i < this->length; i++)
    {
        char *partialMessage = this->sensorvalues[i]->getMessage();
        for (int j = 0; j < this->sensorvalues[i]->size; j++)
        {
            this->messageBuffer[position] = partialMessage[j];
            position++;
        }
    }
    return this->messageBuffer;
}

void SensorValueList::parse(char *data)
{
    uint16_t position = 0;
    for (int i = 0; i < this->length; i++)
    {
        this->sensorvalues[i]->parse(data + position);
        position += this->sensorvalues[i]->size;
    }
}
String SensorValueList::toJSON()
{
    String JSON = "{\"name\": \"" + this->name + "\",\n \"size\": " + this->size + ", \"values\": [\n";
    for (int i = 0; i < this->length; i++)
    {
        JSON += this->sensorvalues[i]->toJSON() + (i + 1 < this->length ? ",\n" : "\n");
    }
    JSON += "]\n}";
    return JSON;
}

SensorValueList::SensorValueList(SensorValue **sensorvalues, uint16_t length, String name)
{
    this->name = name;
    this->sensorvalues = sensorvalues;
    this->length = length;
    this->size = 0;
    for (int i = 0; i < this->length; i++)
    {
        this->size += this->sensorvalues[i]->size;
    }
    this->messageBuffer = new char[this->size];
}

StringSensorValue::StringSensorValue(String name, int maxSize)
{
    this->size = maxSize;
    this->name = name;
}
void StringSensorValue::parse(char *message)
{
    this->value = String(message);
    if (this->value.length() > this->size)
    {
        // message too long
        this->value = "";
    }
}
char *StringSensorValue::getMessage()
{
    if (this->value.length() > this->size)
    {
        this->value = "";
    }
    return this->value.begin();
}
String StringSensorValue::toJSON()
{
    return "{\"name\": " + this->name + ", \"size\":" + this->size + ",\"type\": \"string\"}";
}