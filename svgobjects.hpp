// *********************************************************************
// **
// ** File: svgobjects.hpp
// ** Declarations for classes for 3D polygons an polylines in various shapers
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

#ifndef SVGOBJECTS_HPP
#define SVGOBJECTS_HPP

#include <cassert>
#include <cmath>
#include <vector>
#include <string> // std::string, std::stoi
#include <iostream>
#include <fstream> // std::fstream
#include <sstream> // std::stringstream (http://www.cplusplus.com/reference/sstream/stringstream/)
#include "vector_templates.hpp"

#define SIMPLE_PREC

#ifdef SIMPLE_PREC
typedef float real ;
typedef Vec2f vec2 ;
typedef Vec3f vec3 ;
typedef Vec4f vec4 ;
#else
typedef double real ;
typedef Vec2d vec2 ;
typedef Vec3d vec3 ;
typedef Vec4d vec4 ;
#endif

// *****************************************************************************
// orthonormal camera coordinate system

class CamRefSys
{
   public:
   vec3 origin, xAxis, yAxis, zAxis ;

   CamRefSys() ;
   CamRefSys( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup );
   void initialize( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup );
   vec2 world2cam( const vec3 & p ) const ;
} ;
// *****************************************************************************

class Camera
{
   public:
   CamRefSys src ;
   Camera( const vec3 & look_at, const vec3 & observer, const vec3 & p_vup ) ;
   Camera() ;
   vec2 project( const vec3 & p ) const ;
};

// *****************************************************************************
// SVGContext:
// context information (actualy,  just a stream)

class SVGContext
{
  public:
  std::ostream * os ;
  SVGContext() ;
} ;

// *****************************************************************************
// clase para styles de polígonos y quizás otros tipos de objetos

class PathStyle
{
  public:
  PathStyle();
  void writeSVG( SVGContext & ctx ); // write style attrs to an svg file

  vec3  lines_color,      // lines color, when lines are drawn (draw_lines == true )
        fill_color ;      // fill color, when fill is drawn (draw_filled == true)
  bool  draw_lines,       // draw lines joining points (yes/no)
        close_lines,      // when draw_lines == true, join last point with first (yes/no)
        draw_filled ;     // fill the polygon (yes(no)
  real  lines_width ;
  real  fill_opacity ;    // fill opacity when draw_filled == true (0->transparent; 1->opaque)
  bool  dashed_lines  ;   // when draw_lines == true, draw dashed lines (yes/no)

  bool use_grad_fill ; // use gradient fill (when draw_filled == true)
  std::string grad_fill_name ; // name to use for gradient fill


} ;

// *****************************************************************************
// An abstract class for things which can be drawn to an SVG file and can be
// projected to 2d (must be projected before drawn)

class Object
{
  public:
  Object();
  virtual void drawSVG( SVGContext & ctx ) = 0 ;
  virtual void project( const Camera & cam ) = 0 ;
  virtual ~Object() ;

  bool projected ;  // true when the points have been projected
  vec2 min,max ;    // when projected==true,
} ;

// *****************************************************************************
// class Point
// A class for an isolated point, drawn with a given radius

class Point : public Object
{
  public:
  Point( vec3 ppos3D, vec3 color );
  virtual void drawSVG( SVGContext & ctx ) ;
  virtual void project( const Camera & cam ) ;

  vec3 pos3D, color ;
  vec2 pos2D ;
  real radius ; // 0.03 por defecto, se puede cambiar
};

// *****************************************************************************
// class Polygon
// Any Object which is described by a sequence of points
// it can be a polyline, a polygon, a filled polygon.

class Polygon : public Object // secuencia de points
{
   public:
   Polygon();

   virtual void project( const Camera & cam );
   virtual void drawSVG( SVGContext & ctx )  ;
   virtual ~Polygon() ;

   PathStyle         style ;
   std::vector<vec3> points3D ; // original points
   std::vector<vec2> points2D ; // projected points
};
// *****************************************************************************
// class ObjectsSet
// A set of various objects

class ObjectsSet : public Object
{
   public:
   ObjectsSet();
   virtual void project( const Camera & cam );
   virtual void drawSVG( SVGContext & ctx )  ;
   virtual ~ObjectsSet() ;
   void add( Object * pobj );  // add one object

   std::vector<Object *> objetos ;
} ;

// *****************************************************************************
// class Sphere
// A filled sphere in 3D which is viewed as a filled circle in 2D

class Sphere : public Object
{
   public:
   Sphere( const vec3 & pcenter3D, real pradius3D );

   virtual void project( const Camera & cam );
   virtual void drawSVG( SVGContext & ctx )  ;

   real radius3D,  // original radius
        radius2D ; // projected radius (==radius3D actually)
   vec3 center3D ;
   vec2 center2D ; // when proyectado == true, 2D center
};

// *****************************************************************************
// class Hemisphere
// A filled hemisphere in 3D, drawn as 2 semicircles



class Hemisphere : public ObjectsSet
{
   public:
   Hemisphere( const vec3 & pcenter3D, real pradius3D, vec3 p_view_dir ) ;

   real radius3D,  // original radius
        radius2D ; // projected radius (==radius3D actually)
   vec3 center3D ,
        view_dir ; // view direction, it is used to align the semicircles
   vec2 center2D ; // when proyectado == true, 2D center

   Polygon contour ;
};

// *****************************************************************************
// class Segment
// A polygon with just two points.

class Segment : public Polygon
{
   public:
   Segment( const vec3 & p0, const vec3 & vd, const vec3 & color, real width ) ;
   Segment( const vec3 & p0, const vec3 & p1  );
   Segment( const Point & p0, const Point & p1, real width );

};
// *****************************************************************************
// Class PolQuad
// A four points polygon, filled

