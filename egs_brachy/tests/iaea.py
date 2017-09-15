import ctypes
import glob
import os
import sys

HEN_HOUSE = os.getenv("HEN_HOUSE")
try:
    IAEA_DLL = glob.glob(os.path.join(HEN_HOUSE,"egs++/dso/*/", "libiaea_phsp.so"))[0]
except IndexError:
    raise ValueError("libiaea_phsp.so not fount")
from ctypes import byref

import os

from . import iaea_errors
from . import iaea_types


iaeadll = ctypes.CDLL(IAEA_DLL)


class IAEAPhaseSpace(object):

    header_ext = '.IAEAheader'
    phsp_ext = '.IAEAphsp'

    #--------------------------------------------------------------------------
    def __init__(self,path,mode='r'):
        """Set up access to an IAEA phase space file

        Arguments:
        path -- The path to the iaea phase space file

        Keyword arguments:
        mode -- 'r' for read, 'w' for read/write or 'a' for 'append' (default 'r')

        """
        self._set_path(path)
        self._source_id = iaea_types.IAEA_I32(-1)

        try:
            self.access = iaea_types.iaea_file_modes[mode]
        except:
            err_msg = 'Invalid file mode specified: %s' % mode
            raise iaea_errors.IAEAPhaseSpaceSetupError(err_msg)

        self._create_source()
    #--------------------------------------------------------------------------
    def _create_source(self):
        result = iaea_types.IAEA_I32(0)
        iaeadll.iaea_new_source(byref(self._source_id), self.path.encode(), byref(self.access),
                                byref(result), ctypes.c_int(len(self.path.encode())))

        print( result)
        if 0 > result.value > iaea_types.max_sources:
            raise iaea_errors.IAEAPhaseSpaceError(result.value)
    #--------------------------------------------------------------------------
    def num_particles(self,particle_type = 'all'):
        """Return max number of particles of type particle_type

        Keyword arguments:
        particle_type -- type or category of particle to check (default 'all')

        """

        num_particles = 0
        np = iaea_types.IAEA_I64(0)

        try:
            ptypes = tuple(iaea_types.particle_types[particle_type])
        except TypeError:
            ptypes  = (iaea_types.particle_types[particle_type],)

        for ptype in ptypes:
            ptype = iaea_types.IAEA_I32(ptype)
            iaeadll.iaea_get_max_particles(byref(self._source_id),byref(ptype),byref(np))
            num_particles += np.value

        if num_particles < 0:
            message = "Unable to read number of particles of type: %s" % particle_type
            raise iaea_errors.IAEAPhaseSpaceError(message=message)

        return num_particles
    #--------------------------------------------------------------------------
    def num_orig_particles(self):
        """Return max number of particles of type particle_type

        Keyword arguments:
        particle_type -- type or category of particle to check (default 'all')

        """

        num_particles = 0
        np = iaea_types.IAEA_I64(0)

        iaeadll.iaea_get_total_original_particles(byref(self._source_id), byref(np))
        num_particles = np.value

        if num_particles < 0:
            message = "Unable to read number of particles of type: %s" % particle_type
            raise iaea_errors.IAEAPhaseSpaceError(message=message)

        return num_particles
    #----------------------------------------------------------------------
    def maximum_energy(self):
        """Return maximum energy in this source in (MeV)"""

        emax = iaea_types.IAEA_Float()
        iaeadll.iaea_get_maximum_energy(byref(self._source_id),byref(emax))
        if emax.value <0.:
            raise iaea_errors.IAEAPhaseSpaceError(message="Source not initialized")

        return emax.value
    #--------------------------------------------------------------------------
    @property
    def source_id(self):
        return self._source_id.value
    #--------------------------------------------------------------------------
    def _set_path(self,path):
        self.path = os.path.realpath(path)
        self.path = self.path.replace(self.header_ext,"").replace(self.phsp_ext, "")
