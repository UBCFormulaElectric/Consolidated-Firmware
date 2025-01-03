import os
import string
from argparse import ArgumentParser
from typing import TypedDict

import git
import jinja2


# data["hash"]: 7 char string with short commit hash
# data["clean"]: "0" if dirty, "1" is clean
class GitData(TypedDict):
    hash: str
    clean: str


# dummy data in case of error
DUMMY_DATA: GitData = {"hash": "0000000", "clean": "0"}


def validateData(gitdata: any) -> bool:
    """
    validates data dict
    :param gitdata: Data to validate
    :return: if the data is valid
    """
    if gitdata is None or not isinstance(gitdata, dict):
        return False
    valid_length = len(gitdata["hash"]) == 7
    valid_hex = all([c in string.hexdigits for c in gitdata["hash"]])
    valid_clean = gitdata["clean"] == "false" or gitdata["clean"] == "true"
    return valid_length and valid_hex and valid_clean


if __name__ == "__main__":
    # set up path arg
    parser = ArgumentParser()
    parser.add_argument("--output-header", type=str, required=True)
    parser.add_argument("--output-source", type=str, required=True)
    args = parser.parse_args()

    # get template for rendering
    module_dir = os.path.dirname(os.path.relpath(__file__))
    template_dir = os.path.join(module_dir, "templates")
    env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_dir))
    header_template = env.get_template("./app_commitInfo.h.j2")
    source_template = env.get_template("./app_commitInfo.c.j2")

    # data to expose to header
    data: GitData | None = None

    # return dummy data if gitpython throws any error
    try:
        # get commit info
        repo = git.Repo(search_parent_directories=True)
        commit = repo.head.commit
        clean = not repo.is_dirty(untracked_files=True)
        # short hash is the first 7 chars of the long hash
        short_hash = commit.hexsha[0:7]
        data = {"hash": short_hash, "clean": "true" if clean else "false"}
    except:
        data = DUMMY_DATA
        try:
            print(
                "⚠️ commit_info_gen: GitPython failed to fetch data, returning dummy data."
            )
        except UnicodeEncodeError:
            print(
                "commit_info_gen: GitPython failed to fetch data, returning dummy data."
            )

    # also generate dummy data if data is not valid
    if not validateData(data):
        data = DUMMY_DATA
        try:
            print(
                "⚠️ commit_info_gen: Internally generated invalid data, returning dummy data."
            )
        except UnicodeEncodeError:
            print(
                "commit_info_gen: Internally generated invalid data, returning dummy data."
            )

    # recursively generate output dirs if they do not exist
    os.makedirs(os.path.dirname(args.output_header), exist_ok=True)
    os.makedirs(os.path.dirname(args.output_source), exist_ok=True)

    # write
    with open(args.output_header, "w+") as file:
        file.write(header_template.render(data))

    with open(args.output_source, "w+") as file:
        file.write(
            source_template.render({"header": os.path.basename(args.output_header)})
        )
