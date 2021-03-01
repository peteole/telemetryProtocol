#ifndef MESSAGE_H
#define MESSAGE_H
#include "DataSchema.h"
// A message type which allows setting sensor values and converting them to a sendable format
class Message
{
public:
    // uinique ID for this kind of message. Is sent at the beginning of each message. Must be >1
    uint8_t id;
    SensorValue *value;
    // id must be >1
    Message(SensorValue *value, uint8_t id);
    void send(Stream *medium);
    //parses the message and return if it was successful
    bool parse(char *toParse);
};
#endif