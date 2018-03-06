// *********************************************************************
// **
// ** File: svgobjects.cpp
// ** Implementations for classes for 3D polygons an polylines in various shapers
// ** (mainly vertexes sequences, but some other, too)
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

#include "svgobjects.hpp"

// Aux functions

// -----------------------------------------------------------------------------

void write_radial_gradient_def( std::ostream & os, const std::string & name )
{
   using namespace std ;
   os << "<radialGradient id='" << name << "' cx='50%' cy='50%' r='50%' fx='25%' fy='75%'>" << endl
      << "   <stop offset='0%'   style='stop-color:rgb(100%,100%,100%); stop-opacity:0.2' />" << endl
      << "   <stop offset='100%' style='stop-color:rgb(20%,20%,20%);    stop-opacity:0.2' />" << endl
      << "</radialGradient>" << endl ;
}

// -----------------------------------------------------------------------------

void write_color( std::ostream & os, const vec3 & col )
{

  os << "rgb("
     << col[0]*100.0 << "%,"
     << col[1]*100.0 << "%,"
     << col[2]*100.0 << "%)"  ;
}

// -----------------------------------------------------------------------------

void write_stroke_color( std::ostream & os, const vec3 & col )
{

  os << "stroke='rgb("
     << col[0]*100.0 << "%,"
     << col[1]*100.0 << "%,"
     << col[2]*100.0 << "%)' " << std::endl ;
}
// -----------------------------------------------------------------------------

void write_coord2( std::ostream & os, const vec2 & p )
{
   using namespace std ;
   if ( isnan(p[0]) && isnan(p[1]) )
      cout << "warning: not a number found in vec2. Not written to SVG output file." << endl ;
   else
       os << p[0] << " " << p[1] ;
}

// *****************************************************************************
// class CamRefSys
// -----------------------------------------------------------------------------

CamRefSys::CamRefSys()
{
  origin = vec3( 0.0, 0.0, 0.0 );
  xAxis   = vec3( 1.0, 0.0, 0.0 );
  yAxis   = vec3( 0.0, 1.0, 0.0 );
  zAxis   = vec3( 0.0, 0.0, 1.0 );
}
// -----------------------------------------------------------------------------

CamRefSys::CamRefSys( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup )
{
  initialize( look_at, observer, p_vup );
}
// -----------------------------------------------------------------------------

void CamRefSys::initialize( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup )
{
  constexpr real epsilon = 1e-5 ;

  assert( epsilon <= (observer-look_at).length() ) ;

  origin = observer ;
  zAxis = (observer-look_at).normalized();
  xAxis = (p_vup.cross(zAxis)).normalized();  assert( fabs(xAxis.length()-1.0) < epsilon );
  yAxis = (zAxis.cross(xAxis)).normalized() ; assert( fabs(yAxis.length()-1.0) < epsilon );

  assert( xAxis.dot(yAxis) < epsilon );
  assert( yAxis.dot(zAxis) < epsilon );
  assert( zAxis.dot(xAxis) < epsilon );
}
// -----------------------------------------------------------------------------

vec2 CamRefSys::world2cam( const vec3 & p ) const
{
  const vec3 v = p-origin ;
  return vec2( v.dot(xAxis), v.dot(yAxis) ) ;

}

// *****************************************************************************
// class Camera
// -----------------------------------------------------------------------------

Camera::Camera()
{

}
// -----------------------------------------------------------------------------

Camera::Camera( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup )
{
  src.initialize( look_at, observer, p_vup );
}
// -----------------------------------------------------------------------------

vec2 Camera::project( const vec3 & p ) const   // por ahora, solo paralela
{
  return src.world2cam( p );
}

// *****************************************************************************
// class SVGContext
// -----------------------------------------------------------------------------

SVGContext::SVGContext()
{
  os = nullptr ;
}

// *****************************************************************************
// class PathStyle
// -----------------------------------------------------------------------------

PathStyle::PathStyle()
{
  draw_lines   = true ;
  draw_filled  = true ;
  close_lines  = true ;
  lines_color  = vec3(0.0,0.0,0.0);
  fill_color   = vec3(1.0,0.5,0.5);
  fill_opacity = 0.5 ;
  lines_width  = 0.05 ;
  dashed_lines = false ;

  use_grad_fill = false ;
  grad_fill_name = "no name" ;
}
// -----------------------------------------------------------------------------

