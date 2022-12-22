"""

A test to compare a dose calculation using a phsp source with the equivalent
ab-initio simulation.

"""

import math
import os
import re

from eb_tests.utils import (compare_3ddose_files, doses_approx_equal,
                            extract_all_doses, read3ddose)

EGSINP = "phsp_run.egsinp"
TIME_LIMIT_S_PER_MHZ = 80/2993. # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "phsp_run_compare.phantom.3ddose")
    run_doses = inp_name+".phantom.3ddose"

    doses_close = compare_3ddose_files(gold_standard, run_doses, max_percent_diff=0.01)

    gs_doses = read3ddose(gold_standard)
    actual_doses = read3ddose(run_doses)

    return doses_close, actual_doses, gs_doses
