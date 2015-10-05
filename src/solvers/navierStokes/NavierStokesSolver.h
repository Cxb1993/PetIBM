/***************************************************************************//**
 * \file NavierStokesSolver.h
 * \author Anush Krishnan (anush@bu.edu)
 * \author Pi-Yueh Chuang (pychuang@gwu.edu)
 * \brief Definition of the class `NavierStokesSolver`.
 */


#if !defined(NAVIER_STOKES_SOLVER_H)
#define NAVIER_STOKES_SOLVER_H

#include "CartesianMesh.h"
#include "FlowDescription.h"
#include "SimulationParameters.h"
#include "AmgXSolver.hpp"

#include <fstream>

#include <petscdmda.h>
#include <petscksp.h>


/**
 * \class NavierStokesSolver
 * \brief Solve the incompressible Navier-Stokes equations in a rectangular or
 *        cuboidal domain.
 */
template <PetscInt dim>
class NavierStokesSolver
{
protected:

    PetscMPIInt     rank,
                    size;

public:
  DM qPack,
     lambdaPack;
  DM pda,
     uda,
     vda,
     wda;

  Vec q, qStar, lambda;

  Vec qxLocal, qyLocal, qzLocal;
  Vec pMapping, uMapping, vMapping, wMapping;

  KSP ksp1, ksp2;
  AmgXSolver amgx1, amgx2;

  Mat A,
      QT,
      BNQ,
      QTBNQ;

  Vec bc1,
      rhs1,
      H,
      rn,
      r2,
      rhs2,
      temp;

  Vec BN,
      RInv,
      MHat;

  CartesianMesh *mesh;
  FlowDescription<dim> *flow;
  SimulationParameters *parameters;
  
  PetscInt timeStep;

  std::ofstream iterationCountsFile;

  PetscLogStage stageInitialize,
                stageRHSVelocitySystem,
                stageSolveVelocitySystem,
                stageRHSPoissonSystem,
                stageSolvePoissonSystem,
                stageProjectionStep;

  // initialize data common to NavierStokesSolver and derived classes
  PetscErrorCode initializeCommon();
  // create DMDA structures for flow variables
  virtual PetscErrorCode createDMs();
  // create vectors used to store flow variables
  virtual PetscErrorCode createVecs();
  // create mapping from local flux vectors to global flux vectors
  PetscErrorCode createLocalToGlobalMappingsFluxes();
  // create mapping from local pressure variable to global lambda vector
  PetscErrorCode createLocalToGlobalMappingsLambda();
  // populate flux vectors with initial conditions
  virtual PetscErrorCode initializeFluxes();
  // add initial perturbation to velocity field
  PetscErrorCode addInitialPerturbation();
  // initialize lambda vector with previously saved data
  PetscErrorCode initializeLambda();
  
  // generate diagonal matrices M and Rinv
  PetscErrorCode generateDiagonalMatrices();
  
  // count number of non-zeros in the diagonal and off-diagonal portions of the parallel matrices
  void countNumNonZeros(PetscInt *cols, size_t numCols, PetscInt rowStart, PetscInt rowEnd, 
                        PetscInt &d_nnz, PetscInt &o_nnz);
  
  // generate the matrix A
  PetscErrorCode generateA();
  
  // compute matrix \f$ B^N Q \f$
  virtual PetscErrorCode generateBNQ();
  // compute matrix \f$ Q^T B^N Q \f$
  PetscErrorCode generateQTBNQ();
  // calculate and specify to the Krylov solver the null-space of the LHS matrix
  // in the pressure-force system
  virtual PetscErrorCode setNullSpace();

  // assemble RHS of velocity system
  PetscErrorCode assembleRHSVelocity();
  // calculate explicit convective and diffusive terms
  PetscErrorCode calculateExplicitTerms();
  // update values in ghost nodes at the domain boundaries
  PetscErrorCode updateBoundaryGhosts();
  // assemble velocity boundary conditions vector
  PetscErrorCode generateBC1();

  // aasemble RHS of Poisson system
  PetscErrorCode assembleRHSPoisson();
  // assemble boundary conditions vector for the pressure-force system
  virtual PetscErrorCode generateR2();

