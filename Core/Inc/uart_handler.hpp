/*
 * uart_handler.hpp
 *
 *  Created on: Dec 11, 2022
 *      Author: Radoslaw Sulinski
 */

#ifndef INC_UART_HANDLER_HPP_
#define INC_UART_HANDLER_HPP_

class UartHandler
{
    static std::size_t constexpr OUT_BUFFER_SIZE = 40;
    static std::size_t constexpr IN_BUFFER_SIZE = 5;

public:
    using OutBuffer = std::array<uint8_t, OUT_BUFFER_SIZE>;
    using InBuffer = std::array<uint8_t, IN_BUFFER_SIZE>;

    void write_data(OutBuffer data);
    InBuffer read_data() const;
    void send() const;
    void receive();

private:
    InBuffer m_input_buffer;
    OutBuffer m_output_buffer;
};

class UartInputHandler
{

};

#endif /* INC_UART_HANDLER_HPP_ */
