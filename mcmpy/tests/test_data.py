import pytest
import numpy as np
from mcmpy import Data

# Test information theoretic criteria against previous code
# (https://github.com/clelidm/MinCompSpin_ExhaustiveSearch) by C. de Mulatier;
# Results of the optimal MCM for the binary SCOTUS data in the original basis 

def test_log_evidence(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.isclose(scotus_data_q2.log_evidence(opt_mcm_scotus_q2), -3300.4)

def test_log_likelihood(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.isclose(scotus_data_q2.log_likelihood(opt_mcm_scotus_q2), -3194.36)

def test_parametric_complexity(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.isclose(scotus_data_q2.complexity_parametric(opt_mcm_scotus_q2), 114.056)

def test_geometric_complexity(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.isclose(scotus_data_q2.complexity_geometric(opt_mcm_scotus_q2), -8.95092)

def test_total_complexity(scotus_data_q2, opt_mcm_scotus_q2):
    parametric_complexity = scotus_data_q2.complexity_parametric(opt_mcm_scotus_q2)
    geometric_complexity = scotus_data_q2.complexity_geometric(opt_mcm_scotus_q2)
    assert np.isclose(parametric_complexity + geometric_complexity , 105.105)

def test_mdl(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.isclose(scotus_data_q2.minimum_description_length(opt_mcm_scotus_q2), -3299.46)

def test_log_evidence_icc(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.all(np.isclose(scotus_data_q2.log_evidence_icc(opt_mcm_scotus_q2), [-1754.41, -1545.98]))

def test_log_likelihood_icc(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.all(np.isclose(scotus_data_q2.log_likelihood_icc(opt_mcm_scotus_q2), [-1686.28, -1508.08]))

def test_parametric_complexity_icc(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.all(np.isclose(scotus_data_q2.complexity_parametric_icc(opt_mcm_scotus_q2), [76.8637, 37.1921]))

def test_geometric_complexity_icc(scotus_data_q2, opt_mcm_scotus_q2):
    assert np.all(np.isclose(scotus_data_q2.complexity_geometric_icc(opt_mcm_scotus_q2), [-9.58359, 0.632678]))


# Input array test
def test_partition_input(scotus_data_q2):
    with pytest.raises(ValueError, match="The partition should be a 1D or 2D array."):
        scotus_data_q2.log_evidence(np.ones((2,2,2)))
    
    with pytest.raises(ValueError, match="Entries of the 2D array should be either 0 or 1."):
        scotus_data_q2.log_evidence([[1,2,0], [0,0,1]])

def test_entropy_of_op_input(scotus_data_q2):
    with pytest.raises(ValueError, match="The spin operator should be given as a 1D numpy array."):
        scotus_data_q2.entropy_of_spin_operator(np.ones((2,2)))

    with pytest.raises(ValueError, match="The vector should only contain values between 0 and q-1."):
        scotus_data_q2.entropy_of_spin_operator([1,2,0,1,0,1,0,1,1])

    with pytest.raises(ValueError, match="The given spin operator doesn't contain n elements."):
        scotus_data_q2.entropy_of_spin_operator([1,0,1,0,1,0,1])
        
