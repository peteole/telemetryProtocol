#include "DataSchema.h"
String BasicSensorValue::toJSON()
{
    return "{name:" + this->name + ", size: " + this->size + ", type: " + this->type;
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
String SensorValueList::toJSON()
{
    String JSON = "{name:" + this->name + ", size: " + this->size + ", values:[\n";
    for (int i = 0; i < this->length; i++)
    {
        JSON += this->sensorvalues[i]->toJSON() + ",\n";
    }
    JSON += "]\n}";
    return JSON;
}

SensorValueList::SensorValueList(SensorValue **sensorvalues, uint16_t length)
{
    this->sensorvalues = sensorvalues;
    this->length = length;
    this->size = 0;
    for (int i = 0; i < this->length; i++)
    {
        this->size += this->sensorvalues[i]->size;
    }
    this->messageBuffer = new char[this->size];
}

// template <typename t, const char *type>
// BasicSensorValueTemplate<t, type>::BasicSensorValueTemplate(String name)
// {
//     this->type = String(type);
//     this->size = sizeof(t);
//     this->messageBuffer = new char[this->size];
//     this->name = name;
// }

// template <typename t, const char *type>
// char *BasicSensorValueTemplate<t, type>::getMessage()
// {
//     return reinterpret_cast<char *>(&(this->value));
// }

void Message::send(Stream *medium)
{
    //mark start of message
    medium->write("\0");
    medium->write(1);

    char *message = this->value->getMessage();
    for (int i = 0; i < this->value->size; i++)
    {
        if (message[i] == '\0')
        {
            //mark this is a normal zero
            medium->write("\0");
        }
        medium->write(message[i]);
    }

    //mark end of message
    medium->write("\0");
    medium->write(2);
}
Message::Message(SensorValue *value, uint8_t id){
    this->value=value;
    this->id=id;
}
