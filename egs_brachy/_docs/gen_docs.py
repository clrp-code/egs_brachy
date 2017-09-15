import gen_geom
import gen_specs
import gen_transport
import gen_media
import gen_tests


modules = [
    (gen_geom, "geom.md"),
    (gen_specs, "spectra.md"),
    (gen_transport, "transport.md"),
    (gen_media, "media.md"),
    (gen_tests, "tests.md"),
]


def gen_docs():
    for module, outfile in modules:
        module.gen_docs(outfile)


if __name__ == "__main__":
    gen_docs()

