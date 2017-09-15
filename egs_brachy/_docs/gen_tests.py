import glob
import importlib
import os
import sys

root_tests = os.path.join("..", "tests")
sys.path.append("..")

globber = os.path.join(root_tests, "*", "test.py")

def get_tests():
    items = []


    for f in sorted(glob.glob(globber)):

        test_name = os.path.split(os.path.dirname(f))[1]
        mod_path = "tests.%s.test" % test_name
        test_mod = importlib.import_module(mod_path)
        docstring = test_mod.__doc__
        test_dir = "tests/%s" % test_name
        title = test_name.replace("_", " ").title()
        description = """## %s\n**Test Directory:** %s\n\n%s\n""" % (title, test_dir, docstring)

        items.append(description)

    return "\n".join(items)


def gen_docs(fname):

    docs = """@anchor egs_brachy_test_suite

egs_brachy Test Suite
=====================

The current list of tests available in the egs_brachy test suite

%s

""" % (get_tests())


    open(fname, "w").write(docs)

if __name__ == "__main__":
    outfile = "tests.md"
    gen_docs(outfile)


