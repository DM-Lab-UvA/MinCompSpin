import pytest
import numpy as np
from mcmpy import Data, MCM, MCMSearch

@pytest.fixture
def mcm_searcher():
    return MCMSearch()

## Exhaustive search

# Test against previous code
# (https://github.com/clelidm/MinCompSpin_ExhaustiveSearch) by C. de Mulatier;
# (https://github.com/AaronDC60/MinCompSpin_discrete) by Aaron De Clercq.

class TestMCMSearch:

    def setup_class(self):
        scotus_data = Data("../../input/US_SupremeCourt_n9_N895.dat", 9, 2)
        self.searcher = MCMSearch()

        mcm_in_merging = MCM(9, [0,1,0,3,4,5,6,7,8])

        self.opt_mcms = {}
        self.mcms_out = {}
        self.mcms_in = {}

        self.opt_mcms["simulated_annealing"] = self.searcher.simulated_annealing(scotus_data)
        self.mcms_out["simulated_annealing"] = self.searcher.get_mcm_out()
        self.mcms_in["simulated_annealing"] = self.searcher.get_mcm_in()

        self.opt_mcms["hierarchical_greedy_merging"] = self.searcher.hierarchical_greedy_merging(scotus_data, mcm_in_merging)
        self.mcms_out["hierarchical_greedy_merging"] = self.searcher.get_mcm_out()
        self.mcms_in["hierarchical_greedy_merging"] = self.searcher.get_mcm_in()

        self.opt_mcms["hierarchical_greedy_divisive"] = self.searcher.hierarchical_greedy_divisive(scotus_data)
        self.mcms_out["hierarchical_greedy_divisive"] = self.searcher.get_mcm_out()
        self.mcms_in["hierarchical_greedy_divisive"] = self.searcher.get_mcm_in()

        self.opt_mcms["exhaustive"] = self.searcher.exhaustive(scotus_data)
        self.mcms_out["exhaustive"] = self.searcher.get_mcm_out()

    def test_log_ev_trajectory_exhaustive(self):
        # Exhaustive search stores all parititons
        # Number of partitions for n variables is given by Bell number of n
        assert len(self.searcher.log_evidence_trajectory) == 21_147

    @pytest.mark.parametrize("method", ["exhaustive", "simulated_annealing",  "hierarchical_greedy_merging", "hierarchical_greedy_divisive"])
    def test_opt_mcms_structure(self, method):
        assert self.opt_mcms[method].n == 9
        assert self.opt_mcms[method].n_icc == 2
        assert self.opt_mcms[method].rank == 9
        assert self.opt_mcms[method].is_optimized is True

        if method != "simulated_annealing":
            # Simulated annealing inverts the order sometimes
            assert np.all(self.opt_mcms[method].array == [[1,0,1,1,1,0,1,0,0], [0,1,0,0,0,1,0,1,1]])
            assert np.all(self.opt_mcms[method].array_gray_code == [0,1,0,0,0,1,0,1,1])
        else:
            assert [1,0,1,1,1,0,1,0,0] in self.opt_mcms[method].array
            assert [0,1,0,0,0,1,0,1,1] in self.opt_mcms[method].array

    @pytest.mark.parametrize("method", ["exhaustive", "simulated_annealing",  "hierarchical_greedy_merging", "hierarchical_greedy_divisive"])
    def test_mcms_out_structure(self, method):
        assert self.mcms_out[method].n == 9
        assert self.mcms_out[method].n_icc == 2
        assert self.mcms_out[method].rank == 9
        assert self.mcms_out[method].is_optimized is True

        if method != "simulated_annealing":
            # Simulated annealing inverts the order sometimes
            assert np.all(self.mcms_out[method].array == [[1,0,1,1,1,0,1,0,0], [0,1,0,0,0,1,0,1,1]])
            assert np.all(self.mcms_out[method].array_gray_code == [0,1,0,0,0,1,0,1,1])
        else:
            assert [1,0,1,1,1,0,1,0,0] in self.mcms_out[method].array
            assert [0,1,0,0,0,1,0,1,1] in self.mcms_out[method].array

    @pytest.mark.parametrize("method", ["exhaustive", "simulated_annealing",  "hierarchical_greedy_merging", "hierarchical_greedy_divisive"])
    def test_opt_mcm_log_ev(self, method):
        log_ev_per_icc = self.opt_mcms[method].get_best_log_evidence_icc()
        log_ev = self.opt_mcms[method].get_best_log_evidence()

        assert np.isclose(log_ev, -3300.4)
        assert np.allclose(log_ev_per_icc, [-1754.41, -1545.98]) or np.allclose(log_ev_per_icc, [-1545.98, -1754.41])
       
    @pytest.mark.parametrize("method", ["exhaustive", "simulated_annealing",  "hierarchical_greedy_merging", "hierarchical_greedy_divisive"])
    def test_opt_mcm_log_ev(self, method):
        log_ev_per_icc = self.opt_mcms[method].get_best_log_evidence_icc()
        log_ev = self.opt_mcms[method].get_best_log_evidence()

        assert np.isclose(log_ev, -3300.4)
        assert np.allclose(log_ev_per_icc, [-1754.41, -1545.98]) or np.allclose(log_ev_per_icc, [-1545.98, -1754.41])

    def test_mcm_in_invalid(self):
        # Try before any search has been run
        search = MCMSearch()
        with pytest.raises(RuntimeError, match="No search has been ran yet."):
            search.get_mcm_in()

    def test_mcm_in_exhaustive(self):
        with pytest.raises(RuntimeError, match="Exhaustive search does not have an initial MCM."):
            self.searcher.get_mcm_in()

    def test_mcm_in_greedy_divisive(self):
        # Starts by default from the complete model
        assert self.mcms_in["hierarchical_greedy_divisive"].n == 9
        assert self.mcms_in["hierarchical_greedy_divisive"].n_icc == 1
        assert self.mcms_in["hierarchical_greedy_divisive"].rank == 9
        assert self.mcms_in["hierarchical_greedy_divisive"].is_optimized is False

    def test_mcm_in_greedy_merging(self):
        # mcm_in used is defined in the setup
        assert self.mcms_in["hierarchical_greedy_merging"].n == 9
        assert self.mcms_in["hierarchical_greedy_merging"].n_icc == 8
        assert self.mcms_in["hierarchical_greedy_merging"].rank == 9
        assert self.mcms_in["hierarchical_greedy_merging"].is_optimized is False

        assert np.all(self.mcms_in["hierarchical_greedy_merging"].array_gray_code == [0,1,0,2,3,4,5,6,7])

    def test_mcm_in_sim_annealing(self):
        # Starts by default from the independent model
        assert self.mcms_in["simulated_annealing"].n == 9
        assert self.mcms_in["simulated_annealing"].n_icc == 9
        assert self.mcms_in["simulated_annealing"].rank == 9
        assert self.mcms_in["simulated_annealing"].is_optimized is False

