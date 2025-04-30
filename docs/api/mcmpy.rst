API
===

This is the C++ extension module built with pybind11.

.. autoclass:: mcmpy.Data
   :members:
   :undoc-members:

.. autoclass:: mcmpy.MCM

   .. rubric:: Methods
   .. automethod:: mcmpy.MCM.generate_data_file
   .. automethod:: mcmpy.MCM.generate_data_object
   .. automethod:: mcmpy.MCM.move_variable_in
   .. automethod:: mcmpy.MCM.move_variable_out
   .. automethod:: mcmpy.MCM.move_variable

   .. note::

      The following methods are only available when the :class:`MCM` object is the result of an :class:`MCMSearch` procedure. 

   .. automethod:: mcmpy.MCM.print_info
   .. automethod:: mcmpy.MCM.get_best_log_evidence
   .. automethod:: mcmpy.MCM.get_best_log_evidence_icc
   
   .. rubric:: Attributes
   .. autoattribute:: mcmpy.MCM.n
   .. autoattribute:: mcmpy.MCM.n_icc
   .. autoattribute:: mcmpy.MCM.rank
   .. autoattribute:: mcmpy.MCM.array
   .. autoattribute:: mcmpy.MCM.array_gray_code
   .. autoattribute:: mcmpy.MCM.is_optimized

.. autoclass:: mcmpy.MCMSearch

   .. rubric:: Methods
   .. automethod:: mcmpy.MCMSearch.exhaustive
   .. automethod:: mcmpy.MCMSearch.hierarchical_greedy_merging
   .. automethod:: mcmpy.MCMSearch.hierarchical_greedy_divisive
   .. automethod:: mcmpy.MCMSearch.simulated_annealing
   .. automethod:: mcmpy.MCMSearch.get_mcm_in
   .. automethod:: mcmpy.MCMSearch.get_mcm_out
   
   .. rubric:: Attributes
   .. autoattribute:: mcmpy.MCMSearch.log_evidence_trajectory
   .. autoattribute:: mcmpy.MCMSearch.SA_max_iteration
   .. autoattribute:: mcmpy.MCMSearch.SA_temperature_initial
   .. autoattribute:: mcmpy.MCMSearch.SA_temperature_iteration_update

.. autoclass:: mcmpy.Basis

   .. rubric:: Methods
   .. automethod:: mcmpy.Basis.set_from_file
   .. automethod:: mcmpy.Basis.set_default
   .. automethod:: mcmpy.Basis.set_random
   .. automethod:: mcmpy.Basis.gauge_transform_data
   .. automethod:: mcmpy.Basis.gauge_transform_data_in_place
   .. automethod:: mcmpy.Basis.print_details
   
   .. rubric:: Attributes
   .. autoattribute:: mcmpy.Basis.n
   .. autoattribute:: mcmpy.Basis.q
   .. autoattribute:: mcmpy.Basis.matrix
   
   This attribute can be set using a 2D numpy array where every entry corresponds to a spin operator, and the given set of operators is linearly independent.
   **Note** that in the matrix the columns represent an operator, which is the transpose of the input.

   .. code-block:: python

      import numpy as np
      from mcmpy import Basis
      
      n = 3
      q = 5
      basis = Basis(n, q)
      basis.matrix = np.array([[1,2,4], [2,3,1], [0,2,1]])
      print(basis.matrix)

   .. code-block:: text
      
      [[1 2 0]
      [2 3 2]
      [4 1 1]]
      

.. autoclass:: mcmpy.BasisSearch

   .. rubric:: Methods
   .. automethod:: mcmpy.BasisSearch.exhaustive
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_fixed
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_iterative
   .. automethod:: mcmpy.BasisSearch.get_basis
   .. automethod:: mcmpy.BasisSearch.print_results
