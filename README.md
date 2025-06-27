# MinCompSpin

MinCompSpin is a C++ library with Python bindings for analyzing discrete datasets using Minimally Complex Models (MCMs). 

MCMs are a family of maximum entropy models (known as spin models) that have interactions of arbitrarily high order grouped in a community-like structure. Compared to other spin models, they have minimal information-theoretic complexity. 
The analysis using MCMs can uncover (robust) community structures in the data while taking into account possible higher-order interactions between the variables. 

The best MCM for a given dataset is found by optimising the marginal likelihood (or evidence) in the space of models (i.e., by finding the MCM that has the largest probability to produce the data integrated over all values of the parameters). The approach was initially developed for binary data in Ref.[1] and later extended to discrete data in Ref. [2].

The package can be used to analyse datasets with **up to n = 128 random variables** and is limited to **variables that can take up to q=10 different values** (though it was extensively tested only up to q=5).

[1] *Bayesian inference of minimally complex models with interactions of arbitrary order*, C. de Mulatier and M. Marsili, [Phys. Rev. E 111, 054307](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.111.054307) ([arXiv:2008.00520](https://arxiv.org/abs/2008.00520))<br>
[2] *Modeling higher-order interactions in discrete data with maximum entropy models*, A. de Clercq and C. de Mulatier, arxiv (coming soon)


## General information

The approach searches for the factorisation of the multivariate model probability distribution that best matches the structure of the data.

This optimal factorisation can be in a different basis than the original basis of the data.
Running the best MCM search in the original basis of the data can be thought of as a high-order community detection for the data.

The most likely basis for the optimal factorisation can be found with this package using one of the “best basis”  search algorithms:
 - exhaustive search  (recommended up to $n=12$ variables max for quick runs; For $q=2$, running it for $n=20-25$ variable will of the order of a week);
 - exhaustive search among basis elements up to a fixed order $k$;
 - iterative search (computational time similar to fixed order $k$, but will explore basis elements of higher order via an iterative search).

Within a given basis, the optimal factorisation can be found using one of the “best MCM” search algorithms:

 - exhaustive search (recommended up to $n=12$ to $15$ variables max);
 - hierarchical greedy merging search;
 - hierarchical greedy divisive search;
 - simulated annealing.

The package also allows:
 - to test how good is a chosen MCM for a given dataset (compute maximum log-likelihood, log-evidence, description length (from MDL principle), and model geometric complexity);
 - to generate new samples from a fitted MCM. 

## Documentation

All the information about this package (installation, available objects/functions and how to use them) can be found in the [documentation](https://dm-lab-uva.github.io/MinCompSpin/index.html).

## Requirements

C++11, Python (version 3.11 or above)

**Dependencies:** requires NumPy Python package

**Installation:** using CMake (version 3.26.4) or using Docker

## To cite this repository
A. de Clercq, C. de Mulatier (2025). *MinCompSpin: Python package for analyzing discrete datasets with minimally complex models*. Zenodo. https://github.com/DM-Lab-UvA/MinCompSpin.

## Related previous repositories

In particular, the package includes improved codes from various previous repositories analyzing MCMs:

 - optimal MCM exhaustive search for binary data: [binary exhaustive search](https://github.com/clelidm/MinCompSpin_ExhaustiveSearch) by C. de Mulatier;
 - optimal MCM greedy search for binary data: [binary greedy search](https://github.com/clelidm/MinCompSpin_Greedy) by Sam Kamphof;
 - optimal MCM simulated annealing search for binary data: [binary SA search](https://github.com/DM-Lab-UvA/MCM_SA) by Ebo Peerbooms;
 - optimal best basis search for binary data with 128 bits (exhaustive and iterative): [binary basis search](https://github.com/clelidm/MinCompSpin_BasisSearch128) by C. de Mulatier;
 - optimal MCM and basis search for discrete data: [discrete MCM and basis search](https://github.com/AaronDC60/MinCompSpin_discrete) by Aaron de Clercq.

