"""

 This test ensures the 'single generator=no' and 'single_generator=yes' options give
 the same results.

 Gold standard dose distribution was generated with egs_brachy and multiple_generator.egsinp

"""

from tests.utils import doses_approx_equal, extract_all_doses, compare_3ddose_files, read3ddose

import math
import os
import re

EGSINP = "single_generator.egsinp"
TIME_LIMIT_S_PER_MHZ = 25/2993. # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold_standard.3ddose")
    run_doses = inp_name+".phantom.3ddose"

    doses_close = compare_3ddose_files(gold_standard, run_doses, max_percent_diff=0.01)

    return doses_close, read3ddose(run_doses), read3ddose(gold_standard)
