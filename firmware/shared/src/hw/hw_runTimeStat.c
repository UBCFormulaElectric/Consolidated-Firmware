#ifndef BOOTLOADER
#include "hw_runTimeStat.h"
#include "main.h"
#include <string.h>
#include <cmsis_os.h>
#include "app_utils.h"
#include "tasks.h"

/*
 * Software defines
 */
#define IDLE_TASK_INDEX 0U
#define TMR_SVC_INDEX 1U
#define NUM_FT_TASKS 2U
#define NUM_TOTAL_TASKS  (NUM_FT_TASKS + NUM_OF_TASKS)

/*
 * Global Variables
 */
static TIM_HandleTypeDef *runTimeCounter;

volatile unsigned long ulHighFrequencyTimerTick = 0;

static TaskRuntimeStats *tasks_runtime_stat[NUM_TOTAL_TASKS] = { [IDLE_TASK_INDEX] =
                                                                                 &(TaskRuntimeStats){ 0 },
                                                                             [TMR_SVC_INDEX] =
                                                                                 &(TaskRuntimeStats){ 0 } };
// We are going to assume that from here forth all our processors are going
// single core but if not make the following an array

static CpuRunTimeStats *cpu_runtime_stat;

/*
 * Function Decleration
 */

ExitCode hw_runTimeStat_init(TIM_HandleTypeDef *htim)
{
    if (NULL == htim)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    runTimeCounter = htim;

    return EXIT_CODE_OK;
}

ExitCode hw_runtimeStat_registerCpu(CpuRunTimeStats *cpu_info)
{
    if (NULL == cpu_info->cpu_usage_max_setter || NULL == cpu_info->cpu_usage_setter)
    {
        return EXIT_CODE_INVALID_ARGS;
    }

    cpu_runtime_stat = cpu_info;

    return EXIT_CODE_OK;
}

ExitCode hw_runTimeStat_registerTask(TaskRuntimeStats *task_info)
{
    if (NULL == task_info->cpu_usage_max_setter || NULL == task_info->cpu_usage_setter ||
        NULL == task_info->stack_usage_max_setter || 0 != task_info->stack_size)
    {
        return EXIT_CODE_INVALID_ARGS;
    }
    else if (NUM_OF_TASKS < task_info->task_index)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }

    tasks_runtime_stat[task_info->task_index + NUM_FT_TASKS] = task_info;

    return EXIT_CODE_OK;
}

void hw_runTimeStat_hookCallBack(void)
{
    TaskStatus_t runTimeStats[NUM_TOTAL_TASKS];

    TaskHandle_t idleHandle = xTaskGetIdleTaskHandle();

    uint32_t arraySize = uxTaskGetSystemState(runTimeStats, (UBaseType_t)NUM_TOTAL_TASKS, NULL);

    // given each task that we get from the following getsystemstate call we are gonna calcualte the
    // cpu usage and stack usage

    uint32_t idle_counter = 0;

    for (uint32_t task = 0; task < arraySize; task++)
    {
        if (runTimeStats[task].xHandle == idleHandle)
        {
            idle_counter = runTimeStats[task].ulRunTimeCounter;

            // Calculate total current cpu usage and max cpu usage
            cpu_runtime_stat->cpu_curr_usage = (float)idle_counter / (float)ulHighFrequencyTimerTick;

            cpu_runtime_stat->cpu_max_usage = MAX(cpu_runtime_stat->cpu_curr_usage, cpu_runtime_stat->cpu_max_usage);
            break;
        }
    }

    for (uint32_t task = 0; task < arraySize; task++)
    {
        // get the idle time that we need to calculate the cpu usage associated
        if (NULL != tasks_runtime_stat[task])
        {
            if ((idle_counter + runTimeStats[task].ulRunTimeCounter) != 0)
            {
                // Calculate current cpu usage
                tasks_runtime_stat[task]->cpu_curr_usage = (float)runTimeStats[task].ulRunTimeCounter /
                                                           (float)(idle_counter + runTimeStats[task].ulRunTimeCounter);
                // Calculate the max cpu usage
                tasks_runtime_stat[task]->cpu_max_usage =
                    MAX(tasks_runtime_stat[task]->cpu_max_usage, tasks_runtime_stat[task]->cpu_curr_usage);
            }
            {
                // Calculate max stack usage
                tasks_runtime_stat[task]->stack_usage_max =
                    (float)runTimeStats[task].usStackHighWaterMark / (float)tasks_runtime_stat[task]->stack_size;
            }
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

#else

void hw_runTimeStat_hookCallBack(void) {}

#endif
