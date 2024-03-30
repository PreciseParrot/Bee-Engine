#include "Collision.hpp"

#include <cfloat>
#include <cmath>
#include <vector>

#include "Bee.hpp"
#include "Math/Vector2f.hpp"

static const Vector2f origin(0, 0);

Vector2f tripleProduct(const Vector2f& in1, const Vector2f& in2, const Vector2f& in3)
{
    float x1, x2, x3, out1x, out2x;
    float y1, y2, y3, out1y, out2y;
    float z1, z2, z3, out1z;

    x1 = in1.x;
    y1 = in1.y;
    z1 = 0;

    x2 = in2.x;
    y2 = in2.y;
    z2 = 0;

    x3 = in3.x;
    y3 = in3.y;
    z3 = 0;

    out1x = y1 * z2 - z1 * y2;
    out1y = z1 * x2 - x1 * z2;
    out1z = x1 * y2 - y1 * x2;

    out2x = out1y * z3 - out1z * y3;
    out2y = out1z * x3 - out1x * z3;

    return Vector2f(out2x, out2y);
}

Vector2f getSupportPoint(const HitBox& hitBox, const Vector2f& directionVector)
{
    float largestDotProduct = -FLT_MAX;
    Vector2f supportPoint;

    for (const Vector2f& point : hitBox.hitBoxPoints)
    {
        float dotProduct = point.dot(directionVector);
        if (dotProduct > largestDotProduct)
        {
            largestDotProduct = dotProduct;
            supportPoint = point;
        }
    }
    return supportPoint;
}

Vector2f getMinkowskiPoint(const HitBox& hitBox1, const HitBox& hitBox2, const Vector2f& directionVector)
{
    return getSupportPoint(hitBox1, directionVector) - getSupportPoint(hitBox2, directionVector * -1);
}

Vector2f expandingPolytopeAlgorithm(const std::vector<Vector2f>& simplex, const HitBox& hitBox1, const HitBox& hitBox2)
{
    int minIndex = 0;
    float minDistance = FLT_MAX;
    Vector2f minNormal;
    std::vector<Vector2f> polytope = simplex;

    while (true)
    {
        for (size_t i = 0; i < polytope.size(); i++)
        {
            int j = (i+1) % polytope.size();

            Vector2f pointI = polytope[i];
            Vector2f pointJ = polytope[j];

            Vector2f lineIJ = pointJ - pointI;
            
            Vector2f normal(-lineIJ.y, lineIJ.x);
            normal.normalize();

            float distance = normal.dot(pointI);

            if (distance < 0)
            {
                distance *= -1;
                normal *= -1;
            }

            if (distance < minDistance)
            {
                minDistance = distance;
                minNormal = normal;
                minIndex = j;
            }

            if (distance == 0)
            {
                return Vector2f();
            }
        }
        Vector2f support = getMinkowskiPoint(hitBox1, hitBox2, minNormal);
        float sDistance = minNormal.dot(support);

        if (abs(sDistance - minDistance) < 0.0001f) break;

        minDistance = FLT_MAX;
        polytope.insert(polytope.begin() + minIndex, support);
    }
    return minNormal * minDistance * -1;
}

bool Collision::checkCollision(const HitBox& hitBox1, const HitBox& hitBox2, Intersection* intersection)
{
    std::vector<Vector2f> simplex;

    Vector2f currentDirection(-1, 0);

    simplex.push_back(getMinkowskiPoint(hitBox1, hitBox2, currentDirection));

    currentDirection = origin - simplex[0];

    while (true)
    {
        Vector2f pointA = getMinkowskiPoint(hitBox1, hitBox2, currentDirection);
        if (pointA.dot(currentDirection) < 0)
        {
            return false;
        }
        simplex.push_back(pointA);

        if (simplex.size() == 2)
        {
            Vector2f directionAB = simplex[0] - simplex[1];
            Vector2f directionAO = origin - simplex[1];

            currentDirection = tripleProduct(directionAB, directionAO, directionAB);
            continue;
        }

        Vector2f lineAB = simplex[1] - simplex[2];
        Vector2f lineAC = simplex[0] - simplex[2]; 
        Vector2f lineAO = origin - simplex[2];

        Vector2f orthogonalAB = tripleProduct(lineAC, lineAB, lineAB);
        Vector2f orthogonalAC = tripleProduct(lineAB, lineAC, lineAC);

        if (orthogonalAB.dot(lineAO) > 0)
        {
            currentDirection = orthogonalAB;
            simplex.erase(simplex.begin());
            continue;
        }
        else if (orthogonalAC.dot(lineAO) > 0)
        {
            currentDirection = orthogonalAC;
            simplex.erase(simplex.begin() + 1);
            continue;
        }

        intersection->mtv = expandingPolytopeAlgorithm(simplex, hitBox1, hitBox2);
        return true;
    }
}