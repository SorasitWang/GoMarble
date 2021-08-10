#ifndef UTIL_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTIL_H
#include <string>

#include <iostream>
typedef glm::vec3 v3;



v3 lineIntersection(v3 A, v3 B, v3 C, v3 D)
{
    // Line AB represented as a1x + b1y = c1
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * (A.x) + b1 * (A.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * (C.x) + b2 * (C.y);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return v3(FLT_MAX);
    }
    else
    {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        if (std::min(A.x, B.x) < x && x < std::max(A.x, B.x))
            return v3(x, y, 0);
        else
            return v3(FLT_MAX);
    }
}




float handle0(float x) {
    if (isnan(x)) return FLT_MAX;
    else if (x == 0) return FLT_MIN;
    else if (isinf(abs(x))) return copysign(FLT_MAX,x);
    return x;
}

void printV3(glm::vec3 v, std::string label = "") {
    std::cout << label << " " << v.x << " " << v.y << " " << v.z << std::endl;
}
float round(float value, unsigned int prec)
{
    float pow_10 = pow(10.0f, (float)prec);
    return round(value * pow_10) / pow_10;
}
#endif