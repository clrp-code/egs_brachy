#*****************************************************************************
#
#  $Id: Makefile,v 1.1 2005/04/29 20:50:22 iwan Exp $
#
#*****************************************************************************

include $(EGS_CONFIG)
include $(SPEC_DIR)egspp.spec
include $(SPEC_DIR)egspp_$(my_machine).conf

DEFS = $(DEF1) -DBUILD_EB_IAEA_PHSP_SOURCE_DLL

library = eb_iaeaphsp_source
lib_files = eb_iaeaphsp_source

link2_libs = iaea_phsp egspp

lib_objects = eb_iaeaphsp_source.$(obje)

common_h2 = $(addprefix $(HEN_HOUSE)egs++$(DSEP), $(common_h)) $(HEN_HOUSE)pieces$(DSEP)egs_config1.h

INC2 = -I$(HEN_HOUSE)lib$(DSEP)$(my_machine) -I$(HEN_HOUSE)egs++ \
       -I$(HEN_HOUSE)$(DSEP)egs++$(DSEP)sources$(DSEP)iaea_phsp \
       -I$(HEN_HOUSE)interface \
       -I..

my_deps = $(common_source_deps)
extra_dep = $(addprefix $(HEN_HOUSE)egs++$(DSEP), $(my_deps)) eb_iaeaphsp_source.cpp eb_iaeaphsp_source.h

$(ABS_DSO)$(libpre)$(library)$(libext): $(lib_objects)
	$(CXX) $(INC2) $(DEFS) $(opt) $(shared) $(lib_link1) $(lib_objects) $(extra) $(lib_link2)

$(lib_objects): $(common_h2) $(extra_dep)

$(lib_objects):
	$(obj_rule2)

$(make_depend)
