import os

root = os.path.join("..", "lib")
abs_root = os.path.abspath(root)
media_file = os.path.join(abs_root, "media", "material.dat")
muen_dir = os.path.join(abs_root, "muen")

def get_pegsless_materials():

    data = open(media_file, "r")

    media = []
    cur_media = []
    for line in data:
        line = line.strip()
        if not line or line.startswith("#"):
            continue

        if line.lower().startswith("medium") and cur_media:
            name = cur_media.pop(0).split("=")[1].strip()
            media.append("<dt>%s</dt><dd>%s</dd>" % (name, "<br/>".join(cur_media)))
            cur_media = []

        cur_media.append("    "+line)

    if cur_media:
        name = cur_media.pop(0).split("=")[1].strip()
        media.append("<dt>%s</dt><dd>%s</dd>" % (name, "<br/>".join(cur_media)))

    return "<dl>%s</dl>" % "\n".join(sorted(media))


def get_muen():

    files = []
    doc = ""
    for f in os.listdir(muen_dir):
        if not f.endswith(".muendat"):
            continue


        data = open(os.path.join(muen_dir, f), "r").read().split("\n")

        muens = []
        idx = 0
        while idx < len(data):
            line = data[idx]
            if line.lower().startswith("muen values for medium"):
                muen = line.split("=")[1].strip()
                muens.append("<dt>%s</dt><dd>%s<br/>%s</dd>" % (muen, data[idx+1], data[idx+2]))
                idx += 3
            else:
                idx += 1

        doc += "\subsection %s %s\n" %(f.replace(".",""), f)
        doc += "**File Location:** %s\n" % (os.path.relpath(os.path.join(muen_dir, f)))
        doc += "<dl>%s</dl>" % "\n".join(sorted(muens))


    return doc



def gen_docs(fname):

    params = {
        "materials": get_pegsless_materials(),
        "muen": get_muen()
    }

    docs = """@anchor egs_brachy_media_lib

[TOC]

egs_brachy Media & Muen Data
============================


\section pegsless Pegsless run materials

The current list of media available for pegsless runs is as follows:

{materials}

\section materials Materials with Muen data


The current list of media with muen data available for scoring dose with the
tracklength estimator is as follows:

{muen}

""".format(**params)


    open(fname, "w").write(docs)

if __name__ == "__main__":
    outfile = "media.md"
    gen_docs(outfile)
