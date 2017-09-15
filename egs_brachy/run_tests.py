import glob
import os
import re
import shutil
from subprocess import Popen, PIPE
import sys
import traceback

VERBOSE = False
if "-v" in sys.argv:
    VERBOSE = True
    sys.argv.remove("-v")

timing_hard_fail = "--timing-hard-fail" in sys.argv

EGS_HOME = os.environ["EGS_HOME"]
EGS_BRACHY = os.path.join(EGS_HOME, "egs_brachy")

USER_CODE = "egs_brachy"
TEST_EGSINP_FILE = "eb_test_run"
TEST_EGSINP_PATH_ROOT = os.path.join(EGS_BRACHY, TEST_EGSINP_FILE)
TEST_EGSINP_PATH = TEST_EGSINP_PATH_ROOT + ".egsinp"


PASS_FMT = "%(pass_fail)s - %(test)s - ran in %(actual_time).3G s/MHz (%(real_time).3G s)"

TIMING_WARN_FMT = """%(pass_fail)s - %(test)s
    Timing: %(timing_pass_fail)s
        Limit: %(time_limit).3G s/MHz
        Actual  : %(actual_time).3G s/MHz
    Results: %(results_pass_fail)s
"""

FAIL_FMT = """%(pass_fail)s - %(test)s
    Timing: %(timing_pass_fail)s
        Limit: %(time_limit).3G s/MHz
        Actual  : %(actual_time).3G s/MHz
    Results: %(results_pass_fail)s
        Expected: %(expected_results)s
        Actual  : %(actual_results)s
"""

cpu_speed_cmd = """grep -i "cpu mhz" /proc/cpuinfo | tail -1 | awk -F ":" '{print $2}'"""

try:

    try:
        CPU_MHZ = float(os.environ["CPU_MHZ"])
        source = "CPU_MHZ env variable"
    except (ValueError, KeyError):
        #stdin, stdout, stderr = os.popen3(cpu_speed_cmd)

        p = Popen(cpu_speed_cmd, shell=True,
                stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=True)
        (stdin, stdout, stderr) = (p.stdin, p.stdout, p.stderr)
        CPU_MHZ = float(stdout.read())
        source = "/proc/cpuinfo"

    print(("CPU speed read from %s as %f MHz" % (source, CPU_MHZ)))

except (ValueError, TypeError):
    sys.exit(
        "Unable to determine CPU speed. You must set CPU_MHZ "
        "environment variable before running the test suite."
    )

TIMING_MARGIN = 1.05


def dyn_import(name):
    mod = __import__(name)
    components = name.split('.')
    for comp in components[1:]:
        mod = getattr(mod, comp)
    return mod


def create_egsinp(test_module):
    egsinp = test_module.EGSINP
    path_components = test_module.__name__.split(".") + [egsinp]
    src = os.path.join(*path_components)
    shutil.copy(src, TEST_EGSINP_PATH)


def find_cpu_time(egslst):
    try:
        return float(re.findall("Finished simulation.*?CPU time:(.*?)s.*", egslst, re.S)[0])
    except:
        return None


def run_simulation():

    command = "%s -i %s" % (USER_CODE, TEST_EGSINP_FILE)
    p = Popen(command, shell=True,
            stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=True)
    (stdin, stdout, stderr) = (p.stdin, p.stdout, p.stderr)
    egslst = stdout.read()
    errors = stderr.read()
    if not isinstance(egslst, str):
        egslst = egslst.decode("utf-8")
        errors = errors.decode("utf-8")
    cpu_time = find_cpu_time(egslst) or -1

    return egslst, errors, cpu_time/CPU_MHZ


def cleanup():
    to_clean = glob.glob(TEST_EGSINP_PATH_ROOT+".*")
    for f in to_clean:
        # sanity check to make sure we're not deleting anything outside of this directory
        file_is_in_current_dir = os.path.dirname(f) == os.path.dirname(os.path.abspath(__file__))
        if file_is_in_current_dir:
            os.remove(f)


def find_tests():
    if len(sys.argv) > 1:
        tests = glob.glob(os.path.join(sys.argv[1], "__init__.py"))
    else:
        tests = glob.glob("tests/*/__init__.py")

    return [x.replace("/__init__.py", "").replace("/", ".") for x in tests]


def run_all_tests():

    pass_count = 0
    warn_count = 0
    all_tests = find_tests()
    for t in all_tests:

        print(("Running test '%s'..." % (t,)))
        test_module = dyn_import(t)
        create_egsinp(test_module)

        time_limit = test_module.TIME_LIMIT_S_PER_MHZ

        try:
            egslst, errors, actual_time = run_simulation()
            if VERBOSE:
                print(egslst)
                print(errors)
            if actual_time < 0:
                print("Simulation did not complete. egs_brachy crash likely")
                continue
            results_pass, actual_results, expected_results = test_module.compare_results(egslst, TEST_EGSINP_FILE)
        except:
            print(("Exception while running test '%s'..." % t))
            traceback.print_exc()
            continue

        timing_passes = actual_time <= TIMING_MARGIN*time_limit
        if not (timing_hard_fail and timing_passes):
            warn_count += 1

        passes = results_pass and timing_passes if timing_hard_fail else results_pass

        pass_count += int(passes)

        if results_pass and timing_passes:
            fmt = PASS_FMT
        elif results_pass:
            fmt = TIMING_WARN_FMT
        else:
            fmt = FAIL_FMT

        print((fmt % {
            "test": t,
            "pass_fail": ["FAIL", "PASS"][passes],
            "time_limit": time_limit,
            "actual_time": actual_time,
            "real_time": actual_time*CPU_MHZ,
            "timing_pass_fail": ["FAIL", "PASS"][timing_passes],
            "expected_results": expected_results,
            "actual_results": actual_results,
            "results_pass_fail": ["FAIL", "PASS"][results_pass],
        }))

        cleanup()

    print(("="*80))
    print(("Tests finished %d/%d passed with %d warnings" % (pass_count, len(all_tests), warn_count)))

if __name__ == "__main__":
    run_all_tests()
