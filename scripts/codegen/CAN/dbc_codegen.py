from openpyxl import Workbook, load_workbook
import os

workbook = load_workbook("CanMsgs.xlsx", data_only=True)
worksheet = workbook.active
number_of_rows = 400
number_of_header_rows = 2
start_row = number_of_header_rows + 1

column_designation = {
    'CAN_ID':'B',
    'MESSAGE_NAME':'C',
    'MESSAGE_LENGTH':'D',
    'MESSAGE_SENDER_NODE':'E',
    'MESSAGE_PERIODICITY':'F',
    'SIGNAL_NAME':'G',
    'SIGNAL_LENGTH':'H',
    'SIGNAL_ENDIANNESS':'I',
    'SIGNAL_SIGNED_UNSIGNED':'J',
    'SIGNAL_SCALE':'K',
    'SIGNAL_OFFSET':'L',
    'SIGNAL_MINIMUM':'M',
    'SIGNAL_MAXIMUM':'N',
    'SIGNAL_DATA_TYPE':'O',
    'SIGNAL_UNIT':'P',
    'SIGNAL_REC_NODE1':'Q',
    'SIGNAL_REC_NODE2':'R',
    'SIGNAL_REC_NODE3':'S',
    'SIGNAL_REC_NODE4':'T',
    'SIGNAL_REC_NODE5':'U',
    'SIGNAL_REC_NODE6':'V',
    'SIGNAL_REC_NODE7':'W',
    'SIGNAL_REC_NODE8':'X',
    'SIGNAL_ENUM0':'Y',
    'SIGNAL_ENUM1':'Z',
    'SIGNAL_ENUM2':'AA',
    'SIGNAL_ENUM3':'AB',
    'SIGNAL_ENUM4':'AC',
    'SIGNAL_ENUM5':'AD',
    'SIGNAL_ENUM6':'AE',
}
with open("App_CanMsgs.dbc", 'w') as dbc_file:

    message_bit_total = 0

    dbc_file.write(
        (
        'VERSION ""\n'
        '\n'
        '\n'
        'NS_ :\n'
        '\n'
        'BS_:\n'
        '\n'
        'BU_: FSM DCM BMS PDM INV DIM LOGGER DEBUG\n'
        '\n'
        )
    )

    for signal in range(start_row, number_of_rows):

        signal_info = {
            'CAN_ID': str(worksheet[column_designation.get('CAN_ID') + str(signal)].value),
            'MESSAGE_NAME': str(worksheet[column_designation.get('MESSAGE_NAME') + str(signal)].value),
            'MESSAGE_LENGTH': str(worksheet[column_designation.get('MESSAGE_LENGTH') + str(signal)].value),
            'MESSAGE_SENDER_NODE': str(worksheet[column_designation.get('MESSAGE_SENDER_NODE') + str(signal)].value),
            'MESSAGE_PERIODICITY': str(worksheet[column_designation.get('MESSAGE_PERIODICITY') + str(signal)].value),
            'SIGNAL_NAME': str(worksheet[column_designation.get('SIGNAL_NAME') + str(signal)].value),
            'SIGNAL_LENGTH': worksheet[column_designation.get('SIGNAL_LENGTH') + str(signal)].value,
            'SIGNAL_ENDIANNESS': '@1' if str(worksheet[column_designation.get('SIGNAL_ENDIANNESS') + str(signal)].value) == 'Little' else '@0',
            'SIGNAL_SIGNED_UNSIGNED': '-' if str(worksheet[column_designation.get('SIGNAL_SIGNED_UNSIGNED') + str(signal)].value) == 'Signed' else '+',
            'SIGNAL_SCALE': str(worksheet[column_designation.get('SIGNAL_SCALE') + str(signal)].value),
            'SIGNAL_OFFSET': str(worksheet[column_designation.get('SIGNAL_OFFSET') + str(signal)].value),
            'SIGNAL_MINIMUM': str(worksheet[column_designation.get('SIGNAL_MINIMUM') + str(signal)].value),
            'SIGNAL_MAXIMUM': str(worksheet[column_designation.get('SIGNAL_MAXIMUM') + str(signal)].value),
            'SIGNAL_DATA_TYPE': str(worksheet[column_designation.get('SIGNAL_DATA_TYPE') + str(signal)].value),
            'SIGNAL_UNIT': str(worksheet[column_designation.get('SIGNAL_UNIT') + str(signal)].value) if worksheet[column_designation.get('SIGNAL_UNIT') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE1': str(worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE2': str(worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE3': str(worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE4': str(worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE5': str(worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE6': str(worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE7': str(worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value + ",") if worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE8': str(worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value) if worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value is not None else "",
            'SIGNAL_ENUM0': str(worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value),
            'SIGNAL_ENUM1': str(worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value),
            'SIGNAL_ENUM2': str(worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value),
            'SIGNAL_ENUM3': str(worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value),
            'SIGNAL_ENUM4': str(worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value),
            'SIGNAL_ENUM5': str(worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value),
            'SIGNAL_ENUM6': str(worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value),
}
        if worksheet[column_designation.get('CAN_ID') + str(signal)].value is not None:

            #If the CAN_ID is not the same as the previous signal
            if worksheet[column_designation.get('CAN_ID') + str(signal)].value != worksheet[column_designation.get('CAN_ID') + str(signal-1)].value:
                message_bit_total = 0
                dbc_file.write(
                    (
                    "\nBO_ " + signal_info.get('CAN_ID') + " " + signal_info.get('MESSAGE_NAME') + " : " + signal_info.get('MESSAGE_LENGTH') + " " + signal_info.get("MESSAGE_SENDER_NODE") + "\n"
                    )

                )

            dbc_file.write(
                (
                "\tSG_ " + signal_info.get('SIGNAL_NAME') + " : " + str(message_bit_total) + "|" + str(signal_info.get('SIGNAL_LENGTH')) +
                signal_info.get('SIGNAL_ENDIANNESS') + signal_info.get('SIGNAL_SIGNED_UNSIGNED') + " (" + signal_info.get('SIGNAL_SCALE') + "," +
                signal_info.get('SIGNAL_OFFSET') + ") [" + signal_info.get('SIGNAL_MINIMUM') + "|" + signal_info.get('SIGNAL_MAXIMUM') + "] \"" +
                signal_info.get("SIGNAL_UNIT") + "\" " + signal_info.get("SIGNAL_REC_NODE1") + signal_info.get("SIGNAL_REC_NODE2") + signal_info.get("SIGNAL_REC_NODE3")
                + signal_info.get("SIGNAL_REC_NODE4") + signal_info.get("SIGNAL_REC_NODE5") + signal_info.get("SIGNAL_REC_NODE6") + signal_info.get("SIGNAL_REC_NODE7")
                + signal_info.get("SIGNAL_REC_NODE8") + "\n"
                )
            )

            message_bit_total += int(signal_info.get('SIGNAL_LENGTH'))

    dbc_file.write(
        (
        '\n'
        'BA_DEF_  "BusType" STRING;\n'
        'BA_DEF_ BO_  "GenMsgCycleTime" INT 0 65535;\n'
        'BA_DEF_ SG_  "GenSigStartValue" INT 0 2147483647;\n'
        '\n'
        'BA_DEF_DEF_  "BusType" "CAN";\n'
        'BA_DEF_DEF_  "GenMsgCycleTime" 0;\n'
        'BA_DEF_DEF_  "GenSigStartValue" 0;\n'
        '\n'
        'BA_ "BusType" "CAN";\n'
        '\n'
        )
    )

    #Now do the periodicity definition
    for signal in range(start_row, number_of_rows):

        signal_info = {
            'CAN_ID': str(worksheet[column_designation.get('CAN_ID') + str(signal)].value),
            'MESSAGE_NAME': str(worksheet[column_designation.get('MESSAGE_NAME') + str(signal)].value),
            'MESSAGE_LENGTH': str(worksheet[column_designation.get('MESSAGE_LENGTH') + str(signal)].value),
            'MESSAGE_SENDER_NODE': str(worksheet[column_designation.get('MESSAGE_SENDER_NODE') + str(signal)].value),
            'MESSAGE_PERIODICITY': str(worksheet[column_designation.get('MESSAGE_PERIODICITY') + str(signal)].value),
            'SIGNAL_NAME': str(worksheet[column_designation.get('SIGNAL_NAME') + str(signal)].value),
            'SIGNAL_LENGTH': worksheet[column_designation.get('SIGNAL_LENGTH') + str(signal)].value,
            'SIGNAL_ENDIANNESS': '@1' if str(worksheet[column_designation.get('SIGNAL_ENDIANNESS') + str(signal)].value) == 'Little' else '@0',
            'SIGNAL_SIGNED_UNSIGNED': '-' if str(worksheet[column_designation.get('SIGNAL_SIGNED_UNSIGNED') + str(signal)].value) == 'Signed' else '+',
            'SIGNAL_SCALE': str(worksheet[column_designation.get('SIGNAL_SCALE') + str(signal)].value),
            'SIGNAL_OFFSET': str(worksheet[column_designation.get('SIGNAL_OFFSET') + str(signal)].value),
            'SIGNAL_MINIMUM': str(worksheet[column_designation.get('SIGNAL_MINIMUM') + str(signal)].value),
            'SIGNAL_MAXIMUM': str(worksheet[column_designation.get('SIGNAL_MAXIMUM') + str(signal)].value),
            'SIGNAL_DATA_TYPE': str(worksheet[column_designation.get('SIGNAL_DATA_TYPE') + str(signal)].value),
            'SIGNAL_UNIT': str(worksheet[column_designation.get('SIGNAL_UNIT') + str(signal)].value),
            'SIGNAL_REC_NODE1': str(worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE2': str(worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE3': str(worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE4': str(worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE5': str(worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE6': str(worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE7': str(worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE8': str(worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value is not None else "",
            'SIGNAL_ENUM0': str(worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value),
            'SIGNAL_ENUM1': str(worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value),
            'SIGNAL_ENUM2': str(worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value),
            'SIGNAL_ENUM3': str(worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value),
            'SIGNAL_ENUM4': str(worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value),
            'SIGNAL_ENUM5': str(worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value),
            'SIGNAL_ENUM6': str(worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value),
        }

        if worksheet[column_designation.get('CAN_ID') + str(signal)].value is not None and \
            worksheet[column_designation.get('MESSAGE_PERIODICITY') + str(signal)].value is not None and \
            worksheet[column_designation.get('CAN_ID') + str(signal)].value != worksheet[column_designation.get('CAN_ID') + str(signal-1)].value:
            dbc_file.write(
                (
                "BA_ \"GenMsgCycleTime\" BO_ " + signal_info.get('CAN_ID') + " " + signal_info.get('MESSAGE_PERIODICITY') + ";\n"
                )
            )

    dbc_file.write(
        (
        '\n'
        'BA_ "GenSigStartValue" SG_ 2  tx_overflow_count 0;\n'
        'BA_ "GenSigStartValue" SG_ 2  rx_overflow_count 0;\n'
        '\n'
        )
    )

    #Deal with floats
    for signal in range(start_row, number_of_rows):

        signal_info = {
            'CAN_ID': str(worksheet[column_designation.get('CAN_ID') + str(signal)].value),
            'MESSAGE_NAME': str(worksheet[column_designation.get('MESSAGE_NAME') + str(signal)].value),
            'MESSAGE_LENGTH': str(worksheet[column_designation.get('MESSAGE_LENGTH') + str(signal)].value),
            'MESSAGE_SENDER_NODE': str(worksheet[column_designation.get('MESSAGE_SENDER_NODE') + str(signal)].value),
            'MESSAGE_PERIODICITY': str(worksheet[column_designation.get('MESSAGE_PERIODICITY') + str(signal)].value),
            'SIGNAL_NAME': str(worksheet[column_designation.get('SIGNAL_NAME') + str(signal)].value),
            'SIGNAL_LENGTH': worksheet[column_designation.get('SIGNAL_LENGTH') + str(signal)].value,
            'SIGNAL_ENDIANNESS': '@1' if str(worksheet[column_designation.get('SIGNAL_ENDIANNESS') + str(signal)].value) == 'Little' else '@0',
            'SIGNAL_SIGNED_UNSIGNED': '-' if str(worksheet[column_designation.get('SIGNAL_SIGNED_UNSIGNED') + str(signal)].value) == 'Signed' else '+',
            'SIGNAL_SCALE': str(worksheet[column_designation.get('SIGNAL_SCALE') + str(signal)].value),
            'SIGNAL_OFFSET': str(worksheet[column_designation.get('SIGNAL_OFFSET') + str(signal)].value),
            'SIGNAL_MINIMUM': str(worksheet[column_designation.get('SIGNAL_MINIMUM') + str(signal)].value),
            'SIGNAL_MAXIMUM': str(worksheet[column_designation.get('SIGNAL_MAXIMUM') + str(signal)].value),
            'SIGNAL_DATA_TYPE': str(worksheet[column_designation.get('SIGNAL_DATA_TYPE') + str(signal)].value),
            'SIGNAL_UNIT': str(worksheet[column_designation.get('SIGNAL_UNIT') + str(signal)].value),
            'SIGNAL_REC_NODE1': str(worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE2': str(worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE3': str(worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE4': str(worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE5': str(worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE6': str(worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE7': str(worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE8': str(worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value is not None else "",
            'SIGNAL_ENUM0': str(worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value),
            'SIGNAL_ENUM1': str(worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value),
            'SIGNAL_ENUM2': str(worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value),
            'SIGNAL_ENUM3': str(worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value),
            'SIGNAL_ENUM4': str(worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value),
            'SIGNAL_ENUM5': str(worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value),
            'SIGNAL_ENUM6': str(worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value),
        }
        if worksheet[column_designation.get('CAN_ID') + str(signal)].value is not None:
            if signal_info.get("SIGNAL_DATA_TYPE") == "float":
                dbc_file.write(
                    (
                    "SIG_VALTYPE_ " + signal_info.get('CAN_ID') + " " + signal_info.get('SIGNAL_NAME') + " : 1;\n"
                    )
                )

    #Deal with enums
    for signal in range(start_row, number_of_rows):

        signal_info = {
            'CAN_ID': str(worksheet[column_designation.get('CAN_ID') + str(signal)].value),
            'MESSAGE_NAME': str(worksheet[column_designation.get('MESSAGE_NAME') + str(signal)].value),
            'MESSAGE_LENGTH': str(worksheet[column_designation.get('MESSAGE_LENGTH') + str(signal)].value),
            'MESSAGE_SENDER_NODE': str(worksheet[column_designation.get('MESSAGE_SENDER_NODE') + str(signal)].value),
            'MESSAGE_PERIODICITY': str(worksheet[column_designation.get('MESSAGE_PERIODICITY') + str(signal)].value),
            'SIGNAL_NAME': str(worksheet[column_designation.get('SIGNAL_NAME') + str(signal)].value),
            'SIGNAL_LENGTH': worksheet[column_designation.get('SIGNAL_LENGTH') + str(signal)].value,
            'SIGNAL_ENDIANNESS': '@1' if str(worksheet[column_designation.get('SIGNAL_ENDIANNESS') + str(signal)].value) == 'Little' else '@0',
            'SIGNAL_SIGNED_UNSIGNED': '-' if str(worksheet[column_designation.get('SIGNAL_SIGNED_UNSIGNED') + str(signal)].value) == 'Signed' else '+',
            'SIGNAL_SCALE': str(worksheet[column_designation.get('SIGNAL_SCALE') + str(signal)].value),
            'SIGNAL_OFFSET': str(worksheet[column_designation.get('SIGNAL_OFFSET') + str(signal)].value),
            'SIGNAL_MINIMUM': str(worksheet[column_designation.get('SIGNAL_MINIMUM') + str(signal)].value),
            'SIGNAL_MAXIMUM': str(worksheet[column_designation.get('SIGNAL_MAXIMUM') + str(signal)].value),
            'SIGNAL_DATA_TYPE': str(worksheet[column_designation.get('SIGNAL_DATA_TYPE') + str(signal)].value),
            'SIGNAL_UNIT': str(worksheet[column_designation.get('SIGNAL_UNIT') + str(signal)].value),
            'SIGNAL_REC_NODE1': str(worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE1') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE2': str(worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE2') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE3': str(worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE3') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE4': str(worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE4') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE5': str(worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE5') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE6': str(worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE6') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE7': str(worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE7') + str(signal)].value is not None else "",
            'SIGNAL_REC_NODE8': str(worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value + " ") if worksheet[column_designation.get('SIGNAL_REC_NODE8') + str(signal)].value is not None else "",
            'SIGNAL_ENUM0': str(worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value),
            'SIGNAL_ENUM1': str(worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value),
            'SIGNAL_ENUM2': str(worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value),
            'SIGNAL_ENUM3': str(worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value),
            'SIGNAL_ENUM4': str(worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value),
            'SIGNAL_ENUM5': str(worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value),
            'SIGNAL_ENUM6': str(worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value),
        }

        if worksheet[column_designation.get('CAN_ID') + str(signal)].value is not None:

            if worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    "\nVAL_ " + signal_info.get('CAN_ID') + " " + signal_info.get("SIGNAL_NAME") + " 0 \"" + signal_info.get('SIGNAL_ENUM0') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 1 \"" + signal_info.get('SIGNAL_ENUM1') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 2 \"" + signal_info.get('SIGNAL_ENUM2') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 3 \"" + signal_info.get('SIGNAL_ENUM3') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 4 \"" + signal_info.get('SIGNAL_ENUM4') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 5 \"" + signal_info.get('SIGNAL_ENUM5') + "\""
                    )
                )

            if worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value is not None:
                dbc_file.write(
                    (
                    " 5 \"" + signal_info.get('SIGNAL_ENUM6') + "\""
                    )
                )

            if (worksheet[column_designation.get('SIGNAL_ENUM0') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM1') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM2') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM3') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM4') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM5') + str(signal)].value is not None or
               worksheet[column_designation.get('SIGNAL_ENUM6') + str(signal)].value is not None):
                dbc_file.write(
                    ";"
                )
    dbc_file.write(
        '\n'
    )