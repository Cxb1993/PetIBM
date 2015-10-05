/**
 * @file getIterations.inl
 * @brief a top-level routine that get iterations from different solvers
 * @author Pi-Yueh Chuang (pychuang@gwu.edu)
 * @version alpha
 * @date 2015-10-04
 */


template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::getSolverIterations(
        int & iterSolver1, int & iterSolver2)
{
    PetscErrorCode  ierr;

    ierr = (this->*getSolverIterationsFP1)(iterSolver1); CHKERRQ(ierr);
    ierr = (this->*getSolverIterationsFP2)(iterSolver2); CHKERRQ(ierr);

    return 0;
}


template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::getSolverIterationsKSP1(int & iterSolver)
{
    PetscErrorCode  ierr;

    ierr = KSPGetIterationNumber(ksp1, &iterSolver); CHKERRQ(ierr);
    return 0;
}


template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::getSolverIterationsKSP2(int & iterSolver)
{
    PetscErrorCode  ierr;

    ierr = KSPGetIterationNumber(ksp2, &iterSolver); CHKERRQ(ierr);
    return 0;
}


template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::getSolverIterationsAmgX1(int & iterSolver)
{
    iterSolver = amgx1.getIters();
    return 0;
}


template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::getSolverIterationsAmgX2(int & iterSolver)
{
    iterSolver = amgx2.getIters();
    return 0;
}
