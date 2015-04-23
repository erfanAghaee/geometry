#ifndef BOOST_GEOMETRY_PROJECTIONS_NSPER_HPP
#define BOOST_GEOMETRY_PROJECTIONS_NSPER_HPP

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

#include <boost/concept_check.hpp>
#include <boost/math/special_functions/hypot.hpp>

#include <boost/geometry/extensions/gis/projections/impl/base_static.hpp>
#include <boost/geometry/extensions/gis/projections/impl/base_dynamic.hpp>
#include <boost/geometry/extensions/gis/projections/impl/projects.hpp>
#include <boost/geometry/extensions/gis/projections/impl/factory_entry.hpp>

namespace boost { namespace geometry { namespace projections
{
    #ifndef DOXYGEN_NO_DETAIL
    namespace detail { namespace nsper{ 
            static const double EPS10 = 1.e-10;
            static const int N_POLE = 0;
            static const int S_POLE = 1;
            static const int EQUIT = 2;
            static const int OBLIQ = 3;

            struct par_nsper
            {
                double    height;
                double    sinph0;
                double    cosph0;
                double    p;
                double    rp;
                double    pn1;
                double    pfact;
                double    h;
                double    cg;
                double    sg;
                double    sw;
                double    cw;
                int        mode;
                int        tilt;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_nsper_spheroid : public base_t_fi<base_nsper_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_nsper m_proj_parm;

                inline base_nsper_spheroid(const Parameters& par)
                    : base_t_fi<base_nsper_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                    double  coslam, cosphi, sinphi;

                    sinphi = sin(lp_lat);
                    cosphi = cos(lp_lat);
                    coslam = cos(lp_lon);
                    switch (this->m_proj_parm.mode) {
                    case OBLIQ:
                        xy_y = this->m_proj_parm.sinph0 * sinphi + this->m_proj_parm.cosph0 * cosphi * coslam;
                        break;
                    case EQUIT:
                        xy_y = cosphi * coslam;
                        break;
                    case S_POLE:
                        xy_y = - sinphi;
                        break;
                    case N_POLE:
                        xy_y = sinphi;
                        break;
                    }
                    if (xy_y < this->m_proj_parm.rp) throw proj_exception();;
                    xy_y = this->m_proj_parm.pn1 / (this->m_proj_parm.p - xy_y);
                    xy_x = xy_y * cosphi * sin(lp_lon);
                    switch (this->m_proj_parm.mode) {
                    case OBLIQ:
                        xy_y *= (this->m_proj_parm.cosph0 * sinphi -
                           this->m_proj_parm.sinph0 * cosphi * coslam);
                        break;
                    case EQUIT:
                        xy_y *= sinphi;
                        break;
                    case N_POLE:
                        coslam = - coslam;
                    case S_POLE:
                        xy_y *= cosphi * coslam;
                        break;
                    }
                    if (this->m_proj_parm.tilt) {
                        double yt, ba;

                        yt = xy_y * this->m_proj_parm.cg + xy_x * this->m_proj_parm.sg;
                        ba = 1. / (yt * this->m_proj_parm.sw * this->m_proj_parm.h + this->m_proj_parm.cw);
                        xy_x = (xy_x * this->m_proj_parm.cg - xy_y * this->m_proj_parm.sg) * this->m_proj_parm.cw * ba;
                        xy_y = yt * ba;
                    }
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                    double  rh, cosz, sinz;