void PathStyle::writeSVG( SVGContext & ctx )
{
  using namespace std ;
  std::ostream & os = *(ctx.os) ;

   os << "   style='" ;

   os << "fill:" ;
   if ( draw_filled )
   {
      if ( use_grad_fill )
      {
         if ( grad_fill_name == "no name" )
            cout << "WARNING: using 'no name' as grad fill name." << endl ;
         os << "url(#" << grad_fill_name << ")" ;
      }
      else
         write_color( os, fill_color ) ;
   }
   else
      os << "none" ;
   os << "; " ;

   if ( draw_filled )
      os << "fill-opacity:" << fill_opacity << "; " ;

   os << "stroke:" ;
   if ( draw_lines )
      write_color( os, lines_color ) ;
   else
      os << "none" ;
   os << "; " ;

   if ( draw_lines )
      os << "stroke-width:" << lines_width << "; " ;

   if ( draw_lines && dashed_lines )
      os << "stroke-dasharray:0.01,0.01; " ;

   os << "' " << endl ;

}

// *****************************************************************************
// class Object
// -----------------------------------------------------------------------------

Object::Object()
{
  projected = false ;
}
// -----------------------------------------------------------------------------

Object::~Object()
{

}

// *****************************************************************************
// class Puntos
// -----------------------------------------------------------------------------

Point::Point( vec3 ppos3D, vec3 pcolor )
{
  pos3D = ppos3D ;
  color = pcolor ;
  radius = 0.03 ;
}
// -----------------------------------------------------------------------------

void Point::drawSVG( SVGContext & ctx )
{
  assert( projected );
  using namespace std ;


  PathStyle e ;
  e.draw_lines   = false ;
  e.draw_filled  = true ;
  e.fill_color   = color ;
  e.fill_opacity = 1.0;

  assert( ctx.os != nullptr );
  std::ostream & os = *(ctx.os) ;

  os << "<circle cx='" << pos2D[0] << "' cy='" << pos2D[1]
     <<       "' r='" << radius << "' " ;
  e.writeSVG( ctx );
  os << "/>" << endl ;

}
// -----------------------------------------------------------------------------

void Point::project( const Camera & cam )
{
  pos2D     = cam.project( pos3D );
  min       = pos2D ;
  max       = pos2D ;
  projected = true ;
}

// *****************************************************************************
// class Polygon
// -----------------------------------------------------------------------------

Polygon::Polygon()
{

}
// -----------------------------------------------------------------------------

void Polygon::project( const Camera & cam )
{
  bool primero = true ;
  points2D.clear() ;

  for( const vec3 & p3 : points3D )
  {
    const vec2 p2 = cam.project( p3 );
    if ( primero )
    { min = p2 ;
      max = p2 ;
      primero = false ;
    }
    else
    { min[0] = std::min( min[0], p2[0] );
      min[1] = std::min( min[1], p2[1] );
      max[0] = std::max( max[0], p2[0] );
      max[1] = std::max( max[1], p2[1] );
    }
    points2D.push_back( p2 );
    //using namespace std ;
    //cout << "min ==" << min << ", max == " << max << endl ;
  }
  projected = true ;
}

Polygon::~Polygon()
{

}

// -----------------------------------------------------------------------------

void Polygon::drawSVG( SVGContext & ctx )
{
   using namespace std ;
   assert( ctx.os != nullptr );


   if ( points3D.size() == 0 )
   {
      cout << "WARNING: attempting to draw an empty Polygon object" << endl ;
      return ;
   }

  assert( projected );
  assert( points3D.size() == points2D.size() );

  using namespace std ;
  std::ostream & os = *(ctx.os) ;

  os << "<path " << endl
     << "   stroke-linecap='round' stroke-linejoin='round'" << endl ;

  style.writeSVG( ctx );

  os << "   d=' M " ;
  write_coord2( os, points2D[0] );
  for( unsigned i = 1 ; i < points2D.size() ; i++ )
  {
      os << " L " ;
      write_coord2( os, points2D[i] );
      os << " " ;
      //cout << points_proy[i] << endl ;
  }
  if ( style.close_lines )
    os << " Z" ;

  os << "'/>" << endl;

}

// *****************************************************************************
// class ConjuntoPuntos
// -----------------------------------------------------------------------------

ObjectsSet::ObjectsSet()
{

}

ObjectsSet::~ObjectsSet()
{

}
// -----------------------------------------------------------------------------

void ObjectsSet::add( Object * pobj )
{
  objetos.push_back( pobj );
  projected = false ;
}

// -----------------------------------------------------------------------------

