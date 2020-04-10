import argparse
import os
import subprocess
import filecmp

SOURCE_TEMPLATE = '''\
/******************************************************************************
 * Includes
 ******************************************************************************/
#include <{filename_h}>

/******************************************************************************
 * Module Variable Definitions
 ******************************************************************************/
static const struct git_hash git_hash = {{ .value = "{hash}" }};

/******************************************************************************
 * Function Definitions
 ******************************************************************************/
struct git_hash get_git_hash(void)
{{
    return git_hash;
}}
'''

HEADER_TEMPLATE = '''\
/**
 * @note  This file is auto-generated. !!! Do not modify !!!
 */
/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
/**
 * @brief Number of chars/bytes occupied by our git hash
 */
#define GIT_HASH_SIZE_IN_BYTES 8

/******************************************************************************
 * Typedefs
 ******************************************************************************/
/**
 * @brief Hide the Git hash behind an opaque data type so we can avoid returning
 *        a char pointer, which would be more error-prone.
 */
struct git_hash
{
    /**
     * @brief This contains the first 7 characters of the SHA-1 hash. If there
     *        is any uncommitted changes, the 8th character will be a '*' */
    const char value[GIT_HASH_SIZE_IN_BYTES];
};

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
struct git_hash get_git_hash(void);
'''

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', help='Output files will have the form <filename>.c and <filename>.h')
    parser.add_argument('source_dir', help='Output directory of the generated source file')
    parser.add_argument('header_dir', help='Output directory of the generated header file')
    args = parser.parse_args()

    result = subprocess.run(['git',
                             'describe',
                             '--always',
                             '--match',
                             '"NOT A TAG"',
                             '--dirty=*'], stdout=subprocess.PIPE)
    filename_c = args.filename + '.c'
    filename_h = args.filename + '.h'

    hash = result.stdout.decode('utf-8').replace('\n', '')

    # Generate output folders if they don't exist already
    if not os.path.exists(args.source_dir):
        os.mkdir(args.source_dir)
    if not os.path.exists(args.header_dir):
        os.mkdir(args.header_dir)

    # Create temporary files
    temp_header_path = os.path.join(args.header_dir, filename_h + '.tmp')
    with open(temp_header_path, 'w') as fout:
        fout.write(HEADER_TEMPLATE)
    temp_source_path = os.path.join(args.source_dir, filename_c + '.tmp')
    with open(temp_source_path, 'w') as fout:
        fout.write(SOURCE_TEMPLATE.format(filename_h=filename_h, hash=hash))

    # Compare temporary files with existing files (if any), and only write to
    # desk if they are different.
    output_header_path = os.path.join(args.header_dir, filename_h)
    if os.path.isfile(output_header_path) is not True or \
       filecmp.cmp(temp_header_path, output_header_path) is not True:
        with open(output_header_path, 'w') as fout:
            fout.write(HEADER_TEMPLATE)
    output_source_path = os.path.join(args.source_dir, filename_c)
    if os.path.isfile(output_source_path) is not True or \
        filecmp.cmp(temp_source_path, output_source_path) is not True:
        with open(output_source_path, 'w') as fout:
            fout.write(SOURCE_TEMPLATE.format(filename_h=filename_h, hash=hash))

    # Clean up temporary files
    os.remove(temp_header_path)
    os.remove(temp_source_path)
