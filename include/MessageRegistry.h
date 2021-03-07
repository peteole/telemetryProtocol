#ifndef MESSAGE_REGISTRY_H
#define MESSAGE_REGISTRY_H
#include "Message.h"
#include <Arduino.h>
class MessageRegistry
{
public:
    void addMessage(Message *toAdd);
    void readDataInStream(Stream *stream);
    void sendDataSchema(Stream* stream);
private:
    Message *currentMessage = NULL;
    Message **messages;
    int length = 0;
    char *buffer=NULL;
    int maxMessageLength = 0;
    uint16_t currentPosition;
    bool previousByteZero = false;
    void parseCurrentMessage();
};
#endif