void ObjectsSet::project( const Camera & cam )
{
   bool primero = true ;

   for( Object * pobjeto : objetos )
   {
      assert( pobjeto != nullptr );
      pobjeto->project( cam );

      if ( primero )
      {
         min = pobjeto->min ;
         max = pobjeto->max ;
         primero = false ;
   }
   else
   {
      min[0] = std::min( min[0], pobjeto->min[0] );
      min[1] = std::min( min[1], pobjeto->min[1] );
      max[0] = std::max( max[0], pobjeto->max[0] );
      max[1] = std::max( max[1], pobjeto->max[1] );
   }
   //using namespace std ;
   //cout << "min ==" << min << ", max == " << max << endl ;
  }
  projected = true ;
}

// -----------------------------------------------------------------------------

void ObjectsSet::drawSVG( SVGContext & ctx )
{
  for( Object * pobjeto : objetos )
  {
    assert( pobjeto != nullptr );
    pobjeto->drawSVG( ctx );
  }
}
// *****************************************************************************
// class Sphere : public Object
// -----------------------------------------------------------------------------

Sphere::Sphere( const vec3 & pcenter3D, real pradius3D )
{
  radius3D = pradius3D ;
  center3D = pcenter3D ;
}
// -----------------------------------------------------------------------------

void Sphere::project( const Camera & cam )
{
  center2D = cam.project( center3D );
  radius2D  = radius3D ; // this works fine only assuming parallel projection

  projected = true ;
  min = center2D-vec2(radius2D,radius2D);
  max = center2D+vec2(radius2D,radius2D);
}


// -----------------------------------------------------------------------------

void Sphere::drawSVG( SVGContext & ctx )
{
  assert( projected );

  using namespace std ;
  std::ostream & os = *(ctx.os) ;

  write_radial_gradient_def( os, "grad1" );

  os << "<circle " << endl
     << "   style='fill:url(#grad1); stroke:black; stroke-width:0.003'" << endl
     << "   cx='" << center2D[0] <<  "' cy='" << center2D[1] << "' r='" << radius2D << "'" << endl
     << "/>" << endl ;
}

// *****************************************************************************
// class Hemiphere : public Object
// -----------------------------------------------------------------------------

Hemisphere::Hemisphere( const vec3 & pcenter3D, real pradius3D, vec3 p_view_dir )
{


   radius3D = pradius3D ;
   center3D = pcenter3D ;
   view_dir = p_view_dir ;

   const vec3 view_dir_norm = view_dir.normalized(),
              proj_view_dir = vec3( view_dir[0], 0.0, view_dir[2] ),
              axisz         = proj_view_dir.normalized(),
              axisy         = vec3( 0.0, 1.0, 0.0 ),
              axisx         = vec3( -axisz[2], 0.0, axisz[0] ) ;

      auto * contour = new Polygon();

    // add points in the equator
    const int np = 128 ;

    // semicircunference (front side of the equator) (perp. to Y)
    for( int i = 0 ; i < np ; i++ )
    {
       const float angr = M_PI*float(i)/float(np);
       contour->points3D.push_back( center3D + radius3D*( -cos(angr)*axisx + sin(angr)*axisz )  );
    }


    // semicircunference perp. to Z

    const vec3
      axisy_rot = axisx.cross( view_dir_norm ).normalized(); // axisY, but perp. to view dir

    for( int i = 0 ; i < np ; i++ )
    {
       const float angr = M_PI*float(i)/float(np);
       contour->points3D.push_back( center3D + radius3D*(cos(angr)*axisx + sin(angr)*axisy_rot)  );
    }

    contour->style.draw_filled = true ;
    contour->style.use_grad_fill = true ;
    contour->style.grad_fill_name = "hemisphereGradFill" ;

    contour->style.draw_lines  = true ;
    contour->style.lines_width = 0.0035 ;
    contour->style.lines_color = vec3( 0.5, 0.5, 0.5 );
    contour->style.close_lines = true ;


   // back side of the equator (dashed) (perp to Y)

   auto * equator = new Polygon();

   // semicircunference at the equator (perp. to view_dir) (equalt to contour equator)
   for( int i = 0 ; i <= np ; i++ )
   {
      const float angr = M_PI*float(i)/float(np);
      equator->points3D.push_back( center3D + radius3D*( -cos(angr)*axisx - sin(angr)*axisz )  );
   }

   equator->style.draw_filled = false ;
   equator->style.draw_lines  = true ;
   equator->style.dashed_lines = true ; /// dashed!
   equator->style.lines_width = contour->style.lines_width ;
   equator->style.lines_color = contour->style.lines_color;
   equator->style.close_lines = false ;

   // axes

   add( new Axes( 0.005 ) );

   // add objects to this object set
   add( equator );
   add( contour );


}


