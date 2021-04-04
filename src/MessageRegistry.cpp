#include "MessageRegistry.h"
void MessageRegistry::readDataInStream(Stream *toRead)
{
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
}
MessageRegistry::~MessageRegistry()
{
    while(this)
    delete this->stream;
}