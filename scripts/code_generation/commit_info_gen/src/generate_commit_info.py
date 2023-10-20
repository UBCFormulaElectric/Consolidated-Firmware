import os
import string
from argparse import ArgumentParser
import jinja2
import git

# validates data dict
# data["hash"]: 7 char string with short commit hash
# data["clean"]: "0" if dirty, "1" is clean
def validateData(data):
  validLength = len(data["hash"]) == 7
  validHex = all([c in string.hexdigits for c in data["hash"]])
  validClean = data["clean"] == "0" or data["clean"] == "1"
  return validLength and validHex and validClean

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

  # data to expose to header
  data = {}

  try: 
    # get commit info
    repo = git.Repo(search_parent_directories=True)
    commit = repo.head.commit
    clean = not repo.is_dirty(untracked_files=True)

    # short hash is the first 7 chars of the long hash
    short_hash = commit.hexsha[0:7]

    data["hash"] = short_hash
    data["clean"] = "1" if clean else "0"

    # validate
    if not validateData(data):
      raise Exception("Invalid data.")

  except:
    data["hash"] = "0000000"
    data["clean"] = "0"
    print("commit_info_gen: Failed to get commit data, returning header with dummy data.")

  # write
  with open(args.output_header , "w") as file:
    file.write(header_template.render(data))
  with open(args.output_source, "w") as file:
    file.write(source_template.render({
      "header": os.path.basename(args.output_header)
    }))
    