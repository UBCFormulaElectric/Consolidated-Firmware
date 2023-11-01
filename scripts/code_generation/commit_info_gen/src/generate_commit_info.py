import os
import string
from argparse import ArgumentParser
import jinja2
import git

# dummy data in case of error
DUMMY_DATA = {
  "hash": "0000000",
  "clean": "0"
}

# validates data dict
# data["hash"]: 7 char string with short commit hash
# data["clean"]: "0" if dirty, "1" is clean
def validateData(data):
  valid_length = len(data["hash"]) == 7
  valid_hex = all([c in string.hexdigits for c in data["hash"]])
  valid_clean = data["clean"] == "false" or data["clean"] == "true"
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
  header_template = env.get_template("./App_CommitInfo.h.j2")
  source_template = env.get_template("./App_CommitInfo.c.j2")

  print("-------COMMIT-INFO-----")

  print("--- SYSTEM ---")
  os.system('git status')
  os.system('echo hello')

  print("--- POPEN ---")
  stream = os.popen('git status')
  output = stream.read()
  print(output)

  stream = os.popen('echo hello')
  output = stream.read()
  print(output)

  print("-------COMMIT-INFO-----")
  # data to expose to header
  data = {}

  # return dummy data if gitpython throws any error
  # get commit info
  repo = git.Repo(search_parent_directories=True)
  commit = repo.head.commit
  clean = not repo.is_dirty(untracked_files=True)

  # short hash is the first 7 chars of the long hash
  short_hash = commit.hexsha[0:7]

  data["hash"] = short_hash
  data["clean"] = "true" if clean else "false"

  # also generate dummy data if data is not valid
  if not validateData(data):
    data = DUMMY_DATA
    print("⚠️ commit_info_gen: Internally generated invalid data, returning dummy data.")

  # recursively generate output dirs if they do not exist
  os.makedirs(os.path.dirname(args.output_header), exist_ok=True)
  os.makedirs(os.path.dirname(args.output_source), exist_ok=True)

  # write
  with open(args.output_header, "w+") as file:
    file.write(header_template.render(data))
  
  with open(args.output_source, "w+") as file:
    file.write(source_template.render({
      "header": os.path.basename(args.output_header)
    }))
    