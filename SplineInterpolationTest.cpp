/*
 * SplineInterpolationTest.cpp
 * 
 * Created Date: Sunday, October 20th 2019, 11:16:57 am
 * by Dinger
 */

#include "SplineInterpolation.hpp"
#include <sstream>

int main() {
    uint n = 3;
    SplineInterpolation SI(n);
    std::stringstream ss;
    ss << "0   1     2   3  \n";
    ss << "0   0.5   2   1.5\n";
    for (uint i=0; i<n+1; i++) {
        ss >> SI.X[i];
    }
    for (uint i=0; i<n+1; i++) {
        ss >> SI.Y[i];
    }
    SI.firstBorderCondition[0] = 0.2;
    SI.firstBorderCondition[1] = -1;
    SI.secondBorderCondition[0] = -0.3;
    SI.secondBorderCondition[1] = 3.3;
    std::cout << "First Border Condition Solution:\n";
    SI.solveFirstBorderCondition();
    SI.sayResult();
    std::cout << "Second Border Condition Solution:\n";
    SI.solveSecondBorderCondition();
    SI.sayResult();
    return 0;
}
