#ifndef STREAM_MESSAGE_H
#define STREAM_MESSAGE_H
#include "Message.h"
#include "DataSchema.h"
class StreamSensorValue : public SensorValue
{
public:
    StreamSensorValue()
    {
        this->name = "stream";
    }
    char *getMessage() { return 0; }
    String toJSON() { return "{"; }
    void parse(char *toParse) {}
};
struct MessageNode
{
    String value;
    MessageNode *next;
    MessageNode(String value, MessageNode *next) : value(value), next(next) {}
};
class StreamMessage : public Message
{
private:
    MessageNode *oldestInboundMessage;
    MessageNode *oldestOutboundMessage;

public:
    StreamMessage(int id);
    bool addMessage(String message);
    bool pushToCurrentMessage(String toAppend);
    void send(Stream *medium) override;
    int available();
    String peek();
    String readMessage
    bool parse(char *message) override;
};
#endif