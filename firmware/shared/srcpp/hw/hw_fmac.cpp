#include "hw_fmac.hpp"
#include "main.h"
#include "hw_utils.hpp"

namespace hw::fmac
{

std::expected<void, ErrorCode> FmacIir::init(const FmacIirConfig &config)
{
    // Validate config
    if (config.numBTaps < IIR_MIN_B_TAPS || config.numBTaps > IIR_MAX_B_TAPS)
        return std::unexpected(ErrorCode::INVALID_ARGS);
    if (config.numATaps < IIR_MIN_A_TAPS || config.numATaps > IIR_MAX_A_TAPS)
        return std::unexpected(ErrorCode::INVALID_ARGS);
    if (config.gainExponent < IIR_MIN_GAIN || config.gainExponent > IIR_MAX_GAIN)
        return std::unexpected(ErrorCode::INVALID_ARGS);
    if (config.coefB == nullptr || config.coefA == nullptr)
        return std::unexpected(ErrorCode::INVALID_ARGS);

    const uint16_t x1size = config.numBTaps + 2;
    const uint16_t x2size = config.numATaps + config.numBTaps;
    const uint16_t ysize  = config.numATaps + 2;
    const uint16_t x1Base = 0;
    const uint16_t x2Base = x1size;
    const uint16_t yBase  = x2Base + x2size;

    if (yBase + ysize > 256)
        return std::unexpected(ErrorCode::INVALID_ARGS);

    // HAL IIR config: P=len(B), Q=len(A), R=gain exponent 0..7
    FMAC_FilterConfigTypeDef filterConfig = {};
    filterConfig.InputBaseAddress         = static_cast<uint8_t>(x1Base);
    filterConfig.InputBufferSize          = static_cast<uint8_t>(x1size);
    filterConfig.InputThreshold           = FMAC_THRESHOLD_1;
    filterConfig.CoeffBaseAddress         = static_cast<uint8_t>(x2Base);
    filterConfig.CoeffBufferSize          = static_cast<uint8_t>(x2size);
    filterConfig.OutputBaseAddress        = static_cast<uint8_t>(yBase);
    filterConfig.OutputBufferSize         = static_cast<uint8_t>(ysize);
    filterConfig.OutputThreshold          = FMAC_THRESHOLD_1;
    filterConfig.pCoeffB                  = const_cast<int16_t *>(config.coefB);
    filterConfig.CoeffBSize               = config.numBTaps;
    filterConfig.pCoeffA                  = const_cast<int16_t *>(config.coefA);
    filterConfig.CoeffASize               = config.numATaps;
    filterConfig.InputAccess              = FMAC_BUFFER_ACCESS_POLLING;
    filterConfig.OutputAccess             = FMAC_BUFFER_ACCESS_POLLING;
    filterConfig.Clip                     = FMAC_CLIP_ENABLED; // clip output to 16-bit range
    filterConfig.Filter                   = FMAC_FUNC_IIR_DIRECT_FORM_1;
    filterConfig.P                        = config.numBTaps;
    filterConfig.Q                        = config.numATaps;
    filterConfig.R                        = config.gainExponent;

    // HAL Calls
    RETURN_IF_ERR_SILENT(hw_utils_convertHalStatus(HAL_FMAC_FilterConfig(m_handle, &filterConfig)));
    RETURN_IF_ERR_SILENT(hw_utils_convertHalStatus(HAL_FMAC_FilterPreload(
        m_handle, const_cast<int16_t *>(config.coefB), config.numBTaps, const_cast<int16_t *>(config.coefA),
        config.numATaps)));
    RETURN_IF_ERR_SILENT(hw_utils_convertHalStatus(HAL_FMAC_FilterStart(m_handle, &m_outputSample, &m_outputSize)));

    return {};
}

std::expected<void, ErrorCode> FmacIir::pushSample(int16_t sample_q15)
{
    uint16_t size = 1;
    return hw_utils_convertHalStatus(HAL_FMAC_AppendFilterData(m_handle, &sample_q15, &size));
}

std::expected<void, ErrorCode> FmacIir::popSample(int16_t *output_q15)
{
    RETURN_IF_ERR_SILENT(hw_utils_convertHalStatus(HAL_FMAC_PollFilterData(m_handle, HAL_MAX_DELAY)));
    *output_q15 = m_outputSample;
    return {};
}

std::expected<void, ErrorCode> FmacIir::process(int16_t sample_q15, int16_t *output_q15)
{
    RETURN_IF_ERR_SILENT(pushSample(sample_q15));
    RETURN_IF_ERR_SILENT(popSample(output_q15));
    return {};
}

std::expected<void, ErrorCode> FmacIir::stop()
{
    return hw_utils_convertHalStatus(HAL_FMAC_FilterStop(m_handle));
}

} // namespace hw::fmac
