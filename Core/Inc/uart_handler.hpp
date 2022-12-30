/*
 * uart_handler.hpp
 *
 *  Created on: Dec 11, 2022
 *      Author: Radoslaw Sulinski
 */

#include <cstdio>
#include <array>
#include "stm32f4xx_hal.h"

#ifndef INC_UART_HANDLER_HPP_
#define INC_UART_HANDLER_HPP_

class UartInputHandler
{
    static size_t constexpr IN_BUFFER_SIZE = 5;
    using InBuffer = std::array<uint8_t, IN_BUFFER_SIZE>;

public:
    UartInputHandler(uint8_t & floor, uint8_t & speed) : m_floor(floor), m_speed(speed) {}

    void handle_input(InBuffer const & data);

private:
    using VariableID = std::array<uint8_t, 3>;
    using Value = std::array<uint8_t, 2>;

    uint8_t * get_variable(VariableID const & id);
    uint8_t get_value(Value const & value);

    uint8_t & m_floor;
    uint8_t & m_speed;
};

class UartHandler
{
public:
    static uint16_t constexpr OUT_BUFFER_SIZE = 40;
    static uint16_t constexpr IN_BUFFER_SIZE = 5;
    using OutBuffer = std::array<uint8_t, OUT_BUFFER_SIZE>;
    using InBuffer = std::array<uint8_t, IN_BUFFER_SIZE>;

    UartHandler(UART_HandleTypeDef & uart_handle, uint8_t & floor, uint8_t & speed)
        : m_uart_handle(uart_handle),
          m_input_handler(floor, speed) {}

    void write_data(OutBuffer data);
    void write_data(char * data, std::size_t size);
    InBuffer read_data() const;
    void send();
    void receive();
    void process_input();

private:
    InBuffer m_input_buffer {};
    OutBuffer m_output_buffer {};

    UART_HandleTypeDef & m_uart_handle;
    UartInputHandler m_input_handler;

    uint16_t m_message_size {OUT_BUFFER_SIZE};
};

#endif /* INC_UART_HANDLER_HPP_ */
