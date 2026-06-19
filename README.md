## egs\_brachy

egs\_brachy is an egs++ application for rapid brachytherapy calculations
for both photon and electron sources. Some features of egs\_brachy
include:

- Dose scoring via a tracklength estimator or interaction scoring
- Scoring of the spectrum of particles emitted from a source or particle
  fluence in a voxel
- Primary and scatter separated dose calculations
- Particle recycling for simulations with multiple sources
- Bremsstrahlung cross section enhancement, uniform bremsstrahlung
  splitting, and Russian Roulette for efficient electronic brachytherapy
  simulations.
- A comprehensive library of brachytherapy source geometries ,

For a complete discussion of all egs\_brachy features as well as
discussions about benchmarking and calculation efficiency, please refer
to the paper listed below under [Citing egs\_brachy](#citing-egs_brachy).

When using source or eye-plaque models from the distributed geometry
library, please also cite the relevant CLRP database papers listed under
[CLRP database references](#clrp-database-references).

The initial release of egs\_brachy is version 2017.09.15 beta. We will continue
to update and maintain egs\_brachy, as well as this manual and the html
documentation.


## License

The egs\_brachy code (all pieces of code associated with the egs\_brachy code
    system) is copyrighted Rowan Thomson, Dave Rogers, Randle Taylor, and Marc
Chamberland. egs\_brachy is distributed as free software according to the terms
of the GNU Affero General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option), any later
version (http://www.gnu.org/licenses/).

## Documentation and Installation

Technical documentation for egs\_brachy (including brief installation instructions) is
available in html at https://clrp-code.github.io/egs_brachy/ .

A pdf of the user's guide to egs\_brachy (with more detailed installation instructions) is available
[here](https://clrp-code.github.io/egs_brachy/pdf/egs_brachy_user_manual.pdf).

Hosted CLRP database papers (pdf):

- [CLRP eye plaque database (2021)](https://clrp-code.github.io/egs_brachy/pdf/clrp_eye_plaque_2021.pdf)

Low- and high-energy CLRP TG-43 source database papers are linked by DOI
in [CLRP database references](#clrp-database-references) below.

## Contact

For more information about egs\_brachy please contact:

- Marc Chamberland <marc.chamberland@uvmhealth.org>
- Rowan Thomson <rthomson@physics.carleton.ca>
- Dave Rogers <drogers@physics.carleton.ca>

## Citing egs\_brachy

Citations of egs\_brachy should use the following reference:

Chamberland M J P, Taylor R E P, Rogers D W O, Thomson R M **2016**
egs\_brachy: A versatile and fast Monte Carlo code for brachytherapy,
    _Phys.  Med. Biol._, [**61** 8214–8231](http://dx.doi.org/10.1088/0031-9155/61/23/8214)

## CLRP database references

### Source models (`lib/geometry/sources/`)

Dose-rate constants and TG-43 parameters for the distributed brachytherapy
source models are documented in the CLRP TG-43 parameter database version 2
papers. Air-kerma strength per history factors are given in the `# air kerma:`
line at the top of each source `.geom` file (see the
[user manual](https://clrp-code.github.io/egs_brachy/pdf/egs_brachy_user_manual.pdf)).

Please cite:

Safigholi H, Chamberland M J P, Taylor R E P, Allen C H, Martinov M P,
Rogers D W O, Thomson R M **2020** Update of the CLRP TG-43 parameter
database for low-energy brachytherapy sources, _Med. Phys._ [**47** 4656–4669](https://doi.org/10.1002/mp.14249)

when using low-energy ($^{103}$Pd, $^{125}$I, $^{131}$Cs) source models, and:

Safigholi H, Chamberland M J P, Taylor R E P, Martinov M P, Rogers D W O,
Thomson R M **2023** Update of the CLRP Monte Carlo TG-43 parameter database
for high-energy brachytherapy sources, _Med. Phys._ [**50** 1928–1941](https://doi.org/10.1002/mp.16176)

when using high-energy ($^{169}$Yb, $^{192}$Ir, $^{137}$Cs, $^{60}$Co) source
models.

Database: [https://physics.carleton.ca/clrp/egs_brachy/seed_database_v2](https://physics.carleton.ca/clrp/egs_brachy/seed_database_v2)

### Eye plaques (`lib/geometry/eye_plaques/`)

Please cite:

Safigholi H, Parsons Z, Deering S G, Thomson R M **2021** Update of the CLRP
eye plaque brachytherapy database for photon-emitting sources, _Med. Phys._
[**48** 3373–3383](https://doi.org/10.1002/mp.14844)
([pdf](https://clrp-code.github.io/egs_brachy/pdf/clrp_eye_plaque_2021.pdf))

Database: [https://physics.carleton.ca/clrp/eye_plaque_v2](https://physics.carleton.ca/clrp/eye_plaque_v2)
(DOI [http://doi.org/10.22215/clrp/EPv2](http://doi.org/10.22215/clrp/EPv2))

## Reporting Bugs

Please report any issues you find with egs\_brachy using the [issue
tracker on GitHub](https://github.com/clrp-code/egs_brachy/issues)

## Contributing to egs\_brachy

Bug fixes and additional features are welcomed via pull requests on
GitHub. Please refer to the [EGSnrc Wiki Developers section](https://github.com/nrc-cnrc/EGSnrc/wiki) for
general recommendations about making pull requests.

## Commit history (change log)

The complete commit history (i.e., change log) can be consulted [here](https://github.com/clrp-code/egs_brachy/commits/main).
