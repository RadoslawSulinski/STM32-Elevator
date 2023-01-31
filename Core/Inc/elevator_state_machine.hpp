#ifndef INC_ELEVATOR_STATE_MACHINE_HPP_
#define INC_ELEVATOR_STATE_MACHINE_HPP_

#include "floor_sensors.hpp"
#include "elevator_speed_handler.hpp"
#include <queue>
#include <array>

class ElevatorStateMachine
{
    static constexpr int MAX_QUEUE_SIZE = 3;
    static constexpr int MIN_IDLE_TIMESTEPS = 10;
    static constexpr int MIN_FLOOR = 1;
    static constexpr int MAX_FLOOR = 3;
public:
    enum class State
    {
        IDLE,
        GOING_UP,
        GOING_DOWN
    };

    ElevatorStateMachine(GPIO_TypeDef * floor_1_upper_port,
                         uint16_t floor_1_upper_pin,
                         GPIO_TypeDef * floor_1_lower_port,
                         uint16_t floor_1_lower_pin,

                         GPIO_TypeDef * floor_2_upper_port,
                         uint16_t floor_2_upper_pin,
                         GPIO_TypeDef * floor_2_lower_port,
                         uint16_t floor_2_lower_pin,

                         GPIO_TypeDef * floor_3_upper_port,
                         uint16_t floor_3_upper_pin,
                         GPIO_TypeDef * floor_3_lower_port,
                         uint16_t floor_3_lower_pin,

                         TIM_HandleTypeDef & timer_handle,
                         GPIO_TypeDef * direction_port,
                         uint16_t direction_pin) : m_floor_sensors{FloorSensors{floor_1_upper_port, floor_1_upper_pin, floor_1_lower_port, floor_1_lower_pin},
                                                                   FloorSensors{floor_2_upper_port, floor_2_upper_pin, floor_2_lower_port, floor_2_lower_pin},
                                                                   FloorSensors{floor_3_upper_port, floor_3_upper_pin, floor_3_lower_port, floor_3_lower_pin}},
                                                   m_speed_handler(timer_handle, direction_port, direction_pin)
                                                   {m_floor_queue.push(3);}

    void update();

    void push_floor(int floor)
    {
        if(floor < MIN_FLOOR || floor > MAX_FLOOR)
        {
            return;
        }
        else if(m_floor_queue.size() < MAX_QUEUE_SIZE)
        {
            m_floor_queue.push(floor);
        }
    }

    void set_speed_rpm(float speed_rpm)
    {
        m_speed_rpm = speed_rpm;
    }

    State get_state()
    {
        return m_current_state;
    }

    int get_target_floor()
    {
        return m_target_floor;
    }

    std::array<int, 6> get_sensors_state()
    {
        return {m_floor_sensors[2].get_upper_sensor(), m_floor_sensors[2].get_lower_sensor(), m_floor_sensors[1].get_upper_sensor(), m_floor_sensors[1].get_lower_sensor(), m_floor_sensors[0].get_upper_sensor(), m_floor_sensors[0].get_lower_sensor()};
    }

private:
    State get_next_state();

    State m_current_state {State::IDLE};
    std::queue<int> m_floor_queue {}; // This should be a circular buffer but i couldn't be bothered to implement one or install ETL
    std::array<FloorSensors, 3> m_floor_sensors;
    ElevatorSpeedHandler m_speed_handler;

    int m_target_floor {};
    float m_speed_rpm {100};
    int m_steps_elapsed_in_idle {};
};



#endif /* INC_ELEVATOR_STATE_MACHINE_HPP_ */
