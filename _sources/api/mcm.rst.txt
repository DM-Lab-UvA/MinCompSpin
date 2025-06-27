MCM
===

The :class:`MCM <mcmpy.MCM>` class represents a Minimally Complex Model (MCM).
These are a family of maximum entropy models (known as spin models) that have interactions of arbitrarily high order grouped in a community-like structure, which are called independent complete components (ICCs).

An MCM object can be initialized explicitly using a binary matrix or gray code array, or using standard templates such as "independent", "complete", or "random".
This class supports flexible manipulation of the model structure through methods that add, remove, or move variables across components. It also provides methods for generating data from a system which has this model structure, either as a data file or a :class:`Data <mcmpy.Data>` object.

An :class:`MCM <mcmpy.MCM>` object is also given as the output of a model selection procedure from the :class:`MCMSearch <mcmpy.MCMSearch>` class. In such cases, additional methods are available to retrieve information about the optimal model for a given :class:`Data <mcmpy.Data>` object.

.. autoclass:: mcmpy.MCM
   
   .. rubric:: Initialization

   .. py:method:: __init__(n: int)
   
      Constructs a new MCM object with a given number of variables.

      The default partition is the independent model, where each variable is placed in its own component.

      :param n: Number of variables in the system.
      :type n: int

   .. py:method:: __init__(n: int, partition: numpy.ndarray)
      :noindex:
   
      Constructs a new MCM object with a given number of variables and a specific partition.
      
      The partition must be given as a 2D binary array or a 1D gray code array.
      See the attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>` below for an example.
      
         - In the 2D representation each row represents an ICC and each column a variable. If a variable is present in a component, the corresponding element is one and zero otherwise.
         - In the 1D gray code representation, the elements correspond to the component index in which each variable is present. To indicate that a variable is not present in the model, the component index -1 is used.
      
      :param n: Number of variables in the system.
      :type n: int
      :param partition: Partition of the MCM
      :type partition: numpy.ndarray

   .. py:method:: __init__(n: int, partition: str)
      :noindex:

      Constructs a new MCM object with a given number of variables and a partition type.
      
      Valid partition types:
         - "independent" : All variables are placed in separate components.
         - "complete" : All variables are placed in a single component.
         - "random" : Each variable is assigned to a random component.

      :param n: Number of variables in the system.
      :type n: int
      :param partition: String indicating the type of partition.
      :type partition: str

   .. rubric:: Methods

   .. py:method:: generate_data_file(N_samples: int, data: Data, filename: str)
   
      Generates a file containing N samples from this MCM combined with a given dataset.

      :param N_samples: The number of samples that should be generated.
      :type N_samples: int
      :param data: The dataset from which the model parameters are inferred.
      :type data: Data
      :param filename: Path to the file that will contain the generated data.
      :type filename: str

   .. py:method:: generate_data_object(N_samples: int, data: Data)
   
      Generates a Data object containing N samples from this MCM combined with a given dataset.

      :param N_samples: The number of samples that should be generated.
      :type N_samples: int
      :param data: The dataset from which the model parameters are inferred.
      :type data: Data
      :return: A Data object containing the generated samples.
      :rtype: Data

   .. py:method:: move_variable_in(var_index: int, comp_index: int)

      Adds a variable to a specified component in the partition.

      The variable index must be between 0 and :math:`n-1` and refer to a variable that is not currently assigned to any component.
      The component index must also be between 0 and :math:`n-1`.
      If the specified component index exceeds the index of the first empty component, the variable will instead be added to the first available empty component.
      **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
      The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

      :param var_index: Index of the variable to add.
      :type var_index: int
      :param comp_index: Index of the component to where the variable will move.
      :type comp_index: int

   .. py:method:: move_variable_out(var_index: int)

      Removes a specific variable from the partition.

      The variable index must be between 0 and :math:`n-1` and refer to a variable that is currently assigned to a component in the partition.
      After removal, the variable will no longer be associated with any component.
      **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
      The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

      :param var_index: Index of the variable to remove.
      :type var_index: int

   .. py:method:: move_variable(var_index: int, comp_index: int)

      Moves a specific variable to another component.

      The variable index should be a value between 0 and :math:`n-1` and refer to a variable that is present in the partition.
      The given component index should be a value between 0 and :math:`n-1`. 
      If the specified component index exceeds the index of the first empty component, the variable will instead be moved to the first available empty component.
      **Note** that this function will reset the optimized status of the MCM if it was the result of an MCMSearch before.
      The functions 'print_info', 'get_best_log_evidence' and 'get_best_log_evidence_icc' will no longer be available.

      :param var_index: Index of the variable to move.
      :type var_index: int
      :param comp_index: Index of the component to where the variable will move.
      :type comp_index: int

   .. note::

      The following methods are only available when the :class:`MCM` object is the result of an :class:`MCMSearch` procedure. 

   .. py:method:: print_details()

      Prints out the information about the partition and the log-evidence of the MCM.
      
      Every component of the MCM is printed out as a binary string combined with its log-evidence.
      Works only after a search for the best mcm has been run for a given dataset. 

   .. py:method:: get_best_log_evidence()

      Returns the best log-evidence of the MCM.

      Works only after a search for the best mcm has been run for a given dataset.

      :return: The log-evidence of the MCM.
      :rtype: float

   .. py:method:: get_best_log_evidence_icc()

      Returns the best log-evidence of the partition per component.

      Works only after a search for the best mcm has been run for a given dataset.

      :return: Array containing the log-evidence of every component of the MCM.
      :rtype: numpy.ndarray
   
   .. rubric:: Attributes

   .. py:attribute:: n
      :type: int
   
      The number of variables in the system (read-only).

   .. py:attribute:: n_icc
      :type: int
      
      The number of independent complete components (ICCs) that the MCM has (read-only).

   .. py:attribute:: rank
      :type: int
      
      The number of variables currently assigned to any component in the MCM (read-only).

   .. _mcm_array_attribute:

   .. py:attribute:: array
      :type: numpy.ndarray

      The partition of the MCM represented as a 2D binary array.
      
      This attribute can be set using a 2D numpy array. Each row represents an ICC and each column corresponds to a variable.
      If a variable is present in a component, the corresponding element is one and zero otherwise.
      In the example below, the partition consists of 3 components. Component 1 contains only variable 0.
      Variables 1 and 3 make up the second component. The last component contains variable 2.

      .. code-block:: python

         import numpy as np
         from mcmpy import MCM
         
         n = 4
         partition = np.array([[1,0,0,0], [0,1,0,1], [0,0,1,0]])
         model = MCM(n, partition)

   .. _mcm_array_gray_attribute:

   .. py:attribute:: array_gray_code
      :type: numpy.ndarray

      The partition of the MCM represented as a 1D gray code array.

      This attribute can be set using a 1D numpy array. 
      The elements of the array correspond to the component index in which each variable is present. 
      To indicate that a variable is not present in the model, the component index -1 is used.
      The same partition as in the example above in gray code form would be:

      .. code-block:: python

         import numpy as np
         from mcmpy import MCM
         
         n = 4
         partition = np.array([[0,1,2,1]])
         model = MCM(n, partition)   

   .. py:attribute:: is_optimized
      :type: boolean
      
       Boolean value indicating whether the MCM is the result from an MCMSearch procedure (read-only).
