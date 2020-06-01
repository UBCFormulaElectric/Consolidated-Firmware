#include <assert.h>
#include <stdlib.h>
#include "App_BoardStatusLeds.h"

struct BoardStatusLeds
{
    void (*turn_on_dim_red)(void);
    void (*turn_on_dim_green)(void);
    void (*turn_on_dim_blue)(void);
    void (*turn_on_dcm_red)(void);
    void (*turn_on_dcm_green)(void);
    void (*turn_on_dcm_blue)(void);
    void (*turn_on_fsm_red)(void);
    void (*turn_on_fsm_green)(void);
    void (*turn_on_fsm_blue)(void);
    void (*turn_on_bms_red)(void);
    void (*turn_on_bms_green)(void);
    void (*turn_on_bms_blue)(void);
    void (*turn_on_pdm_red)(void);
    void (*turn_on_pdm_green)(void);
    void (*turn_on_pdm_blue)(void);
};

struct BoardStatusLeds *App_BoardStatusLeds_Create(
    void (*turn_on_dim_red)(void),
    void (*turn_on_dim_green)(void),
    void (*turn_on_dim_blue)(void),
    void (*turn_on_dcm_red)(void),
    void (*turn_on_dcm_green)(void),
    void (*turn_on_dcm_blue)(void),
    void (*turn_on_fsm_red)(void),
    void (*turn_on_fsm_green)(void),
    void (*turn_on_fsm_blue)(void),
    void (*turn_on_bms_red)(void),
    void (*turn_on_bms_green)(void),
    void (*turn_on_bms_blue)(void),
    void (*turn_on_pdm_red)(void),
    void (*turn_on_pdm_green)(void),
    void (*turn_on_pdm_blue)(void))
{
    struct BoardStatusLeds *board_status_leds =
        malloc(sizeof(struct BoardStatusLeds));
    assert(board_status_leds != NULL);

    board_status_leds->turn_on_dim_red   = turn_on_dim_red;
    board_status_leds->turn_on_dim_green = turn_on_dim_green;
    board_status_leds->turn_on_dim_blue  = turn_on_dim_blue;
    board_status_leds->turn_on_dcm_red   = turn_on_dcm_red;
    board_status_leds->turn_on_dcm_green = turn_on_dcm_green;
    board_status_leds->turn_on_dcm_blue  = turn_on_dcm_blue;
    board_status_leds->turn_on_fsm_red   = turn_on_fsm_red;
    board_status_leds->turn_on_fsm_green = turn_on_fsm_green;
    board_status_leds->turn_on_fsm_blue  = turn_on_fsm_blue;
    board_status_leds->turn_on_bms_red   = turn_on_bms_red;
    board_status_leds->turn_on_bms_green = turn_on_bms_green;
    board_status_leds->turn_on_bms_blue  = turn_on_bms_blue;
    board_status_leds->turn_on_pdm_red   = turn_on_pdm_red;
    board_status_leds->turn_on_pdm_green = turn_on_pdm_green;
    board_status_leds->turn_on_pdm_blue  = turn_on_pdm_blue;

    return board_status_leds;
}

void App_BoardStatusLeds_Destroy(struct BoardStatusLeds *board_status_leds)
{
    free(board_status_leds);
}

ExitCode App_BoardStatusLeds_TurnOnRed(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board)
{
    switch (board)
    {
        case DIM:
        {
            board_status_leds->turn_on_dim_red();
        }
        break;
        case DCM:
        {
            board_status_leds->turn_on_dcm_red();
        }
        break;
        case FSM:
        {
            board_status_leds->turn_on_fsm_red();
        }
        break;
        case BMS:
        {
            board_status_leds->turn_on_bms_red();
        }
        break;
        case PDM:
        {
            board_status_leds->turn_on_pdm_red();
        }
        break;
        default:
        {
            return EXIT_CODE_INVALID_ARGS;
        }
    }

    return EXIT_CODE_OK;
}

ExitCode App_BoardStatusLeds_TurnOnGreen(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board)
{
    switch (board)
    {
        case DIM:
        {
            board_status_leds->turn_on_dim_green();
        }
        break;
        case DCM:
        {
            board_status_leds->turn_on_dcm_green();
        }
        break;
        case FSM:
        {
            board_status_leds->turn_on_fsm_green();
        }
        break;
        case BMS:
        {
            board_status_leds->turn_on_bms_green();
        }
        break;
        case PDM:
        {
            board_status_leds->turn_on_pdm_green();
        }
        break;
        default:
        {
            return EXIT_CODE_INVALID_ARGS;
        }
    }

    return EXIT_CODE_OK;
}

ExitCode App_BoardStatusLeds_TurnOnBlue(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board)
{
    switch (board)
    {
        case DIM:
        {
            board_status_leds->turn_on_dim_blue();
        }
        break;
        case DCM:
        {
            board_status_leds->turn_on_dcm_blue();
        }
        break;

        case FSM:
        {
            board_status_leds->turn_on_fsm_blue();
        }
        break;
        case BMS:
        {
            board_status_leds->turn_on_bms_blue();
        }
        break;
        case PDM:
        {
            board_status_leds->turn_on_pdm_blue();
        }
        break;
        default:
        {
            return EXIT_CODE_INVALID_ARGS;
        }
    }

    return EXIT_CODE_OK;
}
