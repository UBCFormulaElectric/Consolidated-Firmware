// Needed to ensure we are not running this does not compile for the bootloadeer binary

#include "hw_runTimeStat.hpp"

#include <cstring>
#include <cmsis_os.h>

/*
 * Software defines
 */
#define IDLE_TASK_INDEX 0U
#define TMR_SVC_INDEX 1U
#define NUM_FT_TASKS 2U
#define NUM_TOTAL_TASKS (NUM_FT_TASKS + NUM_OF_TASKS)

/*
 * Global Variables
 */
static TIM_HandleTypeDef *runTimeCounter;

volatile unsigned long ulHighFrequencyTimerTick = 0;

static TaskRuntimeStats *tasks_runtime_stat[NUM_TOTAL_TASKS] = { [IDLE_TASK_INDEX] = &(TaskRuntimeStats){ nullptr },
                                                                 [TMR_SVC_INDEX]   = &(TaskRuntimeStats){ nullptr } };
// We are going to assume that from here forth all our processors are going
// single core but if not make the following an array

static CpuRunTimeStats *cpu_runtime_stat;

namespace hw::runtimeStat
{
[[nodiscard]] std::expected<void, ErrorCode> init(TIM_HandleTypeDef &htim)
{
    runTimeCounter = htim;
    return {};
}

[[nodiscard]] std::expected<void, ErrorCode> registerCpu(CpuRunTimeStats &cpu_info)
{
    if (cpu_info.cpu_usage_max_setter == nullptr || cpu_info.cpu_usage_setter == nullptr)
    {
        return std::unexpected(ErrorCode::INVALID_ARGS);
    }
    cpu_runtime_stat = cpu_info;
    return {};
}

[[nodiscard]] std::expected<void, ErrorCode> registerTask(TaskRuntimeStats &task_info)
{
    if (task_info->cpu_usage_max_setter == nullptr || task_info->cpu_usage_setter == nullptr ||
        task_info->stack_usage_max_setter == nullptr || task_info->stack_size != 0)
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

void hookCallBack()
{
    TaskStatus_t runTimeStats[NUM_TOTAL_TASKS];

    /* Get the task IDLE handle for processing the time spend outside of idle task*/
    const TaskHandle_t idleHandle = xTaskGetIdleTaskHandle();

    uint32_t arraySize = uxTaskGetSystemState(runTimeStats, static_cast<UBaseType_t>(NUM_TOTAL_TASKS), nullptr);

    if (arraySize == 0)
    {
        LOG_ERROR("TaskGetSystemState failed");
    }

    /*
     * Given each task that we get from the following getsystemstate call we are gonna calcualte the
     * cpu usage and stack usage
     */

    uint32_t idle_counter = 0;

    for (uint32_t task = 0; task < arraySize; task++)
    {
        if (runTimeStats[task].xHandle == idleHandle)
        {
            idle_counter = runTimeStats[task].ulRunTimeCounter;

            // Calculate total current cpu usage and max cpu usage
            cpu_runtime_stat->cpu_curr_usage =
                1.0f - (static_cast<float>(idle_counter) / static_cast<float>(ulHighFrequencyTimerTick));

            cpu_runtime_stat->cpu_max_usage = MAX(cpu_runtime_stat->cpu_curr_usage, cpu_runtime_stat->cpu_max_usage);
            break;
        }
    }

    for (uint32_t task = 0; task < arraySize; task++)
    {
        // get the idle time that we need to calculate the cpu usage associated
        if (tasks_runtime_stat[task] != nullptr)
        {
            if ((idle_counter + runTimeStats[task].ulRunTimeCounter) != 0)
            {
                // Calculate current cpu usage
                tasks_runtime_stat[task]->cpu_curr_usage =
                    static_cast<float>(runTimeStats[task].ulRunTimeCounter) /
                    static_cast<float>(idle_counter + runTimeStats[task].ulRunTimeCounter);
                // Calculate the max cpu usage
                tasks_runtime_stat[task]->cpu_max_usage =
                    MAX(tasks_runtime_stat[task]->cpu_max_usage, tasks_runtime_stat[task]->cpu_curr_usage);
            }

            // Calculate max stack usage
            tasks_runtime_stat[task]->stack_usage_max = static_cast<float>(runTimeStats[task].usStackHighWaterMark) /
                                                        static_cast<float>(tasks_runtime_stat[task]->stack_size);
        }
        else
        {
            LOG_WARN("Task #%d not initalized task", task);
        }
    }
}
} // namespace hw::runtimeStat

void configureTimerForRunTimeStats()
{
    HAL_TIM_Base_Start_IT(runTimeCounter);
}

unsigned long getRunTimeCounterValue()
{
    return ulHighFrequencyTimerTick;
}