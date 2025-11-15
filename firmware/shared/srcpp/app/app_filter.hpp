#pragma once

#include <cstdint>
#include <cstdbool>
#include <memory>
#include <vector>
#include <functional>
#include "app_utils.hpp"

namespace app
{

/**
 * @brief Enumeration for alpha parameter specification type
 */
enum class AlphaParameterType : uint8_t
{
    ALPHA_DIRECT,               // Use alpha value directly
    ALPHA_FROM_CUTOFF_FREQUENCY // Calculate alpha from cutoff frequency and sample rate
};

/**
 * @brief Filter types supported by the filtering system
 */
enum class FilterType : uint8_t
{
    MOVING_AVERAGE = 0,
    EXPONENTIAL_AVERAGE,
    COMBINED, // For future use with multiple filters
    NUM_FILTER_TYPES
};

/**
 * @brief Filter configuration structure
 */
struct FilterConfig
{
    FilterType type;
    uint32_t   window_size;    // For moving average
    float      alpha;          // For exponential average (0.0 to 1.0)
    bool       is_initialized; // Internal state flag

    FilterConfig() : type(FilterType::MOVING_AVERAGE), window_size(10), alpha(0.1f), is_initialized(false) {}
};

/**
 * @brief Base filter interface - all filters implement this
 */
class IFilter
{
  public:
    virtual ~IFilter() = default;

    /**
     * @brief Process a new input sample
     * @param input New input value
     * @return Filtered output value
     */
    virtual float process(float input) = 0;

    /**
     * @brief Get current filter output without processing new input
     * @return Current filtered output
     */
    virtual float getOutput() const = 0;

    /**
     * @brief Reset filter to initial state
     */
    virtual void reset() = 0;

    /**
     * @brief Check if filter is ready (has enough samples)
     * @return true if ready, false otherwise
     */
    virtual bool isReady() const = 0;

    /**
     * @brief Get number of samples processed
     * @return Sample count
     */
    virtual uint32_t getSampleCount() const = 0;

    /**
     * @brief Get filter configuration
     * @return Filter configuration
     */
    virtual FilterConfig getConfig() const = 0;
};

/**
 * @brief Moving average filter implementation
 */
class MovingAverageFilter : public IFilter
{
  public:
    explicit MovingAverageFilter(uint32_t window_size, float initial_value = 0.0f);
    ~MovingAverageFilter() override = default;

    float        process(float input) override;
    float        getOutput() const override;
    void         reset() override;
    bool         isReady() const override;
    uint32_t     getSampleCount() const override;
    FilterConfig getConfig() const override;

  private:
    FilterConfig       config_;
    std::vector<float> buffer_;
    uint32_t           buffer_index_;
    uint32_t           sample_count_;
    float              current_output_;
    float              sum_;
    bool               is_ready_;
};

/**
 * @brief Exponential moving average filter implementation
 */
class ExponentialFilter : public IFilter
{
  public:
    // Constructor with direct alpha value
    explicit ExponentialFilter(float alpha, float initial_value = 0.0f);
    // Constructor with enum-based parameter (alpha or cutoff frequency)
    ExponentialFilter(AlphaParameterType param_type, float param_value, float sample_rate, float initial_value = 0.0f);
    ~ExponentialFilter() override = default;

    float        process(float input) override;
    float        getOutput() const override;
    void         reset() override;
    bool         isReady() const override;
    uint32_t     getSampleCount() const override;
    FilterConfig getConfig() const override;

  private:
    FilterConfig config_;
    float        previous_output_;
    uint32_t     sample_count_;
    float        current_output_;
    bool         is_ready_;
};

/**
 * @brief Combined filter for chaining multiple filters
 */
class CombinedFilter : public IFilter
{
  public:
    explicit CombinedFilter(std::vector<std::unique_ptr<IFilter>> filters);
    ~CombinedFilter() override = default;

    float        process(float input) override;
    float        getOutput() const override;
    void         reset() override;
    bool         isReady() const override;
    uint32_t     getSampleCount() const override;
    FilterConfig getConfig() const override;

  private:
    std::vector<std::unique_ptr<IFilter>> filters_;
    float                                 current_output_;
};

/**
 * @brief Filter factory for creating filters
 */
class FilterFactory
{
  public:
    static std::unique_ptr<IFilter> createMovingAverage(uint32_t window_size, float initial_value = 0.0f);
    static std::unique_ptr<IFilter> createExponential(float alpha, float initial_value = 0.0f);
    static std::unique_ptr<IFilter> createExponential(
        AlphaParameterType param_type,
        float              param_value,
        float              sample_rate,
        float              initial_value = 0.0f);
    static std::unique_ptr<IFilter> createCombined(std::vector<std::unique_ptr<IFilter>> filters);
};

/**
 * @brief Global filter manager for managing multiple filters
 */
class FilterManager
{
  public:
    static FilterManager &getInstance();

    /**
     * @brief Initialize the filter manager
     * @param max_filters Maximum number of filters
     * @return true on success, false on failure
     */
    bool init(uint32_t max_filters);

    /**
     * @brief Register a filter and get its ID
     * @param filter Unique pointer to filter
     * @return Filter ID on success, 0 on failure
     */
    uint32_t registerFilter(std::unique_ptr<IFilter> filter);

    /**
     * @brief Process input through a registered filter
     * @param filter_id ID of the filter
     * @param input Input value
     * @return Filtered output
     */
    float processFilter(uint32_t filter_id, float input);

    /**
     * @brief Get output from a registered filter
     * @param filter_id ID of the filter
     * @return Current filter output
     */
    float getFilterOutput(uint32_t filter_id);

    /**
     * @brief Reset a registered filter
     * @param filter_id ID of the filter
     * @return true on success, false on failure
     */
    bool resetFilter(uint32_t filter_id);

    /**
     * @brief Check if a filter is ready
     * @param filter_id ID of the filter
     * @return true if ready, false otherwise
     */
    bool isFilterReady(uint32_t filter_id);

    /**
     * @brief Unregister a filter
     * @param filter_id ID of the filter
     * @return true on success, false on failure
     */
    bool unregisterFilter(uint32_t filter_id);

    /**
     * @brief Get number of registered filters
     * @return Number of active filters
     */
    uint32_t getFilterCount() const;

    /**
     * @brief Cleanup all filters
     */
    void cleanup();

  private:
    FilterManager()                                 = default;
    ~FilterManager()                                = default;
    FilterManager(const FilterManager &)            = delete;
    FilterManager &operator=(const FilterManager &) = delete;

    std::vector<std::unique_ptr<IFilter>> filters_;
    uint32_t                              max_filters_;
    uint32_t                              next_id_;
};

} // namespace app