# Check the optimal structure and log-evidence for the SCOTUS dataset with different values of q

def test_scotus_q_3(mcm_searcher, scotus_data_q3):
    opt_mcm = mcm_searcher.exhaustive(scotus_data_q3)
    log_ev = opt_mcm.get_best_log_evidence()

    # MCM properties
    assert opt_mcm.n == 9
    assert opt_mcm.n_icc == 2
    assert opt_mcm.rank == 9
    assert opt_mcm.is_optimized is True

    # MCM structure
    assert np.all(opt_mcm.array == [[1,0,1,1,1,0,1,0,0], [0,1,0,0,0,1,0,1,1]])
    assert np.all(opt_mcm.array_gray_code == [0,1,0,0,0,1,0,1,1])

    # MCM log-evidence
    assert np.allclose(log_ev, -3714.65)

def test_scotus_q_4(mcm_searcher, scotus_data_q4):
    opt_mcm = mcm_searcher.exhaustive(scotus_data_q4)
    log_ev = opt_mcm.get_best_log_evidence()

    # MCM properties
    assert opt_mcm.n == 9
    assert opt_mcm.n_icc == 3
    assert opt_mcm.rank == 9
    assert opt_mcm.is_optimized is True

    # MCM structure
    assert np.all(opt_mcm.array == [[1,0,1,0,1,0,0,0,0], 
                                  [0,1,0,0,0,1,0,1,1], 
                                  [0,0,0,1,0,0,1,0,0]])
    assert np.all(opt_mcm.array_gray_code == [0,1,0,2,0,1,2,1,1])

    # MCM log-evidence
    assert np.allclose(log_ev, -4033.14)

def test_scotus_q_5(mcm_searcher, scotus_data_q5):
    opt_mcm = mcm_searcher.exhaustive(scotus_data_q5)
    log_ev = opt_mcm.get_best_log_evidence()

    # MCM properties
    assert opt_mcm.n == 9
    assert opt_mcm.n_icc == 4
    assert opt_mcm.rank == 9
    assert opt_mcm.is_optimized is True

    # MCM structure
    assert np.all(opt_mcm.array == [[1,0,1,0,1,0,0,0,0], 
                                  [0,1,0,0,0,0,0,0,0],
                                  [0,0,0,1,0,0,1,0,0],
                                  [0,0,0,0,0,1,0,1,1]])
    assert np.all(opt_mcm.array_gray_code == [0,1,0,2,0,3,2,3,3])

    # MCM log-evidence
    assert np.allclose(log_ev, -4261.89)



