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

## Installation

egs\_brachy requires the [CLRP fork of EGSnrc](https://github.com/clrp-code/EGSnrc_CLRP)
(`egs_brachy` branch), not the standard NRC distribution.

**Recommended (release tarball):** download `EGSnrc_CLRP-eb-setup-VERSION.tar.gz` from the
[EGSnrc\_CLRP releases](https://github.com/clrp-code/EGSnrc_CLRP/releases) page, then:

```bash
tar -xzf EGSnrc_CLRP-eb-setup-VERSION.tar.gz
cd EGSnrc_CLRP-eb-setup-VERSION
./eb-setup.sh install --egs-home "$HOME/egs_home/"
cd ~/EGSnrc_CLRP && source ./eb-env.sh
```

`eb-setup.sh` downloads the full release, runs EGSnrc `configure`, syncs egs\_brachy to
`$EGS_HOME`, and writes `eb-env.sh`. Use `./eb-setup.sh update` for later upgrades
(`update --from-tarball PATH` when offline). See `./eb-setup.sh help` for options.

**Git checkout (developers):** clone `EGSnrc_CLRP`, `git checkout egs_brachy`,
`git submodule update --init --recursive` (egs\_brachy is the submodule at
`HEN_HOUSE/user_codes/egs_brachy`), then `./HEN_HOUSE/scripts/configure` — or
`./eb-setup.sh install --git`.

## Documentation

Technical documentation for egs\_brachy is available in html at
https://clrp-code.github.io/egs_brachy/ .

The user's guide (detailed installation and usage) is available
[here](https://clrp-code.github.io/egs_brachy/pdf/egs_brachy_user_manual.pdf).

A graphical user interface for egs\_brachy, [eb\_gui](https://github.com/clrp-code/eb_gui), is
available separately. eb\_gui helps set up egs\_brachy calculations and includes tools for DICOM
conversion and 3ddose analysis. Installation instructions and a user guide are in the
[eb\_gui repository](https://github.com/clrp-code/eb_gui).

Hosted CLRP database papers (pdf):

- [CLRP TG-43 low-energy source database (2020)](https://clrp-code.github.io/egs_brachy/pdf/clrp_tg43_le_2020.pdf)
- [CLRP TG-43 high-energy source database (2023)](https://clrp-code.github.io/egs_brachy/pdf/clrp_tg43_he_2023.pdf)
- [CLRP eye plaque database (2021)](https://clrp-code.github.io/egs_brachy/pdf/clrp_eye_plaque_2021.pdf)

## Contact

For more information about egs\_brachy please contact:

- Marc Chamberland <mchamberland@mac.com>
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
([pdf](https://clrp-code.github.io/egs_brachy/pdf/clrp_tg43_le_2020.pdf))

when using low-energy (<sup>103</sup>Pd, <sup>125</sup>I, <sup>131</sup>Cs) source models, and:

Safigholi H, Chamberland M J P, Taylor R E P, Martinov M P, Rogers D W O,
Thomson R M **2023** Update of the CLRP Monte Carlo TG-43 parameter database
for high-energy brachytherapy sources, _Med. Phys._ [**50** 1928–1941](https://doi.org/10.1002/mp.16176)
([pdf](https://clrp-code.github.io/egs_brachy/pdf/clrp_tg43_he_2023.pdf))

when using high-energy (<sup>169</sup>Yb, <sup>192</sup>Ir, <sup>137</sup>Cs, <sup>60</sup>Co) source
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
