# meCANBuffer Library

This Arduino library provides a circular buffer for storing and accessing [CAN](https://en.wikipedia.org/wiki/CAN_bus) messages.

## Features

- Store up to `BUFFER_SIZE` `CAN_Message_t` messages
- Check if the buffer is empty or full
- Push new messages onto the buffer
- Pop messages from the buffer
- Detect if any data has been lost due to buffer overflow
- Clear the data lost flag

## USE Cases

This meCANbuffer was designed for the use with CAN libraries.

- [meCAN](https://github.com/mackelec/meCAN)
- [meFDCAN](https://github.com/mackelec/meFDCAN)


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
```

## Class functions

```c++
        /**
         * @brief Construct a new me CAN Buffer object
         */
        meCANBuffer();

        /**
         * @brief Check if the buffer is empty
         * 
         * @return int 
         *  0 if not empty
         *  1 if empty
         */
        int is_empty();

        /**
         * @brief Check if the buffer is full
         * 
         * @return int 
         *  0 if not full
         *  1 if full
         */
        int is_full();

        /**
         * @brief Push a message onto the buffer
         * 
         * @param data 
         */
        void push(CAN_Message_t data);

        /**
         * @brief Pop a message from the buffer
         * 
         * @return CAN_Message_t 
         */
        CAN_Message_t pop();

        /**
         * @brief Check if any data has been lost due to buffer overflow
         * 
         * @return bool 
         */
        bool is_data_lost();

        /**
         * @brief 
