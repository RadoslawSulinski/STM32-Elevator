/*
 * tasks.cpp
 *
 *  Created on: Nov 23, 2022
 *      Author: Duch Kraftu
 */
#include "tasks.hpp"
#include "cmsis_os.h"

extern void StartFastTask(void const * argument)
{
    while(true)
    {
        osDelay(1000);
    }
}

extern void StartSlowTask(void const * argument)
{
    while(true)
    {
        osDelay(1000);
    }
}


