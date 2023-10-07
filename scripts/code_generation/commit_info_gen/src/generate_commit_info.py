import os
import jinja2
import git

# get template for rendering
module_dir = os.path.dirname(os.path.relpath(__file__))
template_dir = os.path.join(module_dir, "templates")
env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_dir))
template = env.get_template("./app_commit_info.h.j2")

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

with open("test.h", "w") as file:
  file.write(template.render(data))