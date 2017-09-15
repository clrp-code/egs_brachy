"""
A test that generates an IAEAphsp source with egs_brachy and then checks
the IAEA file to make sure it was created correctly.
"""

from tests.utils import values_close, read_csv_spectrum
from tests.iaea import IAEAPhaseSpace
import tests.iaea_types as types

import math
import re

EGSINP = "phsp_score.egsinp"
TIME_LIMIT_S_PER_MHZ = 10/2993. # s / MHz

SOURCE_WEIGHTS = (1., 9.)
MAX_E = (0.05, 0.025,)
NHIST= 1000
RM = 0.511

EXPECTED = {
    'num_e': NHIST*SOURCE_WEIGHTS[0]/sum(SOURCE_WEIGHTS),
    'num_p': NHIST*SOURCE_WEIGHTS[1]/sum(SOURCE_WEIGHTS),
    'max_energy': 0.05,
    'num_orig': NHIST,
}


def compare_results(egslst, inp_name):

    iaea_path = "%s.phsp%s" %(inp_name, IAEAPhaseSpace.phsp_ext)
    phsp = IAEAPhaseSpace(iaea_path)
    num_e = phsp.num_particles("electron")
    num_p = phsp.num_particles("photon")
    num_orig = phsp.num_orig_particles()
    max_energy = phsp.maximum_energy() - RM

    actual = {
        'num_e': num_e,
        'num_p': num_p,
        'num_orig': num_orig,
        'max_energy': max_energy
    }

    num_orig_equal = num_orig == NHIST
    num_e_close = abs(EXPECTED['num_e']- num_e) < 10
    num_p_close = abs(EXPECTED['num_p']- num_p) < 10
    max_e_close = abs( max_energy - MAX_E[0]) < 0.0001

    all_correct = num_e_close and num_p_close and max_e_close and num_orig_equal
    return all_correct, actual, EXPECTED