// *****************************************************************************
// class Segment
// -----------------------------------------------------------------------------

Segment::Segment( const vec3 & p0, const vec3 & vd, const vec3 & color, real width )
{
  style.draw_lines   = true ;
  style.draw_filled  = false ;
  style.close_lines  = false ;
  style.lines_color  = color ;
  style.lines_width  = width ;

  points3D.push_back( p0 );
  points3D.push_back( p0+vd );
}

Segment::Segment( const vec3 & p0, const vec3 & p1 )
{
  style.draw_lines   = true ;
  style.draw_filled  = false ;
  style.close_lines  = false ;
  style.lines_color  = vec3(0.0,0.0,0.0) ;
  style.lines_width  = 0.007 ;

  points3D.push_back( p0 );
  points3D.push_back( p1 );
}

Segment::Segment( const Point & p0, const Point & p1, real width )
{
  style.draw_lines   = true ;
  style.draw_filled  = false ;
  style.close_lines  = false ;
  style.lines_color  = p0.color ;
  style.lines_width  = width ;

  points3D.push_back( p0.pos3D );
  points3D.push_back( p1.pos3D );
}

// *****************************************************************************
// class Ellipse
// an arbitrary ellipse, at any point, with any orientation

Ellipse::Ellipse( unsigned n, const vec3 & center, const vec3 & eje1, const vec3 eje2 )
{
  for( unsigned i= 0 ; i < n ; i++ )
  {
    const real ang = real(i)*real(2.0)*real(M_PI)/real(n) ,
               c   = cos(double(ang)),
               s   = sin(double(ang));

    points3D.push_back( center + c*eje1 + s*eje2 ) ;
  }
  style.draw_filled = false ;
  style.draw_lines  = true ;
  style.close_lines = true ;
  style.lines_width = 0.006 ;
  style.lines_color = vec3( 0.0, 1.0, 0.0 );
}

// *****************************************************************************
// class EllipseSectorZ
// An ellipse sector, perpendicular to Z axis, with center at (0,0,1)
// -----------------------------------------------------------------------------
constexpr double pi = 3.14159265358979323846 ;

EllipseSectorZ::EllipseSectorZ( unsigned n, real lonX, real lonY )
{
  const vec3 center = vec3(0.0,0.0,1.0),
              eje1  = vec3(lonX,0.0,0.0),
              eje2  = vec3(0.0,lonY,0.0) ;

  constexpr double alpha0 = 0.6*pi,
                   alpha1 = -alpha0 ;

  ppol = new Polygon();

  // arco de elipse
  for( unsigned i= 0 ; i < n ; i++ )
  {
    const real ang = alpha0 + (alpha1-alpha0)*real(i)/real(n) ;
    const real c = cos(double(ang)),
               s = sin(double(ang));
    ppol->points3D.push_back( center + c*eje1 + s*eje2 ) ;
  }
  // linea recta a trozos desde alpha1 y vuelta a alpha0
  const vec3 p0 = center+real(cos(alpha1))*eje1 + real(sin(alpha1))*eje2 ,
             p1 = center+real(cos(alpha0))*eje1 + real(sin(alpha0))*eje2 ;

  const unsigned nl = n/2 ;
  for( unsigned i= 0 ; i< nl ; i++ )
  {
    ppol->points3D.push_back( p0 + (real(i)/real(nl))*(p1-p0)) ;
  }
  // style del polígono
  ppol->style.draw_filled = false ;
  ppol->style.draw_lines = true ;
  ppol->style.close_lines = true ;
  ppol->style.lines_width = 0.006 ;
  ppol->style.lines_color  = vec3( 0.0, 1.0, 0.0 );

  // create points
  const vec3 colpoint = vec3(0.0, 1.0, 0.0);
  ppun0 = new Point( p0, colpoint );
  ppun1 = new Point( p1, colpoint );

  // add objects
  add( ppol );
  add( ppun0 );
  add( ppun1 );

}
// *****************************************************************************
// class EllipseSectorRadial
// un sector de una elipse, perpendicular a Z y con center en (0,0,1)
// -----------------------------------------------------------------------------

