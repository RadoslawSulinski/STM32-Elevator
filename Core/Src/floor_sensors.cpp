/*
 * floor_sensors.cpp
 *
 *  Created on: 18 sty 2023
 *      Author: Radoslaw Sulinski
 */
#include "floor_sensors.hpp"


bool
FloorSensors::get_upper_sensor()
{
    return HAL_GPIO_ReadPin(m_upper_port, m_upper_pin);
}

bool
FloorSensors::get_lower_sensor()
{
    return HAL_GPIO_ReadPin(m_lower_port, m_lower_pin);
}
