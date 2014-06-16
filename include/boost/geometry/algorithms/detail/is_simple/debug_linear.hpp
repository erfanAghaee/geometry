// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2014, Oracle and/or its affiliates.

// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_IS_SIMPLE_DEBUG_LINEAR_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_IS_SIMPLE_DEBUG_LINEAR_HPP

#ifdef GEOMETRY_TEST_DEBUG
#include <algorithm>
#include <iostream>
#include <vector>

#include <boost/range.hpp>

#include <boost/geometry/core/point_type.hpp>

#include <boost/geometry/util/range.hpp>

#include <boost/geometry/io/dsv/write.hpp>

#include <boost/geometry/policies/compare.hpp>

#include <boost/geometry/algorithms/equals.hpp>
#include <boost/geometry/algorithms/detail/overlay/debug_turn_info.hpp>
#endif


namespace boost { namespace geometry
{

namespace detail { namespace is_simple
{


template <typename TurnIterator>
inline void debug_print_turns(TurnIterator first, TurnIterator beyond)
{
#ifdef GEOMETRY_TEST_DEBUG
    std::cout << "turns:";
    for (TurnIterator tit = first; tit != beyond; ++tit)
    {
        std::cout << " ["
                  << geometry::method_char(tit->method)
                  << ","
                  << geometry::operation_char(tit->operations[0].operation)
                  << "/"
                  << geometry::operation_char(tit->operations[1].operation)
                  << " "
                  << geometry::dsv(tit->point)
                  << "] ";
    }
    std::cout << std::endl << std::endl;
#endif // GEOMETRY_TEST_DEBUG
}


template <typename MultiLinestring>
inline void debug_print_boundary_points(MultiLinestring const& multilinestring)
{
#ifdef GEOMETRY_TEST_DEBUG
    typedef typename point_type<MultiLinestring>::type point_type;
    typedef std::vector<point_type> point_vector;

    point_vector boundary_points;
    for (typename boost::range_iterator<MultiLinestring const>::type it
             = boost::begin(multilinestring);
         it != boost::end(multilinestring); ++it)
    {
        if ( boost::size(*it) > 1
             && !geometry::equals(range::front(*it), range::back(*it)) )
        {
            boundary_points.push_back( range::front(*it) );
            boundary_points.push_back( range::back(*it) );
        }
    }

    std::sort(boundary_points.begin(), boundary_points.end(),
              geometry::less<point_type>());

    std::cout << "boundary points: ";
    for (typename point_vector::const_iterator pit = boundary_points.begin();
         pit != boundary_points.end(); ++pit)
    {
        std::cout << " " << geometry::dsv(*pit);
    }
    std::cout << std::endl << std::endl;
#endif // GEOMETRY_TEST_DEBUG
}


}} // namespace detail::is_simple

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_IS_SIMPLE_DEBUG_LINEAR_HPP
