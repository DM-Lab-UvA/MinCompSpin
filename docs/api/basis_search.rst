BasisSearch
===========

The :class:`BasisSearch <mcmpy.BasisSearch>` class provides methods to uncover the optimal basis representation for a discrete dataset.
The optimal basis representation is the one for which the set of the :math:`n` one-body spin operators model in that basis has the largest log-evidence.

This class includes the following search algorithms:

- **Exhaustive search**: Considers all spin operators to find the optimal basis, which is only feasible for small systems
- **up_to_order_k_fixed**: Considers all spin operators up to interaction-order :math:`k` to find the optimal basis.
- **up_to_order_k_iterative**: Considers all spin operators up to interaction-order :math:`k` in an interative procedure to find the optimal basis.

Each method returns a :class:`Basis <mcmpy.Basis>` object representing the best-found basis and stores it internally.

.. autoclass:: mcmpy.BasisSearch

   .. rubric:: Initialization

   .. py:method:: __init__()

      Constructs a BasisSearch object containing methods to find the best basis representation for a dataset.

   .. rubric:: Methods

   .. py:method:: exhaustive(data: Data, filename: str)

      Performs a search over all spin operators to find the optimal basis for a given dataset.

      This function searches over all spin operators of the variables and selects the optimal set of :math:`n` linearly independent spin operators with the lowest entropy over the data.
      The result is returned as an :class:`Basis <mcmpy.Basis>` object and stored in the internal variable `basis`.
      **Note** that this search is only feasible for small systems (:math:`n < 15`).

      :param data: The dataset for which the optimal basis representation will be determined.
      :type data: Data
      :param filename: Path to the file where the basis will be written.
                       If not provided, nothing will be written to a file.
      :type filename: str, optional
      :return: The optimal basis for the given dataset found by an exhaustive search.
      :rtype: Basis

   .. py:method:: up_to_order_k_fixed(data: Data, k: int, filename: str)

      Performs a search over all spin operators up to order :math:`k` to find the optimal basis for a given dataset.

      This function searches over all spin operators up to order :math:`k` and selects the set of :math:`n` linearly independent spin operators with the lowest entropy over the data.
      The result is returned as an :class:`Basis <mcmpy.Basis>` object and stored in the internal variable `basis`.

      :param data: The dataset for which the optimal basis representation will be determined.
      :type data: Data
      :param k: The maximum interaction order of the spin operators to consider.
      :type k: int
      :param filename: Path to the file where the basis will be written.
                       If not provided, nothing will be written to a file.
      :type filename: str, optional
      :return: The optimal basis for the given dataset found by a search up to order k.
      :rtype: Basis

   .. py:method:: up_to_order_k_iterative(data: Data, k: int, filename: str)

      Performs an iterative search over all spin operators up to order :math:`k` to find the optimal basis for a given dataset.

      This function searches iteratively over all spin operators up to order :math:`k` and selects the set of :math:`n` linearly independent spin operators with the lowest entropy over the data.
      In each iteration the dataset is transformed using the current best basis, and a new optimal basis is searched for.
      This process continues until the optimal basis remains unchanged between two iterations.
      The result is returned as an :class:`Basis <mcmpy.Basis>` object and stored in the internal variable `basis`.

      :param data: The dataset for which the optimal basis representation will be determined.
      :type data: Data
      :param k: The maximum interaction order of the spin operators to consider in each iteration.
      :type k: int
      :param filename: Path to the file where the basis will be written.
                       If not provided, nothing will be written to a file.
      :type filename: str, optional
      :return: The optimal basis for the given dataset found by an iterative search up to order k.
      :rtype: Basis

   .. py:method:: get_basis()

      Returns a Basis object containing the solution of the last search.

      **Note** that this function can only be called once a search has been run.

      :return: The Basis object that was the result of the last search.
      :rtype: Basis

   .. py:method:: print_result()

      Prints out the result of the last search.

      This function prints the search type, duration and the basis found by the last search.
      For a search up to order k, the maximum interaction order is also printed out.
      For an iterative search, the maximum interaction order and the number of iterations is also printed out.
