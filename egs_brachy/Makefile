###############################################################################
#
#  This is the Makefile of the egs_brachy application.
#
###############################################################################

include $(EGS_CONFIG)
include $(SPEC_DIR)egspp1.spec
include $(SPEC_DIR)egspp_$(my_machine).conf

# Specify the name of the user code.
# The name of the executable is determined from this variable.
#
USER_CODE = egs_brachy

# The following can be used to add user macros and mortran subroutines.
# The file(s) specified here are added after egsnrc.macros, machine.macros
# and egs_c_interface2.macros but before any files that have
# executable code.
#
EGSPP_USER_MACROS = egs_brachy.macros

# Specify from which base class this application is being derived.
# This has the effect of automatically compiling the base application
# class and including it into the list of object files.
#
EGS_BASE_APPLICATION = egs_advanced_application

# Specify the set of mortran sources used.
# Here we simply use the standard set defined in $HEN_HOUSE/specs/egspp1.spec
#
CPP_SOURCES = $(C_ADVANCED_SOURCES)

# Specify here other header files that your user code depends upon.
#

common_egspp_h = egs_vector.h egs_math.h egs_functions.h


common_eb_deps = $(addprefix $(ABS_EGSPP), $(common_egspp_h))

extra_user_files := phantom eb_volcor latch spec_scoring pubsub ginfo recycle phsp gzstream

egs_sources_dir = $(ABS_EGSPP)sources$(DSEP)

eb_debs = egs_brachy.h phantom.h ginfo.h eb_volcor.h recycle.h muen.h latch.h spec_scoring.h phsp.h tree.h gzstream$(DSEP)gzstream.h
other_dep_user_code = $(source_deps) $(ABS_EGSPP)egs_transformations.h $(ABS_EGSPP)egs_scoring.h $(ABS_EGSPP)egs_rndm.h $(eb_deps)

egspp_geom_libs = $(link2_prefix)egs_autoenvelope$(link2_suffix)

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
TURN_OFF_MACOS_WARNINGS = -Wno-macro-redefined -Wno-gnu-static-float-init
TURN_OFF_MACOS_COMPACT_UNWIND = -Wl,-no_compact_unwind
endif

user_libs = $(TURN_OFF_MACOS_COMPACT_UNWIND) $(egspp_geom_libs) $(link2_prefix)iaea_phsp $(link2_prefix)z

include $(HEN_HOUSE)makefiles$(DSEP)cpp_makefile


INCL = -Wall -Wno-sign-compare $(TURN_OFF_MACOS_WARNINGS) -I. -I.$(DSEP)gzstream -I$(HEN_HOUSE)lib$(DSEP)$(my_machine) -I$(abs_egspp) -I$(abs_egspp)$(DSEP)geometry -I$(egs_interface) -I$(HEN_HOUSE)iaea_phsp -I.$(DSEP)zlib

gzstream_$(my_machine).$(obje): gzstream$(DSEP)gzstream.C
phsp_$(my_machine).$(obje): phsp.cpp $(common_eb_deps)
ginfo_$(my_machine).$(obje): ginfo.cpp $(common_eb_deps)
latch_$(my_machine).$(obje): latch.cpp $(common_eb_deps)
phantom_$(my_machine).$(obje): phantom.cpp $(common_eb_deps)
eb_volcor_$(my_machine).$(obje): eb_volcor.cpp $(common_eb_deps)
spec_scoring_$(my_machine).$(obje): spec_scoring.cpp $(common_eb_deps)
recycle_$(my_machine).$(obje): recycle.cpp $(common_eb_deps)
pubsub_$(my_machine).$(obje): pubsub.cpp

sweep:
	make
	rm -f *_$(my_machine).o *_$(my_machine).mortlst mortjob.mortran egsnrc_$(my_machine).F

docs: _docs/egs_brachy.md
	cd _docs; python gen_docs.py
	cd _docs; doxygen ./egs_brachy.dox
	cd docs/latex; make pdf
	mkdir -p docs/pdf; mv docs/latex/refman.pdf docs/pdf/egs_brachy_technical_manual.pdf
	rm -rf docs/latex
	cp -rf docs ../
	cp _docs/egs_brachy_user_manual.pdf ../docs/pdf
	cp _docs/egs_brachy_paper2016.pdf ../docs/pdf
	rm -rf docs/*

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
eb_iaeaphsp:
	cd eb_iaeaphsp_source; make
	ln -sf ${HEN_HOUSE}/egs++/dso/${my_machine}/libeb_iaeaphsp_source.dylib ${HEN_HOUSE}/egs++/dso/${my_machine}/libeb_iaeaphsp_source.so
else
eb_iaeaphsp:
	cd eb_iaeaphsp_source; make
endif

test:
	python run_tests.py

all: eb_iaeaphsp


.PHONY: clean library docs all sweep
