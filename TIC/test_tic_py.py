import ctypes
import os
import sys

# Locate built shared library in TIC directory
libname = "tic"
if sys.platform == "win32":
    libname = "libtic.dll"
elif sys.platform == "darwin":
    libname = "libtic.dylib"
else:
    libname = "libtic.so"

library_paths = [
    os.path.abspath(os.path.join(os.path.dirname(__file__), libname)),
    os.path.abspath(os.path.join(os.path.dirname(__file__), "build", libname)),
    os.path.abspath(os.path.join(os.path.dirname(__file__), "build", "lib", libname)),
    os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "TIC", "build", "lib", libname)),
]
libpath = next((p for p in library_paths if os.path.exists(p)), None)
if libpath is None:
    raise FileNotFoundError(
        "C++ library not found. Build the project before running tests. "
        f"Checked paths: {library_paths}"
    )

# Ensure DLL dependencies can be resolved on Windows by adding the library directory to DLL search path.
if sys.platform == "win32":
    libdir = os.path.dirname(libpath)
    if hasattr(os, "add_dll_directory"):
        os.add_dll_directory(libdir)
    # If add_dll_directory is unavailable, falling back to PATH injection.
    os.environ["PATH"] = libdir + os.pathsep + os.environ.get("PATH", "")

# Load using absolute path to avoid name resolution issues
lib = ctypes.CDLL(libpath)

class TICInput(ctypes.Structure):
    _fields_ = [
        ("driverTorque", ctypes.c_float),
        ("safetyLimit", ctypes.c_float),
        ("thermalLimit", ctypes.c_float),
        ("motorSpeed", ctypes.c_float),
        ("phaseCurrent", ctypes.c_float),
        ("temperature", ctypes.c_float),
    ]

class TICOutput(ctypes.Structure):
    _fields_ = [
        ("finalTorque", ctypes.c_float),
        ("pwmDuty", ctypes.c_float * 3),
        ("faultActive", ctypes.c_bool),
    ]

lib.tic_create.restype = ctypes.c_void_p
lib.tic_destroy.argtypes = [ctypes.c_void_p]
lib.tic_step.argtypes = [ctypes.c_void_p, ctypes.POINTER(TICInput), ctypes.POINTER(TICOutput)]


def run_step(inp):
    handle = lib.tic_create()
    assert handle != 0
    out = TICOutput()
    lib.tic_step(handle, ctypes.byref(inp), ctypes.byref(out))
    lib.tic_destroy(handle)
    return out


def test_init_to_ready_to_run():
    inp = TICInput(0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
    out = run_step(inp)
    assert out.finalTorque == 0.0
    assert out.faultActive is False


def test_run_computation_and_pwm():
    inp = TICInput(100.0, 120.0, 110.0, 0.0, 50.0, 40.0)
    out = run_step(inp)
    assert abs(out.finalTorque - 100.0) < 1e-6
    assert 0.0 <= out.pwmDuty[0] <= 1.0
    assert 0.0 <= out.pwmDuty[1] <= 1.0
    assert 0.0 <= out.pwmDuty[2] <= 1.0
    assert out.faultActive is False


def test_fault_on_overcurrent():
    inp = TICInput(100.0, 100.0, 100.0, 0.0, 250.0, 40.0)
    out = run_step(inp)
    assert out.faultActive is True


def test_fault_on_over_temp():
    inp = TICInput(100.0, 100.0, 100.0, 0.0, 100.0, 150.0)
    out = run_step(inp)
    assert out.faultActive is True
