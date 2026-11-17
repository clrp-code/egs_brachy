# CLRP egs++ brachytherapy source models #

CLRP TG43v2 low-energy seeds

This repository contains geometry model files for 40 low-energy
brachytherapy seeds described and benchmarked in the CLRP TG-43 dosimetry
parameter database. The models are to be used with the open source Monte
Carlo code egs_brachy which is an EGSnrc application available at
https://github.com/clrp-code/egs_brachy.

The CLRP-TG43v2 database is hosted at a Carleton University website
(https://physics.carleton.ca/clrp/egs_brachy/seed_database_v2) 
with DOI   doi.org/10.22215/clrp/tg43v2

If you use geometry models from this site, please cite:

• Habib Safigholi, Marc J.P. Chamberland, Randle E.P. Taylor, Christian H.
Allen, Martin P. Martinov, D. W. O. Rogers and Rowan M. Thomson, Update of
the CLRP TG-43 parameter database for low-energy brachytherapy sources,
Med. Phys. 47 (2020), 4656-4669

and

• M Chamberland, R E P Taylor, D W O Rogers and R M Thomson, egs_brachy: a
versatile and fast Monte Carlo code for brachytherapy Phys Med Biol 61
(2016), 8214-8231

## Files in each source directory

- `<model>.geom`: the source geometry, with its air kerma per history at
  the top.
- `<model>.shape`: the distribution of radioactivity, for the `:start source:`
  block.
- `boundary.shape`: a box that encloses every model in the directory with a
  10 um margin. Use it as the bounding shape for `egs_autoenvelope` region
  discovery and for source volume corrections. It is a box so that the Sobol
  sequence is used.
- `<model>_wrapped.geom` (some LDR seeds): the same seed inside a 10 um water
  cylinder. Wrappers were a workaround for geometry errors and are no longer
  needed; they are kept so that earlier results can be reproduced. Energy
  deposited in the wrapper is not scored, so without volume correction the dose
  in voxels containing a seed is 1.6-2.9% lower than with the unwrapped model.
