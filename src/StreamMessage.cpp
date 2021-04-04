#include "StreamMessage.h"
StreamMessage::StreamMessage(int id) : Message(new StreamSensorValue(), id)
{
    this->oldestInboundMessage = NULL;
}

bool StreamMessage::addMessage(String message)
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
bool StreamMessage::pushToCurrentMessage(String toAppend) {}
void StreamMessage::send(Stream *medium)
{
    while (this->oldestOutboundMessage != NULL)
    {
        medium->print(this->oldestOutboundMessage->value);
        MessageNode *toDelete = this->oldestOutboundMessage;
        this->oldestOutboundMessage = this->oldestOutboundMessage->next;
        delete toDelete;
    }
}
int StreamMessage::available() {
    int messageCount=0;
    for(MessageNode* current=this->oldestInboundMessage;current!=NULL;current=current->next)
        messageCount++;
    return messageCount;
}
String StreamMessage::peek() {}