import ctypes

libc = ctypes.windll.LoadLibrary("./build/libSplineInterpolationLib.dll")
print(libc.getFirstY(1.5))
