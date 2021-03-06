#ifndef DISCRETIZATION_H_
#define DISCRETIZATION_H_
#include "matrix.h"
#include "helper.h"
#include "incident_field.h"
#include "incident_field_package.h"
#include "discretize_curve.h"
#include "types.h"
#include <atomic>

class Discretization {
	typedef IncidentFieldPackage IncidentFields;
    typedef std::vector<DiscretizeCurve::Ptr> CurvesList;

public:
    Discretization(const CurvesList& sCurvesFi, const CurvesList& sCurvesFi0, const IncidentFields& fields);

	MatrixPtr<types::complex> createMatrix(int threads = 4);
    MatrixPtr<types::complex> createHMatrix();
    std::vector<types::complex> createArray();
    std::vector<types::complex> createHArray();

private:
	size_t size = 0;


	const IncidentFields& fields;
    const CurvesList& curvesFi;
    const CurvesList& curvesFi0;


	//waveNumber
	const types::real wN;
	// contour < border
	std::vector<int> borders;
	//left border of contour
	int leftBorderOf(int c) {return c == 0 ? 0 : borders[c - 1] ;}
	void fillMatrixBlock(Matrix<types::complex>&, size_t c1, size_t c2, int);

	//Index taskIndex;
};

#endif /* DISCRETIZATION_H_ */
