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

# expected doses taken from initial run of simple_dose_sph.egsinp
# when egs_brachy believed to be in a good state.
# regions 1-5 (reg 0 excluded because of source overlap)
expected_doses = {
    "tlen": [
        (5.2267e-12, 0.00057),
        (1.1169e-12, 0.00021),
        (4.2949e-13, 0.00022), 
        (2.1264e-13, 0.00023), 
        (1.3321e-13, 0.00016)
    ],
    "edep": [
        (5.1568e-12, 0.00733), 
        (1.1745e-12, 0.18456), 
        (4.2901e-13, 0.00602), 
        (2.1847e-13, 0.28620), 
        (1.3255e-13, 0.00571)
    ]
}



def compare_results(egslst, inp_name):

    actual = {
        "tlen": [],
        "edep": [],
    }

    doses = extract_all_doses(egslst)
    for _, tlen, tlen_unc, edep, edep_unc in doses:
        actual['tlen'].append((float(tlen), float(tlen_unc)/100))
        actual['edep'].append((float(edep), float(edep_unc)/100))

    all_close = True
    for calc_type, region_doses in list(expected_doses.items()):
        for region, (expected_dose, expected_unc) in enumerate(region_doses):

            calc_dose, calc_unc = actual[calc_type][region]

            doses_close = doses_approx_equal(expected_dose, expected_unc, calc_dose, calc_unc, compare_unc=True, max_unc_percent_diff=10)

            if not doses_close:
                msg = "%s dose comparison failed for reg %d: Actual= %.4E +/- %.2F%% Expected= %.4E +/- %.2F%%"
                print((msg % (calc_type, region + 1, calc_dose, 100*calc_unc, expected_dose, 100*expected_unc)))
                all_close = False

    return all_close, actual, expected_doses
