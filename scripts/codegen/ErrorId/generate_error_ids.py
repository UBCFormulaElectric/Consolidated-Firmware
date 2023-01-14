import cantools
import argparse
import os
from scripts.utilities.supported_boards import get_board_names

ERRORID_ENUM_TEMPLATE = '''\
#pragma once

#define DCM_WARNINGS \\
{dcm_warnings}

#define DIM_WARNINGS \\
{dim_warnings}

#define FSM_WARNINGS \\
{fsm_warnings}

#define PDM_WARNINGS \\
{pdm_warnings}

#define GSM_WARNINGS \\
{gsm_warnings}

#define DCM_AIR_SHUTDOWN_ERRORS \\
{dcm_air_shutdown_errors}

#define DIM_AIR_SHUTDOWN_ERRORS \\
{dim_air_shutdown_errors}

#define FSM_AIR_SHUTDOWN_ERRORS \\
{fsm_air_shutdown_errors}

#define PDM_AIR_SHUTDOWN_ERRORS \\
{pdm_air_shutdown_errors}

#define GSM_AIR_SHUTDOWN_ERRORS \\
{gsm_air_shutdown_errors}

#define DCM_MOTOR_SHUTDOWN_ERRORS \\
{dcm_motor_shutdown_errors}

#define DIM_MOTOR_SHUTDOWN_ERRORS \\
{dim_motor_shutdown_errors}

#define FSM_MOTOR_SHUTDOWN_ERRORS \\
{fsm_motor_shutdown_errors}

#define PDM_MOTOR_SHUTDOWN_ERRORS \\
{pdm_motor_shutdown_errors}

#define GSM_MOTOR_SHUTDOWN_ERRORS \\
{gsm_motor_shutdown_errors}

enum ErrorId
{{
    DCM_WARNINGS
    DIM_WARNINGS
    FSM_WARNINGS
    PDM_WARNINGS
    GSM_WARNINGS
    DCM_AIR_SHUTDOWN_ERRORS
    DIM_AIR_SHUTDOWN_ERRORS
    FSM_AIR_SHUTDOWN_ERRORS
    PDM_AIR_SHUTDOWN_ERRORS
    GSM_AIR_SHUTDOWN_ERRORS
    DCM_MOTOR_SHUTDOWN_ERRORS
    DIM_MOTOR_SHUTDOWN_ERRORS
    FSM_MOTOR_SHUTDOWN_ERRORS
    PDM_MOTOR_SHUTDOWN_ERRORS
    GSM_MOTOR_SHUTDOWN_ERRORS
    NUM_ERROR_IDS,
}};
'''

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--dbc', help='Path to the DBC file', required=True)
    parser.add_argument('--output_path', help='Path to the output header file (.h)', required=True)
    args = parser.parse_args()

    database = cantools.database.load_file(args.dbc, database_format='dbc')

    enum_members = {
        'warning': {},
        'air_shutdown': {},
        'motor_shutdown': {},
    }

    # Find warning, AIR shutdown, and motor shutdown error CAN messages for
    # each board
    for board in get_board_names():
        try:
            can_msg = database.get_message_by_name(board + '_WARNINGS')
            enum_members['warning'][board] = \
                ['    %s_WARNING_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            if board != "BMS":
                raise KeyError('Could not find non critical error message for %s' % board)
        try:
            can_msg = database.get_message_by_name(board + '_AIR_SHUTDOWN_ERRORS')
            enum_members['air_shutdown'][board] = \
                ['    %s_AIR_SHUTDOWN_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            if board != "BMS":
                raise KeyError('Could not find AIR shutdown error message for %s' % board)

        try:
            can_msg = database.get_message_by_name(board + '_MOTOR_SHUTDOWN_ERRORS')
            enum_members['motor_shutdown'][board] = \
                ['    %s_MOTOR_SHUTDOWN_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            if board != "BMS":
                raise KeyError('Could not find motor shutdown error message for %s' % board)

    enum = ERRORID_ENUM_TEMPLATE.format(
        dcm_warnings   = '\n'.join(enum_members['warning']['DCM']),
        dim_warnings   = '\n'.join(enum_members['warning']['DIM']),
        fsm_warnings   = '\n'.join(enum_members['warning']['FSM']),
        pdm_warnings   = '\n'.join(enum_members['warning']['PDM']),
        gsm_warnings   = '\n'.join(enum_members['warning']['GSM']),
        dcm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['DCM']),
        dim_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['DIM']),
        fsm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['FSM']),
        pdm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['PDM']),
        gsm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['GSM']),
        dcm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['DCM']),
        dim_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['DIM']),
        fsm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['FSM']),
        pdm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['PDM']),
        gsm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['GSM']))

    # Generate output folder if it doesn't exist yet
    output_dir = os.path.dirname(args.output_path)
    output_dir = os.getcwd() if output_dir == '' else output_dir
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Write file to disk
    with open(args.output_path, 'w') as fout:
        fout.write(enum)
