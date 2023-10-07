#!/usr/bin/env python3

import argparse
import os
import re
import subprocess

# Defines a list of directories to format
dirs_to_format = [
    "WwisePlugin",
    "SoundEnginePlugin",
]
# Defines an exlude pattern for filenames and directories
excluded_path_pattern = re.compile(r".*BinaryData.*|.*JuceLibraryCode.*|.*vcpkg_installed.*")
# Defines an include pattern for filenames
file_pattern = re.compile(r".*\.h$|.*\.cpp$")


def get_file_paths(dirpath):
    filePaths: list[str] = []

    # gather list of all files in the directory
    for root, dirs, files in os.walk(dirpath):
        for path in files:
            filepath = os.path.join(root, path)
            # Ignores files matching excluded_path_pattern
            ignorematch = re.match(excluded_path_pattern, filepath)
            # Only processes files matching file_pattern
            filematch = re.match(file_pattern, filepath)

            if filematch and not ignorematch:
                filePaths.append(filepath)

    return filePaths


def main(args):
    for dirpath in dirs_to_format:
        filePaths = get_file_paths(dirpath)

        if args.print:
            print("\n".join(filePaths))
        # process all files
        elif args.check:
            format_command = [
                "clang-format",
                "-n",
                "-Werror",
                "--style=file",
            ] + filePaths

            try:
                subprocess.check_call(format_command)
            except subprocess.CalledProcessError:
                exit(1)

        else:
            format_command = ["clang-format", "--style=file", "-i"]

            if args.verbose:
                format_command.append("--verbose")

            format_command += filePaths

            subprocess.check_call(format_command)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        "cf", "Script to format codebase files or check the integrity of the format"
    )

    parser.add_argument(
        "-v", "--verbose", help="Prints list of processed files", action="store_true"
    )
    parser.add_argument(
        "-p",
        "--print",
        help="Print file names only, do not format them.",
        action="store_true",
    )
    parser.add_argument(
        "-c",
        "--check",
        help="Uses clang-format to check codebase files for formatted status, returns a failure exit code if code needs formatting.",
        action="store_true",
    )

    args = parser.parse_args()
    main(args)