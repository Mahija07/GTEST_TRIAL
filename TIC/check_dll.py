import os, ctypes, traceback
p=os.path.abspath('build/libtic.dll')
print('exists', os.path.exists(p), p)
try:
    ctypes.WinDLL(p)
    print('loaded')
except Exception as e:
    print('load err', e)
    traceback.print_exc()
