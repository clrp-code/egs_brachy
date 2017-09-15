"""
A test for ensuring x-ray sources mode is working.  The test consists of a
1cm^2 beam of electrons incident on a thin cylindrical disc with dose
being calculated in cylindrical slabs behind the target.  The egs_brachy
energy deposition doses are compared with values calculated by dosrznrc.
"""

from tests.utils import doses_approx_equal, extract_all_doses
import csv
import math
import re


EGSINP = "brem_cyl.egsinp"
TIME_LIMIT_S_PER_MHZ = 20/2993. # s / MHz

# doses calcualted using dosrz_brem_cyl.egsinp
DOSRZ_NRC_DOSES = [
    (3.0107E-013, 0.0002),
    (1.0088E-016, 0.0040),
    (5.6843E-017, 0.0042),
    (3.8797E-017, 0.0041),
]


expected_doses = {
    "tlen": [
        (3.702E-16, 2.84),
        (1.042E-16,  2.90),
        (5.933E-17, 3.04),
        (4.064E-17, 3.14),
    ],
    "edep": [
        (3.002E-13, 0.41),
        (1.073E-16, 3.15),
        (5.943E-17, 3.66),
        (3.912E-17, 4.20),
    ]
}



def compare_results(egslst, inp_name):

    actual = {
        "tlen": [],
        "edep": [],
    }

    doses = extract_all_doses(egslst)
    doses = doses[1:] # ignore first region since covered by source

    for reg, tlen, tlen_unc, edep, edep_unc in doses:
        actual['tlen'].append((float(tlen), float(tlen_unc)))
        actual['edep'].append((float(edep), float(edep_unc)))

    all_close = True
    for calc_type, region_doses in list(expected_doses.items()):
        if calc_type == "edep":
            region_doses = DOSRZ_NRC_DOSES
            compare_unc = False
        else:
            compare_unc = True

        for region, (expected_dose, expected_unc) in enumerate(region_doses):

            calc_dose, calc_unc = actual[calc_type][region]

            doses_close = doses_approx_equal(
                expected_dose, expected_unc, calc_dose, calc_unc,
                compare_unc=compare_unc, max_unc_percent_diff=0.5,
            )

            if not doses_close:
                msg = "%s dose comparison failed for reg %d: Actual= %E +/- %E Expected= %E +/- %E"
                print((msg % (calc_type, region + 1, calc_dose, calc_unc, expected_dose, expected_unc)))
                all_close = False

    return all_close, actual, expected_doses