                    if (this->m_proj_parm.tilt) {
                        double bm, bq, yt;

                        yt = 1./(this->m_proj_parm.pn1 - xy_y * this->m_proj_parm.sw);
                        bm = this->m_proj_parm.pn1 * xy_x * yt;
                        bq = this->m_proj_parm.pn1 * xy_y * this->m_proj_parm.cw * yt;
                        xy_x = bm * this->m_proj_parm.cg + bq * this->m_proj_parm.sg;
                        xy_y = bq * this->m_proj_parm.cg - bm * this->m_proj_parm.sg;
                    }
                    rh = boost::math::hypot(xy_x, xy_y);
                    if ((sinz = 1. - rh * rh * this->m_proj_parm.pfact) < 0.) throw proj_exception();;
                    sinz = (this->m_proj_parm.p - sqrt(sinz)) / (this->m_proj_parm.pn1 / rh + rh / this->m_proj_parm.pn1);
                    cosz = sqrt(1. - sinz * sinz);
                    if (fabs(rh) <= EPS10) {
                        lp_lon = 0.;
                        lp_lat = this->m_par.phi0;
                    } else {
                        switch (this->m_proj_parm.mode) {
                        case OBLIQ:
                            lp_lat = asin(cosz * this->m_proj_parm.sinph0 + xy_y * sinz * this->m_proj_parm.cosph0 / rh);
                            xy_y = (cosz - this->m_proj_parm.sinph0 * sin(lp_lat)) * rh;
                            xy_x *= sinz * this->m_proj_parm.cosph0;
                            break;
                        case EQUIT:
                            lp_lat = asin(xy_y * sinz / rh);
                            xy_y = cosz * rh;
                            xy_x *= sinz;
                            break;
                        case N_POLE:
                            lp_lat = asin(cosz);
                            xy_y = -xy_y;
                            break;
                        case S_POLE:
                            lp_lat = - asin(cosz);
                            break;
                        }
                        lp_lon = atan2(xy_x, xy_y);
                    }
                }
            };

            template <typename Parameters>
            void setup(Parameters& par, par_nsper& proj_parm) 
            {
                boost::ignore_unused_variable_warning(par);
                boost::ignore_unused_variable_warning(proj_parm);
                if ((proj_parm.height = pj_param(par.params, "dh").f) <= 0.) throw proj_exception(-30);
                if (fabs(fabs(par.phi0) - HALFPI) < EPS10)
                    proj_parm.mode = par.phi0 < 0. ? S_POLE : N_POLE;
                else if (fabs(par.phi0) < EPS10)
                    proj_parm.mode = EQUIT;
                else {
                    proj_parm.mode = OBLIQ;
                    proj_parm.sinph0 = sin(par.phi0);
                    proj_parm.cosph0 = cos(par.phi0);
                }
                proj_parm.pn1 = proj_parm.height / par.a;
             /* normalize by radius */
                proj_parm.p = 1. + proj_parm.pn1;
                proj_parm.rp = 1. / proj_parm.p;
                proj_parm.h = 1. / proj_parm.pn1;
                proj_parm.pfact = (proj_parm.p + 1.) * proj_parm.h;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
                par.es = 0.;
            }


            // Near-sided perspective
            template <typename Parameters>
            void setup_nsper(Parameters& par, par_nsper& proj_parm)
            {
                proj_parm.tilt = 0;
                setup(par, proj_parm);
            }

            // Tilted perspective
            template <typename Parameters>
            void setup_tpers(Parameters& par, par_nsper& proj_parm)
            {
                double omega, gamma;
                omega = pj_param(par.params, "dtilt").f * DEG_TO_RAD;
                gamma = pj_param(par.params, "dazi").f * DEG_TO_RAD;
                proj_parm.tilt = 1;
                proj_parm.cg = cos(gamma);
             proj_parm.sg = sin(gamma);
                proj_parm.cw = cos(omega);
             proj_parm.sw = sin(omega);
                setup(par, proj_parm);
            }

        }} // namespace detail::nsper
    #endif // doxygen 

    /*!
        \brief Near-sided perspective projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Azimuthal
         - Spheroid
         - h=
        \par Example
        \image html ex_nsper.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct nsper_spheroid : public detail::nsper::base_nsper_spheroid<Geographic, Cartesian, Parameters>
    {
        inline nsper_spheroid(const Parameters& par) : detail::nsper::base_nsper_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::nsper::setup_nsper(this->m_par, this->m_proj_parm);
        }
    };

    /*!
        \brief Tilted perspective projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Azimuthal
         - Spheroid
         - tilt= azi= h=
        \par Example
        \image html ex_tpers.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct tpers_spheroid : public detail::nsper::base_nsper_spheroid<Geographic, Cartesian, Parameters>
    {
        inline tpers_spheroid(const Parameters& par) : detail::nsper::base_nsper_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            detail::nsper::setup_tpers(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_DETAIL
    namespace detail
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class nsper_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<nsper_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        class tpers_entry : public detail::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<tpers_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void nsper_init(detail::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("nsper", new nsper_entry<Geographic, Cartesian, Parameters>);
            factory.add_to_factory("tpers", new tpers_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace detail 
    #endif // doxygen

}}} // namespace boost::geometry::projections

#endif // BOOST_GEOMETRY_PROJECTIONS_NSPER_HPP

