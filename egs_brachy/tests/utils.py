import math
import re

REG_DOSE_UNC_RE = "\s+(\d)+\s+\d+\s+\d+\..*?\s+(.*?)\s+\+/-\s+(.*?)%\s+(.*?)\s+\+/-\s+(.*?)%"


def extract_all_doses(egslst):
    """return all regionss and doses from egslst file. This may
    include doses from more than one phantom"""
    return sorted(re.findall(REG_DOSE_UNC_RE, egslst))

def values_close(a, b, max_percent_diff=0.001):
    if a == 0:
        return values_close_abs(a, b, max_percent_diff)
    return abs((a-b)/a) < max_percent_diff


def values_close_abs(a, b, max_diff=0.001):
    return abs(a-b) < max_diff

def read_csv_spectrum(fname):
    f = open(fname, "r")
    header = f.readline()
    energies = []
    counts = []
    unc = []

    for line in f.readlines():
        e, c, u = list(map(float, line.strip().split(",")))
        energies.append(e)
        counts.append(c)
        unc.append(u)

    return energies, counts, unc


def doses_approx_equal(d1, d1_unc, d2, d2_unc, max_percent_diff=None, compare_unc=True, max_unc_percent_diff=None):
    if max_percent_diff is None:
        max_percent_diff = max(d1_unc, d2_unc)

    if max_unc_percent_diff is None:
        max_unc_percent_diff = 0.005

    vc = values_close(d1, d2, max_percent_diff)
    if compare_unc:
        uc = values_close_abs(d1_unc, d2_unc, max_unc_percent_diff)
    else:
        uc = True

    return vc and uc


def read3ddose(fname):

    with open(fname, 'r') as f:
        dat = f.readlines()

    nx, ny, nz = list(map(int, dat[0].strip().split()))
    xbounds = list(map(float, dat[1].strip().split()))
    ybounds = list(map(float, dat[2].strip().split()))
    zbounds = list(map(float, dat[3].strip().split()))
    doses = list(map(float, dat[4].strip().split()))
    uncs = list(map(float, dat[5].strip().split()))

    return {
        'nx': nx,
        'ny': ny,
        'nz': nz,
        'xbounds': xbounds,
        'ybounds': ybounds,
        'zbounds': zbounds,
        'doses': doses,
        'uncs': uncs,
    }


def compare_3ddose_files(f1, f2, max_percent_diff=None):

    d1 =  read3ddose(f1)
    d2 =  read3ddose(f2)

    bounds_keys = ('xbounds', 'ybounds', 'zbounds',)
    for k in bounds_keys:
        bounds_close = all([values_close(b1,b2) for b1,b2 in zip(d1[k], d2[k])])
        if not bounds_close:
            print((k, " are different"))
            return False

    doses_and_uncs = list(zip(d1['doses'], d1['uncs'], d2['doses'], d2['uncs']))
    dose_diffs = [abs((a-b)/a) for a,_,b,_ in doses_and_uncs]

    doses_close = all([doses_approx_equal(d1, d1u, d2, d2u, max_percent_diff=max_percent_diff, compare_unc=False)
                        for d1, d1u, d2, d2u in doses_and_uncs])

    return doses_close


