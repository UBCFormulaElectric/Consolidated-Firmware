#include "jobs.hpp"

#include "app_irs.hpp"


void jobs_run100Hz_tick()
{
    app::irs::updateNegativeOpenDebounce();
    
    app::irs::broadcast();
}