  // advance in time
  PetscErrorCode stepTime();
  // project velocity onto divergence-free field with satisfaction of the no-splip condition
  PetscErrorCode projectionStep();

  // print info about simulation
  PetscErrorCode printInfo();
  // read fluxes from files
  PetscErrorCode readFluxes();
  // read pressure field from file
  virtual PetscErrorCode readLambda();
  // write grid coordinates into file
  PetscErrorCode writeGrid();
  // write fluxes into files
  PetscErrorCode writeFluxes();
  // write pressure field into file
  virtual PetscErrorCode writeLambda();
  // write KSP iteration counts into file
  PetscErrorCode writeIterationCounts();
  
  // code-development helpers
  PetscErrorCode helpers();
  // code-development helper: output vectors to files
  PetscErrorCode helperOutputVectors();
  // code-development helper: output matrices to files
  PetscErrorCode helperOutputMatrices();
  

  // typedef of member function pointer that points to funcs creating solvers
  typedef PetscErrorCode (NavierStokesSolver<dim>::*FP_solver)();

  FP_solver  createLinSolver1, ///< mem-func pointer of creating solver1
             createLinSolver2; ///< mem-func pointer of creating solver2

  // a top level function to invoke creating and initializing of solvers
  PetscErrorCode createLinSolvers();
  // set up Krylov solvers used to solve intermediate velocity fluxes
  PetscErrorCode createKSP1();
  // set up Krylov solvers used to solve pressure
  PetscErrorCode createKSP2();
  // Set up AmgX solvers used to solve intermediate velocity fluxes
  PetscErrorCode createAmgX1();
  // Set up AmgX solvers used to solve intermediate velocity fluxes
  PetscErrorCode createAmgX2();
  

  // typedef of member function pointer that points to funcs getting iterations
  typedef PetscErrorCode (NavierStokesSolver<dim>::*FP_getIter)(int &);

  FP_getIter    getSolverIterationsFP1, ///< mem-func pointer of getting iters1
                getSolverIterationsFP2; ///< mem-func pointer of getting iters2

  // top level function that gets iterations of last solve stage
  PetscErrorCode getSolverIterations(int &iterSolver1, int &iterSolver2);
  // function that gets iterations of last solve stage using KSP solver 1
  PetscErrorCode getSolverIterationsKSP1(int &iterSolver);
  // function that gets iterations of last solve stage using KSP solver 2
  PetscErrorCode getSolverIterationsKSP2(int &iterSolver);
  // function that gets iterations of last solve stage using AmgX solver 1
  PetscErrorCode getSolverIterationsAmgX1(int &iterSolver);
  // function that gets iterations of last solve stage using AmgX solver 2
  PetscErrorCode getSolverIterationsAmgX2(int &iterSolver);


  // typedef of member function pointer that points to solving stages
  typedef PetscErrorCode (NavierStokesSolver<dim>::*FP_solving)();

  FP_solving    solveV, ///< mem-func pointer of solving stage of velocity
                solveP; ///< mem-func pointer of solving stage of pressure

  // solving intermediate velocity using a KSP solver
  PetscErrorCode solveV_KSP();
  // solving pressure using a KSP solver
  PetscErrorCode solveP_KSP();
  // solving intermediate velocity using a AmgX solver
  PetscErrorCode solveV_AmgX();
  // solving pressure using a AmgX solver
  PetscErrorCode solveP_AmgX();

  // top level function solving system for intermediate velocity fluxes \f$ q^* \f$
  PetscErrorCode solveIntermediateVelocity();
  // top level function solveing Poisson system for pressure and body forces
  PetscErrorCode solvePoissonSystem();


public:
  // constructors
  NavierStokesSolver(){ };
  NavierStokesSolver(CartesianMesh *cartesianMesh, 
                     FlowDescription<dim> *flowDescription, 
                     SimulationParameters *simulationParameters);
  // destructor
  ~NavierStokesSolver(){ };

  // initialize systems
  virtual PetscErrorCode initialize();
  // clean up at end of simulation
  virtual PetscErrorCode finalize();

  // evaluate if the simulation is completed
  PetscBool finished();

  // write numerical solution into respective files
  virtual PetscErrorCode writeData();

}; // NavierStokesSolver

#endif
