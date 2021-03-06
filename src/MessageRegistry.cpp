#include "MessageRegistry.h"
void MessageRegistry::readDataInStream(Stream *toRead)
{
    bool mustSendMessageDefinitionMessage = false;
    while (toRead->available())
    {
        if (currentPosition >= maxMessageLength)
        {
            // end of message was not recognized
            this->currentPosition = 0;
        }
        char nextValue = toRead->read();
        if (previousByteZero)
        {
            if (nextValue == 0)
            {
                //zero occured in message
                this->buffer[currentPosition] = nextValue;
                currentPosition++;
            }
            else if (nextValue == 1)
            {
                // End of message reached
                if (currentMessage != NULL)
                {
                    currentMessage->parse(this->buffer);
                    currentMessage = NULL;
                }
                this->currentPosition = 0;
            }
            else
            {
                if (nextValue == 255)
                {
                    // 255 is the message definition message id. If it is send, we need to send all message definitions
                    mustSendMessageDefinitionMessage = true;
                }
                else
                {
                    //start of new message
                    for (int i = 0; i < this->length; i++)
                    {
                        if (this->messages[i]->id == nextValue)
                        {
                            this->currentMessage = messages[i];
                            break;
                        }
                    }
                    this->currentPosition = 0;
                }
            }
            previousByteZero = false;
        }
        else
        {
            if (nextValue == 0)
            {
                previousByteZero = true;
            }
            else
            {
                this->buffer[this->currentPosition] = nextValue;
                this->currentPosition++;
            }
        }
    }
    if (mustSendMessageDefinitionMessage)
    {
        this->sendDataSchema(toRead);
    }
}
void MessageRegistry::addMessage(Message *toAdd)
{
    this->length++;
    Message **newMessages = new Message *[this->length];
    for (int i = 0; i < this->length - 1; i++)
    {
        newMessages[i] = this->messages[i];
    }
    newMessages[this->length - 1] = toAdd;
    delete this->messages;
    this->messages = newMessages;
    if (this->maxMessageLength < toAdd->value->size)
        this->maxMessageLength = toAdd->value->size;
    delete this->buffer;
    this->buffer = new char[this->maxMessageLength];
    this->currentPosition = 0;
}
void MessageRegistry::sendDataSchema(Stream *stream)
{
    for (int i = 0; i < this->length; i++)
    {
        this->messages[i]->sendDescription(stream);
    }
}
MessageRegistry::MessageRegistry() : stream(new StreamMessage(254))
{
    this->addMessage(this->stream);
}
MessageRegistry::~MessageRegistry()
{
    while (this)
        delete this->stream;
}