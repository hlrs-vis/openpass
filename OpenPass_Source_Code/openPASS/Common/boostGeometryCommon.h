#pragma once

#include <boost/geometry.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

typedef boost::geometry::model::d2::point_xy<double> point_t;
typedef boost::geometry::model::polygon<point_t> polygon_t;
typedef boost::geometry::model::box<point_t> box_t;

namespace bg = boost::geometry;
namespace bt = boost::geometry::strategy::transform;

constexpr double PI  = boost::math::constants::pi<double>();

///*!
///* \brief Convert degrees to radians
///* \param[in] angle in degree
///* \return angle in radian
///*/
inline double Deg2Rad(double degree){
    return degree * PI/180;
}

///*!
///* \brief Convert radian to degree
///* \param[in] angle in radian
///* \return angle in degree
///*/
inline double Rad2Deg(double rad){
    return rad * 180.0/PI;
}




