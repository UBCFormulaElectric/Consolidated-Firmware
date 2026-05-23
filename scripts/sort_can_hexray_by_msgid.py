#!/usr/bin/env python3
"""
Sort CAN bus hexray JSON files by msg_id.

This script sorts CAN bus hexray JSON files by the msg_id field of each message.

Usage:
    python sort_can_hexray_by_msgid.py                          # Auto-sort all *_tx.json files in can_bus/hexray
    python sort_can_hexray_by_msgid.py <directory>              # Auto-sort all *_tx.json files in <directory>
    python sort_can_hexray_by_msgid.py <input_file> [output]    # Sort a specific file
    python sort_can_hexray_by_msgid.py --all <directory>        # Sort all *_tx.json and *_rx.json files
"""

import json
import sys
from pathlib import Path
from glob import glob


def sort_can_json_by_msgid(input_file, output_file=None, verbose=True):
    """
    Sort a CAN JSON file by msg_id.

    Args:
        input_file (str): Path to the input JSON file
        output_file (str, optional): Path to the output JSON file.
                                      If None, overwrites input_file.
        verbose (bool): Print verbose output. Defaults to True.

    Returns:
        bool: True if successful, False otherwise
    """
    input_path = Path(input_file)

    # Verify input file exists
    if not input_path.exists():
        if verbose:
            print(f"Error: Input file '{input_file}' not found.", file=sys.stderr)
        return False

    if not input_path.suffix.lower() == '.json':
        if verbose:
            print(f"Error: Input file must be a JSON file.", file=sys.stderr)
        return False

    try:
        # Read the JSON file
        with open(input_path, 'r', encoding='utf-8') as f:
            data = json.load(f)

        # Verify it's a dictionary (not a list or other type)
        if not isinstance(data, dict):
            if verbose:
                print("Error: JSON root must be an object/dictionary.", file=sys.stderr)
            return False

        # Sort the dictionary by msg_id
        sorted_data = {}
        try:
            sorted_items = sorted(
                data.items(),
                key=lambda x: x[1].get('msg_id', float('inf'))
                if isinstance(x[1], dict) else float('inf')
            )
            sorted_data = dict(sorted_items)
        except (TypeError, AttributeError) as e:
            if verbose:
                print(f"Error: Failed to sort by msg_id: {e}", file=sys.stderr)
            return False

        # Determine output file path
        if output_file is None:
            output_path = input_path
        else:
            output_path = Path(output_file)

        # Write the sorted JSON file
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(sorted_data, f, indent=2, ensure_ascii=False)

        if verbose:
            output_msg = f"Written to '{output_path}'" if output_file else "Overwrote original file"
            print(f"✓ Successfully sorted {len(sorted_data)} messages by msg_id. {output_msg}")
        return True

    except json.JSONDecodeError as e:
        if verbose:
            print(f"Error: Failed to parse JSON: {e}", file=sys.stderr)
        return False
    except IOError as e:
        if verbose:
            print(f"Error: File I/O error: {e}", file=sys.stderr)
        return False
    except Exception as e:
        if verbose:
            print(f"Error: Unexpected error: {e}", file=sys.stderr)
        return False


def sort_all_tx_files(directory=None, pattern="*_tx.json"):
    """
    Recursively find and sort all matching JSON files in a directory.

    Args:
        directory (str, optional): Directory to search. Defaults to can_bus/hexray
        pattern (str): File pattern to match. Defaults to *_tx.json

    Returns:
        tuple: (success_count, total_count)
    """
    if directory is None:
        # Try to find can_bus/hexray from current or parent directories
        current = Path.cwd()
        for attempt in [current / "can_bus" / "hexray",
                       current.parent / "can_bus" / "hexray",
                       current.parent.parent / "can_bus" / "hexray"]:
            if attempt.exists():
                directory = attempt
                break

        if directory is None:
            print("Error: Could not find can_bus/hexray directory. Please specify a directory.",
                  file=sys.stderr)
            return 0, 0

    directory_path = Path(directory)
    if not directory_path.exists():
        print(f"Error: Directory '{directory}' not found.", file=sys.stderr)
        return 0, 0

    # Find all matching files recursively
    search_pattern = str(directory_path / "**" / pattern)
    files = list(Path(p) for p in glob(search_pattern, recursive=True))

    if not files:
        print(f"No files matching '{pattern}' found in '{directory}'", file=sys.stderr)
        return 0, 0

    print(f"Found {len(files)} file(s) to sort:\n")

    success_count = 0
    for file_path in sorted(files):
        rel_path = file_path.relative_to(directory_path.parent) if directory_path.parent in file_path.parents else file_path
        print(f"  Sorting {rel_path}...", end=" ")
        if sort_can_json_by_msgid(str(file_path), verbose=False):
            print("✓")
            success_count += 1
        else:
            print("✗")

    print(f"\nCompleted: {success_count}/{len(files)} files sorted successfully")
    return success_count, len(files)


def main():
    """Main entry point."""
    args = sys.argv[1:]

    # No arguments: auto-sort all *_tx.json files
    if not args:
        success_count, total_count = sort_all_tx_files()
        sys.exit(0 if success_count == total_count and total_count > 0 else 1)

    # --all flag: sort both tx and rx files
    if args[0] == "--all":
        directory = args[1] if len(args) > 1 else None
        print("=" * 60)
        print("Sorting all *_tx.json files:")
        print("=" * 60)
        tx_success, tx_total = sort_all_tx_files(directory, "*_tx.json")

        print("\n" + "=" * 60)
        print("Sorting all *_rx.json files:")
        print("=" * 60)
        rx_success, rx_total = sort_all_tx_files(directory, "*_rx.json")

        total_success = tx_success + rx_success
        total_files = tx_total + rx_total
        sys.exit(0 if total_success == total_files and total_files > 0 else 1)

    # Check if first arg is a directory
    first_arg = Path(args[0])
    if first_arg.is_dir():
        print(f"Sorting all *_tx.json files in {args[0]}:\n")
        success_count, total_count = sort_all_tx_files(str(first_arg))
        sys.exit(0 if success_count == total_count and total_count > 0 else 1)

    # Otherwise, treat as a single file operation
    if len(args) < 1:
        print(__doc__)
        sys.exit(1)

    input_file = args[0]
    output_file = args[1] if len(args) > 1 else None

    success = sort_can_json_by_msgid(input_file, output_file)
    sys.exit(0 if success else 1)


if __name__ == '__main__':
    main()

