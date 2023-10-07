import os
from argparse import ArgumentParser
import jinja2
import git

if __name__ == "__main__":

  # set up path arg
  parser = ArgumentParser()
  parser.add_argument("--output-header", type=str, required=True)
  args = parser.parse_args() 
  output_path = ""

  # get template for rendering
  module_dir = os.path.dirname(os.path.relpath(__file__))
  template_dir = os.path.join(module_dir, "templates")
  env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_dir))
  template = env.get_template("./App_CommitInfo.h.j2")

  # get commit data
  repo = git.Repo(search_parent_directories=True)
  commit = repo.head.commit
  clean = not repo.is_dirty(untracked_files=True)

  # short hash is the first 7 chars of the long hash
  short_hash = commit.hexsha[0:7]

  data = {
    "hash": short_hash,
    "clean": "1" if clean else "0"
  }

  with open(args.output_header, "w") as file:
    file.write(template.render(data))
    