class PolQuad : public Polygon
{
   public:
   PolQuad( const vec3 & p00, const vec3 & p01, const vec3 & p10, const vec3 & p11 ) ;
};

// *****************************************************************************
// class Ellipse
// A planar ellipse in 3D

class Ellipse : public Polygon
{
   public:
   Ellipse( unsigned n, const vec3 & center, const vec3 & eje1, const vec3 eje2  );
};

// *****************************************************************************
// class EllipseSectorZ
// A sector of an ellipse (the ellipse is perpendicular to Z)
// the 'sector' is defined by two angles about Y-axis (used for the parallel map)

class EllipseSectorZ : public ObjectsSet
{
   public:
   EllipseSectorZ( unsigned n, real lonX, real lonY  );
   Polygon * ppol ;
   Point   * ppun0,
           * ppun1 ;
};

// *****************************************************************************
// class EllipseSectorRadial
// A sector of an ellipse (the ellipse is perpendicular to Z)
// the 'sector' is defined by two angles about Z-axis (used for the radial map)

class EllipseSectorRadial : public ObjectsSet
{
   public:
   EllipseSectorRadial( unsigned n, real plonX, real plonY  );

   Polygon *ppol ;
   Point   *ppun0, *ppun1 ;
   real    alpha0, alpha1 ;  // angulos inicial y final de este sector
   real    lonX, lonY ;      // longitudes de los ejes inicial y final
   vec3    point0, point1 ;  // points extremos del segmento
};

// *****************************************************************************
// class SegmentsVert
// A set of segments joining the points in two polygons (must be the same size)

class SegmentsVert : public ObjectsSet
{
   public:
   SegmentsVert( unsigned dn, const Polygon & p1, const Polygon & p2 );
} ;

// *****************************************************************************
// class Axes
//
// three segments for the orthonormal world reference system

class Axes : public ObjectsSet
{
   public:
   Axes( real widthl );
} ;

// *****************************************************************************
// class YAxisProjectorsSegments
// a set of segments from each vertex of a polygon towards Y axis

class YAxisProjectorsSegments : public ObjectsSet
{
   public:
   YAxisProjectorsSegments( unsigned dn, const Polygon & p1 );
} ;

// *****************************************************************************
// class YAxisCylinder
// A unit-radius cylinder, centered in the origin, from Y=-1 to Y=1

class YAxisCylinder : public ObjectsSet
{
   public:
   YAxisCylinder( const Camera & cam );
} ;

// *****************************************************************************
// class ZAxisCylinder
// A unit-radius cylinder, centered in the origin, from Z=-1 to Z=1

class ZAxisCylinder : public ObjectsSet
{
   public:
   ZAxisCylinder( const Camera & cam );
} ;

// *****************************************************************************
// class JoiningQuads
// A set of filled polygons (PolQuad objects) linking two same-size polygons
// (for each )

class JoiningQuads : public ObjectsSet
{
   public:
   JoiningQuads( const Polygon & p1, const Polygon & p2 );
} ;

// *****************************************************************************
// class SpherePolygon
// a polygon obtained by projecting another one onto the Sphere

class SpherePolygon : public Polygon
{
  public:
  SpherePolygon( const Polygon & orig, bool clip = false ) ;
};

// *****************************************************************************
// class HorPlanePolygon
// a polygon obtained by projecting another one onto the (horizontal) plane at Y=0

class HorPlanePolygon : public Polygon
{
  public:
  HorPlanePolygon( const Polygon & orig, bool clip_neg ) ;
};

// *****************************************************************************
// class ExtrVertSegm
// two vertical segments joining extreme-X vertexes on two polygons
// (two polygons of similar size with vertically aligned vertexes)

class ExtrVertSegm : public ObjectsSet
{
  public:
  ExtrVertSegm( Polygon & pol1, Polygon & pol2, const Camera & cam ) ;
};


// *****************************************************************************
// class YCylinderPoint
// a point obtained by projecting another one onto the Y-axis cylinder

class YCylinderPoint : public Point
{
  public:
  YCylinderPoint( Point & ppoint );
} ;

// *****************************************************************************
// class YCylinderPolygon
// a polygon obtained by projecting another polygon onto the Y-axis cylinder

class YCylinderPolygon : public Polygon
{
  public:
  YCylinderPolygon( const Polygon & orig ) ;
};

// *****************************************************************************
// class ZCylinderPolygon
// a polygon obtained by projecting another polygon onto the Y-axis cylinder

class ZCylinderPolygon : public Polygon
{
  public:
  ZCylinderPolygon( const Polygon & orig ) ;
};

// *****************************************************************************
// class ZCylinderPolygonWithSector
// a ZCylinderPolygon but with a sector added

class ZCylinderPolygonWithSector : public Polygon
{
  public:
  ZCylinderPolygonWithSector( const Polygon & orig, real ang0, real ang1,
                                                    real lonX, real lonY ) ;
};

// *****************************************************************************
// class Figure
// A container for a set of objects which can be drawn to a SVG file.
// It can be written to a SVG file, the output includes the whole SVG elements
// (headers, footers, bounding box, etc....)

class Figure
{
   public:

   Figure( ) ;
   void drawSVG( const std::string & nombre_arch ) ;

   Camera     cam ;      // camera used to project all the points
   ObjectsSet objetos ;  // set of objects in the figure
   real       width_cm ; // width (in centimeters) in the SVG header

   std::vector< std::string > rad_fill_grad_names ; // name of radial fill gradients to output
} ;

#endif
