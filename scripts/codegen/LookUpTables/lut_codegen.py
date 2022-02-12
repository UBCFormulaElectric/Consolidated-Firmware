from openpyxl import Workbook, load_workbook
from openpyxl.utils import get_column_letter
import os

number_of_columns = 21
number_of_rows = 201


#400V data
volt400_t80c =  load_workbook('MotorData/400V/A2370DD_DC400V_T80C.xlsx')
volt400_t100c =  load_workbook('MotorData/400V/A2370DD_DC400V_T100C.xlsx')
volt400_t120c =  load_workbook('MotorData/400V/A2370DD_DC400V_T120C.xlsx')

#500V data
volt500_t80c =  load_workbook('MotorData/500V/A2370DD_DC500V_T80C.xlsx')
volt500_t100c =  load_workbook('MotorData/500V/A2370DD_DC500V_T100C.xlsx')
volt500_t120c =  load_workbook('MotorData/500V/A2370DD_DC500V_T120C.xlsx')

#600V data
volt600_t80c =  load_workbook('MotorData/600V/A2370DD_DC600V_T80C.xlsx')
volt600_t100c =  load_workbook('MotorData/600V/A2370DD_DC600V_T100C.xlsx')
volt600_t120c =  load_workbook('MotorData/600V/A2370DD_DC600V_T120C.xlsx')

workbooks = [volt400_t80c, volt400_t100c, volt400_t120c, volt500_t80c, volt500_t100c, volt500_t120c, volt600_t80c, volt600_t100c, volt600_t120c]

base_path = os.getenv('PYTHONPATH')




for sheet in workbooks[0].worksheets:

    #Generate header (.h) file
    with open(base_path + "/boards/INV/Inc/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.h','w') as hdr_file:
        hdr_file.write(
"#pragma once\n\
\n\
#include \"lookup_tables/motor_lut_interface.h\"\n\
\n\
extern float " + str.lower(sheet.title) + "_lut_array[][LUT_NUM_TEMPS][LUT_NUM_ROWS][LUT_NUM_COLUMNS];\n"
)

    #Generate source (.c) file
    with open(base_path + "/boards/INV/Src/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.c','w') as src_file:
        src_file.write(
"#include \"lookup_tables/" + str.lower(sheet.title) + "_lut.h\"\n\
\n\
float " + str.lower(sheet.title) + "_lut_array[LUT_NUM_VOLTAGES][LUT_NUM_TEMPS][LUT_NUM_ROWS][LUT_NUM_COLUMNS] = \n\
{\n"
)

#Now, for each workbook, iterate sheetnames and append data to each C array
for workbook_index, workbook in enumerate(workbooks):
    for sheet_index, sheet in enumerate(workbook.worksheets):
        with open(base_path + "/boards/INV/Src/App/lookup_tables/"+ str.lower(sheet.title) + '_lut.c','a') as src_file:
            if workbook_index % 3 == 0: #Start start new voltage entry
                src_file.write("\t{\n") #New voltage entry
            src_file.write("\t\t{\n") #Start of new temperature entry
            for row in range(1,number_of_rows+1):
                src_file.write(
                "\t\t\t{")
                for col in range(1,number_of_columns+1):
                    char = get_column_letter(col)
                    val = sheet[char + str(row)].value
                    if "." in str(val):
                        if col < number_of_columns:
                            src_file.write(str(val) + "f, ")
                        else:
                            src_file.write(str(val) + "f")
                    else:
                        if col < number_of_columns:
                            src_file.write(str(val) + ".0f, ")
                        else:
                            src_file.write(str(val) + ".0f")

                if row < number_of_rows:
                    src_file.write("},\n")
                else:
                    src_file.write("}\n")
            if workbook_index == len(workbooks)-1:
                src_file.write("\t\t}\n")
                src_file.write("\t}\n")
                src_file.write("};\n")
            elif workbook_index % 3 == 2: #End new voltage entry
                src_file.write("\t\t}\n")
                src_file.write("\t},\n")
            else:
                src_file.write("\t\t},\n")

        # if sheet_index == 1:
        #     break