EllipseSectorRadial::EllipseSectorRadial( unsigned n, real plonX, real plonY )
{


  // initialize parámetros de este sector
  alpha0 = 0.0;
  alpha1 = pi*0.5*0.7 ;
  lonX = plonX ;
  lonY = plonY ;

  const vec3 center = vec3(0.0,0.0,1.0),
             eje1   = vec3(lonX,0.0,0.0),
             eje2   = vec3(0.0,lonY,0.0) ;

  // crear poligono
  ppol = new Polygon();

  // extremo del radius en alpha0
  const real c0 = real(cos(alpha0)),
             s0 = real(sin(alpha0));
  vec3 extremo0 = center + c0*eje1 + s0*eje2 ;

  // extremo del radius en alpha1
  const real c1 = real(cos(alpha1)),
             s1 = real(sin(alpha1));
  vec3 extremo1 = center + c1*eje1 + s1*eje2 ;

  // radius desde center (incluido) hasta extremo0 (sin incluir)
  const int nprad = n ;
  for( unsigned i = 0 ; i < nprad ; i++ )
     ppol->points3D.push_back( center + real(i)/real(nprad)*extremo0 );


  // arco de elipse (desde extremo0, incluido, hasta extremo1, sin incluir)
  for( unsigned i= 0 ; i < n ; i++ )
  {
    const real ang = alpha0 + (alpha1-alpha0)*real(i)/real(n) ;
    const real c = cos(double(ang)),
               s = sin(double(ang));
    ppol->points3D.push_back( center + c*eje1 + s*eje2 ) ;
  }

  // radius desde extremo1 (incluido) hasta center (sin incluir)
  for( unsigned i = nprad ; i > 0 ; i-- )
    ppol->points3D.push_back( center + real(i)/real(nprad)*extremo1 );


  // style del polígono (da igual pq luego esto no se dibuja: se dibujan sus proyecciones)
  ppol->style.draw_filled = false ;
  ppol->style.draw_lines = true ;
  ppol->style.close_lines = true ;
  ppol->style.lines_width = 0.006 ;
  ppol->style.lines_color  = vec3( 0.0, 1.0, 0.0 );

  // points
  const vec3 colpoint = vec3(0.0, 1.0, 0.0);
  //ppun0 = new Point( p0, colpoint );
  //ppun1 = new Point( p1, colpoint );

  // añadir objetos
  add( ppol );

  // añadir las posiciones del point0 y el point1 (extremos del segmento en alpha1)

  const vec3 extremo1esf = extremo1.normalized();
  real r = real(sqrt(extremo1esf[0]*extremo1esf[0]+extremo1esf[1]*extremo1esf[1]));
  point0 = vec3( extremo1esf[0]/r, extremo1esf[1]/r, extremo1esf[2] );
  point1 = vec3( point0[0], point0[1], 1.0 );
}


// *****************************************************************************
// class PolQuad
// -----------------------------------------------------------------------------

PolQuad::PolQuad( const vec3 & p00, const vec3 & p01, const vec3 & p10, const vec3 & p11 )
{
  points3D.push_back( p00 );
  points3D.push_back( p01 );
  points3D.push_back( p11 );
  points3D.push_back( p10 );

  style.draw_lines    = false ;
  style.close_lines = false ;
  style.draw_filled     = true ;
  style.fill_opacity = 0.2 ;
  style.fill_color    = vec3( 1.0, 0.0, 0.0 );
}

// *****************************************************************************
// class SegmentsVert
// -----------------------------------------------------------------------------

SegmentsVert::SegmentsVert( unsigned dn, const Polygon & p1, const Polygon & p2 )
{
  const int n = p1.points3D.size() ;
  assert( n == p2.points3D.size() );

  const vec3 color = vec3( 0.0, 0.7, 1.0 );
  constexpr real width =  0.0025 ;

  for( unsigned i = 0 ; i < n ; i += dn )
  {
      Segment * psegmento = new Segment( p1.points3D[i], p2.points3D[i]-p1.points3D[i], color, width );
      add( psegmento );
  }
}

// *****************************************************************************
// class Axes
// -----------------------------------------------------------------------------

Axes::Axes( real widthl )  // widthl = width de línea
{
  vec3 o = vec3(0.0,0.0,0.0);

  add( new Segment( o, vec3(1.0,0.0,0.0), vec3(1.0,0.0,0.0), widthl ) );
  add( new Segment( o, vec3(0.0,1.0,0.0), vec3(0.0,0.5,0.0), widthl ) );
  add( new Segment( o, vec3(0.0,0.0,1.0), vec3(0.0,0.0,1.0), widthl ) );
}

