Basis
=====

The :class:`Basis <mcmpy.Basis>` class defines and manages a set of spin operators that can be used as a basis representation for a discrete dataset. 
The basis is used to transform data between different representations.

By default, the class initializes the basis as the standard set of one-body field operators. 
However, it also supports initialization using a user-specified or random basis, provided the spin operators form a linearly independent set.

An :class:`Basis <mcmpy.Basis>` object is the output of search algorithms from the :class:`BasisSearch <mcmpy.BasisSearch>` class, in which case it represents the optimal basis representation of a given dataset.
The resutling :class:`Basis <mcmpy.Basis>` object can then be used to transform that dataset into the optimal basis representation.

.. autoclass:: mcmpy.Basis

   .. rubric:: Initialization

   .. py:method:: __init__(n: int, q: int)

      Constructs a new Basis object with just a number of variables and a number of states.
      
      The basis is set to its default which is the :math:`n` one-body spin operators.

      :param n: The number of variables in the system.
      :type n: int
      :param q: The number of states each variable can take.
      :type q: int

   .. py:method:: __init__(n: int, q: int, spin_operators: numpy.ndarray)
      :noindex:

      Constructs a new Basis object with a specif basis.

      :param n: The number of variables in the system.
      :type n: int
      :param q: The number of states each variable can take.
      :type q: int
      :param spin_operators: Vector with spin operators defining the basis.
      :type spin_operators: numpy.ndarray

   .. py:method:: __init__(n: int, q: int, filename: str)
      :noindex:

      Constructs a new Basis object with a basis specified in a file.

      The basis specified in the given file should satisfy the following constraints to be valid.
      The file should contain :math:`n` lines, each representing an operator.
      On each line, the first :math:`n` characters should be values between 0 and :math:`q-1` which indicate the exponent of each variable in the operator.
      For the basis to be valid, the given set of operators should also be linearly independent.      

      :param n: The number of variables in the system.
      :type n: int
      :param q: The number of states each variable can take.
      :type q: int
      :param filename: Path to the file containing the basis.
      :type filename: str

   .. rubric:: Methods

   .. py:method:: set_from_file(filename: str)

      Updates the basis using a given file.

      If the given file contains a valid basis, then the object will update its internal basis variable.
      The file should contain :math:`n` lines, each representing an operator.
      On each line, the first :math:`n` characters should be values between 0 and :math:`q-1` which indicate the exponent of each variable in the operator.
      For the basis to be valid, the given set of operators should also be linearly independent.

      :param filename: Path to the file where the basis is written.
      :type filename: str

   .. py:method:: set_default()

      Resets the basis to the default one.

      This function will set the internal basis variable to the default basis.
      The default basis corresponds to the :math:`n` field operators on each variable.

   .. py:method:: set_random()

      Sets the basis to a valid random basis.

      This function will generate random :math:`n` by :math:`n` matrices with entries between 0 and :math:`q-1` until its columns form a linearly independent basis.

      :return: The number of matrices generated before finding a linearly independent one.
      :rtype: int

   .. py:method:: gauge_transform_data(data: Data)

      Applies a gauge transformation to the given dataset.

      This function transforms a Data object expressed in the variables **s**
      such that it is expressed in the variables **σ**, where **σ** are the spin operators that make up the internal basis.
      The input data object is not modified.

      :param data: The dataset expressed in the variables **s** that will be transformed.
      :type data: Data
      :return: A Data object containing the dataset expressed in the variables **σ**.
      :rtype: Data

   .. py:method:: gauge_transform_data_in_place(data: Data)

      Applies a gauge transformation to the given dataset in place.

      This function transforms a Data object expressed in the variables **s** 
      such that it is expressed in the variables **σ**, where **σ** are the spin operators that make up the internal basis.
      The input data object is modified and will contain the transformed dataset.

      :param data: The dataset expressed in the variables **s** that will be transformed.
      :type data: Data

   .. py:method:: print_details

      Prints out the current basis.

      Each line corresponds to an operator in the basis.
      Operators are displayed as a string of :math:`n` characters with values between 0 and :math:`q-1` which indicate the exponent of each variable in the operator.
      On each line, after the operator string, the indices of the variables that are included in the spin operator are also printed.
   
   .. rubric:: Attributes

   .. py:attribute:: n
      :type: int
      
      The number of variables in the system (read-only).

   .. py:attribute:: q
      :type: int
      
      The number of states each variable can take (read-only).

   .. py:attribute:: matrix

      The matrix representation of basis, with shape (:math:`n`, :math:`n`). The value at (:math:`i`, :math:`j`) indicates the exponent of variable :math:`i` in operator :math:`j`.
   
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
      