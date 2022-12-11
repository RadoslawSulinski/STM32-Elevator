/*
 * uart_handler.cpp
 *
 *  Created on: Dec 11, 2022
 *      Author: Radoslaw Sulinski
 */

#include "uart_handler.hpp"

UartHandler::InBuffer
UartHandler::read_data() const
{
    return m_input_buffer;
}

void
UartHandler::write_data(UartHandler::OutBuffer data)
{
    return m_output_buffer = data;
}

void
UartHandler::send() const
{
    HAL_UART_Transmit_IT(m_uart_handle, reinterpret_cast<uint8_t*>(m_output_buffer.begin()), UartHandler::OUT_BUFFER_SIZE);
}
