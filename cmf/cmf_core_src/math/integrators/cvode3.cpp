#include "cvode3.h"
#include "../num_array.h"

#include <sstream>

#include <nvector/nvector_serial.h>
#include <cvode/cvode.h>
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype      */
#include <sundials/sundials_version.h>
#include <sundials/sundials_math.h>


#include <cvode/cvode_direct.h>        /* access to CVDls interface            */

// Special includes for dense solver
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver      */

// Special includes for banded solver
#include <sunmatrix/sunmatrix_band.h> /* access to band SUNMatrix            */
#include <sunlinsol/sunlinsol_band.h> /* access to band SUNLinearSolver      */

// Special includes for diagonal solver
#include <cvode/cvode_diag.h>

// Special includes for krylov preconditioned solver with spgmr linear solver
#include <cvode/cvode_spils.h>
#include <sunlinsol/sunlinsol_spgmr.h>
#include <cvode/cvode_bandpre.h>

#include <sunmatrix/sunmatrix_sparse.h>
#include <sunlinsol/sunlinsol_klu.h>

#include <iostream>
#include <iomanip>

using namespace cmf::math;



cmf::math::CVodeOptions::CVodeOptions()
	: max_order(-1), max_non_linear_iterations(-1),
	max_error_test_failures(100000), max_convergence_failures(-1),
	max_num_steps(0), max_hnil_warnings(-1)
{

}


class cmf::math::CVode3::Impl {
public:
	/*******************************
	CVode Implentation Attributes
	********************************/
	// Set to false in CVodeAdams
	bool use_stiff_solver = true;
	// State vector
	N_Vector y = 0;
	// Jacobian matrix
	SUNMatrix J = 0;
	// Linear solver
	SUNLinearSolver LS = 0;
	// System size
	int N = 0;
	long int dxdt_method_calls = 0;

	CVode3 * _integrator;

	// c'tor of CVode3::Impl, does not initialize solver
	Impl(CVode3 * integrator) :
		_integrator(integrator)
	{		}

	static int f(realtype t, N_Vector u, N_Vector udot, void *f_data)
	{
		CVode3::Impl * integ_impl = static_cast<CVode3::Impl*>(f_data);
		CVode3 * integ = integ_impl->_integrator;
		// Get the pointers to the data of the vectors u and udot
		realtype * udata = NV_DATA_S(u);
		realtype * dudata = NV_DATA_S(udot);
		// Get size of the problem
		int nsize = int(integ->size());
		// Update the states from the state vector
		try {
			integ->set_states(udata);
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			integ->set_error_msg(e.what());
			return -8;
		}
		Time T = cmf::math::day * t;
		// Get the derivatives at time T
		try {
			integ->copy_dxdt(T, dudata);
			integ_impl->dxdt_method_calls += integ_impl->N;
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
			integ->set_error_msg(e.what());
			return -8;
		}
		return 0;
	}
	
	void * cvode_mem=0;
	
	void release()
	{
		// If saved vector exists, destroy it
		if (y != 0) N_VDestroy_Serial(y);
		if (cvode_mem != 0) CVodeFree(&cvode_mem);
		if (LS != 0) SUNLinSolFree(LS);
		if (J != 0) SUNMatDestroy(J);
	}
	
	int initialize() {
		release();
		// size of problem
		N = int(_integrator->m_States.size());
		// Allocate vector y
		y = N_VNew_Serial(N);
		// Pointer to data of vector y
		realtype * y_data = NV_DATA_S(y);
		// Copy states to y
		_integrator->copy_states(y_data);
		// Create a implicit (CV_BDF) solver with Newton iteration (CV_NEWTON)
		if (this->use_stiff_solver) {
			cvode_mem = CVodeCreate(CV_BDF);
		}
		else {
			cvode_mem = CVodeCreate(CV_ADAMS);
		}
		
		// Set this wrapper as user data of the solver
		CVodeSetUserData(cvode_mem, (void *)this);
		// Local copy of Epsilon needed for CVodeSVtolerances
		realtype reltol = _integrator->Epsilon;
		N_Vector abstol = N_VNew_Serial(N);
		realtype * abstol_data = NV_DATA_S(abstol);
		for (int i = 0; i < N; ++i)
			abstol_data[i] = _integrator->m_States[i]->get_abs_errtol(reltol*1e-3);
		int flag = 0;
		// Allocate memory for solver and set the right hand side function, start time and error tolerance
		flag = CVodeInit(cvode_mem, f, _integrator->get_t().AsDays(), y);
		flag = CVodeSVtolerances(cvode_mem, reltol, abstol);
		this->set_limits(_integrator->options);
		// Set solver
		this->_integrator->set_solver();

		
		return flag;
	}
	
