#pragma once 

namespace io {
class PCM {
public: 
    /**
     * Enable the PCM.
     * @param enable Whether or not to enable the PCM.
     */
    void pcm_set(bool enable);

    /**
     * Toggle the PCM.
     */
    void pcm_toggle();

    /**
     * Check if the PCM is enabled.
     * @return true if the PCM is enabled, false otherwise.
     */
    bool pcm_enabled();
};
} // namespace io