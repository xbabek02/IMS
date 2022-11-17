#include "distance.hpp"

int Distance::CountDistance(std::vector<int> p1, std::vector<int> p2)
{
    return round(sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2)));
}

int Distance::CountDistance2D(std::vector<int> p1, std::vector<int> p2)
{
    return round(sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2)));
}

bool Distance::InRadiusFrom2D(std::vector<int> p1, std::vector<int> p2, int radius)
{
    return CountDistance2D(p1, p2) <= radius;
}

bool Distance::InRadiusFrom(std::vector<int> p1, std::vector<int> p2, int radius)
{
    return CountDistance(p1, p2) <= radius;
}

int Distance::AngleOfTwoPoints(std::vector<int> p1, std::vector<int> p2)
{
    auto A = p1;
    auto B = p2;
    auto C = std::vector<int>({A[0], B[1]});

    std::vector<double> v1({static_cast<double>(B[0] - A[0]), static_cast<double>(B[1] - A[1])});
    std::vector<double> v2({static_cast<double>(C[0] - B[0]), static_cast<double>(C[1] - B[1])});

    double size1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2));
    double size2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2));

    double angle = asin(size2 / size1) * (180 / M_PI);

    // counting the rest of angle based on quadrants

    // first quadrant
    if (A[0] < B[0] && A[1] >= B[1])
    {
        angle = 90 - angle;
    }
    // second quadrant
    else if (A[0] >= B[0] && B[1] < A[1])
    {
        angle += 90;
    }
    // third quadrant
    else if (A[0] > B[0] && B[1] >= A[1])
    {
        angle = (90 - angle) + 180;
    }
    // fourth quadrant
    else if (A[0] <= B[0] && B[1] > A[1])
    {
        angle += 270;
    }
    else if (A[0] == B[0] && A[1] == B[1])
    {
        return Up;
    }
    else
    {
        // something unexpected happened
        throw;
    }
    return angle;
}

Directions Distance::GetBestDirection(const std::vector<int> &p1, const std::vector<int> &p2)
{
    auto A = p1;
    auto B = p2;
    auto C = std::vector<int>({A[0], B[1]});

    std::vector<double> v1({static_cast<double>(B[0] - A[0]), static_cast<double>(B[1] - A[1])});
    std::vector<double> v2({static_cast<double>(C[0] - B[0]), static_cast<double>(C[1] - B[1])});

    double size1 = sqrt(pow(v1[0], 2) + pow(v1[1], 2));
    double size2 = sqrt(pow(v2[0], 2) + pow(v2[1], 2));

    double angle = asin(size2 / size1) * (180 / M_PI);

    // counting the rest of angle based on quadrants

    // first quadrant
    if (A[0] < B[0] && A[1] >= B[1])
    {
        angle = 90 - angle;
    }
    // second quadrant
    else if (A[0] >= B[0] && B[1] < A[1])
    {
        angle += 90;
    }
    // third quadrant
    else if (A[0] > B[0] && B[1] >= A[1])
    {
        angle = (90 - angle) + 180;
    }
    // fourth quadrant
    else if (A[0] <= B[0] && B[1] > A[1])
    {
        angle += 270;
    }
    else if (A[0] == B[0] && A[1] == B[1])
    {
        return Up;
    }
    else
    {
        // something unexpected happened
        throw;
    }

    return AngleToDirection(angle);
}

Directions Distance::AngleToDirection(double angle)
{
    if ((angle > 337.5 && angle <= 360) || (angle >= 0 && angle <= 22.5))
        return Directions::Right;
    else if (angle > 22.5 && angle <= 67.5)
        return Directions::UpRight;
    else if (angle > 67.5 && angle <= 112.5)
        return Directions::Up;
    else if (angle > 112.5 && angle <= 157.5)
        return Directions::UpLeft;
    else if (angle > 157.5 && angle <= 202.5)
        return Directions::Left;
    else if (angle > 202.5 && angle <= 247.5)
        return Directions::DownLeft;
    else if (angle > 247.5 && angle <= 292.5)
        return Directions::Down;
    else if (angle > 292.5 && angle <= 337.5)
        return Directions::DownRight;
    else
        throw;
}

Directions Distance::BestPossibleFromCurrentDirection(Directions current, Directions desired)
{
    if (current == desired)
        return current;

    // getting the difference of directions
    int difference = current - desired;
    if (difference < 0)
        difference += 8;

    if (difference < 4)
        return static_cast<Directions>(((current - 1) + 8) % 8);

    else if (difference > 4)
        return static_cast<Directions>(((current + 1) + 8) % 8);

    else // ==
    {
        if ((rand() % 2) == 1)
        {
            return static_cast<Directions>(((current - 1) + 8) % 8);
        }
        else
        {
            return static_cast<Directions>(((current + 1) + 8) % 8);
        }
    }
}

std::vector<int> Distance::NewPointInDirection(Directions direction, std::vector<int> position, int times)
{
    auto vec = Distance::DirectionToVector(direction);
    position.at(0) += vec.at(0) * times;
    position.at(1) += vec.at(1) * times;

    return position;
}

std::vector<int> Distance::DirectionToVector(Directions direction)
{
    switch (direction)
    {
    case Up:
        return {0, -1, 0};
    case UpRight:
        return {1, -1, 0};
    case Right:
        return {1, 0, 0};
    case DownRight:
        return {1, 1, 0};
    case Down:
        return {0, 1, 0};
    case DownLeft:
        return {-1, 1, 0};
    case Left:
        return {-1, 0, 0};
    case UpLeft:
        return {-1, -1, 0};
    default:
        throw;
    }
}