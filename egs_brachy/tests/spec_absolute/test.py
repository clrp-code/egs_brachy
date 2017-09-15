"""

A test for comparing the total absolute photon counts on the surface of a
source with the expected value. A uniform spectrum between 15keV-25keV in a
near-vaccuum source is used so the expected spectrum can easily be calculated
analytically.

"""

from tests.utils import values_close, read_csv_spectrum

import math
import re

EGSINP = "spec_absolute.egsinp"
TIME_LIMIT_S_PER_MHZ = 6/2993. # s / MHz

EMIN, EMAX = 0.015, 0.025

def expected(e):
    if EMIN <= e <= EMAX:
        return 1./(EMAX-EMIN)
    return 0

def compare_results(egslst, inp_name):

    energies, counts, uncs = read_csv_spectrum(inp_name+".surfcount.csv")

    for e, c in zip(energies, counts):
        exp = expected(e)
        if not values_close(c, exp, 0.005):
            return False, "Count at %f=%f" %(e, c), "Expected %f" % exp

    return True, [], []