// *****************************************************************************
// class YAxisProjectorsSegments
// -----------------------------------------------------------------------------

YAxisProjectorsSegments::YAxisProjectorsSegments( unsigned dn, const Polygon & p1 )
{
  const int n = p1.points3D.size() ;
  assert( 0 < n );
  const vec3 color = vec3( 0.0, 0.7, 1.0 );
  constexpr real width =  0.0025 ;

  for( unsigned i = 0 ; i < n ; i += dn )
  {
      const vec3 pesf = p1.points3D[i] ,
                 pyAxis = vec3(0.0,pesf[1],0.0);

      add( new Segment( pyAxis, pesf-pyAxis, color, width ));
  }
}
// *****************************************************************************
// class YAxisCylinder
// -----------------------------------------------------------------------------

YAxisCylinder::YAxisCylinder( const Camera & cam )
{

  // circunferencia inferior
  Ellipse * pcinf = new Ellipse( 64, vec3(0.0,-1.0,0.0), vec3(1.0,0.0,0.0), vec3(0.0,0.0,1.0));
  pcinf->style.draw_filled = false ;
  pcinf->style.lines_color = vec3(1.0,0.0,0.0) ;

  // circunferencia ecuador
  Ellipse * pcecu = new Ellipse( 64, vec3(0.0,0.0,0.0), vec3(1.0,0.0,0.0), vec3(0.0,0.0,1.0));
  pcecu->style.draw_filled = false ;
  pcecu->style.close_lines = true ;
  pcecu->style.lines_color = vec3(0.4,0.4,0.4) ;
  pcecu->style.lines_width = 0.002 ;

  // circunferencia superior
  Ellipse * pcsup = new Ellipse( 64, vec3(0.0,1.0,0.0), vec3(1.0,0.0,0.0), vec3(0.0,0.0,1.0));
  pcsup->style.draw_filled = false ;
  pcsup->style.lines_color = vec3(1.0,0.0,0.0) ;

  // segmentos verticales que delimitan el cilindro
  const vec3 v  = cam.src.zAxis.normalized() ;
  const real s  = sqrt(v[0]*v[0]+v[2]*v[2]);
  const vec3 n1 = vec3(  v[2]/s, -1.0, -v[0]/s ),
             n2 = vec3( -v[2]/s, -1.0,  v[0]/s );
  Segment
    * s1 = new Segment( n1, vec3(0.0,2.0,0.0), vec3(1.0,0.0,0.0), pcinf->style.lines_width ),
    * s2 = new Segment( n2, vec3(0.0,2.0,0.0), vec3(1.0,0.0,0.0), pcinf->style.lines_width );

    add( pcinf );
    add( pcecu );
    add( pcsup );
    add( s1 );
    add( s2 );
}

// *****************************************************************************
// class ZAxisCylinder
// -----------------------------------------------------------------------------

ZAxisCylinder::ZAxisCylinder( const Camera & cam )
{

  // circunferencia inferior
  Ellipse * pcinf = new Ellipse( 64, vec3(0.0,0.0,-1.0), vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0));
  pcinf->style.draw_filled = false ;
  pcinf->style.lines_color = vec3(1.0,0.0,0.0) ;

  // circunferencia ecuador
  Ellipse * pcecu = new Ellipse( 64, vec3(0.0,0.0,0.0), vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0));
  pcecu->style.draw_filled = false ;
  pcecu->style.close_lines = true ;
  pcecu->style.lines_color = vec3(0.4,0.4,0.4) ;
  pcecu->style.lines_width = 0.002 ;

  // circunferencia superior
  Ellipse * pcsup = new Ellipse( 64, vec3(0.0,0.0,1.0), vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0));
  pcsup->style.draw_filled = false ;
  pcsup->style.lines_color = vec3(1.0,0.0,0.0) ;

  // segmentos verticales que delimitan el cilindro
  const vec3 v  = cam.src.zAxis.normalized() ;
  const real s  = sqrt(v[0]*v[0]+v[1]*v[1]);
  const vec3 n1 = vec3(  v[1]/s, -v[0]/s, -1.0 ),
             n2 = vec3( -v[1]/s,  v[0]/s, -1.0 );
  Segment
    * s1 = new Segment( n1, vec3(0.0,0.0,2.0), vec3(1.0,0.0,0.0), pcinf->style.lines_width ),
    * s2 = new Segment( n2, vec3(0.0,0.0,2.0), vec3(1.0,0.0,0.0), pcinf->style.lines_width );

    add( pcinf );
    add( pcecu );
    add( pcsup );
    add( s1 );
    add( s2 );
}

