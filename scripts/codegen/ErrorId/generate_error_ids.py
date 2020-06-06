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

#define BMS_CRITICAL_ERRORS \\
{bms_critical_errors}

#define DCM_CRITICAL_ERRORS \\
{dcm_critical_errors}

#define DIM_CRITICAL_ERRORS \\
{dim_critical_errors}

#define FSM_CRITICAL_ERRORS \\
{fsm_critical_errors}

#define PDM_CRITICAL_ERRORS \\
{pdm_critical_errors}

enum ErrorId
{{
    BMS_NON_CRITICAL_ERRORS
    DCM_NON_CRITICAL_ERRORS
    DIM_NON_CRITICAL_ERRORS
    FSM_NON_CRITICAL_ERRORS
    PDM_NON_CRITICAL_ERRORS
    BMS_CRITICAL_ERRORS
    DCM_CRITICAL_ERRORS
    DIM_CRITICAL_ERRORS
    FSM_CRITICAL_ERRORS
    PDM_CRITICAL_ERRORS
    NUM_ERROR_IDS,
}};
'''

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--dbc', help='Path to the DBC file')
    parser.add_argument('--output_path', help='Path to the output header file (.h)')
    args = parser.parse_args()

    database = cantools.database.load_file(args.dbc, database_format='dbc')

    enum_members = {
        'critical': {},
        'non_critical': {}
    }

    # Find non-critical and critical error CAN messages for each board
    for board in get_board_names():
        try:
            can_msg = database.get_message_by_name(board + '_CRITICAL_ERRORS')
            enum_members['critical'][board] = \
                ['    %s_CRITICAL_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find critical error message for %s' % board)

        try:
            can_msg = database.get_message_by_name(board + '_NON_CRITICAL_ERRORS')
            enum_members['non_critical'][board] = \
                ['    %s_NON_CRITICAL_%s, \\' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find critical error message for %s' % board)

    enum = ERRORID_ENUM_TEMPLATE.format(
        bms_non_critical_errors = '\n'.join(enum_members['non_critical']['BMS']),
        dcm_non_critical_errors = '\n'.join(enum_members['non_critical']['DCM']),
        dim_non_critical_errors = '\n'.join(enum_members['non_critical']['DIM']),
        fsm_non_critical_errors = '\n'.join(enum_members['non_critical']['FSM']),
        pdm_non_critical_errors = '\n'.join(enum_members['non_critical']['PDM']),
        bms_critical_errors  = '\n'.join(enum_members['critical']['BMS']),
        dcm_critical_errors  = '\n'.join(enum_members['critical']['DCM']),
        dim_critical_errors  = '\n'.join(enum_members['critical']['DIM']),
        fsm_critical_errors  = '\n'.join(enum_members['critical']['FSM']),
        pdm_critical_errors  = '\n'.join(enum_members['critical']['PDM']))

    # Generate output folder if it doesn't exist yet
    output_dir = os.path.dirname(args.output_path)
    output_dir = os.getcwd() if output_dir is '' else output_dir
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Write file to disk
    with open(args.output_path, 'w') as fout:
        fout.write(enum)