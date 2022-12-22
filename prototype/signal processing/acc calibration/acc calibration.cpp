/*
cree par : robinAZERTY
version du 13/10/2022
*/

#if false // if it is validated
#include "../..\tools\ellipsoidFitting\ellipsoidFitting.cpp"
#else
#include "../../..\validated\tools\ellipsoidFitting\ellipsoidFitting.cpp"
#endif

class AccCalibration
{
public:
    // constructor
    AccCalibration();
    ~AccCalibration();

    void calibrate(const double *samples, unsigned short int nbData);
    void reset();

    const Vector apply(Vector &acc_raw) const;

//private:
    // the ellipsoid fitting object
    Ellipsoid ellipsoidFitting;
};

AccCalibration::AccCalibration()
{
}

AccCalibration::~AccCalibration()
{
}

void AccCalibration::calibrate(const double *samples, unsigned short int nbData)
{

    ellipsoidFitting.points = (double *)samples;
    ellipsoidFitting.n = nbData;
    ellipsoidFitting.fit_grad_descent(0.015)[12];
}

const Vector AccCalibration::apply(Vector &acc_raw) const
{
    return ellipsoidFitting.correctionMatrix * (acc_raw - ellipsoidFitting.B);
}

void AccCalibration::reset()
{
    ellipsoidFitting = Ellipsoid();
}