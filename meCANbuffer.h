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

class meCANBuffer
{
    private:
        CAN_Message_t buffer[BUFFER_SIZE];
        volatile uint32_t head;
        volatile uint32_t tail;
        volatile uint32_t count;
        CAN_Message_t empty_message;
        bool data_lost;

    public:
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

        int is_empty()
        {
            return count == 0;
        }

        int is_full()
        {
            return count == BUFFER_SIZE;
        }

        void push(CAN_Message_t data)
        {
            if (is_full()) 
            {
                head = (head + 1) % BUFFER_SIZE;
                count--;
                data_lost = true;
            }
            //uint8_t *dest = buffer[tail].raw;
            memcpy(buffer[tail].raw, data.raw, sizeof(data.raw)/sizeof(data.raw[0]));
            tail = (tail + 1) % BUFFER_SIZE;
            count++;
        }

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

        bool is_data_lost()
        {
            return data_lost;
        }

        void clear_data_lost()
        {
            data_lost = false;
        }
};
