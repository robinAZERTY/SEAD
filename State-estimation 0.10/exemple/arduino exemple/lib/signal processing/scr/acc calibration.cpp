/*
cree par : robinAZERTY
version du 13/10/2022
*/
#ifndef ACC_CALIBRATION_CPP
#define ACC_CALIBRATION_CPP


#include "..\../tools/scr/ellipsoidFitting.cpp"


class AccCalibration
{
public:
    // constructor
    AccCalibration();
    ~AccCalibration();

    void calibrate(const double *samples, unsigned short int nbData, const double &eps);//eps=0.015 is good ?
    float calibrate(const double *samples, unsigned short int nbData, const double &eps, const double *initialParameters);
    void reset();

    const Vector apply(Vector &acc_raw) const;

//private:
    // the ellipsoid fitting object
    Ellipsoid ellipsoidFitting;
    double* parameters;
};

AccCalibration::AccCalibration()
{
}

AccCalibration::~AccCalibration()
{
}

void AccCalibration::calibrate(const double *samples, unsigned short int nbData, const double &eps)
{   
    ellipsoidFitting.points = (double *)samples;
    ellipsoidFitting.n = nbData;
    parameters = (double*)ellipsoidFitting.fit_grad_descent(eps);
}

float AccCalibration::calibrate(const double *samples, unsigned short int nbData, const double &eps, const double *initialParameters)
{   
    ellipsoidFitting.points = (double *)samples;
    ellipsoidFitting.n = nbData;
    parameters = (double*)ellipsoidFitting.fit_grad_descent_step(eps,initialParameters);
    return ellipsoidFitting.convergence_rate;
}

const Vector AccCalibration::apply(Vector &acc_raw) const
{
    return ellipsoidFitting.correctionMatrix * (acc_raw - ellipsoidFitting.B);
}

void AccCalibration::reset()
{
    ellipsoidFitting = Ellipsoid();
}
#endif