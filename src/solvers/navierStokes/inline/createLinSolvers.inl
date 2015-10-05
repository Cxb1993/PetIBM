/**
 * @file createLinSolvers.inl
 * @brief A top level function to invoke the process of creating solvers
 * @author Pi-Yueh Chuang (pychuang@gwu.edu)
 * @version Alpha
 * @date 2015-10-04
 */



/**
 * @brief A top level function to invoke the process of creating solvers
 *
 * @tparam dim dimension of simulation
 *
 * @return PetscErrorCode
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::createLinSolvers()
{
    PetscErrorCode      ierr;

    ierr = (this->*createLinSolver1)(); CHKERRQ(ierr);
    ierr = (this->*createLinSolver2)(); CHKERRQ(ierr);

    return 0;
}
