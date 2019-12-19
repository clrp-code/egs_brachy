"""

A test for comparing dose calculated by egs_brachy in a simple rectilinear
phantom containing multiple 6702 sources (some of which are
rotated/translated). The calculated dose is compared with dose values
calculated when egs_brachy was in a known good state.

"""



import math
import os
import re

from eb_tests.utils import (compare_3ddose_files, doses_approx_equal,
                            extract_all_doses, read3ddose)

EGSINP = "seeds_in_xyz.egsinp"
TIME_LIMIT_S_PER_MHZ = 25/2993. # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose")
    run_doses = inp_name+".phantom.3ddose"

    doses_close = compare_3ddose_files(gold_standard, run_doses, max_percent_diff=0.01)

    return doses_close, read3ddose(run_doses), read3ddose(gold_standard)
