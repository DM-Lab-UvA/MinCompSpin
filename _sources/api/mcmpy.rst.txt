API
===

This is the C++ extension module built with pybind11.

.. autoclass:: mcmpy.Data
   :members:
   :undoc-members:

.. autoclass:: mcmpy.MCM

   .. rubric:: Constructors
   .. autofunction:: mcmpy.MCM.__init__

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
   
   This attribute can be set using a 2D numpy array. Each row represents an ICC and each column a variable.
   If a variable is present in a component, the corresponding element is one and zero otherwise.
   In the example below, the partition consists of 3 components. Component 1 contains only variable 0.
   Variables 1 and 3 make up the second component. The last component contains variable 2.

   .. code-block:: python

      import numpy as np
      from mcmpy import MCM
      
      n = 4
      partition = np.array([[1,0,0,0], [0,1,0,1], [0,0,1,0]])
      model = MCM(n, partition)

   .. autoattribute:: mcmpy.MCM.array_gray_code

   This attribute can be set using a 1D numpy array. 
   The elements of the array correspond to the component index in which each variable is present. 
   To indicate that a variable is not present in the model, the component index -1 is used.
   The same partition as in the example above in this form would be:

   .. code-block:: python

      import numpy as np
      from mcmpy import MCM
      
      n = 4
      partition = np.array([[0,1,2,1]])
      model = MCM(n, partition)   

   .. autoattribute:: mcmpy.MCM.is_optimized

.. autoclass:: mcmpy.MCMSearch

   .. rubric:: Constructors
   .. automethod:: mcmpy.MCMSearch.__init__

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

   .. rubric:: Constructors
   .. automethod:: mcmpy.BasisSearch.__init__

   .. rubric:: Methods
   .. automethod:: mcmpy.BasisSearch.exhaustive
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_fixed
   .. automethod:: mcmpy.BasisSearch.up_to_order_k_iterative
   .. automethod:: mcmpy.BasisSearch.get_basis
   .. automethod:: mcmpy.BasisSearch.print_results
