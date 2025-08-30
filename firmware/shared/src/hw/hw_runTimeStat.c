#include "hw_runTimeStat.h"
#include "main.h"
#include <string.h>
#include <cmsis_os.h>
#include "app_utils.h"
#include "tasks.h"

// Global variables
static TIM_HandleTypeDef *runTimeCounter;

volatile unsigned long ulHighFrequencyTimerTick = 0;

static TaskRuntimeStats *tasks_runtime_stat[NUM_OF_TASKS];

// TODO need to change the format of this such that each task has its own function registeration meaning it has a
// function pointer to convery info

// Function Decleration

ExitCode hw_runTimeStat_init(TIM_HandleTypeDef *htim)
{
    if (NULL == htim)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    runTimeCounter = htim;

    return EXIT_CODE_OK;
}

ExitCode hw_runTimeStat_registerTask(TaskRuntimeStats *task_info)
{
    if (NULL == task_info->cpu_usage_max_setter || NULL == task_info->cpu_usage_setter ||
        NULL == task_info->stack_usage_max_setter)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    else if (NUM_OF_TASKS < task_info->task_index)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }

    tasks_runtime_stat[task_info->task_index] = task_info;

    return EXIT_CODE_OK;
}

void hw_runTimeStat_hookCallBack(void)
{
    TaskStatus_t runTimeStats[NUM_OF_TASKS];

    TaskHandle_t idleHandle = xTaskGetIdleTaskHandle();

    uint32_t arraySize = uxTaskGetSystemState(runTimeStats, (UBaseType_t)NUM_OF_TASKS, NULL);

    // given each task that we get from the following getsystemstate call we are gonna calcualte the
    // cpu usage and stack usage

    uint32_t idle_counter = 0;

    for (uint32_t task = 0; task < arraySize; task++)
    {
        // get the idle time that we need to calculate the cpu usage associated
        if (runTimeStats[task].xHandle == idleHandle)
        {
            idle_counter = runTimeStats[task].ulRunTimeCounter;
        }
        else
        {
            // Calculate current cpu usage
            tasks_runtime_stat[task]->cpu_curr_usage = (float)runTimeStats[task].ulRunTimeCounter /
                                                     (float)(idle_counter + runTimeStats[task].ulRunTimeCounter);

            // Calculate max stack usage
            tasks_runtime_stat[task]->stack_usage_max = runTimeStats[task].usStackHighWaterMark;

            // Calculate the max cpu usage
            tasks_runtime_stat[task]->cpu_max_usage =
                MAX(tasks_runtime_stat[task]->cpu_max_usage, tasks_runtime_stat[task]->cpu_curr_usage);
        }
    }
}

void configureTimerForRunTimeStats(void)
{
    HAL_TIM_Base_Start_IT(runTimeCounter);
}

unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTick;
}