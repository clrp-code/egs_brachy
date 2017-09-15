import os

root = os.path.join("..", "lib")
abs_root = os.path.abspath(root)
specs = os.path.join(abs_root, "spectra")


def get_spectra():
    items = []

    for root, dirs, files in os.walk(specs):
        for f in sorted(files):
            if not f.endswith("spectrum"):
                continue

            f = os.path.relpath(os.path.join(root, f))
            description = open(f, "r").readline().strip()
            fname = os.path.split(f)[1]
            description = """## %s\n**File Location:** %s\n\n%s\n""" % (fname, f.strip("./"), description)

            items.append(description)

    return "\n".join(items)


def gen_docs(fname):

    docs = """@anchor egs_brachy_spectra_lib

[TOC]

egs_brachy Spectra
==================

The current list of spectra available in the egs_brachy geometry
library.

%s

""" % (get_spectra())


    open(fname, "w").write(docs)

if __name__ == "__main__":
    outfile = "spectra.md"
    gen_docs(outfile)


