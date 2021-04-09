#include "Message.h"

void Message::send(Stream *medium)
{
    //mark start of message
    medium->write((uint8_t)0);
    medium->write(this->id);

    char *message = this->value->getMessage();
    for (int i = 0; i < this->value->size; i++)
    {
        if (message[i] == 0)
        {
            //mark this is a normal zero
            medium->write((uint8_t)0);
        }
        medium->write(message[i]);
    }

    //mark end of message
    medium->write((uint8_t)0);
    medium->write(1);
}
Message::Message(SensorValue *value, uint8_t id)
{
    while (id <= 1 || id >= 255)
    {
        // id must be >2
    }
    this->value = value;
    this->id = id;
}

bool Message::parse(char *toParse)
{
    uint8_t messageID = *reinterpret_cast<uint8_t *>(toParse);
    if (this->id != messageID)
    {
        return false;
    }
    this->value->parse(toParse);
    return true;
}
void Message::sendDescription(Stream *medium)
{
    //mark start of description
    medium->write((uint8_t)0);
    medium->write(255);

    // write id and description
    medium->write(this->id);
    String json = this->value->toJSON();
    for (int i = 0; i < json.length(); i++)
    {
        char toPrint = json.charAt(i);
        if (toPrint == '\0')
        {
            medium->write((uint8_t)0);
        }
        medium->write(toPrint);
    }

    //mark end of message
    medium->write((uint8_t)0);
    medium->write(1);
}