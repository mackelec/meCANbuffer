# meCANBuffer Library

This library provides a circular buffer for storing and accessing [CAN](https://en.wikipedia.org/wiki/CAN_bus) messages.

## Features

- Store up to `BUFFER_SIZE` `CAN_Message_t` messages
- Check if the buffer is empty or full
- Push new messages onto the buffer
- Pop messages from the buffer
- Detect if any data has been lost due to buffer overflow
- Clear the data lost flag

## Types

```c++
#define BUFFER_SIZE 100

typedef union
{
    struct
    {
        uint16_t id;
        uint8_t data[8];
        uint8_t port:4;
        uint8_t length:4;
    };
    uint8_t raw[11];
} CAN_Message_t;
