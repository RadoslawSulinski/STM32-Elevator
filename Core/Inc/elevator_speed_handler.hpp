/*
 * elevator_speed_handler.hpp
 *
 *  Created on: Dec 29, 2022
 *      Author: Radosław Suliński
 */

#include "stm32f4xx_hal.h"

#ifndef INC_ELEVATOR_SPEED_HANDLER_HPP_
#define INC_ELEVATOR_SPEED_HANDLER_HPP_

class ElevatorSpeedHandler
{
    static double constexpr CLOCK_FREQUENCY_HZ = 84e6 / 4; // clock divison set to 4 in .ioc
    static double constexpr STEP_SIZE_DEG = 1.8;
    static int constexpr STEPS_PER_ROTATION = 360/STEP_SIZE_DEG;
    static uint32_t constexpr MAX_COUNTER_VALUE = 65535;

public:
    ElevatorSpeedHandler(TIM_HandleTypeDef & timer_handle, GPIO_TypeDef * direction_port, uint16_t direction_pin) :
        m_timer_handle(timer_handle), m_direction_port(direction_port), m_direction_pin(direction_pin) {}

    void set_speed_rpm(double speed_rpm);

private:
    double rpm_to_pwm_frequency(double speed_rpm);

    TIM_HandleTypeDef & m_timer_handle;
    GPIO_TypeDef * const m_direction_port;
    uint16_t const m_direction_pin;
};



#endif /* INC_ELEVATOR_SPEED_HANDLER_HPP_ */
