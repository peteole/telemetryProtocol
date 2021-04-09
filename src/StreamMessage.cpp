#include "StreamMessage.h"
MessageNode *getLatestMessageNode(MessageNode *start)
{
    if (start == NULL)
        return NULL;
    while (start->next != NULL)
        start = start->next;
    return start;
}
StreamMessage::StreamMessage(int id) : Message(new StreamSensorValue(), id)
{
    this->oldestInboundMessage = NULL;
    this->oldestOutboundMessage = NULL;
}

void StreamMessage::addMessage(String message)
{
    MessageNode *newMessageNode = new MessageNode(message, NULL);
    if (this->oldestOutboundMessage == NULL)
    {
        this->oldestOutboundMessage = newMessageNode;
    }
    else
    {
        MessageNode *current = oldestOutboundMessage;
        while (current->next != NULL)
            current = current->next;
        current->next = newMessageNode;
    }
}
void StreamMessage::pushToCurrentMessage(String toAppend)
{
    if (this->oldestOutboundMessage == NULL)
        this->oldestOutboundMessage = new MessageNode("", NULL);
    getLatestMessageNode(this->oldestOutboundMessage)->value += toAppend;
}
void StreamMessage::send(Stream *medium)
{
    while (this->oldestOutboundMessage != NULL)
    {

        //mark start of message
        medium->write((uint8_t)0);
        medium->write(this->id);
        // printing a string will not include any null characters since it is null-terminated
        medium->print(this->oldestOutboundMessage->value);
        // mark end of message with a null character
        medium->write((uint8_t)0);
        medium->write((uint8_t)0);
        //mark end of message
        medium->write((uint8_t)0);
        medium->write(1);
        MessageNode *toDelete = this->oldestOutboundMessage;
        this->oldestOutboundMessage = this->oldestOutboundMessage->next;
        delete toDelete;
    }
}
int StreamMessage::available()
{
    int messageCount = 0;
    for (MessageNode *current = this->oldestInboundMessage; current != NULL; current = current->next)
        messageCount++;
    return messageCount;
}
String StreamMessage::peek()
{
    if (this->oldestInboundMessage == NULL)
        return "";
    return getLatestMessageNode(this->oldestInboundMessage)->value;
}
String StreamMessage::readMessage()
{
    if (this->oldestInboundMessage == NULL)
        return "";
    if (this->oldestInboundMessage->next == NULL)
    {
        String toReturn = this->oldestInboundMessage->value;
        delete this->oldestInboundMessage;
        return toReturn;
    }
    MessageNode *last = this->oldestInboundMessage;
    MessageNode *current = this->oldestInboundMessage->next;
    while (current->next != NULL)
    {
        last = current;
        current = current->next;
    }
    String toReturn = current->value;
    delete current;
    last->next = NULL;
    return toReturn;
}
bool StreamMessage::parse(char *message)
{
    // The message will be a null-terminated string
    MessageNode *newMessageNode = new MessageNode(String(message), NULL);
    if (this->oldestInboundMessage == NULL)
    {
        this->oldestInboundMessage = newMessageNode;
    }
    else
    {
        MessageNode *current = oldestInboundMessage;
        while (current->next != NULL)
            current = current->next;
        current->next = newMessageNode;
    }
    return true;
}
StreamMessage::~StreamMessage()
{
    while (this->oldestInboundMessage)
        this->readMessage();
    while (this->oldestOutboundMessage)
    {

        while (this->oldestOutboundMessage != NULL)
        {
            MessageNode *toDelete = this->oldestOutboundMessage;
            this->oldestOutboundMessage = this->oldestOutboundMessage->next;
            delete toDelete;
        }
    }
}