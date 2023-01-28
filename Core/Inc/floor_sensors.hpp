#include "stm32f4xx_hal.h"

#ifndef INC_FLOOR_SENSORS_HPP_
#define INC_FLOOR_SENSORS_HPP_

class FloorSensors
{
public:
    FloorSensors(GPIO_TypeDef * upper_port, uint16_t upper_pin, GPIO_TypeDef * lower_port, uint16_t lower_pin) : m_upper_port(upper_port),
                                                                                                                 m_upper_pin(upper_pin),
                                                                                                                 m_lower_port(lower_port),
                                                                                                                 m_lower_pin(lower_pin)
    {}

    bool get_upper_sensor();
    bool get_lower_sensor();

private:
    GPIO_TypeDef * const m_upper_port;
    uint16_t const m_upper_pin;
    GPIO_TypeDef * const m_lower_port;
    uint16_t const m_lower_pin;
};

#endif /* INC_FLOOR_SENSORS_HPP_ */
