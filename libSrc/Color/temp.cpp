
#include <iostream>
#include "../Eigen/Core"
#include "../Eigen/Eigen"

using namespace std;

int main(){


    Eigen::Vector3f tv;
    tv << 1,2,3;

    Eigen::Matrix3f twos;
    twos << 2,2,2,2,2,2,2,2,2;

    Eigen::Matrix3f tbt = twos * tv.asDiagonal();

    Eigen::Matrix3f forward;
    forward << 1,2,3,4,5,6,7,2,9;

    cout  << forward.inverse() << endl;

    return 0;
}
