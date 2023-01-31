/*
 * tasks.cpp
 *
 *  Created on: Nov 23, 2022
 *      Author: Duch Kraftu
 */
#include "tasks.hpp"
#include "cmsis_os.h"
#include "uart_handler.hpp"
#include "elevator_state_machine.hpp"
//#include "elevator_speed_handler.hpp"
//#include "floor_sensors.hpp"

extern uint8_t floor_number;
extern uint8_t speed_percent;
extern UartHandler uart;
extern ElevatorStateMachine elevator_state_machine;
//extern ElevatorSpeedHandler speed_handler;

//extern std::array<FloorSensors, 3> floors;

extern void StartFastTask(void const * argument)
{
    double const min_rpm = 100;
    double const max_rpm = 250;
    char local_message_buffer[UartHandler::OUT_BUFFER_SIZE];
    while(true)
    {
        uint32_t wake_time = xTaskGetTickCount();
        double const speed_rpm_to_set = std::max(min_rpm, max_rpm * speed_percent / 100);
//        speed_handler.set_speed_rpm(speed_rpm_to_set);
        elevator_state_machine.set_speed_rpm(speed_rpm_to_set);
        elevator_state_machine.update();
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
        ElevatorStateMachine::State const current_state = elevator_state_machine.get_state();
        int const target_floor = elevator_state_machine.get_target_floor();
        std::size_t message_size = 0;
        switch(current_state)
        {
            case ElevatorStateMachine::State::IDLE:
                message_size = sprintf(local_message_buffer, "IDLE state, target floor: %d\r\n", target_floor);
                break;
            case ElevatorStateMachine::State::GOING_UP:
                message_size = sprintf(local_message_buffer, "GOING UP state, target floor: %d\r\n", target_floor);
                break;
            case ElevatorStateMachine::State::GOING_DOWN:
                message_size = sprintf(local_message_buffer, "GOING DOWN state, target floor: %d\r\n", target_floor);
                break;
        }
        uart.write_data(local_message_buffer, message_size);
        uart.send();
        std::array<int, 6> const sensors = elevator_state_machine.get_sensors_state();
        message_size = sprintf(local_message_buffer, "Sensors state: %d %d %d %d %d %d", sensors[0], sensors[1], sensors[2], sensors[3], sensors[4], sensors[5]);
        uart.write_data(local_message_buffer, message_size);
        uart.send();
        osDelayUntil(&wake_time, 1000);
    }
}


