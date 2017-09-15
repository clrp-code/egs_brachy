import os

def find_file_descriptions(dir_path, include_key=None):

    items = []

    for root, dirs, files in os.walk(dir_path):
        for f in sorted(files):

            f = os.path.relpath(os.path.join(root, f))
            try:
                contents = open(f, "r").read()
            except:
                continue


            # exclude files that don't contain the include_key search string
            if include_key is not None and include_key not in contents:
                continue

            comments = []
            for l in contents.split("\n"):
                if l[:2] == "##":
                    comments.append(l[2:])

            description = "".join(comments) if comments else  "_No description available_"
            fname = os.path.split(f)[1]
            description = """## %s\n**File Location:** %s\n\n%s\n""" % (fname, f.strip("./"), description)

            items.append(description)

    return "\n".join(items)


