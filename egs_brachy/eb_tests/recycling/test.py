"""

A test for ensuring doses calculated with recycling turned on are the same as
doses without recycling.

"""

import math
import os
import re

from eb_tests.utils import (compare_3ddose_files, doses_approx_equal,
                            extract_all_doses, read3ddose)

EGSINP = "recycling.egsinp"
TIME_LIMIT_S_PER_MHZ = 16/2993. # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose")
    run_doses = inp_name+".phantom.3ddose"

    doses_close = compare_3ddose_files(gold_standard, run_doses, max_percent_diff=0.01)

    return doses_close, read3ddose(run_doses), read3ddose(gold_standard)
