#ifndef BOOST_GEOMETRY_PROJECTIONS_LCCA_HPP
#define BOOST_GEOMETRY_PROJECTIONS_LCCA_HPP

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
#include <boost/geometry/extensions/gis/projections/impl/pj_mlfn.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace lcca{ 
            static const int MAX_ITER = 10;
            static const double DEL_TOL = 1e-12;

            struct par_lcca
            {
                double    en[EN_SIZE];
                double    r0, l, M0;
                double    C;
            };


                inline double /* func to compute dr */
            fS(double S, double C) {
                    return(S * ( 1. + S * S * C));
            }
                inline double /* deriv of fs */
            fSp(double S, double C) {
                return(1. + 3.* S * S * C);
            }

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_lcca_ellipsoid : public base_t_fi<base_lcca_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_lcca m_proj_parm;

                inline base_lcca_ellipsoid(const Parameters& par)
                    : base_t_fi<base_lcca_ellipsoid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double S, r, dr;

                    S = pj_mlfn(lp_lat, sin(lp_lat), cos(lp_lat), this->m_proj_parm.en) - this->m_proj_parm.M0;
                    dr = fS(S, this->m_proj_parm.C);
                    r = this->m_proj_parm.r0 - dr;
                    xy_x = this->m_par.k0 * (r * sin( lp_lon *= this->m_proj_parm.l ) );
                    xy_y = this->m_par.k0 * (this->m_proj_parm.r0 - r * cos(lp_lon) );
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double theta, dr, S, dif;
                    int i;

                    xy_x /= this->m_par.k0;
                    xy_y /= this->m_par.k0;
                    theta = atan2(xy_x , this->m_proj_parm.r0 - xy_y);
                    dr = xy_y - xy_x * tan(0.5 * theta);
                    lp_lon = theta / this->m_proj_parm.l;
                    S = dr;
                    for (i = MAX_ITER; i ; --i) {
                        S -= (dif = (fS(S, this->m_proj_parm.C) - dr) / fSp(S, this->m_proj_parm.C));
                        if (fabs(dif) < DEL_TOL) break;
                    }
                    if (!i) throw proj_exception();
                    lp_lat = pj_inv_mlfn(S + this->m_proj_parm.M0, this->m_par.es, this->m_proj_parm.en);
                }
            };

            // Lambert Conformal Conic Alternative
            template <typename Parameters>
            void setup_lcca(Parameters& par, par_lcca& proj_parm)
            {
                double s2p0, N0, R0, tan0, tan20;
                boost::ignore_unused(tan20);
                    pj_enfn(par.es, proj_parm.en);
                if (!pj_param(par.params, "tlat_0").i) throw proj_exception(50);
                if (par.phi0 == 0.) throw proj_exception(51);
                proj_parm.l = sin(par.phi0);
                proj_parm.M0 = pj_mlfn(par.phi0, proj_parm.l, cos(par.phi0), proj_parm.en);
                s2p0 = proj_parm.l * proj_parm.l;
                R0 = 1. / (1. - par.es * s2p0);
                N0 = sqrt(R0);
                R0 *= par.one_es * N0;
                tan0 = tan(par.phi0);
                tan20 = tan0 * tan0;
                proj_parm.r0 = N0 / tan0;
                proj_parm.C = 1. / (6. * R0 * N0);
                // par.inv = e_inverse;
                // par.fwd = e_forward;
            }

        }} // namespace detail::lcca
    #endif // doxygen 

    /*!
        \brief Lambert Conformal Conic Alternative projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
         - Ellipsoid
         - lat_0=
        \par Example
        \image html ex_lcca.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct lcca_ellipsoid : public detail::lcca::base_lcca_ellipsoid<Geographic, Cartesian, Parameters>
    {
        inline lcca_ellipsoid(const Parameters& par) : detail::lcca::base_lcca_ellipsoid<Geographic, Cartesian, Parameters>(par)
        {
            detail::lcca::setup_lcca(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class lcca_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<lcca_ellipsoid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void lcca_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("lcca", new lcca_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_LCCA_HPP

