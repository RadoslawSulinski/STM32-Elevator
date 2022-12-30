/*
 * uart_handler.cpp
 *
 *  Created on: Dec 11, 2022
 *      Author: Radoslaw Sulinski
 */

#include "uart_handler.hpp"

/**
 * get data sent from PC
 */
UartHandler::InBuffer
UartHandler::read_data() const
{
    return m_input_buffer;
}

/**
 * set data to be sent, discards any data held currently
 */
void
UartHandler::write_data(UartHandler::OutBuffer data)
{
    m_output_buffer = data;
    m_message_size = OUT_BUFFER_SIZE;
}

/**
 * as above but accepts c-style array, might be more convenient to use
 * this only validates size so beware of null characters and similar stuff
 */
void UartHandler::write_data(char * data, std::size_t size)
{
    if(size > OUT_BUFFER_SIZE)
    {
        return;
    }
    for(std::size_t ii = 0; ii < OUT_BUFFER_SIZE; ii++)
    {
        if(ii >= size)
        {
            m_output_buffer[ii] = '\0';
        }
        else
        {
            m_output_buffer[ii] = data[ii];
        }
    }
    m_message_size = size;
}

/**
 * send the data through uart and empty the buffer
 */
void
UartHandler::send()
{
    HAL_UART_Transmit(&m_uart_handle, m_output_buffer.data(), m_message_size, 10);
}

void
UartHandler::receive()
{
    HAL_UART_Receive_IT(&m_uart_handle, m_input_buffer.begin(), IN_BUFFER_SIZE);
}

void
UartHandler::process_input()
{
    m_input_handler.handle_input(m_input_buffer);
}

/**
 * parse input message
 * message format is : XXXYY where X make up variable identifier and Y make up value
 */
void
UartInputHandler::handle_input(UartHandler::InBuffer const & data)
{
    uint8_t * variable = get_variable(VariableID {data[0], data[1], data[2]});
    if (variable)
    {
       uint8_t const value = get_value(Value {data[3], data[4]});
       if (value != 100)
       {
           *variable = value;
       }
    }
}

uint8_t * UartInputHandler::get_variable(VariableID const & id)
{
    if((id[0] == 'F' || id[0] == 'f') && (id[1] == 'L' || id[1] == 'l') && (id[2] == 'R' || id[2] == 'r'))
    {
        return &m_floor;
    }
    else if((id[0] == 'S' || id[0] == 's') && (id[1] == 'P' || id[1] == 'p') && (id[2] == 'D' || id[2] == 'd'))
    {
        return &m_speed;
    }
    else
    {
        return nullptr;
    }
}

uint8_t UartInputHandler::get_value(Value const & value)
{
    if(value[0] >= '0' && value[0] <= '9' && value[1] >= '0' && value[1] <= '9')
    {
        return (value[0] - '0') * 10 + value[1] - '0';
    }
    else
    {
        return 100; //invalid value
    }
}
