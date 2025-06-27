Data
====

The :class:`Data <mcmpy.Data>` class represents a dataset. Each datapoint consists of a discrete value for each variable and corresponds to a configuration of the system.
This class supports the computation of key information-theoretic quantities—such as the log-likelihood, log-evidence, entropy, and both geometric and parametric model complexities.
Additionally, a Data object serves as the input parameter for search algorithms implemented in the :class:`MCMSearch <mcmpy.MCMSearch>` and :class:`BasisSearch <mcmpy.BasisSearch>` classes
to infer either the best minimally complex (MCM) or the optimal basis representation of the system, respectively.

.. autoclass:: mcmpy.Data

   .. rubric:: Initialization

   .. py:method:: __init__(filename: str, n_var: int, n_states: int)
   
      Constructs a new Data object by loading a dataset from a file.

      :param filename: Path to the file.
      :type filename: str
      :param n_var: Number of variables in the system.
      :type n_var: int
      :param n_states: Number of values each variable can take.
      :type n_states: int

   .. rubric:: Methods

   .. py:method:: log_evidence_icc(mcm: MCM)

      Computes the log-evidence per ICC of the dataset for a given MCM.

      :param mcm: The MCM object for which the log-evidence will be computed per ICC.
      :type mcm: MCM
      :return: The log-evidence per ICC.
      :rtype: numpy.ndarray

   .. py:method:: log_evidence_icc(partition: numpy.ndarray)
      :noindex:

      Computes the log-evidence per ICC of the dataset for a given partition.

      :param partition: The partition of the MCM for which the log-evidence will be computed per ICC.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The log-evidence per ICC.
      :rtype: numpy.ndarray

   .. py:method:: log_evidence(mcm: MCM)

      Computes the total log-evidence of the dataset for a given MCM.

      :param mcm: The MCM object for which the log-evidence will be computed.
      :type mcm: MCM
      :return: The total log-evidence.
      :rtype: float

   .. py:method:: log_evidence(partition: numpy.ndarray)
      :noindex:

      Computes the log-evidence of the dataset for a given partition.

      :param partition: The partition of the MCM for which the log-evidence will be computed.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The total log-evidence.
      :rtype: float

   .. py:method:: log_likelihood_icc(mcm: MCM)

      Computes the log-likelihood per ICC of the dataset for a given MCM.

      :param mcm: The MCM object for which the log-likelihood will be computed per ICC.
      :type mcm: MCM
      :return: The log-likelihood per ICC.
      :rtype: numpy.ndarray

   .. py:method:: log_likelihood_icc(partition: numpy.ndarray)
      :noindex:

      Computes the log-likelihood per ICC of the dataset for a given partition.

      :param partition: The partition of the MCM for which the log-likelihood will be computed per ICC.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The log-likelihood per ICC.
      :rtype: numpy.ndarray

   .. py:method:: log_likelihood(mcm: MCM)

      Computes the total log-likelihood of the dataset for a given MCM.

      :param mcm: The MCM object for which the log-likelihood will be computed.
      :type mcm: MCM
      :return: The total log-likelihood.
      :rtype: float

   .. py:method:: log_likelihood(partition: numpy.ndarray)
      :noindex:

      Computes the log-likelihood of the dataset for a given partition.

      :param partition: The partition of the MCM for which the log-likelihood will be computed.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The total log-likelihood.
      :rtype: float

   .. py:method:: complexity_geometric_icc(mcm: MCM)

      Computes the geometric complexity per ICC of the dataset for a given MCM.

      :param mcm: The MCM object for which the geometric complexity will be computed per ICC.
      :type mcm: MCM
      :return: The geometric complexity per ICC.
      :rtype: numpy.ndarray

   .. py:method:: complexity_geometric_icc(partition: numpy.ndarray)
      :noindex:

      Computes the geometric complexity per ICC of the dataset for a given partition.

      :param partition: The partition of the MCM for which the geometric complexity will be computed per ICC.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The geometric complexity per ICC.
      :rtype: numpy.ndarray

   .. py:method:: complexity_geometric(mcm: MCM)

      Computes the total geometric complexity of the dataset for a given MCM.

      :param mcm: The MCM object for which the geometric complexity will be computed.
      :type mcm: MCM
      :return: The total geometric complexity.
      :rtype: float

   .. py:method:: complexity_geometric(partition: numpy.ndarray)
      :noindex:

      Computes the geometric complexity of the dataset for a given partition.

      :param partition: The partition of the MCM for which the geometric complexity will be computed.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The total geometric complexity.
      :rtype: float

   .. py:method:: complexity_parametric_icc(mcm: MCM)

      Computes the parametric complexity per ICC of the dataset for a given MCM.

      :param mcm: The MCM object for which the parametric complexity will be computed per ICC.
      :type mcm: MCM
      :return: The parametric complexity per ICC.
      :rtype: numpy.ndarray

   .. py:method:: complexity_parametric_icc(partition: numpy.ndarray)
      :noindex:

      Computes the parametric complexity per ICC of the dataset for a given partition.

      :param partition: The partition of the MCM for which the parametric complexity will be computed per ICC.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The parametric complexity per ICC.
      :rtype: numpy.ndarray

   .. py:method:: complexity_parametric(mcm: MCM)

      Computes the total parametric complexity of the dataset for a given MCM.

      :param mcm: The MCM object for which the parametric complexity will be computed.
      :type mcm: MCM
      :return: The total parametric complexity.
      :rtype: float

   .. py:method:: complexity_parametric(partition: numpy.ndarray)
      :noindex:

      Computes the parametric complexity of the dataset for a given partition.

      :param partition: The partition of the MCM for which the parametric complexity will be computed.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The total parametric complexity.
      :rtype: float

   .. py:method:: minimum_description_length(mcm: MCM)

      Computes the minimum description length of the dataset for a given MCM.

      :param mcm: The MCM object for which the minimum description length will be computed.
      :type mcm: MCM
      :return: The minimum description length.
      :rtype: float

   .. py:method:: minimum_description_length(partition: numpy.ndarray)
      :noindex:

      Computes the minimum description length of the dataset for a given partition.

      :param partition: The partition of the MCM for which the minimum description length will be computed.
                        The required format for this parameter is described in the MCM attributes :ref:`array <mcm_array_attribute>` and :ref:`array_gray_code <mcm_array_gray_attribute>`.
      :type partition: numpy.ndarray
      :return: The minimum description length.
      :rtype: float


   .. py:method:: entropy(base: int)

      Computes the entropy of the dataset using a given base.

      :param base: Optional argument to indicate the base used for the computation of the entropy. The default option is :math:`q`.
      :type base: int, optional
      :return: The entropy of the dataset.
      :rtype: float

   .. py:method:: entropy_of_spin_operator(spin_op: numpy.ndarray)

      Computes the entropy of a given spin operator applied on the dataset.

      :param spin_op: Spin operator for which the entropy will be computed.
                      The spin operator should be given as an array of length :math:`n` where each entry represent the exponent of the corresponding variable in the operator.
      :type spin_op: numpy.ndarray
      :return: The entropy of the spin operator when applied on the dataset.
      :rtype: float

   .. rubric:: Attributes
   
   .. py:attribute:: n
      :type: int
      
      The number of variables in the system (read-only).

   .. py:attribute:: q
      :type: int
      
      The number of states each variable can take (read-only).

   .. py:attribute:: N
      :type: int
      
      The actual number of datapoints in the dataset (read-only).

   .. py:attribute:: N_assumed
      :type: int

      The assumed number of datapoints in the dataset.

      This attribute can be changed to perform an analysis of the dataset under the assumption that it is either larger or smaller.

   .. py:attribute:: N_unique
      :type: int
      
       The number of unique datapoints in the dataset (read-only).
