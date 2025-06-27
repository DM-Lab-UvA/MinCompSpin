MCMSearch
=========

The :class:`MCMSearch <mcmpy.MCMSearch>` class provides multiple search algorithms for identifying the optimal Minimally Complex Model (MCM) for a given dataset.
The optimal MCM is the MCM that has the largest probability to produce the data integrated over all values of the parameters.

This class includes the following search algorithms:

- **Exhaustive search**: Evaluates all possible partitions and guarantees the global optimum (only feasible for small systems).
- **Hierarchical greedy algorithms**: Merge or divide components iteratively to improve log-evidence, offering faster but approximate solutions.
- **Simulated annealing**: A stochastic optimization method that balances exploration and exploitation to avoid local optima.

Each method returns an :class:`MCM <mcmpy.MCM>` object representing the best-found model and stores it internally.

.. autoclass:: mcmpy.MCMSearch

   .. rubric:: Initialization

   .. py:method:: __init__()

      Constructs an MCMSearch object that provides several methods to find the optimal partition.

   .. rubric:: Methods

   .. py:method:: exhaustive(data: Data)

      Performs an exhaustive search to find the optimal MCM for a given dataset.

      This function searches over all possible partitions of the variables to find the MCM with the largest log-evidence for the provided dataset.
      The result is returned as an :class:`MCM <mcmpy.MCM>` object and stored in the internal variable `mcm_out`.

      **Note** that this search is only feasible for small systems (:math:`n < 15`).

      :param data: The dataset for which the optimal MCM will be determined.
      :type data: Data
      :return: The MCM that has the largest log-evidence for the given dataset.
      :rtype: MCM

   .. py:method:: hierarchical_greedy_merging(data: Data, mcm_in: MCM, filename: str)

      Performs a hierarchical greedy merging procedure to find the optimal MCM for a given dataset.
      
      This search function consists of an iterative merging procedure.
      Since this is a heuristic algorithm, it doesn't go through all partitions and therefore does not guarantee recovering the MCM with the largest log-evidence.
      The result is returned as an :class:`MCM <mcmpy.MCM>` object and stored in the internal variable `mcm_out`.

      :param data: The dataset for which the optimal MCM will be determined.
      :type data: Data
      :param mcm_in: An optional MCM object representing the starting partition of the algorithm. 
                     If not provided, the independent model is used as the default starting partition.
      :type mcm_in: MCM, optional
      :param filename: Path to the file where the search details will be written.
                        If not provided, nfothing will be written to a file.
      :type filename: str, optional
      :return: The best fitting MCM for the given dataset found by the hierarchical greedy merging algorithm.
      :rtype: MCM

   .. py:method:: hierarchical_greedy_divisive(data: Data, mcm_in: MCM, filename: str)

      Performs a hierarchical greedy divisive procedure to find the optimal MCM for a given dataset.

      This search function consists of an iterative division procedure.
      Since this is a heuristic algorithm, it doesn't go through all partitions and therefore does not guarantee recovering the MCM with the largest log-evidence.
      The result is returned as an :class:`MCM <mcmpy.MCM>` object and stored in the internal variable `mcm_out`.

      :param data: The dataset for which the optimal MCM will be determined.
      :type data: Data
      :param mcm_in: An optional MCM object representing the starting partition of the algorithm. 
                     If not provided, the complete model is used as the default starting partition.
      :type mcm_in: MCM, optional
      :param filename: Path to the file where the search details will be written.
                        If not provided, nothing will be written to a file.
      :type filename: str, optional
      :return: The best fitting MCM for the given dataset found by the hierarchical greedy divisive algorithm.
      :rtype: MCM

   .. py:method:: simulated_annealing(data: Data, mcm_in: MCM, filename: str)

      Performs a simulated annealing algorithm to find the optimal MCM for a given dataset.

      Since this is a heuristic algorithm, it doesn't go through all partitions and therefore does not guarantee recovering the MCM with the largest log-evidence.
      The result is returned as an :class:`MCM <mcmpy.MCM>` object and stored in the internal variable `mcm_out`.

      :param data: The dataset for which the optimal MCM will be determined.
      :type data: Data
      :param mcm_in: An optional MCM object representing the starting partition of the algorithm. 
                     If not provided, the independent model is used as the default starting partition.
      :type mcm_in: MCM, optional
      :param filename: Path to the file where the search details will be written.
                        If not provided, nothing will be written to a file.
      :type filename: str, optional
      :return: The best fitting MCM for the given dataset found by the simulated annealing algorithm.
      :rtype: MCM

   .. py:method:: get_mcm_in()

      Returns an MCM object containing the starting partition of the last search.

      **Note** that this function can only be called once a search has been run and if the search was not an exhaustive.

      :return: The MCM that was used as starting point of the last search.
      :rtype: MCM

   .. py:method:: get_mcm_out()

      Returns an MCM object containing the solution of the last search.

      **Note** that this function can only be called once a search has been run.

      :return: The MCM that was the result of the last search.
      :rtype: MCM
   
   .. rubric:: Attributes

   .. py:attribute:: log_evidence_trajectory
      :type: numpy.ndarray
      
      Array with the log-evidence values encountered during the last search (read-only).
      
      - Exhaustive search: the log-evidence of all possible partitions is stored.
      - Simulated annealing: the log-evidence value of the current best solution at every iteration is stored. 
      - Hierarchical greedy algorithms: the log-evidence is stored only when an improvement is found.

   .. py:attribute:: SA_max_iteration
      :type: int
      
      The maximum number of iterations in the simulated annealing algorithm. 
      The default number of iterations is 50 000.

   .. py:attribute:: SA_temperature_initial
      :type: int
      
      The initial temperature used in the annealing process. 
      The default temperature is 100.

   .. py:attribute:: SA_temperature_iteration_update
      :type: int
      
      The number of iterations after which the temperature is updated. 
      The default number of iterations is 100.
