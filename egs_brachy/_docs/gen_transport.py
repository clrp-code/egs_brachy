from doc_utils import find_file_descriptions
import os

root = os.path.join("..", "lib")
abs_root = os.path.abspath(root)
transport = os.path.join(abs_root, "transport")


def gen_docs(fname):

    docs = """@anchor egs_brachy_transport_lib

egs_brachy Sample Transport Parameters
======================================

Sample Monte Carlo transport parameters for various simulations.

%s

""" % (find_file_descriptions(transport, "start MC transport parameter"))


    open(fname, "w").write(docs)

if __name__ == "__main__":
    outfile = "transport.md"
    gen_docs(outfile)


