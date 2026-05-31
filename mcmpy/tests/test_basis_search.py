import pytest
import numpy as np
from mcmpy import Data, MCM, MCMSearch, Basis, BasisSearch

# Best basis search of supreme court

# Test against previous code
# (https://github.com/clelidm/MinCompSpin_ExhaustiveSearch) by C. de Mulatier;
# (https://github.com/AaronDC60/MinCompSpin_discrete) by Aaron De Clercq.

class TestBasisSearch:

    def setup_class(self):
        self.searcher = BasisSearch()
        self.mcmsearcher = MCMSearch()

    def test_exhaustive_search(self):
        dataset = Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 2)
        opt_basis_1 = self.searcher.exhaustive(dataset)
        opt_basis_2 = self.searcher.get_basis()
        assert np.all(opt_basis_1.matrix == [[0,0,0,1,1,1,0,0,0],
                                           [0,0,0,0,0,0,1,0,0],
                                           [0,0,0,0,0,1,0,1,0],
                                           [1,0,0,0,0,0,0,0,0],
                                           [0,0,0,1,0,0,0,0,0],
                                           [0,1,0,0,0,0,0,0,0],
                                           [1,0,0,0,1,0,0,0,1],
                                           [0,1,1,0,0,0,1,0,0],
                                           [0,0,1,0,0,0,0,1,0]])

        assert np.all(opt_basis_1.matrix == opt_basis_2.matrix)
        assert opt_basis_1.n == 9
        assert opt_basis_2.n == 9

        assert opt_basis_1.q == 2
        assert opt_basis_2.q == 2

    def get_best_mcm(self, q):
        dataset = Data("../../input/US_SupremeCourt_n9_N895.dat", 9, q)
        # Calculate the best basis
        opt_basis = self.searcher.exhaustive(dataset)
        # Transform data
        transformed_data = opt_basis.gauge_transform_data(dataset)
        # Get best mcm in the new basis
        best_mcm = self.mcmsearcher.exhaustive(transformed_data)

        return best_mcm
    
    def test_mcm_opt_basis_q2(self):
        best_mcm = self.get_best_mcm(2)

        # Check the mcm and log-evidence
        assert np.all(best_mcm.array == [[1,0,0,0,0,0,0,0,0],
                                         [0,1,1,0,0,0,1,0,0],
                                         [0,0,0,1,1,1,0,1,1]])

        assert np.isclose(best_mcm.get_best_log_evidence(), -3154.42)

    def test_mcm_opt_basis_q3(self):
        best_mcm = self.get_best_mcm(3)

        # Check the mcm and log-evidence
        assert np.all(best_mcm.array == [[1,0,0,0,1,0,0,1,0],
                                         [0,1,0,1,0,0,1,0,0],
                                         [0,0,1,0,0,1,0,0,1]])

        assert np.isclose(best_mcm.get_best_log_evidence(), -3587.68)

    def test_mcm_opt_basis_q4(self):
        best_mcm = self.get_best_mcm(4)

        # Check the mcm and log-evidence
        assert np.all(best_mcm.array == [[1,0,0,0,0,0,0,0,0],
                                         [0,1,0,0,0,0,1,0,0],
                                         [0,0,1,0,0,1,0,0,0],
                                         [0,0,0,1,0,0,0,0,1],
                                         [0,0,0,0,1,0,0,1,0]])

        assert np.isclose(best_mcm.get_best_log_evidence(), -3763.43)

    def test_mcm_opt_basis_q5(self):
        best_mcm = self.get_best_mcm(5)

        # Check the mcm and log-evidence
        assert np.all(best_mcm.array == [[1,0,0,0,0,0,0,0,0],
                                         [0,1,0,0,0,0,0,0,0],
                                         [0,0,1,0,0,1,0,0,0],
                                         [0,0,0,1,0,0,1,0,0],
                                         [0,0,0,0,1,0,0,1,0],
                                         [0,0,0,0,0,0,0,0,1]])

        assert np.isclose(best_mcm.get_best_log_evidence(), -3848.06)
