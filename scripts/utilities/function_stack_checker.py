import os
import argparse
'''
    A function that read all .su file generate by gcc to check the stack usage for all function in firmware we use
    Use this scripe after compile with option -Wfstack-usage
'''

def find_all_su_file(root_path):
    """
    Find all the .su files in the root_path and return a list of them.
    """
    su_files = []
    for root, dirs, files in os.walk(root_path):
        for file in files:
            if file.endswith('.su'):
                su_files.append(os.path.join(root, file))
    return su_files

def read_all_lines(file_name):
    """
    Read all the lines from the file and return them as a list.
    """
    with open(file_name, 'r', encoding='utf-8') as f:
        lines = f.readlines()
        lines = [line.strip().split('\t') for line in lines]
    return lines

def main(root_path, output_file):
    su_files = find_all_su_file(root_path)
    all_lines = []
    for file in su_files:
        lines = read_all_lines(file)
        all_lines.extend(lines)
    
    # Sort lines by the second element in descending order
    all_lines.sort(key=lambda x: int(x[1]), reverse=True)
    
    # Write sorted lines to output file
    with open(output_file, 'w', encoding='utf-8') as f:
        for line in all_lines:
            f.write('\t'.join(line) + '\n')
    


# Example usage
if __name__ == "__main__":
    # root_path = "/home/jiajun/Consolidated-Firmware/build_fw_deploy/firmware/"  # Replace with the actual root path
    # output_file = "/home/jiajun/Consolidated-Firmware/build_fw_deploy/firmware/stack.su"  # Replace with the desired output file path
    parser = argparse.ArgumentParser(description="Function stack checker")
    parser.add_argument("--root_path", help="Root path of the project")
    parser.add_argument("--output_file", help="Output file path")
    args = parser.parse_args()
    root_path = args.root_path
    output_file = args.output_file


    main(root_path, output_file)
