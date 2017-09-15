"""

A test to ensure initialized/escaping source/escaping geometry energy tallies
are consistent with previously calculated values.

"""

from tests.utils import values_close

import math
import os
import re

EGSINP = "source_energies.egsinp"
TIME_LIMIT_S_PER_MHZ = 2/2993. # s / MHz

expected_results = {
    'initialized': 1135.1,
    'escaping_source': 898.8,
    'escaping_geom': 702.01,
}


def compare_results(egslst, inp_name):


    actual_results = {
        "initialized": float(re.findall("Total Energy Initialized\s*:(.*?) MeV", egslst,re.S)[0]),
        "escaping_source": float(re.findall("Total Energy Escaping Sources\s*:(.*?) MeV", egslst,re.S)[0]),
        "escaping_geom": float(re.findall("Total Energy Escaping Geometry\s*:(.*?) MeV", egslst,re.S)[0]),
    }

    all_good = True
    for etype, expected in list(expected_results.items()):
        actual =  actual_results[etype]
        if not values_close(actual, expected, max_percent_diff=0.01):
            all_good = False


    return all_good, actual_results, expected_results
