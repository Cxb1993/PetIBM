/**
 * @file createAmgXs.inl
 * @brief Containging routines that create and initialize AmgX solvers
 * @author Pi-Yueh Chuang (pychuang@gwu.edu)
 * @version alpha
 * @date 2015-10-04
 */


/**
 * \brief Instantiates the AmgX solver for the intermediate velocity fluxes.
 *
 * `amgx1` is the solver created here. Settings should be included in the file
 * `solversAmgXOptions_v.info`.
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::createAmgX1()
{

  // file with KSP options
  std::string       AmgXOptionsFile = 
      parameters->directory + "/solversAmgXOptions_v.info";

  // create KSP for intermaediate fluxes system
  amgx1.initialize(PETSC_COMM_WORLD, size, rank, "dDDI", AmgXOptionsFile);

  amgx1.setA(A);

  return 0;
} // createAmgX1


/**
 * \brief Instantiates the AmgX solver for the pressure.
 *
 * `amgx2` is the solver created here. Settings should be included in the file
 * `solversAmgXOptions_p.info`.
 */
template <PetscInt dim>
PetscErrorCode NavierStokesSolver<dim>::createAmgX2()
{

  // file with KSP options
  std::string       AmgXOptionsFile = 
      parameters->directory + "/solversAmgXOptions_p.info";

  // create KSP for intermaediate fluxes system
  amgx2.initialize(PETSC_COMM_WORLD, size, rank, "dDDI", AmgXOptionsFile);

  amgx2.setA(QTBNQ);

  return 0;
} // createAmgX2
