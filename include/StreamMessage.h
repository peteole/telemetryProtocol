#ifndef STREAM_MESSAGE_H
#define STREAM_MESSAGE_H
#include "Message.h"
#include "DataSchema.h"

// Attention, does not yet reflect in json!
#define MAX_MESSAGE_LENGTH 500
class StreamSensorValue : public SensorValue
{
public:
    StreamSensorValue()
    {
        this->name = "stream";
        this->size = MAX_MESSAGE_LENGTH;
    }
    char *getMessage() { return 0; }
    String toJSON() { return "{\"type\":\"stream\",\"name\":\"stream\",\"size\": 500}"; }
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
    ~StreamMessage();
    void addMessage(String message);
    void pushToCurrentMessage(String toAppend);
    void send(Stream *medium) override;
    int available();
    String peek();
    String readMessage();
    bool parse(char *message) override;
};
#endif