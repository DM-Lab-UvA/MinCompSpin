MCMPy documentation
===================

MinCompSpin is a C++ library with Python bindings for analyzing discrete datasets using Minimally Complex Models (MCMs).

MCMs are a family of maximum entropy models (known as spin models) that have interactions of arbitrarily high order grouped in a community-like structure. 
Compared to other spin models, they have minimal information-theoretic complexity. 
The analysis using MCMs can uncover (robust) community structures in the data while taking into account possible higher-order interactions between the variables.

The best MCM for a given dataset is found by optimising the marginal likelihood (or evidence) in the space of models (i.e., by finding the MCM that has the largest probability to produce the data integrated over all values of the parameters).

The package can be used to analyse datasets with up to n = 128 random variables and is limited to variables that can take up to q=10 different values

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   user_guide/index
   examples/index
   api/index
