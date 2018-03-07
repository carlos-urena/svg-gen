// *********************************************************************
// **
// ** File: svgobjects.cpp
// ** Declarations for classes which output svg figures files.
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

#ifndef FIGURES_HPP
#define FIGURES_HPP

#include "svgobjects.hpp"

// -----------------------------------------------------------------------------

class Figure1_HatBox : public Figure
{
  public:
  Figure1_HatBox();
};
// -----------------------------------------------------------------------------

class Figure2_ParamCil : public Figure
{
  public:
  Figure2_ParamCil();
};
// -----------------------------------------------------------------------------

class Figure3_ParamConPol : public Figure
{
  public:
  Figure3_ParamConPol();
};
// -----------------------------------------------------------------------------

class Figure4_EllipseSectorZ : public Figure
{
  public:
  Figure4_EllipseSectorZ();
};

// -----------------------------------------------------------------------------

class Figure5_EllipseSectorRadial : public Figure
{
  public:
  Figure5_EllipseSectorRadial();
};

// -----------------------------------------------------------------------------

class Figure6_SuplPara : public Figure
{
  public:
  Figure6_SuplPara();
};

// -----------------------------------------------------------------------------

class FigurePSA_Base : public Figure
{
  public:
  FigurePSA_Base() {}
  void initialize();

  vec3  disk_center ;
  float disk_radius  ;
  bool  draw_projectors ;
  bool  clip_neg ;
  Polygon * original_pol ;
};

// -----------------------------------------------------------------------------


class Figure7_PSA_shape : public FigurePSA_Base
{
   public:
   Figure7_PSA_shape();
} ;

// -----------------------------------------------------------------------------


class Figure8_PSA_ellipse : public FigurePSA_Base
{
   public:
   Figure8_PSA_ellipse();
} ;


// -----------------------------------------------------------------------------


class Figure9_PSA_ellipse_lune : public FigurePSA_Base
{
   public:
   Figure9_PSA_ellipse_lune();
} ;

// -----------------------------------------------------------------------------


class Figure10_PSA_lune : public FigurePSA_Base
{
   public:
   Figure10_PSA_lune();
} ;

#endif
