#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <string.h>

#define GPIO9_MASK 0x08u     /* bit-3 of CFGBR.byte0 – Table 39 */
#define WRCFGB     0x0024u
#define RDCFGB     0x0026u

static ExitCode read_cfgb(CFGBR *cfg)
{
    ltc6813_tx tx = io_ltc6813_build_tx_cmd(RDCFGB);
    struct { CFGBR cfg; PEC pec; } rx;
    RETURN_IF_ERR(hw_spi_transmitThenReceive(&ltc6813_spi_ls,
                                             (uint8_t *)&tx,sizeof(tx),
                                             (uint8_t *)&rx,sizeof(rx)));
    if (!io_ltc6813_check_pec((uint8_t *)&rx.cfg,sizeof(CFGBR),&rx.pec))
        return EXIT_CODE_CHECKSUM_FAIL;
    *cfg = rx.cfg;
    return EXIT_CODE_OK;
}

ExitCode io_ltc6813_getGpio679(uint8_t *state)
{
    CFGBR cfg;
    RETURN_IF_ERR(read_cfgb(&cfg));
    *state = cfg.gpio_6_9;                  /* bit3-bit0 = GPIO9-6 */
    return EXIT_CODE_OK;
}

ExitCode io_ltc6813_setGpio9(bool level_high)
{
    CFGBR cfg;
    RETURN_IF_ERR(read_cfgb(&cfg));

    if (level_high) cfg.gpio_6_9 |=  GPIO9_MASK;   /* pull-down OFF  */
    else            cfg.gpio_6_9 &= ~GPIO9_MASK;   /* pull-down ON   */

    struct { ltc6813_tx cmd; CFGBR cfg; PEC pec; } tx = {
        .cmd = io_ltc6813_build_tx_cmd(WRCFGB),
        .cfg = cfg,
    };
    tx.pec = io_ltc6813_build_data_pec((uint8_t *)&tx.cfg,sizeof(CFGBR));
    return hw_spi_transmit(&ltc6813_spi_ls,(uint8_t *)&tx,sizeof(tx));
}