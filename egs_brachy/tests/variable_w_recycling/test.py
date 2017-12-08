"""

A test for comparing the dose from sources with different source weighting and
recyling on with calculations from when egs_brachy was in a known good state.

Gold standard dose distribution was generated with egs_brachy and
gold_standard.egsinp

"""

from tests.utils import doses_approx_equal, extract_all_doses, compare_3ddose_files, read3ddose

import math
import os
import re

EGSINP = "variable_w_recycling.egsinp"
TIME_LIMIT_S_PER_MHZ = 15/2993. # s / MHz

# benchmark doses calcualted using egs_brachy and gold_standard.egsinp
BENCHMARK_DOSES = [(1.061E-13, 0.0001), (1.644E-13, 0.0001)]


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose")
    run_doses = inp_name+".phantom.3ddose"

    doses_close = compare_3ddose_files(
        gold_standard, run_doses, max_percent_diff=0.0015)

    return doses_close, read3ddose(run_doses), read3ddose(gold_standard)
