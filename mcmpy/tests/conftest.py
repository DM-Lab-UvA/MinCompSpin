import pytest
from mcmpy import Data, MCM, MCMSearch

# Data

@pytest.fixture(scope="module")
def scotus_data_q2():
    return Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 2)

@pytest.fixture(scope="module")
def scotus_data_q3():
    return Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 3)

@pytest.fixture(scope="module")
def scotus_data_q4():
    return Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 4)

@pytest.fixture(scope="module")
def scotus_data_q5():
    return Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 5)


# MCM

@pytest.fixture(scope="module")
def opt_mcm_scotus_q2():
    return MCM(9, [[1,0,1,1,1,0,1,0,0], [0,1,0,0,0,1,0,1,1]])

@pytest.fixture(scope="module")
def random_mcm():
    return MCM(9, "random")
