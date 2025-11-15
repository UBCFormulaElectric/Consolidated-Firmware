#include "app_filter.hpp"
#include "io_log.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace app
{

// MovingAverageFilter implementation
MovingAverageFilter::MovingAverageFilter(uint32_t window_size, float initial_value)
  : buffer_(window_size, initial_value),
    buffer_index_(0),
    sample_count_(0),
    current_output_(initial_value),
    sum_(initial_value * window_size),
    is_ready_(false)
{
    config_.type           = FilterType::MOVING_AVERAGE;
    config_.window_size    = window_size;
    config_.is_initialized = true;
}

float MovingAverageFilter::process(float input)
{
    // Update running sum by removing oldest value and adding new value
    sum_ = sum_ - buffer_[buffer_index_] + input;

    // Store new value in buffer
    buffer_[buffer_index_] = input;

    // Update buffer index (circular buffer)
    buffer_index_ = (buffer_index_ + 1) % config_.window_size;

    // Calculate and return average
    current_output_ = sum_ / config_.window_size;
    sample_count_++;

    // Mark as ready after processing first sample
    if (!is_ready_)
    {
        is_ready_ = true;
    }

    return current_output_;
}

float MovingAverageFilter::getOutput() const
{
    return current_output_;
}

void MovingAverageFilter::reset()
{
    sample_count_ = 0;
    is_ready_     = false;
    // Reset to the last previous output to avoid jolting/discontinuity
    // Reset internal state while keeping the current output value
    // This ensures the filter continues smoothly from the current state
    sum_          = current_output_ * config_.window_size;
    buffer_index_ = 0;

    // Reset buffer to current output value to maintain continuity
    std::fill(buffer_.begin(), buffer_.end(), current_output_);
}

bool MovingAverageFilter::isReady() const
{
    return is_ready_;
}

uint32_t MovingAverageFilter::getSampleCount() const
{
    return sample_count_;
}

FilterConfig MovingAverageFilter::getConfig() const
{
    return config_;
}

// ExponentialFilter implementation
ExponentialFilter::ExponentialFilter(float alpha, float initial_value)
  : previous_output_(initial_value), sample_count_(0), current_output_(initial_value), is_ready_(false)
{
    config_.type           = FilterType::EXPONENTIAL_AVERAGE;
    config_.alpha          = alpha;
    config_.is_initialized = true;
}

ExponentialFilter::ExponentialFilter(
    AlphaParameterType param_type,
    float              param_value,
    float              sample_rate,
    float              initial_value)
  : previous_output_(initial_value), sample_count_(0), current_output_(initial_value), is_ready_(false)
{
    float alpha;

    switch (param_type)
    {
        case AlphaParameterType::ALPHA_DIRECT:
            // Use alpha value directly
            if (param_value < 0.0f || param_value > 1.0f)
            {
                throw std::invalid_argument("Alpha value must be between 0.0 and 1.0");
            }
            alpha = param_value;
            break;

        case AlphaParameterType::ALPHA_FROM_CUTOFF_FREQUENCY:
            // Calculate alpha from cutoff frequency
            if (param_value <= 0.0f || sample_rate <= 0.0f)
            {
                throw std::invalid_argument("Cutoff frequency and sample rate must be greater than 0");
            }
            // Formula: α = 1 - exp(-2π * fc / fs)
            // where fc is cutoff frequency and fs is sample rate
            alpha = 1.0f - std::exp(-2.0f * static_cast<float>(M_PI) * param_value / sample_rate);
            break;

        default:
            throw std::invalid_argument("Invalid alpha parameter type");
    }

    config_.type           = FilterType::EXPONENTIAL_AVERAGE;
    config_.alpha          = alpha;
    config_.is_initialized = true;
}

float ExponentialFilter::process(float input)
{
    // Exponential moving average: output = alpha * input + (1 - alpha) * previous_output
    current_output_ = config_.alpha * input + (1.0f - config_.alpha) * previous_output_;

    // Update previous output for next iteration
    previous_output_ = current_output_;
    sample_count_++;

    // Mark as ready after processing first sample
    if (!is_ready_)
    {
        is_ready_ = true;
    }

    return current_output_;
}

float ExponentialFilter::getOutput() const
{
    return current_output_;
}

void ExponentialFilter::reset()
{
    sample_count_ = 0;
    is_ready_     = false;
    // Reset to the last previous output to avoid jolting/discontinuity
    // Set previous_output to current_output to maintain continuity
    // The filter will continue smoothly from the current state
    previous_output_ = current_output_;
}

bool ExponentialFilter::isReady() const
{
    return is_ready_;
}

uint32_t ExponentialFilter::getSampleCount() const
{
    return sample_count_;
}

FilterConfig ExponentialFilter::getConfig() const
{
    return config_;
}

// CombinedFilter implementation
CombinedFilter::CombinedFilter(std::vector<std::unique_ptr<IFilter>> filters)
  : filters_(std::move(filters)), current_output_(0.0f)
{
    if (filters_.empty())
    {
        throw std::invalid_argument("CombinedFilter requires at least one filter");
    }
}

float CombinedFilter::process(float input)
{
    float output = input;

    // Process through each filter in sequence
    for (auto &filter : filters_)
    {
        output = filter->process(output);
    }

    current_output_ = output;
    return output;
}

float CombinedFilter::getOutput() const
{
    return current_output_;
}

void CombinedFilter::reset()
{
    for (auto &filter : filters_)
    {
        filter->reset();
    }
    current_output_ = 0.0f;
}

bool CombinedFilter::isReady() const
{
    // All filters must be ready
    for (const auto &filter : filters_)
    {
        if (!filter->isReady())
        {
            return false;
        }
    }
    return true;
}

uint32_t CombinedFilter::getSampleCount() const
{
    // Return the minimum sample count among all filters
    uint32_t min_count = UINT32_MAX;
    for (const auto &filter : filters_)
    {
        min_count = std::min(min_count, filter->getSampleCount());
    }
    return min_count;
}

FilterConfig CombinedFilter::getConfig() const
{
    // Return config of first filter (could be enhanced)
    return filters_[0]->getConfig();
}

// FilterFactory implementation
std::unique_ptr<IFilter> FilterFactory::createMovingAverage(uint32_t window_size, float initial_value)
{
    if (window_size == 0)
    {
        return nullptr;
    }

    return std::make_unique<MovingAverageFilter>(window_size, initial_value);
}

std::unique_ptr<IFilter> FilterFactory::createExponential(float alpha, float initial_value)
{
    if (alpha < 0.0f || alpha > 1.0f)
    {
        return nullptr;
    }

    return std::make_unique<ExponentialFilter>(alpha, initial_value);
}

std::unique_ptr<IFilter> FilterFactory::createExponential(
    AlphaParameterType param_type,
    float              param_value,
    float              sample_rate,
    float              initial_value)
{
    try
    {
        return std::make_unique<ExponentialFilter>(param_type, param_value, sample_rate, initial_value);
    }
    catch (const std::invalid_argument &)
    {
        return nullptr;
    }
}

std::unique_ptr<IFilter> FilterFactory::createCombined(std::vector<std::unique_ptr<IFilter>> filters)
{
    if (filters.empty())
    {
        return nullptr;
    }

    return std::make_unique<CombinedFilter>(std::move(filters));
}

// FilterManager implementation
FilterManager &FilterManager::getInstance()
{
    static FilterManager instance;
    return instance;
}

bool FilterManager::init(uint32_t max_filters)
{
    if (max_filters == 0)
    {
        return false;
    }

    cleanup();

    filters_.reserve(max_filters);
    max_filters_ = max_filters;
    next_id_     = 1;

    return true;
}

uint32_t FilterManager::registerFilter(std::unique_ptr<IFilter> filter)
{
    if (filter == nullptr)
    {
        return 0;
    }

    if (filters_.size() >= max_filters_)
    {
        return 0;
    }

    uint32_t filter_id = next_id_++;
    filters_.push_back(std::move(filter));

    return filter_id;
}

float FilterManager::processFilter(uint32_t filter_id, float input)
{
    if (filter_id == 0 || filter_id > filters_.size())
    {
        return input; // Return input unchanged if filter not found
    }

    uint32_t index = filter_id - 1;
    if (filters_[index] == nullptr)
    {
        return input;
    }

    return filters_[index]->process(input);
}

float FilterManager::getFilterOutput(uint32_t filter_id)
{
    if (filter_id == 0 || filter_id > filters_.size())
    {
        return 0.0f;
    }

    uint32_t index = filter_id - 1;
    if (filters_[index] == nullptr)
    {
        return 0.0f;
    }

    return filters_[index]->getOutput();
}

bool FilterManager::resetFilter(uint32_t filter_id)
{
    if (filter_id == 0 || filter_id > filters_.size())
    {
        return false;
    }

    uint32_t index = filter_id - 1;
    if (filters_[index] == nullptr)
    {
        return false;
    }

    filters_[index]->reset();
    return true;
}

bool FilterManager::isFilterReady(uint32_t filter_id)
{
    if (filter_id == 0 || filter_id > filters_.size())
    {
        return false;
    }

    uint32_t index = filter_id - 1;
    if (filters_[index] == nullptr)
    {
        return false;
    }

    return filters_[index]->isReady();
}

bool FilterManager::unregisterFilter(uint32_t filter_id)
{
    if (filter_id == 0 || filter_id > filters_.size())
    {
        return false;
    }

    uint32_t index = filter_id - 1;
    if (filters_[index] == nullptr)
    {
        return false;
    }

    filters_[index] = nullptr;
    return true;
}

uint32_t FilterManager::getFilterCount() const
{
    uint32_t count = 0;
    for (const auto &filter : filters_)
    {
        if (filter != nullptr)
        {
            count++;
        }
    }
    return count;
}

void FilterManager::cleanup()
{
    filters_.clear();
    max_filters_ = 0;
    next_id_     = 1;
}

} // namespace app
