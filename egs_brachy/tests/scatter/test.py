"""

A test for comparing egs_brachy scatter dose calculations for an Ir192 sources
with previously calculated values when egs_brachy was in a known good state.

"""

from tests.utils import doses_approx_equal, extract_all_doses, read3ddose, values_close

import math
import os
import re

EGSINP = "scatter.egsinp"
TIME_LIMIT_S_PER_MHZ = 65/2993. # s / MHz
NCOMPARE = 10 # only compare 10 highest doses


def get_n_highest_doses(doses, uncs, n=NCOMPARE):
    return list(zip(*sorted(zip(doses, uncs))))[-NCOMPARE:]


def compare_results(egslst, inp_name):
    base_gold = os.path.join(os.path.dirname(os.path.abspath(__file__)), "gold%s.3ddose")
    base_eb = inp_name+".phantom%s.3ddose"
    extensions = ["",".to",".pr",".ss", ".ms"]

    gold = [read3ddose(base_gold % e) for e in extensions]
    eb = [read3ddose(base_eb % e) for e in extensions]

    for kind, actual, expected in zip(extensions, eb, gold):
        act_doses, act_unc = get_n_highest_doses(actual['doses'], actual['uncs'])
        act_doses, act_unc = get_n_highest_doses(expected['doses'], expected['uncs'])
        exp_doses, exp_unc = list(zip(*sorted(zip(expected['doses'], expected['uncs']))))[:NCOMPARE]


        close = [doses_approx_equal(a, a_unc, e, e_unc, compare_unc=False, max_percent_diff=0.005) for a, a_unc, e, e_unc in zip(act_doses, act_unc, exp_doses, exp_unc)]
        diffs = [(a-e)/e*100 for a, a_unc, e, e_unc in zip(act_doses, act_unc, exp_doses, exp_unc)]
        if not all(close):
            print((kind, diffs))
            return False, (kind, act_doses), (kind, exp_doses)


    return True, [], []
