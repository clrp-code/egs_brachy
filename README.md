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
to the paper listed below.

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

## Reporting Bugs

Please report any issues you find with egs\_brachy using the [issue
tracker on GitHub](https://github.com/clrp-code/egs_brachy/issues)

## Contributing to egs\_brachy

Bug fixes and additional features are welcomed via pull requests on
GitHub. Please refer to the [EGSnrc Wiki Developers section](https://github.com/nrc-cnrc/EGSnrc/wiki) for
general recommendations about making pull requests.

## Commit history (change log)

The complete commit history (i.e., change log) can be consulted [here](https://github.com/clrp-code/egs_brachy/commits/main).
