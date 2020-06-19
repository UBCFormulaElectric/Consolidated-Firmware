import cantools
import argparse
import os
from scripts.utilities.supported_boards import get_board_names

ERRORID_ENUM_TEMPLATE = '''\
#pragma once

#define BMS_NON_CRITICAL_ERRORS \\
{bms_non_critical_errors}

#define DCM_NON_CRITICAL_ERRORS \\
{dcm_non_critical_errors}

#define DIM_NON_CRITICAL_ERRORS \\
{dim_non_critical_errors}

#define FSM_NON_CRITICAL_ERRORS \\
{fsm_non_critical_errors}

#define PDM_NON_CRITICAL_ERRORS \\
{pdm_non_critical_errors}

#define BMS_AIR_SHUTDOWN_ERRORS \\
{bms_air_shutdown_errors}

#define DCM_AIR_SHUTDOWN_ERRORS \\
{dcm_air_shutdown_errors}

#define DIM_AIR_SHUTDOWN_ERRORS \\
{dim_air_shutdown_errors}

#define FSM_AIR_SHUTDOWN_ERRORS \\
{fsm_air_shutdown_errors}

#define PDM_AIR_SHUTDOWN_ERRORS \\
{pdm_air_shutdown_errors}

#define BMS_MOTOR_SHUTDOWN_ERRORS \\
{bms_motor_shutdown_errors}

#define DCM_MOTOR_SHUTDOWN_ERRORS \\
{dcm_motor_shutdown_errors}

#define DIM_MOTOR_SHUTDOWN_ERRORS \\
{dim_motor_shutdown_errors}

#define FSM_MOTOR_SHUTDOWN_ERRORS \\
{fsm_motor_shutdown_errors}

#define PDM_MOTOR_SHUTDOWN_ERRORS \\
{pdm_motor_shutdown_errors}

enum ErrorId
{{
    BMS_NON_CRITICAL_ERRORS
    DCM_NON_CRITICAL_ERRORS
    DIM_NON_CRITICAL_ERRORS
    FSM_NON_CRITICAL_ERRORS
    PDM_NON_CRITICAL_ERRORS
    BMS_AIR_SHUTDOWN_ERRORS
    DCM_AIR_SHUTDOWN_ERRORS
    DIM_AIR_SHUTDOWN_ERRORS
    FSM_AIR_SHUTDOWN_ERRORS
    PDM_AIR_SHUTDOWN_ERRORS
    BMS_MOTOR_SHUTDOWN_ERRORS
    DCM_MOTOR_SHUTDOWN_ERRORS
    DIM_MOTOR_SHUTDOWN_ERRORS
    FSM_MOTOR_SHUTDOWN_ERRORS
    PDM_MOTOR_SHUTDOWN_ERRORS
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
        'non_critical': {},
        'air_shutdown': {},
        'motor_shutdown': {},
    }

    # Find non-critical, AIR shutdown, and motor shutdown error CAN messages for
    # each board
    for board in get_board_names():
        try:
            can_msg = database.get_message_by_name(board + '_NON_CRITICAL_ERRORS')
            enum_members['non_critical'][board] = \
                ['    %s_NON_CRITICAL_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find non critical error message for %s' % board)

        try:
            can_msg = database.get_message_by_name(board + '_AIR_SHUTDOWN_ERRORS')
            enum_members['air_shutdown'][board] = \
                ['    %s_AIR_SHUTDOWN_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find AIR shutdown error message for %s' % board)

        try:
            can_msg = database.get_message_by_name(board + '_MOTOR_SHUTDOWN_ERRORS')
            enum_members['motor_shutdown'][board] = \
                ['    %s_MOTOR_SHUTDOWN_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find motor shutdown error message for %s' % board)

    enum = ERRORID_ENUM_TEMPLATE.format(
        bms_non_critical_errors   = '\n'.join(enum_members['non_critical']['BMS']),
        dcm_non_critical_errors   = '\n'.join(enum_members['non_critical']['DCM']),
        dim_non_critical_errors   = '\n'.join(enum_members['non_critical']['DIM']),
        fsm_non_critical_errors   = '\n'.join(enum_members['non_critical']['FSM']),
        pdm_non_critical_errors   = '\n'.join(enum_members['non_critical']['PDM']),
        bms_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['BMS']),
        dcm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['DCM']),
        dim_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['DIM']),
        fsm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['FSM']),
        pdm_air_shutdown_errors   = '\n'.join(enum_members['air_shutdown']['PDM']),
        bms_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['BMS']),
        dcm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['DCM']),
        dim_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['DIM']),
        fsm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['FSM']),
        pdm_motor_shutdown_errors = '\n'.join(enum_members['motor_shutdown']['PDM']))

    # Generate output folder if it doesn't exist yet
    output_dir = os.path.dirname(args.output_path)
    output_dir = os.getcwd() if output_dir is '' else output_dir
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Write file to disk
    with open(args.output_path, 'w') as fout:
        fout.write(enum)
