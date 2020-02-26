from boards.shared.generate_CAN.generate_c_code_from_dbc import generate_c_code_from_dbc

if __name__ == "__main__":
    # TODO: a lot of this should probably be in a util function
    generate_c_code_from_dbc("BMS", 
            "boards/BMS/Src/auto_generated", 
            "boards/BMS/Inc/auto_generated", 
            "boards/shared/Src/auto_generated", 
            "boards/shared/Inc/auto_generated", 
            "boards/shared/generate_CAN/CanMsgs.dbc")
