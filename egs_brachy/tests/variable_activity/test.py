"""

A test for comparing the dose from sources with different source weighting with
calculations from when egs_brachy was in a known good state.

"""

from tests.utils import doses_approx_equal, extract_all_doses, values_close, read3ddose

import math
import os
import re

EGSINP = "variable.egsinp"
TIME_LIMIT_S_PER_MHZ = 100/2993. # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose")
    run_doses_file = inp_name+".phantom.3ddose"

    # central axis doses
    gold_doses = read3ddose(gold_standard)['doses'][:5]
    run_doses = read3ddose(run_doses_file)['doses'][:5]

    close = [values_close(g, r, 0.005) for g, r in zip(gold_doses, run_doses)]


    return all(close), run_doses, gold_doses
