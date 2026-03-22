@echo off
cd /d m:\git\GTest_Trial\TIC
python -c "import test_tic_py; print('import OK')"
python run_tic_py_tests.py
