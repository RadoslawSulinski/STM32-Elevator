/*
 * elevator_speed_handler.cpp
 *
 *  Created on: Dec 29, 2022
 *      Author: Radosław Suliński
 */

#include "elevator_speed_handler.hpp"

#include <cmath>

void
ElevatorSpeedHandler::set_speed_rpm(double speed_rpm)
{
    double frequency = rpm_to_pwm_frequency(speed_rpm);
    uint32_t const counter_value = [&frequency]{
            uint32_t counter_value_temp = std::round(ElevatorSpeedHandler::CLOCK_FREQUENCY_HZ / frequency);
            if (counter_value_temp > ElevatorSpeedHandler::MAX_COUNTER_VALUE || counter_value_temp < 0)
            {
                return ElevatorSpeedHandler::MAX_COUNTER_VALUE;
            }
            else
            {
                return counter_value_temp;
            }}();

    __HAL_TIM_SET_AUTORELOAD(&m_timer_handle, counter_value);
    // Compare always equal to half of Period, since the PWM is used as a square wave, not a real PWM
    // Unless Stop, then compare = 0
    if(speed_rpm == 0.0f)
    {
        __HAL_TIM_SET_COMPARE(&m_timer_handle, TIM_CHANNEL_1, 0);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&m_timer_handle, TIM_CHANNEL_1, counter_value/2);
    }
    bool const direction = speed_rpm < 0;
    HAL_GPIO_WritePin(m_direction_port, m_direction_pin, direction ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

double
ElevatorSpeedHandler::rpm_to_pwm_frequency(double speed_rpm)
{
   return abs(speed_rpm) / 60.0 * ElevatorSpeedHandler::STEPS_PER_ROTATION;
}
