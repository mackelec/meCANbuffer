/**
 * @file meCANBuffer.h
 * @brief Definition of the meCANBuffer class
 * 
 * This class implements a buffer for CAN messages using a circular buffer.
 */

#define BUFFER_SIZE 100

/**
 * @brief Union representing a CAN message
 * 
 * This union provides access to the individual fields of a CAN message as well
 * as the raw data array.
 */
typedef union
{
    struct
    {
        uint16_t id;          ///< Message ID
        uint8_t data[8];      ///< Message data
        uint8_t port:4;       ///< Message port
        uint8_t length:4;     ///< Message length
    };
    uint8_t raw[11];         ///< Raw data
} CAN_Message_t;

/**
 * @class meCANBuffer
 * @brief Implements a buffer for CAN messages
 * 
 * This class implements a buffer for CAN messages using a circular buffer.
 * The buffer is implemented using an array of CAN_Message_t structures,
 * along with variables to keep track of the head and tail indices and the
 * current number of messages in the buffer.
 */
class meCANBuffer
{
    private:
        CAN_Message_t buffer[BUFFER_SIZE];   ///< Array of CAN messages
        volatile uint32_t head;              ///< Index of the head of the buffer
        volatile uint32_t tail;              ///< Index of the tail of the buffer
        volatile uint32_t count;             ///< Number of messages in the buffer
        CAN_Message_t empty_message;         ///< Empty message to return when buffer is empty
        bool data_lost;                      ///< Flag indicating whether data has been lost due to buffer overflow

    public:
        /**
         * @brief Constructor
         * 
         * Initializes the head, tail, and count variables, sets the data_lost flag to false,
         * and initializes the empty message.
         */
        meCANBuffer()
        {
            head = 0;
            tail = 0;
            count = 0;
            data_lost = false;
            empty_message.id = 0;
            for(int i = 0; i < 8; i++) 
            {
                empty_message.data[i] = 0;
            }
            empty_message.port = 0;
            empty_message.length = 0;
        }

        /**
         * @brief Check if the buffer is empty
         * 
         * Returns true if the buffer is empty, false otherwise.
         * 
         * @return true if the buffer is empty, false otherwise.
         */
        int is_empty()
        {
            return count == 0;
        }

        /**
         * @brief Check if the buffer is full
         * 
         * Returns true if the buffer is full, false otherwise.
         * 
         * @return true if the buffer is full, false otherwise.
         */
        int is_full()
        {
            return count == BUFFER_SIZE;
        }

        /**
         * @brief Push a message onto the buffer
         *
         * If the buffer is full, the head is incremented and the oldest message is overwritten. The count is decremented to indicate that a message was lost. The data_lost flag is set to true to indicate that data has been lost.
         *
         * @param[in] data The message to be pushed onto the buffer
         */
        void push(CAN_Message_t data)
        {
            if (is_full()) 
            {
                head = (head + 1) % BUFFER_SIZE;
                count--;
                data_lost = true;
            }
            memcpy(buffer[tail].raw, data.raw, sizeof(data.raw)/sizeof(data.raw[0]));
            tail = (tail + 1) % BUFFER_SIZE;
            count++;
        }

/**
         * @brief Pop a message from the buffer
         *
         * If the buffer is empty, the function returns the empty_message object.
         *
         * @return The message at the head of the buffer
         */
        CAN_Message_t pop()
        {
            if (!is_empty()) 
            {
                CAN_Message_t data = buffer[head];
                head = (head + 1) % BUFFER_SIZE;
                count--;
                return data;
            }
            return empty_message;
        }

/**
         * @brief Check if data has been lost
         *
         * @return true if data has been lost, false otherwise
         */
        bool is_data_lost()
        {
            return data_lost;
        }

/**
         * @brief Clear the data lost flag
         */
        void clear_data_lost()
        {
            data_lost = false;
        }
