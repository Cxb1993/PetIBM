/**
 * @file solveVP.inl
 * @brief Routines that solve linear systems using different executing space
 * @author Anush Krishnan (anush@bu.edu)
 * @author Pi-Yueh Chuang (pychuang@gwu.edu)
 * @version alpha
 * @date 2015-10-04
 */


/**
 * @brief solving intermediate velocity using a KSP solver
 *
 * @tparam dim dimension of the simulation
 *
 * @return PetscErrorCode
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::solveV_KSP()
{ 
    PetscErrorCode ierr;

    ierr = PetscLogStagePush(stageSolveVelocitySystem); CHKERRQ(ierr);

    ierr = KSPSolve(ksp1, rhs1, qStar); CHKERRQ(ierr);
  
    ierr = PetscLogStagePop(); CHKERRQ(ierr);

    KSPConvergedReason reason;
    ierr = KSPGetConvergedReason(ksp1, &reason); CHKERRQ(ierr);
    if (reason < 0)
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD, 
                "\n[time-step %d]", timeStep); CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD, 
                "\nERROR: velocity solver diverged due to reason: %d\n", 
                reason); CHKERRQ(ierr);

        exit(0);
    }

    return 0;
}


/**
 * @brief solving pressure using a KSP solver
 *
 * @tparam dim dimension of the simulation
 *
 * @return PetscErrorCode
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::solveP_KSP()
{ 
    PetscErrorCode ierr;

    ierr = PetscLogStagePush(stageSolvePoissonSystem); CHKERRQ(ierr);

    ierr = KSPSolve(ksp2, rhs2, lambda); CHKERRQ(ierr);

    ierr = PetscLogStagePop(); CHKERRQ(ierr);

    KSPConvergedReason reason;
    ierr = KSPGetConvergedReason(ksp2, &reason); CHKERRQ(ierr);
    if (reason < 0)
    {
        ierr = PetscPrintf(PETSC_COMM_WORLD, 
                "\n[time-step %d]", timeStep); CHKERRQ(ierr);
        ierr = PetscPrintf(PETSC_COMM_WORLD, 
                "\nERROR: Poisson solver diverged due to reason: %d\n", 
                reason); CHKERRQ(ierr);

        exit(0);
    }

    return 0;
}


/**
 * @brief solving intermediate velocity using a AmgX solver
 *
 * @tparam dim dimension of the simulation
 *
 * @return PetscErrorCode
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::solveV_AmgX()
{ 
    PetscErrorCode ierr;

    ierr = PetscLogStagePush(stageSolveVelocitySystem); CHKERRQ(ierr);

    amgx1.solve(qStar, rhs1);
  
    ierr = PetscLogStagePop(); CHKERRQ(ierr);

    return 0;
}


/**
 * @brief solving pressure using a AmgX solver
 *
 * @tparam dim dimension of the simulation
 *
 * @return PetscErrorCode
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::solveP_AmgX()
{ 
    PetscErrorCode ierr;
  
    ierr = PetscLogStagePush(stageSolvePoissonSystem); CHKERRQ(ierr);

    amgx2.solve(lambda, rhs2);

    ierr = PetscLogStagePop(); CHKERRQ(ierr);

    return 0;
}