// *****************************************************************************
// class JoiningQuads
// -----------------------------------------------------------------------------

JoiningQuads::JoiningQuads( const Polygon & p1, const Polygon & p2 )
{
  const int n = p1.points3D.size() ;
  assert( n == p2.points3D.size() );

  for( unsigned i = 0 ; i < n-1 ; i++ )
  {
      // crear poligono y popularlo y añadirlo
      const vec3
        p00 = p1.points3D[i], p01 = p1.points3D[i+1],
        p10 = p2.points3D[i], p11 = p2.points3D[i+1] ;
      add( new PolQuad( p00, p01, p10, p11 ) );
  }
  // añadir cierre
  const vec3
    p00 = p1.points3D[n-1], p01 = p1.points3D[0],
    p10 = p2.points3D[n-1], p11 = p2.points3D[0] ;
  add( new PolQuad( p00, p01, p10, p11 ) );
}

// *****************************************************************************
// class Figure
// -----------------------------------------------------------------------------

Figure::Figure()
{
   width_cm = 20.0 ;
}
// -----------------------------------------------------------------------------

void Figure::drawSVG( const std::string & nombre_arch )
{
   using namespace std ;
   std::fstream fout( nombre_arch, ios_base::out) ;

   if ( ! objetos.projected )
      objetos.project( cam ) ;

   SVGContext ctx ;
   ctx.os = &fout ;

   constexpr real fmrg = 0.01 ; // width del margen en X y en Y, expresado en porcentaje del width en X
   //cout << "objetos.min == " << objetos.max << ", objetos.max == " << objetos.min << endl ;

   const real ax       = objetos.max[0]-objetos.min[0] ;
   const vec2 vmargen  = vec2( fmrg*ax, fmrg*ax );
   const vec2 ptos_min = objetos.min,
              ptos_w   = objetos.max-objetos.min,
              box_min  = ptos_min-vmargen,
              box_w    = ptos_w+real(2.0)*vmargen ;

   assert( 0 < box_w[0] && 0 < box_w[1] );
   real ratio = box_w[1]/box_w[0] ;

   const real wx = width_cm,      // width en centimetros
              wy = wx*ratio ;

   // cabecera svg
   fout << "<svg xmlns='http://www.w3.org/2000/svg' " << endl
        << "     width='" << wx << "cm' height='" << wy << "cm' " << endl
        << "     viewBox='" << box_min[0] << " " << box_min[1] << " " << box_w[0] << " " << box_w[1] << "'" << endl
        << ">" << endl ;

   // output radial gradient fill names (if any)
   for( auto & name :  rad_fill_grad_names )
      write_radial_gradient_def( fout, name );

   fout << "<g transform='translate(0.0 " << real(2.0)*box_min[1]+box_w[1] << ") scale(1.0 -1.0)'> <!-- transf global (inv y) -->"<< endl ;

   // objetos svg
   objetos.drawSVG( ctx );

   // pie svg
   fout << "</g>" << endl ;
   fout << "</svg>" << endl ;

   fout.close();

   cout << "end svg " << nombre_arch << endl ;
}

//******************************************************************************


// -----------------------------------------------------------------------------

SpherePolygon::SpherePolygon( const Polygon & orig )
{
  style.draw_lines     = true ;
  style.draw_filled      = true ;
  style.close_lines  = true ;
  style.lines_width   = 0.007 ;
  style.fill_opacity = 0.2 ;
  style.lines_color   = vec3( 0.0, 0.0, 1.0 );
  style.fill_color    = vec3( 0.0, 0.0, 1.0 );

  assert( orig.points3D.size() > 0 );
  for( unsigned i = 0 ; i < orig.points3D.size() ; i++ )
  {
    vec3 porg = orig.points3D[i];
    points3D.push_back( porg.normalized() );
  }
}

// -----------------------------------------------------------------------------

YCylinderPoint::YCylinderPoint( Point & ppoint )

: Point(vec3(0,0,0),vec3(1.0,0.0,0.0))
{
  assert( ! ppoint.projected );

  vec3       pesf = ppoint.pos3D.normalized();
  const real f    = real(1.0)/real(sqrt( double( pesf[0]*pesf[0]+pesf[2]*pesf[2] )));

  pos3D = vec3(  f*pesf[0], pesf[1], f*pesf[2]) ;
  color = vec3(1.0,0.0,0.0) ;
}


