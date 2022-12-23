/*
cree par : robinAZERTY
version du 22/12/2022

the class MagCalibration is exactly the same as the class AccCalibration
so we use inheritance of the class AccCalibration
 
*/


#if true // if it is validated
#include "../acc calibration/acc calibration.cpp"
#else
#include "..\..\../validated/signal processing/acc calibration/acc calibration.cpp"
#endif

class MagCalibration : public AccCalibration
{
};
