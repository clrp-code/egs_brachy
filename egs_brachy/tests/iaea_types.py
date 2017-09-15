import ctypes

IAEA_Float = ctypes.c_float

PIAEA_Float = ctypes.POINTER(IAEA_Float)

IAEA_I16  = ctypes.c_short
PIAEA_I16 = ctypes.POINTER(IAEA_I16)
IAEA_I32  = ctypes.c_int
PIAEA_I32 = ctypes.POINTER(IAEA_I32)
IAEA_I64  = ctypes.c_longlong
PIAEA_I64 = ctypes.POINTER(IAEA_I64)

iaea_file_modes = {
    'r': IAEA_I32(1),
    'w': IAEA_I32(2),
    'a': IAEA_I32(3)
}

all_particles = -1
photons       = 1
electrons     = 2
positrons     = 3
neutrons      = 4
protons       = 5

particle_types = {
    'all'     : all_particles,
    'photon'  : photons,
    'electron': electrons,
    'positron': positrons,
    'neutron' : neutrons,
    'proton'  : protons,
    'charged' : (electrons,positrons,protons),
    'neutral' : (photons,neutrons)
}


max_sources = 30
