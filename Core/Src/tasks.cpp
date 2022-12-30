/*
 * tasks.cpp
 *
 *  Created on: Nov 23, 2022
 *      Author: Duch Kraftu
 */
#include "tasks.hpp"
#include "cmsis_os.h"
#include "uart_handler.hpp"
#include "elevator_speed_handler.hpp"

extern uint8_t floor_number;
extern uint8_t speed_percent;
extern UartHandler uart;
extern ElevatorSpeedHandler speed_handler;

extern void StartFastTask(void const * argument)
{
    double const min_rpm = 100;
    double const max_rpm = 300;
    char local_message_buffer[UartHandler::OUT_BUFFER_SIZE];
    while(true)
    {
        uint32_t wake_time = xTaskGetTickCount();
        double const speed_rpm_to_set = std::max(min_rpm, max_rpm * speed_percent / 100);
        speed_handler.set_speed_rpm(speed_rpm_to_set);
        std::size_t message_size = sprintf(local_message_buffer, "speed: %4.2lf rpm\r\n", speed_rpm_to_set);
        uart.write_data(local_message_buffer, message_size);
        osDelayUntil(&wake_time, 100);
    }
}

extern void StartSlowTask(void const * argument)
{
    char local_message_buffer[UartHandler::OUT_BUFFER_SIZE];
    while(true)
    {
        uint32_t wake_time = xTaskGetTickCount();
        uart.receive();
        uart.send();
        std::size_t message_size = sprintf(local_message_buffer, "slow task ran\r\n");
        uart.write_data(local_message_buffer, message_size);
        uart.send();
        osDelayUntil(&wake_time, 1000);
    }
}


