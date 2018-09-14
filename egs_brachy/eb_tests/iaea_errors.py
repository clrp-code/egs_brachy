error_messages = {}


new_source_errors = (

    #errors returned in iaea_new_source

    (105,'Header file not set'),

    (-1,'Unable to initialize phase space'),
    (-91,'Failed to get record contents'),
    (-93,'Failed to read header'),
    (-94,'Null file pointer to phase space file'),
    (-95,'Failed to set record for new phase space file'),
    (-96,'Phase space file failed to open'),
    (-98,'Maximumn number of sources exceeded or invalid source id'),
    (-99,'Invalid access mode'),
    (-100,'Path to phase space file is too long'),
    (-101,'Path to phase space file is too short')

)

error_messages.update(new_source_errors)

class IAEAPhaseSpaceSetupError(Exception):
    pass

class IAEAPhaseSpaceError(Exception):
    def __init__(self,err_id=None,message=""):
        if err_id is not None:
            try:
                self.message = error_messages[err_id]
            except KeyError:
                self.message = "Unknown IAEAPhsp Error %s" % str(err_id)
        else:
            self.message = message
