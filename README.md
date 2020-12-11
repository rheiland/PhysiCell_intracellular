# PhysiCell_intracellular: demonstrate Intracellular modeling, using SBML and libRoadrunner, in PhysiCell.

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

```
$ make roadrunner_simple1
$ make
$ test_rr1
$ cd output
$ cp ../scripts/anim_svg_substrate_grid_step.py .
$ python anim_svg_substrate_grid_step.py 0 0 0 600 0 600 -300 300 -300 300 0
```
