import os


def load_template(template_path):
    current_dir = os.path.dirname(os.path.abspath(__file__))
    return open(os.path.join(current_dir, "template", template_path)).read()
