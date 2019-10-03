"""
Generate the MD5 checksum on the input file and write the result to the output
file.
"""
import os
import hashlib
import argparse

def generate_md5_checksum(file_name, output_file):
    # Generate output folders if they don't exist already 
    output_dir = os.path.dirname(output_file)
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)

    # Calculate MD5 checksum for intput file
    with open(file_name, 'r', encoding='utf-8') as f:
        md5_returned = hashlib.md5(f.read().encode('utf-8')).hexdigest()
        f.close()

    # Write MD5 checksum to disk
    with open(file_name + '.md5', 'w') as f:
        f.write(md5_returned)
        f.close()

if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', help='Input File')
    parser.add_argument('output_file', help='Output File')
    args = parser.parse_args()

    generate_md5_checksum(args.input_file, args.output_file)
