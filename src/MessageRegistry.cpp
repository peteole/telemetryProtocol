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

    this->maxMessageLength = max(this->maxMessageLength, toAdd->value->size);
    delete this->buffer;
    this->buffer = new char[this->maxMessageLength];
    this->currentPosition = 0;
}