"""

A test of the egs_brachy Monte Carlo volume correction routines.  The volume of
phantom voxels overlapped by sources and other phantoms are calculated by
egs_brachy and compared with analytical values.

"""

import csv
import math
import re

EGSINP = "vc.egsinp"
TIME_LIMIT_S_PER_MHZ = 1 / 2993.# s / MHz

expected_volumes = {
    "source_volume": 4/3.*math.pi*0.1**3,
    "bounding_shape_volume": 4/3.*math.pi*0.11**3,
    "extra_bounding_shape_vol": 8**3,
    "box_reg_0": 10**3 - 6**3,
    "phantom_reg_0": 6*6*3 - 0.5*(4./3*math.pi*1**3),
    "phantom_reg_1": 6*6*3 - 0.5*(4./3*math.pi*1**3),
    "sph_phantom_reg_0": 4/3.*math.pi*(0.3**3 - 0.1**3),
    "sph_phantom_reg_1": 4./3*math.pi*(1**3 - 0.3**3)
}


def approx_equal( a, b, eps=0.001 ):
    same_sign = a*b >= 0
    return same_sign and abs(math.log( a ) - math.log(b)) <=  eps


def read_vols(phant, inp_name):
    """Read voxel values for the phanton named 'phant' and
    return the region volumes for that phantom"""

    path = "%s.%s.voxels" % (inp_name, phant)
    f = open(path, "r")
    dialect = csv.Sniffer().sniff(f.read(1024))
    f.seek(0)
    reader = csv.reader(f, dialect)
    header = next(reader)
    reg_vols = []
    for row in reader:
        reg_vols.append((float(row[1]), float(row[2])))

    return reg_vols


def compare_results(egslst, inp_name):

    box_vols = read_vols("box", inp_name)
    phant_vols = read_vols("phantom", inp_name)
    sph_phant_vols = read_vols("sph_phantom", inp_name)

    vsource = float(re.findall("Volume of Source\s*=\s*(.*)\s*cm", egslst, re.IGNORECASE)[0])
    bounding_vols = list(map(float, re.findall("Bounding shape volume\s*=\s*(.*)\s*cm", egslst, re.IGNORECASE)))

    actual = {
        "source_volume": vsource,
        "bounding_shape_volume": bounding_vols[0],
        "extra_bounding_shape_vol": bounding_vols[1],
        "box_reg_0": box_vols[0][0],
        "phantom_reg_0": phant_vols[0][0],
        "phantom_reg_1": phant_vols[1][0],
        "sph_phantom_reg_0": sph_phant_vols[0][0],
        "sph_phantom_reg_1": sph_phant_vols[1][0],
    }

    all_close = True
    for k,v in list(expected_volumes.items()):
        if not approx_equal(v, actual[k]):
            print(("Vol comparison failed for %s: Actual=%E Expected=%E" % (k, actual[k], v)))
            all_close = False

    return all_close, actual, expected_volumes
