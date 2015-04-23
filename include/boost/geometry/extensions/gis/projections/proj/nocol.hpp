#ifndef BOOST_GEOMETRY_PROJECTIONS_NOCOL_HPP
#define BOOST_GEOMETRY_PROJECTIONS_NOCOL_HPP

// Boost.Geometry - extensions-gis-projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright (c) 2008-2012 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Boost.Geometry by Barend Gehrels

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace nocol{ 
            static const double EPS = 1e-10;


            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_nocol_spheroid : public base_t_f<base_nocol_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_nocol_spheroid(const Parameters& par)
                    : base_t_f<base_nocol_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    if (fabs(lp_lon) < EPS) {
                        xy_x = 0;
                        xy_y = lp_lat;
                    } else if (fabs(lp_lat) < EPS) {
                        xy_x = lp_lon;
                        xy_y = 0.;
                    } else if (fabs(fabs(lp_lon) - HALFPI) < EPS) {
                        xy_x = lp_lon * cos(lp_lat);
                        xy_y = HALFPI * sin(lp_lat);
                    } else if (fabs(fabs(lp_lat) - HALFPI) < EPS) {
                        xy_x = 0;
                        xy_y = lp_lat;
                    } else {
                        double tb, c, d, m, n, r2, sp;

                        tb = HALFPI / lp_lon - lp_lon / HALFPI;
                        c = lp_lat / HALFPI;
                        d = (1 - c * c)/((sp = sin(lp_lat)) - c);
                        r2 = tb / d;
                        r2 *= r2;
                        m = (tb * sp / d - 0.5 * tb)/(1. + r2);
                        n = (sp / r2 + 0.5 * d)/(1. + 1./r2);
                        xy_x = cos(lp_lat);
                        xy_x = sqrt(m * m + xy_x * xy_x / (1. + r2));
                        xy_x = HALFPI * ( m + (lp_lon < 0. ? -xy_x : xy_x));
                        xy_y = sqrt(n * n - (sp * sp / r2 + d * sp - 1.) /
                            (1. + 1./r2));
                        xy_y = HALFPI * ( n + (lp_lat < 0. ? xy_y : -xy_y ));
                    }
                }
            };

            // Nicolosi Globular
            template <typename Parameters>
            void setup_nicol(Parameters& par)
            {
                par.es = 0.;
                // par.fwd = s_forward;
            }

        }} // namespace detail::nocol
    #endif // doxygen 

    /*!
        \brief Nicolosi Globular projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
         - no inverse
        \par Example
        \image html ex_nicol.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct nicol_spheroid : public detail::nocol::base_nocol_spheroid<Geographic, Cartesian, Parameters>
    {
        inline nicol_spheroid(const Parameters& par) : detail::nocol::base_nocol_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::nocol::setup_nicol(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class nicol_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_f<nicol_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void nocol_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("nicol", new nicol_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_NOCOL_HPP

