#pragma once

#include "App_BoardEnum.h"
#include "App_SharedExitCode.h"

/**
 * Allocate and initialize a group of board status LEDs
 * @param turn_on_dim_red A function that can be called to turn the DIM status
 *                        LED red
 * @param turn_on_dim_green A function that can be called to turn the DIM status
 *                          LED green
 * @param turn_on_dim_blue A function that can be called to turn the DIM status
 *                         LED blue
 * @param turn_on_dim_red A function that can be called to turn the DCM status
 *                        LED red
 * @param turn_on_dim_green A function that can be called to turn the DCM status
 *                          LED green
 * @param turn_on_dim_blue A function that can be called to turn the DCM status
 *                         LED blue
 * @param turn_on_dim_red A function that can be called to turn the FSM status
 *                        LED red
 * @param turn_on_dim_green A function that can be called to turn the FSM status
 *                          LED green
 * @param turn_on_dim_blue A function that can be called to turn the FSM status
 *                         LED blue
 * @param turn_on_dim_red A function that can be called to turn the BMS status
 *                        LED red
 * @param turn_on_dim_green A function that can be called to turn the BMS status
 *                          LED green
 * @param turn_on_dim_blue A function that can be called to turn the BMS status
 *                         LED blue
 * @param turn_on_dim_red A function that can be called to turn the PDM status
 *                        LED red
 * @param turn_on_dim_green A function that can be called to turn the PDM status
 *                          LED green
 * @param turn_on_dim_blue A function that can be called to turn the PDM status
 *                         LED blue
 * @return The created group of board status LEDs, whose ownership is given to
 *         the caller
 */
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
    void (*turn_on_pdm_blue)(void));

/**
 * Deallocate the memory used by the given group of board status LEDs
 * @param board_status_leds The group of board status LEDs to deallocate
 */
void App_BoardStatusLeds_Destroy(struct BoardStatusLeds *board_status_leds);

/**
 * Turn the status LED for the given board red, using the given group of board
 * status LEDs
 * @param board_status_leds The group of board status LEDs containing the status
 *                          LED for the given board
 * @param board The board of the status LED to turn red
 * @return EXIT_CODE_INVALID_ARGS if board isn't valid
 */
ExitCode App_BoardStatusLeds_TurnOnRed(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board);

/**
 * Turn the status LED for the given board green, using the given group of board
 * status LEDs
 * @param board_status_leds The group of board status LEDs containing the status
 *                          LED for the given board
 * @param board The board of the status LED to turn green
 * @return EXIT_CODE_INVALID_ARGS if board isn't valid
 */
ExitCode App_BoardStatusLeds_TurnOnGreen(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board);

/**
 * Turn the status LED for the given board blue, using the given group of board
 * status LEDs
 * @param board_status_leds The group of board status LEDs containing the status
 *                          LED for the given board
 * @param board The board of the status LED to turn blue
 * @return EXIT_CODE_INVALID_ARGS if board isn't valid
 */
ExitCode App_BoardStatusLeds_TurnOnBlue(
    const struct BoardStatusLeds *board_status_leds,
    enum Board                    board);
