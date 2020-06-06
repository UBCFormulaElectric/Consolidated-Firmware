import cantools
import argparse
import os

from scripts.utilities.supported_boards import get_board_names


ERRORID_ENUM_TEMPLATE = '''\
#pragma once

enum ErrorId
{{
    // BMS non-critical errors
{bms_non_critical_errors}

    // DCM non-critical errors
{dcm_non_critical_errors}

    // DIM non-critical errors
{dim_non_critical_errors}

    // FSM non-critical errors
{fsm_non_critical_errors}

    // PDM non-critical errors
{pdm_non_critical_errors}

    // BMS critical errors
{bms_critical_errors}

    // DCM critical errors
{dcm_critical_errors}

    // DIM critical errors
{dim_critical_errors}

    // FSM critical errors
{fsm_critical_errors}

    // PDM critical errors
{pdm_critical_errors}

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
                ['    %s_CRITICAL_%s,' %(board, signal.name.upper()) for signal in can_msg.signals]
        except KeyError:
            raise KeyError('Could not find critical error message for %s' % board)

        try:
            can_msg = database.get_message_by_name(board + '_NON_CRITICAL_ERRORS')
            enum_members['non_critical'][board] = \
                ['    %s_NON_CRITICAL_%s,' %(board, signal.name.upper()) for signal in can_msg.signals]
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