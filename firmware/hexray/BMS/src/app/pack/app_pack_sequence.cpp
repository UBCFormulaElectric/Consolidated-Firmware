#include "app_pack.hpp"
#include "app_pack_internal.hpp"
#include "app_timer.hpp"
#include "io_time.hpp"
namespace {

    app::pack::Snapshot local{};
    ADBMSState state;
    const SequenceState *current;
    const SequenceState *next;
    bool entry_done;

    void setNextState(const SequenceState *s) {
        next = s;
    }

    void runStateMachine() {
        if (!entry_done) {
            entry_done = current->run_on_entry == nullptr || current->run_on_entry();
            return;
        }
        if (next != current) {
            if (current->run_on_exit != nullptr && !current->run_on_exit()) 
                return;
            current = next;
            entry_done = false;
            return;
        }

        if (current->run_on_tick != nullptr)
            current->run_on_tick();
    }

    bool measureOnEntry() {

    }
    void measureOnTick() {

    }
    bool measureOnExit() {

    }

    bool balanceOnEntry() { 

    }
    void balanceOnTick() {

    }
    bool balanceOnExit() {

    }

    bool diagnosticOnEntry() {

    }
    void diagnosticOnTick() {

    }
    void diagnosticOnExit() {

    }

    const SequenceState measure_state{ "MEASURE", measureOnEntry, measureOnTick, measureOnExit};
    const SequenceState balance_state{ "BALANCE", balanceOnEntry, balanceOnTick, balanceOnExit};
    const SequenceState diagnostic_state{ "DIAGNOSTIC", diagnosticOnEntry, diagnosticOnTick, diagnosticOnExit};

    //state machine
    //everything is non-blocking

    //measure state
    //run and read cadc measurements continuosly (with redundancy check)
    //run sadc measurement continuosuly (dont need to read it)
    //run auxadc measurements 
    //run flag reg reads

    //balance state 
    //run and read cadc measurements continuosly (without redundancy check)
    //run auxadc measurements 
    //run flag reg reads
    
    //diagnostic state
    //run and read cadc measurements continuosly (with redundancy check)
    //run sadc measurement single shot
    //run and read cadc measurements continuosly (without redundancy check)
    //run sadc measurement single shot even owc
    //run sadc measurement single shot odd owc
    //run auxadc measurements 
    //run flag reg reads

    //if the car is balacning then state machine will stay in balance state and go to diagnostic state every once and a while
    //if the car is not balancing then state machine will stay in measure state and go to diagnostic state every once and a while

}

namespace app::pack::sequence {
    void init() {
        local = Snapshot{};
        setNextState(balance_state);
    }

    void tick() {
        bool diag_due = diag.updateAndGetState() == app::Timer::TimerState::EXPIRED;
        //get balancing request (balance/not balance)

        switch (state) {
            case ADBMSState::MEASURE:
                if (!cadc_started) {
                    if (io::adbms::command::startCADC(true)) {
                        cadc_started = true;
                        red_on = true;
                    }
                } else {
                    if (io::adbms::read::CADC(red_on))
                }
                break;
            case ADBMSState::BALANCE:
                if (!) {

                }

                break;
            case ADBMSState::DIAGNOSTIC:

                break;
        }
    }
}
