API
===

This is the C++ extension module built with pybind11.

.. autoclass:: mcmpy.Data
   :members:
   :undoc-members:

.. autoclass:: mcmpy.MCM
   :members:
   :undoc-members:

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
   :members:
   :undoc-members:


.. autoclass:: mcmpy.BasisSearch

   .. rubric:: Methods
   .. automethod:: mcmpy.BasisSearch.exhaustive
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_fixed
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_iterative
   .. automethod:: mcmpy.BasisSearch.get_basis
   .. automethod:: mcmpy.BasisSearch.print_results

   .. rubric:: Attributes