	int reset() {
		if (cvode_mem) {
			// Copy current states to internal state vector
			_integrator->copy_states(NV_DATA_S(y));
			// Reinitialize solver
			return CVodeReInit(cvode_mem, get_t(), y);
		}
		else {
			return 0;
		}
	}

	void set_limits(const CVodeOptions& options) {

		if (options.max_order >= 0) CVodeSetMaxOrd(cvode_mem, options.max_order);
		// Set stability limit detection, see CVode Guide 2.3. Reasons to apply this algo are true for cmf.
		if (options.max_order >= 3) CVodeSetStabLimDet(cvode_mem, 1);
		if (options.max_non_linear_iterations >= 0) CVodeSetMaxNonlinIters(cvode_mem, options.max_non_linear_iterations);
		if (options.max_error_test_failures >= 0) CVodeSetMaxErrTestFails(cvode_mem, options.max_error_test_failures);
		if (options.max_convergence_failures >= 0) CVodeSetMaxConvFails(cvode_mem, options.max_convergence_failures);
		CVodeSetMaxNumSteps(cvode_mem, options.max_num_steps);
		CVodeSetMaxHnilWarns(cvode_mem, options.max_hnil_warnings);

	}

	realtype get_t() const {
		return _integrator->get_t().AsDays();
	}
	int integrate(cmf::math::Time t_max, cmf::math::Time dt) {
		if (_integrator->m_States.size() == 0)
			throw std::out_of_range("No states to integrate!");

		// If solver or y is not initialized, initialize them
		if (!cvode_mem || !y)
		{
			_integrator->m_dt = t_max - _integrator->get_t();
			this->initialize();
		}
		// Get data of y
		realtype * y_data = NV_DATA_S(y);
		// Time step, needed as return value
		realtype t_ret = get_t(),
				 t_step = 0;
		
		CVodeSetStopTime(cvode_mem, t_max.AsDays());
		_integrator->error_msg = "";
		int res = CVode(cvode_mem, t_max.AsDays(), y, &t_ret, CV_ONE_STEP);
		if (res<0) {
			_integrator->set_states(y_data);
			if (_integrator->error_msg != "")
				throw std::runtime_error(_integrator->error_msg + " - " + (day * t_ret).AsDate().to_string());
			else
				throw std::runtime_error("CVode could not integrate due to failure (see message above) at t=" + (day*t_ret).AsDate().to_string());
		}
		long iterations;
		CVodeGetNumRhsEvals(cvode_mem, &iterations);
		_integrator->m_Iterations = (int)iterations;
		CVodeGetLastStep(cvode_mem, &t_step);
		_integrator->m_dt = day * t_step;
		real sec_step = t_step * 24 * 60 * 60;
		double sec_to_go = (t_max - t_ret * day) / sec;
		if (res == CV_TSTOP_RETURN) {
			if ((day*t_ret - t_max).AsMilliseconds()>5 || (day*t_ret - t_max).AsMilliseconds()<-5) {
				throw std::runtime_error("Time returned by CVODE (" + (day*t_ret).to_string()
					+ ") differs from given end time("
					+ t_max.to_string() + "). Please inform cmf maintainer");
			}
			_integrator->m_t = t_max;
		}
		else {
			_integrator->m_t = t_ret * day;
		}


		// Copy result to state variables
		_integrator->set_states(y_data);
		return res;
	}

	cmf::math::num_array get_error() const {
		sunindextype N = _integrator->size();
		N_Vector ele = N_VNew_Serial(N);
		N_Vector eweight = N_VNew_Serial(N);
		CVodeGetEstLocalErrors(cvode_mem, ele);
		CVodeGetErrWeights(cvode_mem, eweight);
		N_Vector result = N_VNew_Serial(N);
		N_VProd(ele, eweight, result);
		num_array res(NV_DATA_S(result), NV_DATA_S(result) + N);
		N_VDestroy_Serial(ele);
		N_VDestroy_Serial(eweight);
		return res;
	}

