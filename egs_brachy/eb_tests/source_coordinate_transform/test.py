"""

Compare dose from a source definition that uses local transforms plus a
source coordinate transform against the seeds_in_xyz gold standard, which
uses pre-composed world transforms directly.

Local transforms subtract (0.1, 0.2, 0.3) cm from each world placement;
the source coordinate transform adds that offset back during initialization.

"""

import os

from eb_tests.utils import compare_3ddose_files, read3ddose

EGSINP = "source_coordinate_transform.egsinp"
TIME_LIMIT_S_PER_MHZ = 25/2993.  # s / MHz


def compare_results(egslst, inp_name):

    gold_standard = os.path.join(
        os.path.dirname(os.path.abspath(__file__)),
        "..",
        "seeds_in_xyz",
        "gold_standard.3ddose",
    )
    run_doses = inp_name + ".phantom.3ddose"

    doses_close = compare_3ddose_files(
        gold_standard, run_doses, max_percent_diff=0.01
    )

    return doses_close, read3ddose(run_doses), read3ddose(gold_standard)
