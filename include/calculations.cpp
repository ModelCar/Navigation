#include<iostream>
#include<iomanip>
#include<cmath>


// Source: // http://w...content-available-to-author-only...o.uk/scripts/latlong.html

static const double PI = 3.14159265358979323846, earthDiameterMeters = 6371.0 * 2 * 1000;

double degreeToRadian (const double degree) { return (degree * PI / 180); };
double radianToDegree (const double radian) { return (radian * 180 / PI); };

double CoordinatesToAngle (double latitude1,
                           const double longitude1,
                           double latitude2,
                           const double longitude2)
{
  const auto longitudeDifference = degreeToRadian(longitude2 - longitude1);
  latitude1 = degreeToRadian(latitude1);
  latitude2 = degreeToRadian(latitude2);

  using namespace std;
  const auto x = (cos(latitude1) * sin(latitude2)) -
                 (sin(latitude1) * cos(latitude2) * cos(longitudeDifference));
  const auto y = sin(longitudeDifference) * cos(latitude2);

  const auto degree = radianToDegree(atan2(y, x));
  return (degree >= 0)? degree : (degree + 360);
}

double CoordinatesToMeters (double latitude1,
                            double longitude1,
                            double latitude2,
                            double longitude2)
{
  latitude1 = degreeToRadian(latitude1);
  longitude1 = degreeToRadian(longitude1);
  latitude2 = degreeToRadian(latitude2);
  longitude2 = degreeToRadian(longitude2);

  using namespace std;
  auto x = sin((latitude2 - latitude1) / 2), y = sin((longitude2 - longitude1) / 2);
#if 1
  return earthDiameterMeters * asin(sqrt((x * x) + (cos(latitude1) * cos(latitude2) * y * y)));
#else
  auto value = (x * x) + (cos(latitude1) * cos(latitude2) * y * y);
  return earthDiameterMeters * atan2(sqrt(value), sqrt(1 - value));
#endif
}

std::pair<double,double> CoordinateToCoordinate (double latitude,
                                                 double longitude,
                                                 double angle,
                                                 double meters)
{
  latitude = degreeToRadian(latitude);
  longitude = degreeToRadian(longitude);
  angle = degreeToRadian(angle);
  meters *= 2 / earthDiameterMeters;

  using namespace std;
  pair<double,double> coordinate;

  coordinate.first = asin((sin(latitude) * cos(meters))
                        + (cos(latitude) * sin(meters) * cos(angle)));
  coordinate.second = longitude + atan2((sin(angle) * sin(meters) * cos(latitude)),
                                        cos(meters) - (sin(latitude) * sin(coordinate.first)));

  coordinate.first = radianToDegree(coordinate.first);
  coordinate.second = radianToDegree(coordinate.second);

  return coordinate;
}

