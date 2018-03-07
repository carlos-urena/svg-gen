// *********************************************************************
// **
// ** File: svgobjects.cpp
// ** Implementations for classes which output svg figures files.
// ** Copyright (C) 2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
//

#include <cmath>
#include "figures.hpp"

// *****************************************************************************

Figure1_HatBox::Figure1_HatBox()
{
  vec3 observador = vec3(0.4,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // elipse plana original  (no en esfera ni cilindro: no se dibuja)
  Ellipse * pe = new Ellipse( 64, vec3(1.0,1.3,0.0), vec3(0.0,0.4,0.0), vec3(0.0,0.0,0.2) );

  // esfera con relleno de gradiente radial
  Sphere * pes2D = new Sphere( vec3(0.0,0.0,0.0), 1.0 );

  // region
  SpherePolygon *    pesf  = new SpherePolygon( *pe );
  YCylinderPolygon * pcy   = new YCylinderPolygon( *pe );

  //añadir objetos
  const int cadacuantos = 4 ;

  objetos.add( new Axes( 0.010 ) );
  objetos.add( pes2D );
  objetos.add( new YAxisProjectorsSegments( cadacuantos, *pesf ) );
  objetos.add( pesf );
  objetos.add( new SegmentsVert( cadacuantos, *pesf, *pcy ) );
  objetos.add( pcy );
  objetos.add( new YAxisCylinder( cam ) );

}

// *****************************************************************************

Figure2_ParamCil::Figure2_ParamCil()
{
  vec3 observador = vec3(0.9,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // elipse plana tangente (perp. al eje Z, con eje mayor Y y eje menor X)
  const vec3 ejex = vec3(1.0,0.0,0.0),
             ejey = vec3(0.0,1.0,0.0),
             ejez = vec3(0.0,0.0,1.0);

  Ellipse * pe = new Ellipse( 64, vec3(0.0,0.0,1.0), real(0.6)*ejex, real(1.5)*ejey );

  // esfera con relleno de gradiente radial
  Sphere * pes2D = new Sphere( vec3(0.0,0.0,0.0), 1.0 );

  // region
  SpherePolygon *    pesf  = new SpherePolygon( *pe );
  YCylinderPolygon * pcy   = new YCylinderPolygon( *pe );

  //añadir objetos
  const int cadacuantos = 4 ;

  objetos.add( new Axes( 0.010 ));

  objetos.add( pes2D );
  //addYAxisProjectorsSegments( cadacuantos, *pesf );
  objetos.add( pesf );
  //addSegments( cadacuantos, *pesf, *pcy );
  objetos.add( pcy );

  objetos.add( new YAxisCylinder( cam ) );
}

// *****************************************************************************

Figure3_ParamConPol::Figure3_ParamConPol()
{
  vec3 observador = vec3(1.3,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // elipse plana tangente (perp. al eje Z, con eje mayor Y y eje menor X)
  const vec3 ejex = vec3(1.0,0.0,0.0),
             ejey = vec3(0.0,1.0,0.0),
             ejez = vec3(0.0,0.0,1.0);

  const int nump = 128 ;
  Ellipse * pe = new Ellipse( nump, vec3(0.0,0.0,1.0), real(0.6)*ejex, real(1.5)*ejey );

  // esfera con relleno de gradiente radial
  Sphere * pes2D = new Sphere( vec3(0.0,0.0,0.0), 1.0 );

  // region
  SpherePolygon *    pesf  = new SpherePolygon( *pe );
  ZCylinderPolygon * pcz   = new ZCylinderPolygon( *pe );

  //añadir objetos
  const int cadacuantos = 4 ;

  objetos.add( new Axes( 0.010 ) );

  objetos.add( pes2D );
  //addYAxisProjectorsSegments( cadacuantos, *pesf );
  objetos.add( pesf );
  //addSegments( cadacuantos, *pesf, *pcy );
  objetos.add( pcz );

  // circunferencia delantera del cilindro
  Ellipse * cd = new Ellipse( nump, vec3(0.0,0.0,1.0), ejex, ejey );
  cd->style.draw_filled = false ;
  cd->style.draw_lines = true ;
  cd->style.lines_color = vec3(1.0,0.0,0.0);
  cd->style.lines_width = 0.015 ;

  objetos.add( new JoiningQuads( *pcz, *cd ) );

  objetos.add( cd );

  // cilindro
  objetos.add( new ZAxisCylinder( cam ) );
}

// *****************************************************************************

Figure4_EllipseSectorZ::Figure4_EllipseSectorZ()
{
  vec3 observador = vec3(0.5,0.5,1.0), //vec3(0.9,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // elipse plana tangente (perp. al eje Z, con eje mayor Y y eje menor X)
  const vec3 ejex = vec3(1.0,0.0,0.0),
             ejey = vec3(0.0,1.0,0.0),
             ejez = vec3(0.0,0.0,1.0);

  Ellipse * pell = new Ellipse( 128, vec3(0.0,0.0,1.0), vec3(0.6,0.0,0.0), vec3(0.0,1.5,0.0));

  EllipseSectorZ * peZ = new EllipseSectorZ( 64, real(0.6), real(1.5) );

  // esfera con relleno de gradiente radial
  Sphere * pes2D = new Sphere( vec3(0.0,0.0,0.0), 1.0 );

  // region
  SpherePolygon *    pesf  = new SpherePolygon( *(peZ->ppol) );
  YCylinderPolygon * pcy   = new YCylinderPolygon( *(peZ->ppol) );

  // polilineas proyecciones de la elipse completas, no rellenas
  SpherePolygon *    pellesf  = new SpherePolygon( *pell );
  YCylinderPolygon * pellcy   = new YCylinderPolygon( *pell );
  pellesf->style.draw_filled = false ;
  pellcy->style.draw_filled = false ;

  // points
  YCylinderPoint * ppun0 = new YCylinderPoint( *(peZ->ppun0)),
                   * ppun1 = new YCylinderPoint( *(peZ->ppun1));


  ppun0->color = vec3( 0.0, 0.7, 0.0 );
  ppun1->color = vec3( 0.0, 0.7, 0.0 );
  ppun0->radius = 0.05 ;
  ppun1->radius = 0.05 ;

  // segmento verde entre los points:
  Segment * segmv = new Segment( ppun0->pos3D, ppun1->pos3D - ppun0->pos3D,
                                ppun0->color, 0.025 );

  //añadir objetos
  const int cadacuantos = 4 ;

  objetos.add( new Axes( 0.010 ));
  objetos.add( new YAxisCylinder( cam ) );

  objetos.add( pellesf );
  objetos.add( pellcy );
  objetos.add( pes2D );
  objetos.add( pesf );
  objetos.add( pcy );
  objetos.add( ppun0 );
  objetos.add( ppun1 );
  objetos.add( segmv );
}

// *****************************************************************************

Figure5_EllipseSectorRadial::Figure5_EllipseSectorRadial()
{
  vec3 observador = vec3(0.7,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // elipse plana tangente (perp. al eje Z, con eje mayor Y y eje menor X)
  const vec3 ejex = vec3(1.0,0.0,0.0),
             ejey = vec3(0.0,1.0,0.0),
             ejez = vec3(0.0,0.0,1.0);

  const int nump = 256 ;
  //Ellipse * pe = new Ellipse( nump, vec3(0.0,0.0,1.0), real(0.6)*ejex, real(1.5)*ejey );
  const real lonx = 0.8 , lony = 3.0 ;

  Ellipse *             pe    = new Ellipse( nump, vec3(0.0,0.0,1.0), lonx*ejex, lony*ejey );
  EllipseSectorRadial * pser  = new EllipseSectorRadial( nump*3, lonx, lony );
  Sphere *              pes2D = new Sphere( vec3(0.0,0.0,0.0), 1.0 );
  SpherePolygon *       pesf  = new SpherePolygon( *pe );
  ZCylinderPolygon *    pcz   = new ZCylinderPolygon( *pe );

  pesf->style.draw_filled = false ;
  pcz->style.draw_filled = false ;

  //
  SpherePolygon *    pseresf  = new SpherePolygon( *(pser->ppol) );
  ZCylinderPolygonWithSector * psercil
       = new ZCylinderPolygonWithSector( *(pser->ppol), pser->alpha0, pser->alpha1, pser->lonX, pser->lonY );

  pseresf->style.draw_filled = true ;
  psercil->style.draw_filled = true ;

  // circunferencia delantera del cilindro
  Ellipse * cd = new Ellipse( nump, vec3(0.0,0.0,1.0), ejex, ejey );
  cd->style.draw_filled = false ;
  cd->style.draw_lines  = true ;
  cd->style.lines_color = vec3(1.0,0.0,0.0);
  cd->style.lines_width = 0.015 ;

  // points, sacados del sector de elipse radial
  Point * ppoint0 = new Point( pser->point0, vec3(0.0,0.7,0.0) ),
        * ppoint1 = new Point( pser->point1, vec3(0.0,0.7,0.0) );

  ppoint0->radius = 0.05 ;
  ppoint1->radius = 0.05 ;

  // segmento verde entre los points:
  Segment * segmv = new Segment( ppoint0->pos3D, ppoint1->pos3D - ppoint0->pos3D,
                                ppoint0->color, 0.025 );

  //añadir objetos
  const int cadacuantos = 4 ;

  objetos.add( new Axes( 0.010 ) );
  objetos.add( new ZAxisCylinder( cam ) );
  objetos.add( pes2D );
  objetos.add( pesf );
  objetos.add( pcz );
  objetos.add( cd );
  objetos.add( pseresf );
  objetos.add( psercil );
  objetos.add( ppoint0 );
  objetos.add( ppoint1 );
  objetos.add( segmv );

  // cilindro

}

// *****************************************************************************

Figure6_SuplPara::Figure6_SuplPara()
{
  vec3 observador = vec3(-0.6,0.5,1.0),
       lookat     = vec3(0.0,0.0,0.0),
       vup        = vec3(0.0,1.0,0.0);

  cam = Camera( lookat, observador, vup );

  // longitud de los ejes menor (bt) y mayor (at) de la elipse tangente (bt<=at)
  // (bt en el eje X, y at en el eje Y)
  constexpr real bt = 0.7, at = 1.3 ;
  constexpr real bt2 = bt*bt, at2 = at*at ;

  // elipse plana tangente (perp. al eje Z, con eje mayor Y y eje menor X)
  const vec3 org  = vec3(0.0,0.0,0.0),
             ejex = vec3(1.0,0.0,0.0),
             ejey = vec3(0.0,1.0,0.0),
             ejez = vec3(0.0,0.0,1.0);

  const int nump = 256 ;
  const real lonx = 0.8 , lony = 3.0 ;

  // color de la elipse tangente
  vec3 colEllTan = vec3( 0.0,0.6,0.0 );

  Sphere *           pEsfera      = new Sphere( vec3(0.0,0.0,0.0), 1.0 );
  Ellipse *          pEllipseTan  = new Ellipse( nump, ejez, bt*ejex, at*ejey );
  SpherePolygon *    pEllipseEsf  = new SpherePolygon( *pEllipseTan );
  YCylinderPolygon * pEllipseCilY = new YCylinderPolygon( *pEllipseTan );
  Ellipse *          pcd          = new Ellipse( nump, vec3(0.0,1.0,0.0), ejex, ejez );

  // style de la elipse tangente
  pEllipseTan->style.lines_width = 0.015 ;
  pEllipseTan->style.lines_color = colEllTan ;

  // style de la circunferencia superior del cilindro

  pcd->style.draw_filled = false ;
  pcd->style.draw_lines = true ;
  pcd->style.lines_color = vec3(1.0,0.0,0.0);
  pcd->style.lines_width = 0.015 ;


  // segmentos de los ejes mayor y menor de la elipse tangente
Point * pbt0 = new Point( vec3(-bt,0.0,1.0), colEllTan ),
      * pbt1 = new Point( vec3(+bt,0.0,1.0), colEllTan ),
      * pat0 = new Point( vec3(0.0,-at,1.0), colEllTan ),
      * pat1 = new Point( vec3(0.0,+at,1.0), colEllTan );

  const float widthatbt = 0.008 ;

  Segment * segbt = new Segment( *pbt0, *pbt1, widthatbt ),
          * segat = new Segment( *pat0, *pat1, widthatbt );

  // points t0 y t1, s0 y s1
  const real x  = 0.7*bt ,
             x2 = x*x ,
             y  = at*real(sqrt(1.0-x2/bt2));

  const vec3 colpt0t1 = pEllipseTan->style.lines_color ;
  const vec3 vpt0     = vec3(x,-y,1.0),
             vptm     = vec3(x,0.0,1.0),
             vpt1     = vec3(x,+y,1.0);

  Point * pt0 = new Point( vpt0, colpt0t1 ),
        * ptm = new Point( vptm, colpt0t1 ),
        * pt1 = new Point( vpt1, colpt0t1 );

  Segment * segpt0pt1 = new Segment( *pt0, *pt1, widthatbt );

  const vec3 vps0 = vpt0.normalized(),
             vps1 = vpt1.normalized();

  Point * ps0 = new Point( vps0, pEllipseEsf->style.lines_color ),
        * ps1 = new Point( vps1, pEllipseEsf->style.lines_color  );



  //Segment * segpt0pt1 = new Segment( *pt0, *pt1, widthatbt );

  Segment * rads0t0 = new Segment( org, vpt0 ),
           * rads1t1 = new Segment( org, vpt1 );

  Segment * segm = new Segment( org, ptm->pos3D ),
           * segx = new Segment( ejez, ptm->pos3D );

  Segment * segt1 = new  Segment( vpt1, vec3(0.0,vpt1[1],0.0));
  segt1->style.dashed_lines = true ;
  segt1->style.lines_color = vec3( 0.7,0.7,0.7 );
  segt1->style.lines_width = 0.004 ;

  Segment * segs1 = new  Segment( vps1, vec3(0.0,vps1[1],0.0));
  segs1->style.dashed_lines = true ;
  segs1->style.lines_color = vec3( 0.7,0.7,0.7 );
  segs1->style.lines_width = 0.004 ;

  segx->style.lines_color = vec3(1.0,0.0,0.0);

  //añadir objetos

  objetos.add( new Axes( 0.004 ) );
  //objetos.add( new YAxisCylinder( cam ) );
  objetos.add( pEsfera );

  objetos.add( pEllipseTan );

  objetos.add( segbt );
  objetos.add( segat );

  objetos.add( segpt0pt1 );
  objetos.add( segm );
  objetos.add( segx );
  objetos.add( rads0t0 );
  objetos.add( rads1t1 );
  objetos.add( segt1 );
  objetos.add( segs1 );

  objetos.add( pt0 );
  objetos.add( ptm );
  objetos.add( pt1 );
  objetos.add( ps0 );
  objetos.add( ps1 );

}

// ***********************************************************************

void FigurePSA_Base::initialize(  )
{

   const vec3 org    = vec3(0.0,0.0,0.0),
              ejex   = vec3(1.0,0.0,0.0),
              ejey   = vec3(0.0,1.0,0.0),
              ejez   = vec3(0.0,0.0,1.0),
              observ = vec3(1.2,0.6,1.0),
              lookat = vec3(0.0,0.0,0.0),
              vup    = vec3(0.0,1.0,0.0);

   cam = Camera( lookat, observ, vup );


   // create objects
   auto * sphere_cap    = new SpherePolygon( *original_pol, true );
   auto * hor_plane_pol = new HorPlanePolygon( *sphere_cap, true );
   auto * hemisphere    = new Hemisphere( org, 1.0, (observ-lookat).normalized() ) ;

   sphere_cap->style.use_grad_fill = true ;
   sphere_cap->style.fill_opacity = 0.5 ;
   sphere_cap->style.grad_fill_name = "spherecapGradFill" ;

   auto * dashed_ellipse = new HorPlanePolygon( *original_pol, false );
   dashed_ellipse->style.draw_filled = false ;
   dashed_ellipse->style.draw_lines = true ;
   dashed_ellipse->style.dashed_lines = true ;
   objetos.add( dashed_ellipse );


   // add objects to set

   if ( draw_projectors )
   {
      auto * projectors    = new ExtrVertSegm( *sphere_cap, *hor_plane_pol, cam );
      objetos.add( projectors );
   }

   objetos.add( hemisphere );
   objetos.add( sphere_cap );
   objetos.add( hor_plane_pol );


}
// ***********************************************************************


Figure7_PSA_shape::Figure7_PSA_shape()
{
   // configure base class

   disk_center = vec3( 3.0, 3.0, 0.0 );
   disk_radius = 2.2 ;
   draw_projectors = true ;

   // create original disk
   const vec3
      disk_center_norm = disk_center.normalized(),
      disk_axis_1      = disk_center.cross( {0.0,1.0,0.0} ).normalized(),
      disk_axis_2      = disk_axis_1.cross( disk_center_norm  ).normalized() ;

   original_pol = new Ellipse( 256, disk_center, disk_radius*disk_axis_1,
                                                 disk_radius*disk_axis_2 );

   // initilize base class
   initialize();
}

// ***********************************************************************


Figure8_PSA_ellipse::Figure8_PSA_ellipse()
{
   // configure base class

   disk_center = vec3( 3.0, 3.5, 0.0 );
   disk_radius = 3.0 ;
   draw_projectors = true ;

   // create original disk
   const vec3
      disk_center_norm = disk_center.normalized(),
      disk_axis_1      = disk_center.cross( {0.0,1.0,0.0} ).normalized(),
      disk_axis_2      = disk_axis_1.cross( disk_center_norm  ).normalized() ;

   original_pol = new Ellipse( 256, disk_center, disk_radius*disk_axis_1,
                                                 disk_radius*disk_axis_2 );

   // initilize base class
   initialize();
}

// ***********************************************************************


Figure9_PSA_ellipse_lune::Figure9_PSA_ellipse_lune()
{
   // configure base class

   disk_center = vec3( 3.0, 1.0, 0.0 );
   disk_radius = 5.0 ;
   draw_projectors = true ;

   // create original disk
   const vec3
      disk_center_norm = disk_center.normalized(),
      disk_axis_1      = disk_center.cross( {0.0,1.0,0.0} ).normalized(),
      disk_axis_2      = disk_axis_1.cross( disk_center_norm  ).normalized() ;

   original_pol = new Ellipse( 256, disk_center, disk_radius*disk_axis_1,
                                                 disk_radius*disk_axis_2 );

   // initilize base class
   initialize();
}

// ***********************************************************************


Figure10_PSA_lune::Figure10_PSA_lune()
{
   // configure base class

   disk_center = vec3( 3.0, -1.0, 0.0 );
   disk_radius = 5.0 ;
   draw_projectors = false ;

   // create original disk
   const vec3
      disk_center_norm = disk_center.normalized(),
      disk_axis_1      = disk_center.cross( {0.0,1.0,0.0} ).normalized(),
      disk_axis_2      = disk_axis_1.cross( disk_center_norm  ).normalized() ;

   original_pol = new Ellipse( 256, disk_center, disk_radius*disk_axis_1,
                                                 disk_radius*disk_axis_2 );

   // initilize base class
   initialize();
}
