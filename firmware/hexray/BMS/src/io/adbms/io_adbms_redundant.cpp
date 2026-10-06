#include "io_adbms_internal.hpp"
#include "io_adbms.hpp"
#include "util_errorCodes.hpp"
#include "io_semaphore.hpp"
#include <algorithm>

namespace {
    using namespace io::adbms;

    ChainHealth latest_health{};
    io::semaphore redunant_lock{true};

    bool allOk(const Segments<result<RegBuffer>> &r) {
        return std::ranges::all_of(r, [](const auto &seg) { return seg.has_value(); });
    }
}

namespace io::adbms {
    int8_t detectBreakLocation(const Segments<result<RegBuffer>> &fwd, const Segments<result<RegBuffer>> &rev) {
        int8_t last_fwd_good = -1;
        for (int8_t seg = 0; seg < NUM_SEGMENTS; seg++){
            if (fwd[seg].has_value())
                last_fwd_good = seg;
            else 
                break;
        }

        int8_t first_rev_good = NUM_SEGMENTS;
        for (int8_t seg = NUM_SEGMENTS - 1; seg >= 0; seg--) {
            if (rev[seg].has_value()) 
                first_rev_good = seg;
            else
                break;
        }   

        // TODO: recheck this logic
        if (last_fwd_good < 0 || first_rev_good >= NUM_SEGMENTS)
            return -1;                                 
        if (first_rev_good != last_fwd_good + 1)
            return -1;                                 
        return last_fwd_good;  
    }

    ChainHealth classifyChain(const Segments<result<RegBuffer>> &fwd, const Segments<result<RegBuffer>> &rev) {
        ChainHealth h;
        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            const bool f = fwd[seg].has_value();
            const bool r = rev[seg].has_value();

            // LS and HS cannot communicate to segment
            if (!f & !r)
                h.unrecovered[seg] = true;
            else if (!f && r) {
                h.recovered[seg] = true;
                h.link_break_present = true;
            }
        }
        h.break_index = detectBreakLocation(fwd,rev);
        return h;
    }

    Segments<result<RegBuffer>> readRegGroupRedundant(const uint16_t cmd) {
        
        // Return default struct which indicates communication on LS is working
        Segments<result<RegBuffer>> fwd = readRegGroup(cmd, Port::LS);
        if (allOk(fwd)) {
            const io::unique_semaphore lock{ redunant_lock };
            latest_health = ChainHealth{};
            return fwd;
        }

        // Only uses HS when LS is unable to communicate to a segment
        const Segments<result<RegBuffer>> fwd_orig = fwd;
        const Segments<result<RegBuffer>> rev = readRegGroup(cmd, Port::HS);

        for (size_t seg = 0; seg < NUM_SEGMENTS; seg++) {
            if (fwd[seg].has_value())
                continue;
            if (rev[seg].has_value()) 
                fwd[seg] = rev[seg];
        }

        const io::unique_semaphore lock { redunant_lock };
        latest_health = classifyChain(fwd_orig, rev);
        return fwd;
    }

namespace chain {
    ChainHealth getLatestHealth()
    {
        const io::unique_semaphore lock{ redunant_lock };
        return latest_health;
    }
} // namespace chain
} // namespace io::adbms

