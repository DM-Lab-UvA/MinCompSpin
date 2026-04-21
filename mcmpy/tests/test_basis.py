import pytest
import numpy as np
from mcmpy import Basis


def test_init_default():
    n = 3
    q = 5
    basis = Basis(n,q)

    # Default basis should be the n one-body spin operators
    assert np.all(basis.matrix == np.eye(n))
    assert(basis.n == n)
    assert(basis.q == q)

def test_init_array():
    n = 4
    q = 3
    array = np.array([[1,2,0,0],[2,2,0,0],[0,0,1,0],[0,0,2,1]])
    basis = Basis(n,q, array)

    # Check the matrix representation
    # Should be equal to the transpose of the input given that columns represent operators
    assert np.all(basis.matrix == array.T)
    assert(basis.n == n)
    assert(basis.q == q)

    # Check reset to default
    basis.set_default()
    assert np.all(basis.matrix == np.eye(n))

def test_init_file():
    n = 4
    q = 3
    basis = Basis(n,q, "../../tests/basis2.dat")

    # Check the matrix representation
    assert np.all(basis.matrix == [[1,2,0,0],[2,2,0,0],[0,0,1,2],[0,0,0,1]])
    assert(basis.n == n)
    assert(basis.q == q)

def test_set_from_file():
    n = 4
    q = 3

    basis = Basis(n,q)
    basis.set_from_file("../../tests/basis2.dat")

    # Check the matrix representation
    assert np.all(basis.matrix == [[1,2,0,0],[2,2,0,0],[0,0,1,2],[0,0,0,1]])
    assert(basis.n == n)
    assert(basis.q == q)
