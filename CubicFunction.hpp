/*
 * CubicFunction.hpp
 * 
 * Created Date: Sunday, October 20th 2019, 10:38:06 pm
 * by Dinger
 */

#ifndef __CUBIC_FUNCTION
#define __CUBIC_FUNCTION

typedef unsigned int uint;

class CubicFunction
{
public:
    double a[4];    // It's the coefficient of x in power of i.
    CubicFunction();
    CubicFunction(double d);
    CubicFunction(const CubicFunction& cf);
    ~CubicFunction() {}
    CubicFunction operator+(CubicFunction cf) const;
    CubicFunction operator-(CubicFunction cf) const;
    CubicFunction operator*(CubicFunction cf) const;
    CubicFunction operator+(double d) const;
    CubicFunction operator-(double d) const;
    CubicFunction operator*(double d) const;
    CubicFunction operator/(double d) const;
    CubicFunction square() const;
    friend CubicFunction operator+(double d, const CubicFunction cf);
    friend CubicFunction operator-(double d, const CubicFunction cf);
    friend CubicFunction operator*(double d, const CubicFunction cf);
    friend std::ostream& operator<<(std::ostream& cout, CubicFunction cf);
};

CubicFunction::CubicFunction() {
    for (uint i=0; i<4; i++) {
        a[i] = 0;
    }
}

CubicFunction::CubicFunction(double d) :
        CubicFunction() {
    a[0] = d;
}

CubicFunction::CubicFunction(const CubicFunction& cf) {
    for (uint i=0; i<4; i++) {
        a[i] = cf.a[i];
    }
}

CubicFunction CubicFunction::operator+(CubicFunction cf) const {
    CubicFunction res;
    for (uint i=0; i<4; i++) {
        res.a[i] = a[i] + cf.a[i];
    }
    return res;
}

CubicFunction CubicFunction::operator-(CubicFunction cf) const {
    CubicFunction res;
    for (uint i=0; i<4; i++) {
        res.a[i] = a[i] - cf.a[i];
    }
    return res;
}

CubicFunction CubicFunction::operator*(CubicFunction cf) const {
    CubicFunction res;
    for (int i=3; i>=0; i--) {
        for (int j=i; j>=0; j--) {
            res.a[i] += a[j] * cf.a[i-j];
        }
    }
    return res;
}

CubicFunction CubicFunction::operator+(double d) const {
    CubicFunction res(*this);
    res.a[0] += d;
    return res;
}

CubicFunction CubicFunction::operator-(double d) const {
    CubicFunction res(*this);
    res.a[0] -= d;
    return res;
}

CubicFunction CubicFunction::operator*(double d) const {
    CubicFunction res(*this);
    for (uint i=0; i<4; i++) {
        res.a[i] *= d;
    }
    return res;
}

CubicFunction CubicFunction::operator/(double d) const {
    CubicFunction res(*this);
    for (uint i=0; i<4; i++) {
        res.a[i] /= d;
    }
    return res;
}

CubicFunction CubicFunction::square() const{
    return (*this) * (*this);
}

CubicFunction operator+(double d, CubicFunction cf) {
    return cf + d;
}

CubicFunction operator-(double d, CubicFunction cf) {
    return cf * (-1) + d;
}

CubicFunction operator*(double d, CubicFunction cf) {
    return cf * d;
}

std::ostream& operator<<(std::ostream& cout, CubicFunction cf) {
    for (int j=3; j>0; j--) {
        cout << cf.a[j] << "x^" << j << " + ";
    }
    cout << cf.a[0] << std::endl;
    return cout;
}

#endif // __CUBIC_FUNCTION
