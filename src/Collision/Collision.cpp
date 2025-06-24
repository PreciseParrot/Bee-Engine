#include "Collision.hpp"

#include <cfloat>
#include <cmath>
#include <vector>

#include "Bee/Collision/Hitbox.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Math/Vector2f.hpp"

static const Vector2f origin(0, 0);

Vector2f tripleProduct(const Vector2f& in1, const Vector2f& in2, const Vector2f& in3)
{
    const float x1 = in1.x;
    const float y1 = in1.y;
    constexpr float z1 = 0;

    const float x2 = in2.x;
    const float y2 = in2.y;
    constexpr float z2 = 0;

    const float x3 = in3.x;
    const float y3 = in3.y;
    constexpr float z3 = 0;

    const float out1X = y1 * z2 - z1 * y2;
    const float out1Y = z1 * x2 - x1 * z2;
    const float out1Z = x1 * y2 - y1 * x2;

    float out2X = out1Y * z3 - out1Z * y3;
    float out2Y = out1Z * x3 - out1X * z3;

    return {out2X, out2Y};
}

Vector2f supportPointEllipse(const Hitbox& hitbox, Vector2f& directionVector)
{
    directionVector.normalize();
    return hitbox.center + hitbox.ellipse * directionVector;
}

Vector2f supportPoint(const Hitbox& hitbox, Vector2f& directionVector)
{
    if (hitbox.isEllipse)
    {
        return supportPointEllipse(hitbox, directionVector);
    }

    float largestDotProduct = -FLT_MAX;
    Vector2f supportPoint;

    for (const Vector2f& point : hitbox.vertices)
    {
        if (const float dotProduct = point.dot(directionVector); dotProduct > largestDotProduct)
        {
            largestDotProduct = dotProduct;
            supportPoint = point;
        }
    }

    return supportPoint;
}

Vector2f minkowskiPoint(const Hitbox& hitbox1, const Hitbox& hitbox2, Vector2f& directionVector)
{
    Vector2f oppositeDirection = directionVector * -1;
    return supportPoint(hitbox1, directionVector) - supportPoint(hitbox2, oppositeDirection);
}

void expandingPolytopeAlgorithm(Intersection& intersection, std::vector<Vector2f>& polytope, const Hitbox& hitbox1, const Hitbox& hitbox2)
{
    float minDistance = FLT_MAX;
    int minIndex = 0;
    Vector2f minNormal;

    intersection.mtv = {0, 0};

    while (polytope.size() < 50)
    {
        for (size_t i = 0; i < polytope.size(); i++)
        {
            const size_t j = (i + 1) % polytope.size();

            Vector2f pointI = polytope[i];
            Vector2f pointJ = polytope[j];

            const Vector2f lineIJ = pointJ - pointI;

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
                return;
            }
        }

        Vector2f support = minkowskiPoint(hitbox1, hitbox2, minNormal);

        if (fabsf(minNormal.dot(support) - minDistance) < 0.0001f) break;

        minDistance = FLT_MAX;
        polytope.insert(polytope.begin() + minIndex, support);
    }

    intersection.penetrationDepth = minDistance;
    intersection.mtv = minNormal * minDistance * -1;
}

bool Collision::checkCollision(const Hitbox& hitbox1, const Hitbox& hitbox2, Intersection& intersection)
{
    std::vector<Vector2f> simplex;

    Vector2f currentDirection(-1, 0);

    simplex.push_back(minkowskiPoint(hitbox1, hitbox2, currentDirection));

    currentDirection = origin - simplex[0];

    while (true)
    {
        Vector2f pointA = minkowskiPoint(hitbox1, hitbox2, currentDirection);
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
        if (orthogonalAC.dot(lineAO) > 0)
        {
            currentDirection = orthogonalAC;
            simplex.erase(simplex.begin() + 1);
            continue;
        }

        expandingPolytopeAlgorithm(intersection, simplex, hitbox1, hitbox2);
        return true;
    }
}