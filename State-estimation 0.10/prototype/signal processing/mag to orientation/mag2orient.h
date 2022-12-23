/*
cree par : RobinAZERTY
version du 22/12/2022

The magnometer is used to determine the orientation in yaw axe. 
*/

#include "..\..\..\../common/exchange data/state/State.cpp"

class mag2orient
{
public:
    mag2orient();
    ~mag2orient();

    void set_prev_quat(const Quaternion &prev_orientation);
    void set_prev_ypr(const Vector &prev_ypr);

    void compute(const Vector &current_ypr, const Vector &magData, const double &dt);
    void compute(const Quaternion &current_orientation, const Vector &magData, const double &dt);

    OrientationState get_OrientationState();
    Vector get_ypr();

private:
    void general_compute(const Vector &magData);
    Quaternion prev_quat;
    OrientationState orientationState;
    Vector ypr, prev_ypr;
};