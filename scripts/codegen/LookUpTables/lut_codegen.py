from openpyxl import Workbook, load_workbook
from openpyxl.utils import get_column_letter
import os

number_of_voltages = 3
number_of_columns = 200
number_of_rows = 200

def write_sheet_to_file(workbook, file, comma_termination = True):
    worksheet = workbook.active

    file.write(
        '\t{\n'
    )

    for row in range(1,number_of_rows+1):
        file.write(
            '\t\t{'
        )
        for col in range(1,number_of_columns+1):
            char = get_column_letter(col)
            val = int(worksheet[char + str(row)].value*100)

            if col == 1:
                file.write(
                    str(val)
                )
            else:
                file.write(
                    ',\t ' + str(val)
                )
        if row == number_of_rows:
            file.write(
                '}\n'
            )
        else:
            file.write(
                '},\n'
            )
    if comma_termination:
        file.write(
            '\t},\n'
        )
    else:
        file.write(
            '\t}\n'
        )

#400V data
id_400v =  load_workbook('MotorData/id_400V_rebased.xlsx')
iq_400v =  load_workbook('MotorData/iq_400V_rebased.xlsx')

#500V data
id_500v =  load_workbook('MotorData/id_500V_rebased.xlsx')
iq_500v =  load_workbook('MotorData/iq_500V_rebased.xlsx')

#600V data
id_600v =  load_workbook('MotorData/id_600V_rebased.xlsx')
iq_600v =  load_workbook('MotorData/iq_600V_rebased.xlsx')

base_path = os.getenv('PYTHONPATH')

#generate header (.h) file containing lookup table definition
with open(base_path + "/boards/INV/Inc/App/lookup_tables/id_peak_lut.h", 'w') as hdr_file:
    hdr_file.write(
        '#pragma once\n'
        '\n'
        '#include \"App_MotorLutInterface.h\"\n'
        '\n'
        'static const int16_t id_peak_lut_times100[LUT_NUM_VOLTAGES][LUT_NUM_ROWS][LUT_NUM_COLUMNS] =\n'
        '{\n'
    )
    write_sheet_to_file(id_400v, hdr_file)
    write_sheet_to_file(id_500v, hdr_file)
    write_sheet_to_file(id_500v, hdr_file, comma_termination = False)
    hdr_file.write(
        '};\n'
    )

with open(base_path + "/boards/INV/Inc/App/lookup_tables/iq_peak_lut.h", 'w') as hdr_file:
    hdr_file.write(
            '#pragma once\n'
            '\n'
            '#include \"App_MotorLutInterface.h\"\n'
            '\n'
            'static const int16_t iq_peak_lut_times100[LUT_NUM_VOLTAGES][LUT_NUM_ROWS][LUT_NUM_COLUMNS] =\n'
            '{\n'
    )
    write_sheet_to_file(iq_400v, hdr_file)
    write_sheet_to_file(iq_500v, hdr_file)
    write_sheet_to_file(iq_500v, hdr_file, comma_termination = False)
    hdr_file.write(
        '};\n'
    )











#
# for sheet in workbooks[0].worksheets:
#
#     #Generate header (.h) file
#     with open(base_path + "/boards/INV/Inc/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.h','w') as hdr_file:
#         hdr_file.write(
# "#pragma once\n\
# \n\
# #include \"lookup_tables/motor_lut_interface.h\"\n\
# \n\
# extern float " + str.lower(sheet.title) + "_lut_array[][LUT_NUM_TEMPS][LUT_NUM_ROWS][LUT_NUM_COLUMNS];\n"
# )
#
#     #Generate source (.c) file
#     with open(base_path + "/boards/INV/Src/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.c','w') as src_file:
#         src_file.write(
# "#include \"lookup_tables/" + str.lower(sheet.title) + "_lut.h\"\n\
# \n\
# float " + str.lower(sheet.title) + "_lut_array[LUT_NUM_VOLTAGES][LUT_NUM_TEMPS][LUT_NUM_ROWS][LUT_NUM_COLUMNS] = \n\
# {\n"
# )
#
# #Now, for each workbook, iterate sheetnames and append data to each C array
# for workbook_index, workbook in enumerate(workbooks):
#     for sheet_index, sheet in enumerate(workbook.worksheets):
#         with open(base_path + "/boards/INV/Src/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.c','a') as src_file:
#             if workbook_index % 3 == 0: #Start start new voltage entry
#                 src_file.write("\t{\n") #New voltage entry
#             src_file.write("\t\t{\n") #Start of new temperature entry
#             for row in range(1,number_of_rows+1):
#                 src_file.write(
#                 "\t\t\t{")
#                 for col in range(1,number_of_columns+1):
#                     char = get_column_letter(col)
#                     val = sheet[char + str(row)].value
#                     if "." in str(val):
#                         if col < number_of_columns:
#                             src_file.write(str(val) + "f, ")
#                         else:
#                             src_file.write(str(val) + "f")
#                     else:
#                         if col < number_of_columns:
#                             src_file.write(str(val) + ".0f, ")
#                         else:
#                             src_file.write(str(val) + ".0f")
#
#                 if row < number_of_rows:
#                     src_file.write("},\n")
#                 else:
#                     src_file.write("}\n")
#             if workbook_index == len(workbooks)-1:
#                 src_file.write("\t\t}\n")
#                 src_file.write("\t}\n")
#                 src_file.write("};\n")
#             elif workbook_index % 3 == 2: #End new voltage entry
#                 src_file.write("\t\t}\n")
#                 src_file.write("\t},\n")
#             else:
#                 src_file.write("\t\t},\n")
#
#         # if sheet_index == 1:
#         #     break
#


