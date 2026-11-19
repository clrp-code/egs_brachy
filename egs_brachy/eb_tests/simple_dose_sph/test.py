"""

A very simple dose calculation in a spherical phantom with multiple media. The
simple geometry allows a fast calculation with high precision for comparing
against a previously calculated dose.

"""

import math
import re

from eb_tests.utils import doses_approx_equal, extract_all_doses

EGSINP = "simple_dose_sph.egsinp"
TIME_LIMIT_S_PER_MHZ = 22/2993. # s / MHz

# Expected values are {region: (dose, relative uncertainty)}.
#
# Doses are the mean of 40 runs of simple_dose_sph.egsinp with different RNG
# seeds (egs_brachy main @ 22dd7b2), so their own statistical uncertainty is
# ~6x smaller than that of a single run and is neglected below. The
# uncertainty is the typical value reported by a single run.
#
# Region 0 contains the source and is volume corrected. Energy deposition
# doses in the air shells (regions 1 and 3) have ~17% uncertainty at this
# number of histories so they are not compared.
expected_doses = {
    "tlen": {
        0: (5.2249e-12, 0.00055),
        1: (1.1168e-12, 0.00021),
        2: (4.2923e-13, 0.00015),
        3: (2.1266e-13, 0.00016),
        4: (1.3321e-13, 0.00015),
    },
    "edep": {
        0: (5.214e-12, 0.0073),
        2: (4.286e-13, 0.0057),
        4: (1.3303e-13, 0.0057),
    },
}

# a dose passes if it is within N_SIGMA of the expected dose, where sigma
# is the uncertainty reported by the run being tested
N_SIGMA = 4

# the reported uncertainty must be within this fraction of the expected one
MAX_UNC_REL_DIFF = 0.5


def compare_results(egslst, inp_name):

    actual = {
        "tlen": {},
        "edep": {},
    }

    for reg, tlen, tlen_unc, edep, edep_unc in extract_all_doses(egslst):
        actual['tlen'][int(reg)] = (float(tlen), float(tlen_unc)/100)
        actual['edep'][int(reg)] = (float(edep), float(edep_unc)/100)

    all_close = True
    for calc_type, region_doses in expected_doses.items():
        for region, (expected_dose, expected_unc) in region_doses.items():

            calc_dose, calc_unc = actual[calc_type][region]

            doses_close = doses_approx_equal(
                expected_dose, expected_unc, calc_dose, calc_unc,
                max_percent_diff=N_SIGMA * calc_unc,
                compare_unc=True,
                max_unc_percent_diff=MAX_UNC_REL_DIFF * expected_unc,
            )

            if not doses_close:
                msg = "%s dose comparison failed for reg %d: Actual= %.4E +/- %.3F%% Expected= %.4E +/- %.3F%%"
                print((msg % (calc_type, region, calc_dose, 100*calc_unc, expected_dose, 100*expected_unc)))
                all_close = False

    return all_close, actual, expected_doses

