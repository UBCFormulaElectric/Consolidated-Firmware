#include "hw_runTimeStat.h"
#include "main.h"
#include <rsm.pb.h>
#include <string.h>
#include <cmsis_os.h>
#include "app_utils.h"

#define MAX_TASKS 10

//Global variables
static TIM_HandleTypeDef *runTimeCounter;

volatile unsigned long ulHighFrequencyTimerTick = 0;

static RunTimeStats runtimestatistics[MAX_TASKS];

static const RunTimeStatsPublish *publish_info;

//Function Decleration

// What we need to do is use fill the runtimestats buffer with the information assocaited with each task

void hw_runTimeStat_init(TIM_HandleTypeDef *htim, const RunTimeStatsPublish *const task_publish_info)
{
    memset(runtimestatistics, 0, sizeof(runtimestatistics));
    runTimeCounter = htim;
    publish_info = task_publish_info;
}

void hw_runTimeStat_hookCallBack(void)
{
    TaskStatus_t runTimeStats[MAX_TASKS];

    TaskHandle_t idleHandle = xTaskGetIdleTaskHandle();

    uint32_t arraySize = uxTaskGetSystemState(runTimeStats, (UBaseType_t)MAX_TASKS, NULL);

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
            //Calculate current cpu usage
            runtimestatistics[task].cpu_curr_usage = (float)runTimeStats[task].ulRunTimeCounter /
                                                     (float)(idle_counter + runTimeStats[task].ulRunTimeCounter);
            
            //Calculate max stack usage 
            runtimestatistics[task].stack_usage_max = runTimeStats[task].usStackHighWaterMark;

            //Calculate the max cpu usage
            runtimestatistics[task].cpu_max_usage =
                MAX(runtimestatistics[task].cpu_max_usage, runtimestatistics[task].cpu_curr_usage);
        }
    }
}

void hw_runTimeStat_publish()
{

}
void configureTimerForRunTimeStats(void)
{
    HAL_TIM_Base_Start_IT(runTimeCounter);
}

unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTick;
}