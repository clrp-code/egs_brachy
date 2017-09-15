"""

A test for ensuring that the fluorescent photon cutoff works.  A monoenergetic
source of 0.015keV is placed within a thin Ti sphere and photon counts are
scored on the outer surface of the sphere. Normally there would be a
fluorescent peak at ~4.5keV so when the `fluorescent photon cutoff` setting is
set to 0.005 MeV that peak should not be present.

Two energy bins from 0-10keV and 10-20keV are used. If fluorescent photon cutoff
feature is working correctly 100% of photons should be in the 10-20keV range.

"""

from tests.utils import values_close, read_csv_spectrum

import math
import re

EGSINP = "flu_cutoff.egsinp"
TIME_LIMIT_S_PER_MHZ = 2/2993. # s / MHz


def compare_results(egslst, inp_name):

    energies, counts, uncs = read_csv_spectrum(inp_name+".surfcount.csv")

    lower, upper = counts
    if not (lower == 0 and upper == 100):
        return False, "Photon Count < 10keV = %f" %(lower), "Expected 0"

    return True, "", ""