// -----------------------------------------------------------------------------

YCylinderPolygon::YCylinderPolygon( const Polygon & orig )
{
  style.draw_lines    = true ;
  style.draw_filled     = true ;
  style.close_lines = true ;
  style.lines_width  = 0.015 ;
  style.fill_opacity = 0.2 ;
  style.lines_color   = vec3( 1.0, 0.0, 0.0 );
  style.fill_color    = vec3( 1.0, 0.0, 0.0 );

  assert( orig.points3D.size() > 0 );
  for( unsigned i = 0 ; i < orig.points3D.size() ; i++ )
  {
    vec3       pesf = orig.points3D[i].normalized();
    const real f    = real(1.0)/real(sqrt( double( pesf[0]*pesf[0]+pesf[2]*pesf[2] )));
    vec3       pcil = vec3(  f*pesf[0], pesf[1], f*pesf[2]) ;

    points3D.push_back( pcil );
  }
}

// -----------------------------------------------------------------------------

ZCylinderPolygon::ZCylinderPolygon( const Polygon & orig )
{
  style.draw_lines    = true ;
  style.draw_filled     = false ;
  style.close_lines = true ;
  style.lines_width  = 0.015 ;
  style.fill_opacity = 0.2 ;
  style.lines_color   = vec3( 1.0, 0.0, 0.0 );
  style.fill_color    = vec3( 1.0, 0.0, 0.0 );

  assert( orig.points3D.size() > 0 );
  for( unsigned i = 0 ; i < orig.points3D.size() ; i++ )
  {
    vec3       pesf = orig.points3D[i].normalized();
    const real f    = real(1.0)/real(sqrt( double( pesf[0]*pesf[0]+pesf[1]*pesf[1] )));
    vec3       pcil = vec3(  f*pesf[0], f*pesf[1], pesf[2]) ;

    points3D.push_back( pcil );
  }
}

// -----------------------------------------------------------------------------

ZCylinderPolygonWithSector::ZCylinderPolygonWithSector( const Polygon & orig,
                                                         real ang0, real ang1,
                                                         real lonX, real lonY  )
{
   style.draw_lines   = true ;
   style.draw_filled  = false ;
   style.close_lines  = true ;
   style.lines_width  = 0.015 ;
   style.fill_opacity = 0.2 ;
   style.lines_color  = vec3( 1.0, 0.0, 0.0 );
   style.fill_color   = vec3( 1.0, 0.0, 0.0 );

   assert( orig.points3D.size() > 0 );
   for( unsigned i = 0 ; i < orig.points3D.size() ; i++ )
   {

      const vec3 porig = orig.points3D[i] ;
      const vec3 pesf  = porig.normalized();
      const real len   = real(sqrt( double( pesf[0]*pesf[0]+pesf[1]*pesf[1] ))) ;

      if ( len > 1e-6 )
      {
         const real f    = real(1.0)/len;
         const vec3 pcil = vec3(  f*pesf[0], f*pesf[1], pesf[2]) ;
         points3D.push_back( pcil );
      }
      else
      {
         using namespace std ;
         cout << "porig.length() == " << porig.length() << endl ;
      }
  }

  // añadir el sector

  using namespace std ;

  vec2 p0 = vec2( lonX*cos(ang0), lonY*sin(ang0)),
       p1 = vec2( lonX*cos(ang1), lonY*sin(ang1));


  cout << "ang1 == " << ang1/M_PI  << "*pi, ang0 == " << ang0/M_PI << "*pi " << endl ;

  real ang0corr = atan2( p0[1], p0[0] ),
       ang1corr = atan2( p1[1], p1[0] );
  cout << "ang1 (c) == " << ang1corr/M_PI  << "*pi, ang0 (c) == " << ang0corr/M_PI << "*pi " << endl ;
  // insertar points en el borde del cilindro, desde ang1 (incluido) hasta ang0 (sin incluir)

  if (ang1corr < ang0corr ) ang1corr += 2.0*M_PI ; // corrige ang1 para saltar discontinuidades de atan2, por si acaso ;
  assert( ang0corr < ang1corr ); // check

  const int nump = points3D.size() ;

  for( int i = nump ; 0 <= i ; i-- )
  {
    const real ang = ang0corr + (ang1corr-ang0corr)*real(i)/real(nump) ;
    points3D.push_back( vec3( cos(ang), sin(ang), 1.0 ) );
  }

}
