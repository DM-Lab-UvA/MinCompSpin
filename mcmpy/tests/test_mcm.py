import pytest
import numpy as np
from mcmpy import MCM, Data

@pytest.fixture
def model():
    return MCM(8, [0,1,2,0,1,-1,5])

# Test input with specific partition
def test_mcm_array(model):
    assert np.all(model.array == [[1,0,0,1,0,0,0,0],
                                  [0,1,0,0,1,0,0,0],
                                  [0,0,1,0,0,0,0,0],
                                  [0,0,0,0,0,0,1,0]])

def test_mcm_gray_code(model):
    assert np.all(model.array_gray_code == [0,1,2,0,1,-1,3,-1])

def test_read_only_properties(model):
    assert(model.n == 8)
    assert(model.n_icc == 4)
    assert(model.rank == 6)
    assert(model.is_optimized is False)

def test_move_in_invalid(model):
    with pytest.raises(ValueError, match="The variable index should be between 0 and n-1."):
        model.move_variable_in(8,5)

    with pytest.raises(ValueError, match="This variable is already present in the partition."):
        model.move_variable_in(1,6)

def test_move_in(model):
    model.move_variable_in(5,2)
    model.move_variable_in(7,7)

    # Check gray code
    assert np.all(model.array_gray_code == [0,1,2,0,1,2,3,4])

    # Check binary array
    assert np.all(model.array == [[1,0,0,1,0,0,0,0],
                                  [0,1,0,0,1,0,0,0],
                                  [0,0,1,0,0,1,0,0],
                                  [0,0,0,0,0,0,1,0],
                                  [0,0,0,0,0,0,0,1]])

    # Check properties
    assert(model.n == 8)
    assert(model.n_icc == 5)
    assert(model.rank == 8)
    assert(model.is_optimized is False)

def test_move_out_invalid(model):
    with pytest.raises(ValueError, match="The variable index should be between 0 and n-1."):
        model.move_variable_out(8)

    with pytest.raises(ValueError, match="This variable is not present in the partition."):
        model.move_variable_out(7)

def test_move_out(model):
    model.move_variable_out(2)

    # Check gray code
    assert np.all(model.array_gray_code == [0,1,-1,0,1,-1,2,-1])

    # Check binary array
    assert np.all(model.array == [[1,0,0,1,0,0,0,0],
                                  [0,1,0,0,1,0,0,0],
                                  [0,0,0,0,0,0,1,0]])

    # Check properties
    assert(model.n == 8)
    assert(model.n_icc == 3)
    assert(model.rank == 5)
    assert(model.is_optimized is False)    

def test_move(model):
    model.move_variable(6,0)

    # Check gray code
    assert np.all(model.array_gray_code == [0,1,2,0,1,-1,0,-1])

    # Check binary array
    assert np.all(model.array == [[1,0,0,1,0,0,1,0],
                                  [0,1,0,0,1,0,0,0],
                                  [0,0,1,0,0,0,0,0]])

    # Check properties
    assert(model.n == 8)
    assert(model.n_icc == 3)
    assert(model.rank == 6)
    assert(model.is_optimized is False)

def test_properties():
    # Independent model
    model = MCM(12, "independent")
    assert(model.n == 12)
    assert(model.n_icc == 12)
    assert(model.rank == 12)
    assert(model.is_optimized is False) 

    # Complete model
    model = MCM(12, "complete")
    assert(model.n == 12)
    assert(model.n_icc == 1)
    assert(model.rank == 12)
    assert(model.is_optimized is False)

def test_data_object(scotus_data_q2, opt_mcm_scotus_q2):
    data = opt_mcm_scotus_q2.generate_data_object(10_000, scotus_data_q2)

    assert data.N == 10_000
    assert data.N_synthetic == 10_000
    assert data.q == scotus_data_q2.q
    assert data.n == scotus_data_q2.n

def test_data_file(scotus_data_q2, opt_mcm_scotus_q2, tmp_path):
    filepath = str(tmp_path / "test.dat")
    print(type(filepath))
    opt_mcm_scotus_q2.generate_data_file(10_000, scotus_data_q2, filepath)
    # Create data object from file
    data = Data(filepath, scotus_data_q2.n, scotus_data_q2.q)

    assert data.N == 10_000
    assert data.N_synthetic == 10_000
    assert data.q == scotus_data_q2.q
    assert data.n == scotus_data_q2.n

