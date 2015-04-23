#ifndef BOOST_GEOMETRY_PROJECTIONS_AIRY_HPP
#define BOOST_GEOMETRY_PROJECTIONS_AIRY_HPP

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
    namespace detail { namespace airy{ 
            static const double EPS = 1.e-10;
            static const int N_POLE = 0;
            static const int S_POLE = 1;
            static const int EQUIT = 2;
            static const int OBLIQ = 3;

            struct par_airy
            {
                double    p_halfpi;
                double    sinph0;
                double    cosph0;
                double    Cb;
                int        mode;
                int        no_cut;    /* do not cut at hemisphere limit */
            };





            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_airy_spheroid : public base_t_f<base_airy_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_airy m_proj_parm;

                inline base_airy_spheroid(const Parameters& par)
                    : base_t_f<base_airy_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  sinlam, coslam, cosphi, sinphi, t, s, Krho, cosz;

                    sinlam = sin(lp_lon);
                    coslam = cos(lp_lon);
                    switch (this->m_proj_parm.mode) {
                    case EQUIT:
                    case OBLIQ:
                        sinphi = sin(lp_lat);
                        cosphi = cos(lp_lat);
                        cosz = cosphi * coslam;
                        if (this->m_proj_parm.mode == OBLIQ)
                            cosz = this->m_proj_parm.sinph0 * sinphi + this->m_proj_parm.cosph0 * cosz;
                        if (!this->m_proj_parm.no_cut && cosz < -EPS)
                            throw proj_exception();;
                        if (fabs(s = 1. - cosz) > EPS) {
                            t = 0.5 * (1. + cosz);
                            Krho = -log(t)/s - this->m_proj_parm.Cb / t;
                        } else
                            Krho = 0.5 - this->m_proj_parm.Cb;
                        xy_x = Krho * cosphi * sinlam;
                        if (this->m_proj_parm.mode == OBLIQ)
                            xy_y = Krho * (this->m_proj_parm.cosph0 * sinphi -
                                this->m_proj_parm.sinph0 * cosphi * coslam);
                        else
                            xy_y = Krho * sinphi;
                        break;
                    case S_POLE:
                    case N_POLE:
                        lp_lat = fabs(this->m_proj_parm.p_halfpi - lp_lat);
                        if (!this->m_proj_parm.no_cut && (lp_lat - EPS) > HALFPI)
                            throw proj_exception();;
                        if ((lp_lat *= 0.5) > EPS) {
                            t = tan(lp_lat);
                            Krho = -2.*(log(cos(lp_lat)) / t + t * this->m_proj_parm.Cb);
                            xy_x = Krho * sinlam;
                            xy_y = Krho * coslam;
                            if (this->m_proj_parm.mode == N_POLE)
                                xy_y = -xy_y;
                        } else
                            xy_x = xy_y = 0.;
                    }
                }
            };

            // Airy
            template <typename Parameters>
            void setup_airy(Parameters& par, par_airy& proj_parm)
            {
                double beta;
                proj_parm.no_cut = pj_param(par.params, "bno_cut").i;
                beta = 0.5 * (HALFPI - pj_param(par.params, "rlat_b").f);
                if (fabs(beta) < EPS)
                    proj_parm.Cb = -0.5;
                else {
                    proj_parm.Cb = 1./tan(beta);
                    proj_parm.Cb *= proj_parm.Cb * log(cos(beta));
                }
                if (fabs(fabs(par.phi0) - HALFPI) < EPS)
                    if (par.phi0 < 0.) {
                        proj_parm.p_halfpi = -HALFPI;
                        proj_parm.mode = S_POLE;
                    } else {
                        proj_parm.p_halfpi =  HALFPI;
                        proj_parm.mode = N_POLE;
                    }
                else {
                    if (fabs(par.phi0) < EPS)
                        proj_parm.mode = EQUIT;
                    else {
                        proj_parm.mode = OBLIQ;
                        proj_parm.sinph0 = sin(par.phi0);
                        proj_parm.cosph0 = cos(par.phi0);
                    }
                }
                // par.fwd = s_forward;
                par.es = 0.;
            }

        }} // namespace detail::airy
    #endif // doxygen 

    /*!
        \brief Airy projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Miscellaneous
         - Spheroid
         - no inverse
         - no_cut lat_b=
        \par Example
        \image html ex_airy.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct airy_spheroid : public detail::airy::base_airy_spheroid<Geographic, Cartesian, Parameters>
    {
        inline airy_spheroid(const Parameters& par) : detail::airy::base_airy_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::airy::setup_airy(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class airy_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_f<airy_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void airy_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("airy", new airy_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_AIRY_HPP

