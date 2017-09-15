import os

from doc_utils import find_file_descriptions


root = os.path.join("..", "lib")
abs_root = os.path.abspath(root)
geom = os.path.join(abs_root, "geometry")

def get_readme(dir_path):

    """Look in directory dir_path for a file called README.md and return
    it's contents if available"""

    try:
        readme = open(os.path.join(dir_path, "README.md")).read()
        return "\n%s\n" % readme
    except IOError:
        return ""


def get_filetype_links(dir_path, extension):
    relpath = os.path.relpath(dir_path)
    files = [os.path.join(relpath, g) for g in os.listdir(dir_path) if g.endswith(extension)]
    links = ["[%s](%s)" % (os.path.split(f)[1], f) for f in files]
    return links


def get_images(dir_path):
    relpath = os.path.relpath(dir_path)
    files = [os.path.join(relpath, g) for g in os.listdir(dir_path) if g.endswith(".png")]
    images = ['<img style="width: 200px;" src="%s"  />' % f for f in files]
    return images


def gen_geom_docs(droot, title, is_sources=False):

    types = [os.path.join(droot, t) for t in os.listdir(droot) if os.path.isdir(os.path.join(droot, t))]

    docs = []

    for t in sorted(types):
        _, type_ = os.path.split(t)
        section = type_.replace(" ", "")

        name = type_.replace("_", " ")
        docs.append("\subsection %s %s %s" % (section, name, title))

        dirs = [os.path.join(t, f) for f in sorted(os.listdir(t))]

        for d in dirs:
            relpath = os.path.relpath(d)
            _, subname = os.path.split(d)
            subsection = (type_+subname).replace(" ", "").replace(".", "")

            docs.append("\subsubsection %s %s" % (subsection, subname))


            geom_links = get_filetype_links(d, ".geom")
            shape_links = get_filetype_links(d, ".shape")
            docs.append("<dl>")
            docs.append("<dt>Description</dt><dd>%s</dd>" % (get_readme(d) or "<em>No description available</em>"))
            if geom_links:
                docs.append("<dt>Geometry Files</dt><dd>%s</dt>" % ','.join(geom_links))
            if shape_links:
                docs.append("<dt>Shape Files</dt><dd>%s</dt>" % ', '.join(shape_links))

            images = get_images(d)
            docs.append("<dt>Images</dt><dd>%s</dd>" % ('\n'.join(images) or "<em>No images available</em>"))

            docs.append("</dl>")

    return "\n".join(docs)


def gen_docs(fname):
    params = {}
    params["sources"] = gen_geom_docs(os.path.join(geom, "sources"), "Sources")
    params["eye_plaques"] = find_file_descriptions(os.path.join(geom, "eye_plaques"), "start geometry")
    params["transforms"] = find_file_descriptions(os.path.join(geom, "transformations"), "start transformation")
    params["phantoms"] = find_file_descriptions(os.path.join(geom, "phantoms"), "start geometry")
    params["applicators"] = find_file_descriptions(os.path.join(geom, "applicators"), "start geometry")

    docs = """@anchor egs_brachy_geom_lib

[TOC]

The egs_brachy Geometry Library
===============================

\section egs_brachysources Source Library


The current list of sources available in the egs_brachy geometry
library.

{sources}

\section egs_brachyphantoms Phantom Library

The current list of phantoms available in the egs_brachy geometry
library.

{phantoms}


\section egs_brachyapplicators Applicator Library

The current list of applicators available in the egs_brachy geometry
library.

{applicators}


\section egs_brachyplaques Eye Plaques Library

The current list of eye plaques available in the egs_brachy geometry
library.

{eye_plaques}

\section egs_brachytransforms Transformation Sets

The current list of predefined transformation sets available in the
egs_brachy geometry library.

{transforms}
""".format(**params)


    open(fname, "w").write(docs)

if __name__ == "__main__":
    outfile = "geom.md"
    gen_docs(outfile)


