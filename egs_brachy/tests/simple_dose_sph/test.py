"""

A very simple dose calculation in a spherical phantom with multiple media. The
simple geometry allows a fast calculation with high precision for comparing
against a previously calculated dose.

"""

from tests.utils import doses_approx_equal, extract_all_doses

import math
import re

EGSINP = "simple_dose_sph.egsinp"
TIME_LIMIT_S_PER_MHZ = 22/2993. # s / MHz

# expected doses taken from initial run of simple_dose_sph.egsinp
# when egs_brachy believed to be in a good state.
# regions 1-5 (reg 0 excluded because of source overlap)
expected_doses = {
    "tlen": [
        (5.219E-12, 0.0004),
        (1.117E-12, 0.0002),
        (4.292E-13, 0.0002),
        (2.126E-13, 0.0002),
        (1.332E-13, 0.0001),
    ],
    "edep": [
        (5.212E-12, 0.0073),
    ]
}



def compare_results(egslst, inp_name):

    actual = {
        "tlen": [],
        "edep": [],
    }

    doses = extract_all_doses(egslst)
    for reg, tlen, tlen_unc, edep, edep_unc in doses:
        actual['tlen'].append((float(tlen), float(tlen_unc)/100))
        actual['edep'].append((float(edep), float(edep_unc)/100))

    tolerances = {
        'tlen': 0.001,
        'edep': 0.015,
    }

    all_close = True
    for calc_type, region_doses in list(expected_doses.items()):
        for region, (expected_dose, expected_unc) in enumerate(region_doses):

            calc_dose, calc_unc = actual[calc_type][region]

            tol = tolerances[calc_type]
            doses_close = doses_approx_equal(expected_dose, expected_unc, calc_dose, calc_unc, max_percent_diff=tol)

            if not doses_close:
                msg = "%s dose comparison failed for reg %d: Actual= %.4E +/- %.2F%% Expected= %.4E +/- %.2F%%"
                print((msg % (calc_type, region + 1, calc_dose, 100*calc_unc, expected_dose, 100*expected_unc)))
                all_close = False

    return all_close, actual, expected_doses
