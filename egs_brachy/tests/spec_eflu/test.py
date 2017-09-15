"""

A test for comparing the calculated energy fluence spectrum on the surface of a
source with the expected value. A uniform spectrum between 15keV-25keV in a
near-vaccuum source is used so the expected spectrum can easily be calculated
analytically.

"""

from tests.utils import values_close, read_csv_spectrum

import math
import re

EGSINP = "spec_eflu.egsinp"
TIME_LIMIT_S_PER_MHZ = 6/2993. # s / MHz

EMIN, EMAX = 0.015, 0.025

NHIST = 1

BIN_WIDTH = 0.001
AVG_E = (EMAX+EMIN)/2.
TOTAL_E = AVG_E * NHIST
N_BINS_IN_RANGE = (EMAX-EMIN)/BIN_WIDTH

SCORED_IN_BIN = NHIST/N_BINS_IN_RANGE
SCORED_IN_BIN_PER_MEV = SCORED_IN_BIN / BIN_WIDTH

def expected(e):
    if EMIN <= e <= EMAX:
        return e*SCORED_IN_BIN_PER_MEV / TOTAL_E
    return 0

def compare_results(egslst, inp_name):

    energies, counts, uncs = read_csv_spectrum(inp_name+".ewsurf.csv")

    for e, c in zip(energies, counts):
        exp = expected(e)
        if (exp > 0 and not  values_close(c, exp, 0.005)) or (exp == 0 and c > 0.0001):
            return False, "Fluence at E = %f MeV was %f" %(e, c), "Expected %f" % exp

    return True, [], []
