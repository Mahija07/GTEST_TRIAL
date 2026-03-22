import os, sys
from test_tic_py import test_init_to_ready_to_run, test_run_computation_and_pwm, test_fault_on_overcurrent, test_fault_on_over_temp

if __name__ == '__main__':
    tests = [
        test_init_to_ready_to_run,
        test_run_computation_and_pwm,
        test_fault_on_overcurrent,
        test_fault_on_over_temp,
    ]
    print('Running TIC Python tests...')
    failed = 0
    for t in tests:
        try:
            t()
            print(f'PASS: {t.__name__}')
        except AssertionError as e:
            print(f'FAIL: {t.__name__} - {e}')
            failed += 1
    if failed:
        sys.exit(1)
    print('All tests passed.')
