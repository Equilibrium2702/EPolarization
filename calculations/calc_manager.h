#ifndef CALC_MANAGER_H_
#define CALC_MANAGER_H_

#include "given.h"
#include <iostream>
#include "equation_matrix_solver.h"
#include "array.h"
#include "curve.h"
#include "incident_field.h"
#include <vector>
#include "discretization.h"
#include "incident_field_package.h"
#include "timer.h"
#include "box.h"
#include "discretize_curve.h"
#include "field_solver.h"


class CalcManager {
public:
	CalcManager(Given);

	void run();

private:
	Given given;
};


CalcManager::CalcManager(Given params):given(params){
}

void CalcManager::run(){
	DonationBox<DiscretizeCurve> discCurves;
	for (size_t i = 0; i < given.curves.size(); i++)
		discCurves << new DiscretizeCurve(
		    given.curves[i], 20, ch1Nodes);

	Discretization d(discCurves, given.fields);

	std::cout << "Vector x(t_i):" <<std::endl;

	EquationMatrixSolver<std::complex<double>> ems;

	d.createArray();
	Timer timer;
	timer.start();
	auto matr = d.createMatrix();
	timer.stop();
	std::cout << "fill matrix time:" << timer.interval()<<std::endl;
	timer.start();
	CArrayPtr<double> x = ems(*matr, *d.createArray());
	timer.stop();
	std::cout << "calculation time:" << timer.interval()<<std::endl;

	for (size_t i = 0; i < x->size(); i++) {
		std::cout << (*x)[i] << std::endl;
	}

	DonationBox<Array<std::complex<double>>> currents;
	size_t p = 0;
	for (size_t i = 0; i < discCurves.size(); i++) {
		Array<std::complex<double>>* current =
				new Array<std::complex<double>>(discCurves[i].size());
		for (size_t j = 0; j < discCurves[i].size(); j++, p++) {
			(*current)[j] = (*x)[p];
		}
		currents << current;
	}

	FieldSolver f(discCurves, currents, given.wavenumber);
	std::cout <<std::endl<<std::endl<< f.field({2,2})<<std::endl
			<<std::endl<< f.farField(1);
}

#endif /* CALC_MANAGER_H_ */
