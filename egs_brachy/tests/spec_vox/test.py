"""

A test for comparing the calculated energy fluence spectrum in a phantom region
with the expected value. A uniform spectrum between 15keV-25keV in a
near-vaccuum geometry is used so the expected spectrum can easily be calculated
analytically.

"""
from tests.utils import values_close, read_csv_spectrum

import math
import re

EGSINP = "spec_vox.egsinp"
TIME_LIMIT_S_PER_MHZ = 7/2993. # s / MHz

BIN_WIDTH = 0.001
EMIN, EMAX = 0.015, 0.025
R1, R2 = 0.1, 0.2

# vacuum and point source in spherical region so tracklength should just be R2-R1
TRACK_LENGTH = R2 - R1

N_BINS_IN_RANGE = (EMAX-EMIN)/BIN_WIDTH

SCORED_IN_BIN = TRACK_LENGTH/N_BINS_IN_RANGE

VOLUME = 4/3.*math.pi*(0.2**3-0.1**3)

def expected(e):
    if EMIN <= e <= EMAX:
        return SCORED_IN_BIN/(VOLUME*BIN_WIDTH)
    return 0

def compare_results(egslst, inp_name):

    energies, counts, uncs = read_csv_spectrum(inp_name+".voxelflu.csv")

    for e, c in zip(energies, counts):
        exp = expected(e)
        if (exp > 0 and not  values_close(c, exp, 0.005)) or (exp == 0 and c > 0.01):
            return False, "Fluence at E = %f MeV was %f" %(e, c), "Expected %f" % exp

    return True, [], []
