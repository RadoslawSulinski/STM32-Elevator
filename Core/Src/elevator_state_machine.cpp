#include "elevator_state_machine.hpp"

void
ElevatorStateMachine::update()
{
    switch(m_current_state)
    {
        case State::IDLE:
            m_speed_handler.set_speed_rpm(0.0f);
        case State::GOING_UP:
            if(m_floor_sensors[m_target_floor].get_lower_sensor())
            {
                m_speed_handler.set_speed_rpm(m_speed_rpm/2.0f);
            }
            else
            {
                m_speed_handler.set_speed_rpm(m_speed_rpm);
            }
            break;
        case State::GOING_DOWN:
            if(m_floor_sensors[m_target_floor].get_upper_sensor())
            {
                m_speed_handler.set_speed_rpm(-m_speed_rpm/2.0f);
            }
            else
            {
                m_speed_handler.set_speed_rpm(-m_speed_rpm);
            }
            break;
    }
    m_current_state = get_next_state();
}

ElevatorStateMachine::State
ElevatorStateMachine::get_next_state()
{
    switch(m_current_state)
    {
        case State::IDLE:
            if(!m_floor_queue.empty() && m_steps_elapsed_in_idle >= MIN_IDLE_TIMESTEPS)
            {
                int const next_floor = m_floor_queue.front();
                m_floor_queue.pop();
                State const next_state = next_floor > m_target_floor ? State::GOING_UP  : State::GOING_DOWN;

                m_steps_elapsed_in_idle = 0;
                m_target_floor = next_floor;
                return next_state;
            }
            else
            {
                m_steps_elapsed_in_idle++;
                return State::IDLE;
            }
        case State::GOING_UP:
            if(m_floor_sensors[m_target_floor].get_lower_sensor() && m_floor_sensors[m_target_floor].get_upper_sensor())
            {
                return State::IDLE;
            }
            else
            {
                return State::GOING_UP;
            }
        case State::GOING_DOWN:
            if(m_floor_sensors[m_target_floor].get_lower_sensor() && m_floor_sensors[m_target_floor].get_upper_sensor())
            {
                return State::IDLE;
            }
            else
            {
                return State::GOING_DOWN;
            }
    }
    return State::IDLE;
}

