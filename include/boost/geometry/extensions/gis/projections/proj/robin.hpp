#ifndef BOOST_GEOMETRY_PROJECTIONS_ROBIN_HPP
#define BOOST_GEOMETRY_PROJECTIONS_ROBIN_HPP

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
#include <boost/geometry/extensions/gis/projections/impl/function_overloads.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace robin{ 
            static const double FXC = 0.8487;
            static const double FYC = 1.3523;
            static const double C1 = 11.45915590261646417544;
            static const double RC1 = 0.08726646259971647884;
            static const int NODES = 18;
            static const double ONEEPS = 1.000001;
            static const double EPS = 1e-8;



            struct COEFS {
                double c0, c1, c2, c3;
            };

            static const struct COEFS X[] = {
                {1, 2.2199e-17, -7.15515e-05, 3.1103e-06},
                {0.9986, -0.000482243, -2.4897e-05, -1.3309e-06},
                {0.9954, -0.00083103, -4.48605e-05, -9.86701e-07},
                {0.99, -0.00135364, -5.9661e-05, 3.6777e-06},
                {0.9822, -0.00167442, -4.49547e-06, -5.72411e-06},
                {0.973, -0.00214868, -9.03571e-05, 1.8736e-08},
                {0.96, -0.00305085, -9.00761e-05, 1.64917e-06},
                {0.9427, -0.00382792, -6.53386e-05, -2.6154e-06},
                {0.9216, -0.00467746, -0.00010457, 4.81243e-06},
                {0.8962, -0.00536223, -3.23831e-05, -5.43432e-06},
                {0.8679, -0.00609363, -0.000113898, 3.32484e-06},
                {0.835, -0.00698325, -6.40253e-05, 9.34959e-07},
                {0.7986, -0.00755338, -5.00009e-05, 9.35324e-07},
                {0.7597, -0.00798324, -3.5971e-05, -2.27626e-06},
                {0.7186, -0.00851367, -7.01149e-05, -8.6303e-06},
                {0.6732, -0.00986209, -0.000199569, 1.91974e-05},
                {0.6213, -0.010418, 8.83923e-05, 6.24051e-06},
                {0.5722, -0.00906601, 0.000182, 6.24051e-06},
                {0.5322, -0.00677797, 0.000275608, 6.24051e-06}
            };
            static const struct COEFS Y[] = {
                {-5.20417e-18, 0.0124, 1.21431e-18, -8.45284e-11},
                {0.062, 0.0124, -1.26793e-09, 4.22642e-10},
                {0.124, 0.0124, 5.07171e-09, -1.60604e-09},
                {0.186, 0.0123999, -1.90189e-08, 6.00152e-09},
                {0.248, 0.0124002, 7.10039e-08, -2.24e-08},
                {0.31, 0.0123992, -2.64997e-07, 8.35986e-08},
                {0.372, 0.0124029, 9.88983e-07, -3.11994e-07},
                {0.434, 0.0123893, -3.69093e-06, -4.35621e-07},
                {0.4958, 0.0123198, -1.02252e-05, -3.45523e-07},
                {0.5571, 0.0121916, -1.54081e-05, -5.82288e-07},
                {0.6176, 0.0119938, -2.41424e-05, -5.25327e-07},
                {0.6769, 0.011713, -3.20223e-05, -5.16405e-07},
                {0.7346, 0.0113541, -3.97684e-05, -6.09052e-07},
                {0.7903, 0.0109107, -4.89042e-05, -1.04739e-06},
                {0.8435, 0.0103431, -6.4615e-05, -1.40374e-09},
                {0.8936, 0.00969686, -6.4636e-05, -8.547e-06},
                {0.9394, 0.00840947, -0.000192841, -4.2106e-06},
                {0.9761, 0.00616527, -0.000256, -4.2106e-06},
                {1, 0.00328947, -0.000319159, -4.2106e-06}
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_robin_spheroid : public base_t_fi<base_robin_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;


                inline base_robin_spheroid(const Parameters& par)
                    : base_t_fi<base_robin_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline double  V(COEFS const& C, double z) const
                { return (C.c0 + z * (C.c1 + z * (C.c2 + z * C.c3))); }
                inline double DV(COEFS const& C, double z) const
                { return (C.c1 + z * (C.c2 + C.c2 + z * 3. * C.c3)); }

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    int i;
                    double dphi;

                    i = int_floor((dphi = fabs(lp_lat)) * C1);
                    if (i >= NODES) i = NODES - 1;
                    dphi = RAD_TO_DEG * (dphi - RC1 * i);
                    xy_x = V(X[i], dphi) * FXC * lp_lon;
                    xy_y = V(Y[i], dphi) * FYC;
                    if (lp_lat < 0.) xy_y = -xy_y;
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    int i;
                    double t, t1;
                    struct COEFS T;

                    lp_lon = xy_x / FXC;
                    lp_lat = fabs(xy_y / FYC);
                    if (lp_lat >= 1.) { /* simple pathologic cases */
                        if (lp_lat > ONEEPS) throw proj_exception();
                        else {
                            lp_lat = xy_y < 0. ? -HALFPI : HALFPI;
                            lp_lon /= X[NODES].c0;
                        }
                    } else { /* general problem */
                        /* in Y space, reduce to table interval */
                        for (i = int_floor(lp_lat * NODES);;) {
                            if (Y[i].c0 > lp_lat) --i;
                            else if (Y[i+1].c0 <= lp_lat) ++i;
                            else break;
                        }
                        T = Y[i];
                        /* first guess, linear interp */
                        t = 5. * (lp_lat - T.c0)/(Y[i+1].c0 - T.c0);
                        /* make into root */
                        T.c0 -= lp_lat;
                        for (;;) { /* Newton-Raphson reduction */
                            t -= t1 = V(T,t) / DV(T,t);
                            if (fabs(t1) < EPS)
                                break;
                        }
                        lp_lat = (5 * i + t) * DEG_TO_RAD;
                        if (xy_y < 0.) lp_lat = -lp_lat;
                        lp_lon /= V(X[i], t);
                    }
                }
            };

            // Robinson
            template <typename Parameters>
            void setup_robin(Parameters& par)
            {
                par.es = 0.;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            }

        }} // namespace detail::robin
    #endif // doxygen 

    /*!
        \brief Robinson projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Pseudocylindrical
         - Spheroid
        \par Example
        \image html ex_robin.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct robin_spheroid : public detail::robin::base_robin_spheroid<Geographic, Cartesian, Parameters>
    {
        inline robin_spheroid(const Parameters& par) : detail::robin::base_robin_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::robin::setup_robin(this->m_par);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class robin_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<robin_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void robin_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("robin", new robin_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_ROBIN_HPP

