#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <string.h>

typedef struct __attribute__((__packed__))
{
    uint16_t SC;   // sum of cells
    uint16_t ITMP; // internal temperature
    uint16_t VA;   // analog power supply voltage
} StatA;
static_assert(sizeof(StatA) == REGISTER_GROUP_SIZE);

typedef struct __attribute__((__packed__))
{
    uint16_t VD;        // digital power supply voltage
    uint32_t CVBF : 24; // cell voltage bound faults

    // final byte
    uint8_t THSD : 1;    // thermal shutdown
    uint8_t MUXFAIL : 1; // mux fail
    uint8_t RSVD : 2;    // reserved bits
    uint8_t REV : 4;     // revision code
} StatB;
static_assert(sizeof(StatB) == REGISTER_GROUP_SIZE);

ExitCode io_ltc6813_clearStatRegisters()
{
#define CLRSTAT (0x0713)
    return io_ltc6813_sendCommand(CLRSTAT);
}

ExitCode io_ltc6813_startInternalADCConversions(const ADCSpeed speed)
{
    const uint16_t MD_shift_7 = (speed & 0x3) << 7;
#define CHST (0x0)
#define ADSTAT (0x0468 | MD_shift_7 | CHST)
    return io_ltc6813_sendCommand(ADSTAT);
}

void io_ltc6813_getStatus(StatusRegGroups status[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
{
    for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        success[i] = EXIT_INDETERMINATE;

    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
            success[i] = poll_ok;
        return;
    }

#define RDSTATA (0x0010)
    struct
    {
        StatA stat;
        PEC   pec;
    } reg_stat_a[NUM_SEGMENTS];
    static_assert(sizeof(reg_stat_a) == NUM_SEGMENTS * (sizeof(StatA) + sizeof(PEC)));
    const ltc6813_tx tx_cmd        = io_ltc6813_build_tx_cmd(RDSTATA);
    const ExitCode   stat_a_status = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)reg_stat_a, sizeof(reg_stat_a));
    if (IS_EXIT_ERR(stat_a_status))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
            success[i] = stat_a_status;
        return;
    }

#define RDSTATB (0x0012)
    struct
    {
        StatB stat;
        PEC   pec;
    } reg_stat_b[NUM_SEGMENTS];
    static_assert(sizeof(reg_stat_b) == NUM_SEGMENTS * (sizeof(StatB) + sizeof(PEC)));
    const ltc6813_tx tx_cmd_2      = io_ltc6813_build_tx_cmd(RDSTATB);
    const ExitCode   stat_b_status = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_cmd_2, sizeof(tx_cmd_2), (uint8_t *)reg_stat_b, sizeof(reg_stat_b));
    if (IS_EXIT_ERR(stat_b_status))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
            success[i] = stat_b_status;
        return;
    }

    for (int seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        const StatA *reg_a = &reg_stat_a[(NUM_SEGMENTS - 1) - seg].stat;
        const StatB *reg_b = &reg_stat_b[(NUM_SEGMENTS - 1) - seg].stat;
        if (!io_ltc6813_check_pec((uint8_t *)reg_a, sizeof(StatA), &reg_stat_a[(NUM_SEGMENTS - 1) - seg].pec) ||
            !io_ltc6813_check_pec((uint8_t *)reg_b, sizeof(StatB), &reg_stat_b[(NUM_SEGMENTS - 1) - seg].pec))
        {
            success[seg] = EXIT_CODE_CHECKSUM_FAIL;
            continue;
        }
        success[seg]        = EXIT_CODE_OK;
        status[seg].SC      = reg_a->SC;
        status[seg].ITMP    = reg_a->ITMP;
        status[seg].VA      = reg_a->VA;
        status[seg].VD      = reg_b->VD;
        status[seg].CVBF    = reg_b->CVBF;
        status[seg].THSD    = reg_b->THSD;
        status[seg].MUXFAIL = reg_b->MUXFAIL;
        status[seg].RSVD    = reg_b->RSVD;
        status[seg].REV     = reg_b->REV;
    }
}