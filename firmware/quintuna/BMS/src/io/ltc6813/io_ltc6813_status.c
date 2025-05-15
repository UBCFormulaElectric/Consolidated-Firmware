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
    RETURN_IF_ERR(io_ltc6813_clearStatRegisters());
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
#define RDSTATB (0x0012)
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

    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        if (!io_ltc6813_check_pec((uint8_t *)&reg_stat_a[i].stat, sizeof(reg_stat_a[i].stat), &reg_stat_a[i].pec) ||
            !io_ltc6813_check_pec((uint8_t *)&reg_stat_b[i].stat, sizeof(reg_stat_b[i].stat), &reg_stat_b[i].pec))
        {
            continue;
        }
        success[i]        = EXIT_CODE_OK;
        status[i].SC      = reg_stat_a[i].stat.SC;
        status[i].ITMP    = reg_stat_a[i].stat.ITMP;
        status[i].VA      = reg_stat_a[i].stat.VA;
        status[i].VD      = reg_stat_b[i].stat.VD;
        status[i].CVBF    = reg_stat_b[i].stat.CVBF;
        status[i].THSD    = reg_stat_b[i].stat.THSD;
        status[i].MUXFAIL = reg_stat_b[i].stat.MUXFAIL;
        status[i].RSVD    = reg_stat_b[i].stat.RSVD;
        status[i].REV     = reg_stat_b[i].stat.REV;
    }
}