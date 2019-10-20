/*
 * SplineInterpolation.hpp
 * 
 * Created Date: Sunday, October 20th 2019, 11:16:32 am
 * by Dinger
 */

#ifndef __SPLINE_INTERPOLATION_HPP
#define __SPLINE_INTERPOLATION_HPP

#include "GaussEliminationWithPivoting.hpp"
#include "CubicFunction.hpp"

template <typename T>
T pow(T x, uint n) {
    T res = x;
    for (uint i=0; i<n-1; i++) {
        res *= x;
    }
    return res;
}

// just for debug
void sayVector(std::vector<double> vec) {
    for (uint i=0; i<vec.size(); i++) {
        std::cout << vec[i] << ' ';
    }
    std::cout << std::endl;
}

class SplineInterpolation
{
private:
    uint n; // number of cubic functions.
    std::vector<double> h, lambda, d, m;
    void calculate_d();
    void calculate_result();
    double calculate_y(uint n, double x) const;
public:
    std::vector<double> X;
    std::vector<double> Y;
    double firstBorderCondition[2];
    double secondBorderCondition[2];
    Matrix<double> result;
    SplineInterpolation(uint n);
    ~SplineInterpolation() {}
    void solveFirstBorderCondition();
    void solveSecondBorderCondition();
    double getY(double x) const;
    void sayResult() const;
};

SplineInterpolation::SplineInterpolation(uint n) :
        n(n),
        result(n, 4) {
    if (n < 1) {
        std::cout << "Spline Interpolation Wrong: n < 1!\n";
    }
    X.resize(n + 1);
    Y.resize(n + 1);
    h.resize(n);
    lambda.resize(n);
    d.resize(n);
    m.resize(n + 1);
}

void SplineInterpolation::calculate_d() {
    for (uint i=0; i<n; i++) {
        h[i] = X[i+1] - X[i];
    }
    for (uint i=1; i<n; i++) {
        lambda[i] = h[i] / (h[i] + h[i-1]);
    }
    for (uint i=1; i<n; i++) {
        d[i] = 3 * ((1 - lambda[i]) * (Y[i+1] - Y[i]) / h[i] + lambda[i] * (Y[i] - Y[i-1]) / h[i-1]);
    }
}

void SplineInterpolation::calculate_result() {
    CubicFunction x;
    x.a[1] = 1;
    for (uint i=0; i<n; i++) {
        // this equation is absolutely the same with 6.6.8 in the book. --by dxd :)
        CubicFunction S = (h[i] + 2 * (x - X[i])) * (x - X[i+1]).square() / pow(h[i], 3) * Y[i] +
                          (h[i] - 2 * (x - X[i+1])) * (x - X[i]).square() / pow(h[i], 3) * Y[i+1] +
                          (x - X[i]) * (x - X[i+1]).square() / pow(h[i], 2) * m[i] +
                          (x - X[i+1]) * (x - X[i]).square() / pow(h[i], 2) * m[i+1];
        for (uint j=0; j<4; j++) {
            result.access(i, j) = S.a[3-j];
        }
    }
}

void SplineInterpolation::solveFirstBorderCondition() {
    calculate_d();
    m[0] = firstBorderCondition[0];
    m[n] = firstBorderCondition[1];
    Matrix<double> linearEquations(n-1, n);
    for (uint i=1; i<n; i++) {
        linearEquations.access(i-1, i-1) = 2;
        linearEquations.access(i-1, n-1) = d[i];
    }
    for (uint i=1; i<n-1; i++) {
        linearEquations.access(i-1, i) = 1 - lambda[i];
        linearEquations.access(i, i-1) = lambda[i+1];
    }
    linearEquations.access(0, n-1) -= lambda[1] * firstBorderCondition[0];
    linearEquations.access(n-2, n-1) -= (1 - lambda[n-1]) * firstBorderCondition[1];
    GaussEliminationWithPivoting<double> GEP(linearEquations);
    std::vector<double> m1;
    m1.resize(n-1);
    GEP.solve(m1);
    std::copy(m1.begin(), m1.end(), ++m.begin());
    m[0] = firstBorderCondition[0];
    calculate_result();
}

void SplineInterpolation::solveSecondBorderCondition() {
    calculate_d();
    // 6.6.16 in the book.
    d[0] = 3 * (Y[1] - Y[0]) / h[0] - h[0] * secondBorderCondition[0] / 2;
    d[n] = 3 * (Y[n] - Y[n-1]) / h[n-1] + h[n-1] * secondBorderCondition[1] / 2;
    Matrix<double> linearEquations(n+1, n+2);
    for (uint i=0; i<n+1; i++) {
        linearEquations.access(i, i) = 2;
        linearEquations.access(i, n+1) = d[i];
    }
    linearEquations.access(0, 1) = 1;
    linearEquations.access(n, n-1) = 1;
    for (uint i=1; i<n; i++) {
        linearEquations.access(i, i-1) = lambda[i];
        linearEquations.access(i, i+1) = 1 - lambda[i];
    }
    GaussEliminationWithPivoting<double> GEP(linearEquations);
    GEP.solve(m);
    calculate_result();
}

double SplineInterpolation::calculate_y(uint n, double x) const {
    double res = result.read(n, 0);
    for (uint i=1; i<4; i++) {
        res *= x;
        res += result.read(n, i);
    }
    return res;
}

double SplineInterpolation::getY(double x) const {
    if (x <= X[0]) {
        return calculate_y(0, x);
    }
    for (uint i=0; i<n; i++) {
        if (x >= X[i] && x <= X[i+1]) {
            return calculate_y(i, x);
        }
    }
    return calculate_y(n-2, x);
}

void SplineInterpolation::sayResult() const {
    for (uint i=0; i<n; i++) {
        std::cout << "S" << i << "(x) = ";
        for (uint j=0; j<3; j++) {
            std::cout << result.read(i, j) << "x^" << 3 - j << " + ";
        }
        std::cout << result.read(i, 3)  << "\t, " << X[i] << " <= x <= " << X[i+1] << std::endl;
    }
}

#endif // __SPLINE_INTERPOLATION_HPP