	~Impl() {
		release();
	}

	static int sparse_jacobian(realtype t, N_Vector y, N_Vector fy, SUNMatrix J, void * userdata, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
	static int dense_jacobian(realtype t, N_Vector y, N_Vector fy, SUNMatrix J, void * userdata, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3);
};


int cmf::math::CVode3::initialize()
{
	return _implementation->initialize();
}

int cmf::math::CVode3::integrate(cmf::math::Time t_max, cmf::math::Time dt)
{

	return _implementation->integrate(t_max, dt);
}

void cmf::math::CVode3::reset()
{
	_implementation->reset();
}

cmf::math::CVode3::CVode3(cmf::math::StateVariableOwner & states, real epsilon)
	: Integrator(states, epsilon)
{
	cmf::math::CVode3::Impl* p_impl = new cmf::math::CVode3::Impl(this);
    _implementation=std::unique_ptr<cmf::math::CVode3::Impl>(p_impl);
}

void cmf::math::CVode3::set_error_msg(std::string error)
{
	_error_msg = error;
}

CVode3 * cmf::math::CVode3::copy() const
{
	return nullptr;
}
cmf::math::num_array cmf::math::CVode3::_get_jacobian() const
{
	throw std::runtime_error(this->to_string() + " has no retreivable Jacobian");
}

CVode3::~CVode3() = default;

CVodeInfo cmf::math::CVode3::get_info() const
{
	CVodeInfo ci;
	void * cvm = _implementation->cvode_mem;

	ci.size = size();
	CVodeGetWorkSpace(cvm, &ci.workspace_real, &ci.workspace_int);
	ci.workspace_byte = ci.workspace_real * sizeof(realtype) + ci.workspace_int * sizeof(long int);
	int qlast, qcur;
	realtype hinused, hlast, hcur, tcur;
	CVodeGetIntegratorStats(cvm, &ci.steps, &ci.rhs_evaluations, &ci.linear_solver_setups, &ci.error_test_fails,
		&qlast, &qcur, &hinused, &hlast, &hcur, &tcur);
	ci.current_order = qcur;
	CVodeGetNonlinSolvStats(cvm, &ci.nonlinear_solver_iterations, &ci.nonlinear_solver_convergence_failures);

	int version_len = 25;
	char version_buffer[25];
	SUNDIALSGetVersion(version_buffer, version_len);
	ci.sundials_version = std::string(version_buffer, version_len);
	ci.dxdt_method_calls = _implementation->dxdt_method_calls;
	return ci;
}

cmf::math::num_array cmf::math::CVode3::get_error() const
{
	return _implementation->get_error();
}

cmf::math::CVodeDense::CVodeDense(cmf::math::StateVariableOwner & states, real epsilon)
	: CVode3(states, epsilon)
{}



cmf::math::num_array cmf::math::CVodeDense::_get_jacobian() const
{
	CVode3::Impl& i = *_implementation;
	void * cvm = i.cvode_mem;
	if (cvm == 0 || i.J == 0) {
		throw std::runtime_error(this->to_string() + ": No access to Jacobian matrix, if the solver is not initialized or the solver has none. Run the solver for any timestep");
	}
	cmf::math::num_array res(SM_DATA_D(i.J), SM_DATA_D(i.J) + SM_LDATA_D(i.J));
	return res;
}

void cmf::math::CVodeDense::set_solver()
{
	CVode3::Impl& i = *_implementation;
	if (i.cvode_mem == 0) {
		throw std::runtime_error(this->to_string() + ": Tried to create dense solver for uninitialized cvode");
	}
	i.J = SUNDenseMatrix(i.N, i.N);
	i.LS = SUNDenseLinearSolver(i.y, i.J);
	// Check for LS / J = NULL
	int flag = 0;
	flag = CVDlsSetLinearSolver(i.cvode_mem, i.LS, i.J);
	// Check flag and raise an error
	if (flag != CVLS_SUCCESS) {
		throw std::runtime_error(this->to_string() + ": Failed to create linear solver");
	}
	flag = CVodeSetJacFn(i.cvode_mem, CVode3::Impl::dense_jacobian);
	// Check flag and raise an error
	if (flag != CVLS_SUCCESS) {
		throw std::runtime_error(this->to_string() + ": Failed to set custom DQ Jacobian function");
	}

}

cmf::math::CVodeBanded::CVodeBanded(cmf::math::StateVariableOwner & states, real epsilon, int _bandwidth)
	: CVode3(states, epsilon), bandwidth(_bandwidth)
{}

inline std::string cmf::math::CVodeBanded::to_string() const {
	return "CVodeBanded(w=" + std::to_string(bandwidth) + ")";

}

void cmf::math::CVodeBanded::set_solver()
{
	CVode3::Impl& i = *_implementation;
	if (i.cvode_mem == 0) {
		throw std::runtime_error("Tried to create banded solver for uninitialized cvode");
	}
	i.J = SUNBandMatrix(i.N, bandwidth, bandwidth);
	i.LS = SUNBandLinearSolver(i.y, i.J);
	int flag = CVDlsSetLinearSolver(i.cvode_mem, i.LS, i.J);
	// Check flag and raise an error
	if (flag != CVLS_SUCCESS) {
		throw std::runtime_error("CVODE: Failed to create linear solver");
	}

}

cmf::math::CVodeDiag::CVodeDiag(cmf::math::StateVariableOwner & states, real epsilon)
	: CVode3(states, epsilon)
{}

void cmf::math::CVodeDiag::set_solver()
{
	CVode3::Impl& i = *_implementation;
	if (i.cvode_mem == 0) {
		throw std::runtime_error("Tried to create solver for uninitialized cvode");
	}
	int flag = CVDiag(i.cvode_mem);

}

cmf::math::CVodeKrylov::CVodeKrylov(cmf::math::StateVariableOwner & states, real epsilon,
	int _bandwidth, char _preconditioner)
	: CVode3(states, epsilon), bandwidth(_bandwidth), preconditioner(_preconditioner)
{}

inline std::string cmf::math::CVodeKrylov::to_string() const {
	return "CVodeKrylov(w=" + std::to_string(bandwidth) + ", p='" + preconditioner + "')";
}

void cmf::math::CVodeKrylov::set_solver()
{
	CVode3::Impl& i = *_implementation;
	if (i.cvode_mem == 0) {
		throw std::runtime_error("Tried to create banded solver for uninitialized cvode");
	}
	int prec = PREC_LEFT;
	switch (preconditioner) {
	case 'L':
		prec = PREC_LEFT;
		break;
	case 'R':
		prec = PREC_RIGHT;
		break;
	case 'B':
		prec = PREC_BOTH;
		break;
	case 'N':
		prec = PREC_NONE;
		break;
	}
	i.LS = 0;
	i.LS = SUNSPGMR(i.y, prec, 0);
	if (i.LS == 0) {
		throw std::runtime_error("Linear solver not created");
	}
	int flag = CVodeSetLinearSolver(i.cvode_mem, i.LS, NULL);
	if (flag == CVLS_SUCCESS)
		flag = CVBandPrecInit(i.cvode_mem, i.N, bandwidth, bandwidth);
	else {
		throw std::runtime_error("Setting linear solver failed");
	}
}

std::string cmf::math::CVodeInfo::to_string() const
{
	std::ostringstream out;
	out << size << " state variables" << std::endl;
	out << "workspace (real/int/bytes): " << workspace_real << "/" << workspace_int << "/" << workspace_byte << std::endl;
	out << steps << " steps" << std::endl;
	out << "order is " << current_order << std::endl;
	out << rhs_evaluations << " rhs evaluations" << std::endl;
	out << linear_solver_setups << " linear solver setups" << std::endl;
	out << error_test_fails << " error test failures" << std::endl;
	out << nonlinear_solver_iterations << " non linear solver iterations" << std::endl;
	out << nonlinear_solver_convergence_failures << " non linear solver convergence failures" << std::endl;
	out << dxdt_method_calls << " calls to any dxdt method of a state" << std::endl;
	return out.str();
}

cmf::math::CVodeAdams::CVodeAdams(cmf::math::StateVariableOwner & states, real epsilon)
	:CVode3(states, epsilon)
{
	this->_implementation->use_stiff_solver = false;
}

std::string cmf::math::CVodeAdams::to_string() const
{
	return "CVodeAdams()";
}

cmf::math::CVodeKLU::CVodeKLU(cmf::math::StateVariableOwner & states, real epsilon)
	: CVode3(states, epsilon)
{
}

std::string cmf::math::CVodeKLU::to_string() const
{
	return "CVodeKLU()";
}


std::vector<realtype> copy_to_dbg_vec(N_Vector v) {
	size_t le = NV_LENGTH_S(v);
	realtype* nv_begin = NV_DATA_S(v);
	realtype* nv_end = nv_begin + le;
	return std::vector<realtype>(nv_begin, nv_end);
}

int copy_sparse_to_sparse(SUNMatrix from, SUNMatrix to) {
	sunindextype
		fN = SM_COLUMNS_S(from),
		fNP = SM_NP_S(from),
		fNNZ = SM_NNZ_S(from);

	sunindextype
		tN = SM_COLUMNS_S(to),
		tNP = SM_NP_S(to),
		tNNZ = SM_NNZ_S(to);
	if (fN != tN) return -1;
	if (fNP != tNP) return -2;
	if (fNNZ != tNNZ) return -3;

	std::copy(SM_INDEXPTRS_S(from), SM_INDEXPTRS_S(from) + fNP + 1, SM_INDEXPTRS_S(to));
	std::copy(SM_INDEXVALS_S(from), SM_INDEXVALS_S(from) + fNNZ, SM_INDEXVALS_S(to));
	std::copy(SM_DATA_S(from), SM_DATA_S(from) + fNNZ, SM_DATA_S(to));
	return 0;
}

cmf::math::num_array copy_sparse_to_array(SUNMatrix A_sparse) {
	if (SUNMatGetID(A_sparse) != SUNMATRIX_SPARSE) {
		throw std::runtime_error("copy_sparse_to_dense: Incoming matrix is not sparse");
	}
	sunindextype NP = SM_NP_S(A_sparse); // Problem size
	sunindextype NNZ = SM_NNZ_S(A_sparse); // Number of Non-Zero values

	cmf::math::num_array res(size_t(SM_ROWS_S(A_sparse) * SM_COLUMNS_S(A_sparse)));
	sunindextype		 // local variables in loop
		data_pos_start,  // Starting position in a row
		data_pos_end,    // End position in a row
		col, row;			 // current column number

	for (col = 0; col < NP; ++col) {
		// Loop through column positions
		data_pos_start = SM_INDEXPTRS_S(A_sparse)[col];
		data_pos_end = SM_INDEXPTRS_S(A_sparse)[col + 1];
		for (sunindextype data_pos = data_pos_start; data_pos < data_pos_end; ++data_pos) {
			// Get the column number
			row = SM_INDEXVALS_S(A_sparse)[data_pos];
			// Set the Matrix value
			res[col * NP + row] = SM_DATA_S(A_sparse)[data_pos];
		}
	}
	return res;

}

cmf::math::num_array cmf::math::CVodeKLU::_get_jacobian() const
{
	CVode3::Impl& i = *_implementation;
	void * cvm = i.cvode_mem;
	if (cvm == 0 || i.J == 0) {
		throw std::runtime_error(this->to_string() + ": No access to Jacobian matrix, if the solver is not initialized or the solver has none. Run the solver for any timestep");
	}
	return copy_sparse_to_array(i.J);
}


int CVode3::Impl::sparse_jacobian(
	realtype t,
	N_Vector y, N_Vector fy,
	SUNMatrix J,
	void *userdata,
	N_Vector tmp1, N_Vector tmp2, N_Vector tmp3
) {
	// Use lib/lib/cvode_ls.c:815 (cvLsDenseDQJac) as template

	/*
	Needs information about Jacobian structure already build in J. 
	Can be gained from J->indexvals and J->indexptrs
	*/
	CVode3::Impl * integ_impl = static_cast<CVode3::Impl*>(userdata);
	CVodeKLU * integ = static_cast<CVodeKLU *>(integ_impl->_integrator);
	void * cvode_mem = integ_impl->cvode_mem;

	Time time = cmf::math::day * t;

	sunindextype N = integ_impl->N;

	realtype 
		altered_dxdt = 0.0, 
		old_y = 0.0;

	realtype * J_data = SM_DATA_S(J);
	realtype * y_data = NV_DATA_S(y);
	realtype * f_data = NV_DATA_S(fy);
	
	typedef std::vector<realtype> real_vec;

	// Copy current y data to ytmp
	N_Vector ytmp = tmp1;
	std::copy(NV_DATA_S(y), NV_DATA_S(y) + NV_LENGTH_S(y), NV_DATA_S(ytmp));
	// Get current error weights
	N_Vector ewt = tmp2;
	CVodeGetErrWeights(integ_impl->cvode_mem, ewt);
	realtype inc = 1e-12; 
	/* Obtain pointers to the data for ewt, y */
	realtype * ewt_data = NV_DATA_S(ewt);
	// if (cv_mem->cv_constraints != NULL)
	// 	cns_data = N_VGetArrayPointer(cv_mem->cv_constraints);
	// 
	/* Set minimum increment based on uround and norm of f */
	realtype srur = SUNRsqrt(UNIT_ROUNDOFF);
	realtype fnorm = N_VWrmsNorm(fy, ewt);
	realtype h;
	CVodeGetCurrentStep(cvode_mem, &h);
	realtype minInc = (fnorm != 0.0) ?
				(1000.0 * SUNRabs(h) * UNIT_ROUNDOFF * N * fnorm) : 1.0;

	// .. populate structure of SUNDIALS matrix
	std::copy(integ->sps.indexvalues.begin(), integ->sps.indexvalues.end(), SM_INDEXVALS_S(J));
	std::copy(integ->sps.indexpointers.begin(), integ->sps.indexpointers.end(), SM_INDEXPTRS_S(J));

	// local variables in loop
	sunindextype		 
		// current row number
		data_pos, row, col;			 

	integ->set_states(NV_DATA_S(ytmp));
	// real_vec dbg_y = copy_to_dbg_vec(ytmp);
	// integ->copy_dxdt(time, NV_DATA_S(tmp3));
	// real_vec dbg_dxdt = copy_to_dbg_vec(tmp3);
	// real_vec dbg_J(J_data, J_data + SM_NNZ_S(J));
	
	for (col = 0; col < integ_impl->N; ++col) {
		// Change state for the row
		old_y = NV_Ith_S(ytmp, col);
		// Set the increment
		inc = SUNMAX(srur*SUNRabs(old_y), minInc/ewt_data[col]);
		NV_Ith_S(ytmp, col) += inc;
		integ->set_state(col, NV_Ith_S(ytmp, col));
		// integ->set_states(NV_DATA_S(ytmp));
		// dbg_y = copy_to_dbg_vec(ytmp);
		// Loop through column positions
		for (data_pos = SM_INDEXPTRS_S(J)[col]; 
			 data_pos < SM_INDEXPTRS_S(J)[col + 1];
			 ++data_pos) 
		{
			// Get the column number
			row = SM_INDEXVALS_S(J)[data_pos];
			// Calculate the new dxdt for that row
			// integ->copy_dxdt(time, NV_DATA_S(tmp3));
			// dbg_dxdt = copy_to_dbg_vec(tmp3);
			// altered_dxdt = NV_Ith_S(tmp3, row);
			altered_dxdt = integ->m_States[row]->dxdt(time);
			integ_impl->dxdt_method_calls += 1;
			// Set the Jacobian value
			J_data[data_pos] = altered_dxdt / inc - f_data[row] / inc;
			// dbg_J = real_vec(J_data, J_data + SM_NNZ_S(J));
		}
		// Undo state change
		NV_Ith_S(ytmp, col) = old_y;
		integ->set_states(NV_DATA_S(ytmp));
	}
	int flag = copy_sparse_to_sparse(J, integ_impl->J);
	// real_vec iiJ(SM_DATA_S(integ_impl->J), SM_DATA_S(integ_impl->J) + SM_NNZ_S(integ_impl->J));
	return CVLS_SUCCESS;
}

int cmf::math::CVode3::Impl::dense_jacobian(realtype t, N_Vector y, N_Vector fy, SUNMatrix J, void * userdata, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
	realtype fnorm, minInc, inc, inc_inv, yjsaved, srur, conj, h;
	realtype *y_data, *ewt_data, *cns_data;
	N_Vector ftemp, jthCol, ewt;

	sunindextype j, N;
	int retval = 0;

	/* access solver interface structure */
	CVode3::Impl * integ_impl = static_cast<CVode3::Impl*>(userdata);
	CVodeKLU * integ = static_cast<CVodeKLU *>(integ_impl->_integrator);
	void * cvode_mem = integ_impl->cvode_mem;

	/* access matrix dimension */
	N = SUNDenseMatrix_Rows(J);

	/* Rename work vector for readibility */
	ftemp = tmp1;

	/* Create an empty vector for matrix column calculations */
	jthCol = N_VCloneEmpty(tmp1);

	/* Obtain pointers to the data for ewt, y */
	// Get current error weights
	ewt = tmp2;
	CVodeGetErrWeights(integ_impl->cvode_mem, ewt);
	/* Obtain pointers to the data for ewt, y */
	ewt_data = NV_DATA_S(ewt);
	y_data = N_VGetArrayPointer(y);
	/* // No access to constraints
	if (cv_mem->cv_constraints != NULL)
		cns_data = N_VGetArrayPointer(cv_mem->cv_constraints);
	*/
	/* Set minimum increment based on uround and norm of f */
	srur = SUNRsqrt(UNIT_ROUNDOFF);
	fnorm = N_VWrmsNorm(fy, ewt);
	srur = SUNRsqrt(UNIT_ROUNDOFF);
	fnorm = N_VWrmsNorm(fy, ewt);
	CVodeGetCurrentStep(cvode_mem, &h);
	minInc = (fnorm != 0.0) ?
		(1000.0 * SUNRabs(h) * UNIT_ROUNDOFF * N * fnorm) : 1.0;

	for (j = 0; j < N; j++) {

		/* Generate the jth col of J(tn,y) */
		N_VSetArrayPointer(SUNDenseMatrix_Column(J, j), jthCol);

		yjsaved = y_data[j];
		inc = SUNMAX(srur * SUNRabs(yjsaved), minInc / ewt_data[j]);

		// Contraints not accesible
		/* // Adjust sign(inc) if y_j has an inequality constraint.
		if (cv_mem->cv_constraints != NULL) {
			conj = cns_data[j];
			if (SUNRabs(conj) == ONE) { if ((yjsaved + inc)*conj < ZERO)  inc = -inc; }
			else if (SUNRabs(conj) == TWO) { if ((yjsaved + inc)*conj <= ZERO) inc = -inc; }
		}
		*/
		y_data[j] += inc;

		retval = f(t, y, ftemp, userdata);
		
		if (retval != 0) break;

		y_data[j] = yjsaved;

		inc_inv = 1.0 / inc;
		N_VLinearSum(inc_inv, ftemp, -inc_inv, fy, jthCol);

	}
	/* Destroy jthCol vector */
	N_VSetArrayPointer(NULL, jthCol);  /* SHOULDN'T BE NEEDED */
	N_VDestroy(jthCol);

	return(retval);
}


void cmf::math::CVodeKLU::set_solver()
{
	CVode3::Impl& i = *_implementation;
	int retval = 0;
	
	// Create sparse matrix
	// .. get sparse structure from my states
	sps.generate(this->get_states());
	// .. create the SUNDIALS sparse matrix
	i.J = SUNSparseMatrix(sps.N, sps.N, sps.NNZ, CSC_MAT);
	if (i.J == NULL) throw std::runtime_error("CVODE-KLU: Failed to construct sparse matrix");

	// Create Linear Solver and attach to CVode
	i.LS = SUNLinSol_KLU(i.y, i.J);
	if (i.LS == NULL) throw std::runtime_error("CVODE-KLU: Failed to construct sparse KLU linear solver");
	retval = CVodeSetLinearSolver(i.cvode_mem, i.LS, i.J);
	if (retval) throw std::runtime_error("CVODE-KLU: Failed to attach sparse KLU linear solver");

	/* Set the user-supplied Jacobian routine Jac */
	retval = CVodeSetJacFn(i.cvode_mem, CVode3::Impl::sparse_jacobian);
	if (retval) throw std::runtime_error("CVODE-KLU: Failed to set sparse jacobian function");

}
