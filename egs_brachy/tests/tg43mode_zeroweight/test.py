"""

A test to ensure superposition mode calculations work when a source has zero
weighting.

Gold standard dose distribution was generated with egs_brachy and  gold_standard.egsinp

"""

from tests.utils import doses_approx_equal, extract_all_doses, compare_3ddose_files, read3ddose

import math
import os
import re

EGSINP = "tg43mode_zeroweight.egsinp"
TIME_LIMIT_S_PER_MHZ = 2000/2993. # s / MHz
NCOMPARE = 10

def get_n_highest_dose_pairs(dose1, dose2, n=NCOMPARE):
    return list(sorted(zip(dose1, dose2)))[-n:]

def compare_results(egslst, inp_name):

    gold_standard = read3ddose(os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose"))
    run_doses = read3ddose(inp_name+".phantom.3ddose")
    # only look at voxels overlapped by zero activity source
    dose_pairs = list(zip(gold_standard['doses'], run_doses['doses']))[7:12]
    dose_diff = [abs(a-b)/a for a, b in dose_pairs]
    doses_close = max(dose_diff) < 0.01

    # make sure energy deposition and tlen doses roughly equal
    run_doses_edep = read3ddose(inp_name+".phantom.edep.3ddose")
    dose_pairs_edep = get_n_highest_dose_pairs(run_doses['doses'], run_doses_edep['doses'],n=5)
    dose_diff_edep = [abs(a-b)/a for a, b in dose_pairs_edep]
    doses_close_edep = max(dose_diff_edep) < 0.10

    actual = {
        'edep': run_doses_edep['doses'],
        'tlen': run_doses['doses']
    }

    expected = {
        'edep': run_doses['doses'],
        'tlen': gold_standard['doses']
    }
    return doses_close and doses_close_edep, actual, expected

