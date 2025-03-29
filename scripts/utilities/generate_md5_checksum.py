"""
Generate the MD5 checksum on the input file and write the result to the output
file.
"""
import argparse
import hashlib
import os


def generate_md5_checksum(file_name) -> str:
    # Calculate MD5 checksum for intput file
    with open(file_name, 'r', encoding='utf-8') as f:
        md5_returned = hashlib.md5(f.read().encode('utf-8')).hexdigest()
        f.close()
    return md5_returned


if __name__ == '__main__':
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file', help='Input File')
    parser.add_argument('output_file', help='Output File')
    args = parser.parse_args()

    input_file_md5 = generate_md5_checksum(args.input_file)

    # Generate output folders if they don't exist already
    output_dir = os.path.dirname(args.output_file)
    if not os.path.exists(output_dir):
        os.mkdir(output_dir)
    # Write MD5 checksum to disk
    with open(f"{args.input_file}.md5", 'w') as f:
        f.write(input_file_md5)
        f.close()
