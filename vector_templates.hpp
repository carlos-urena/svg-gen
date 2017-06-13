// *********************************************************************
// **
// ** File: vector_templates.hpp
// ** Templates declarations for fixed-size vectors with numeric values.
// ** Copyright (C) 2014 Carlos Ureña
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


#ifndef VECTOR_TEMPL_HPP
#define VECTOR_TEMPL_HPP

#include <cmath>
#include <cassert>
#include <iostream>

// constantes para acceder a las componentes de las tuplas
const unsigned
   X = 0,
   Y = 1,
   Z = 2,

   R = 0,
   G = 1,
   B = 2 ;

// definir alias de 'unsigned int' cuyo descriptor tiene un solo token
typedef unsigned int uint ;


// *********************************************************************
//
// plantilla de clase: VectorTempl
// clase para tuplas genéricas
//
// *********************************************************************

template< class T, unsigned n >
class VectorTempl
{
   private:
   T coo[n] ;  // vector de valores escalares

   public:
   // constructor por defecto: no hace nada
   inline VectorTempl();

   // constructor usando un array C++
   inline VectorTempl( const T * org ) ;

   // acceso de lectura/escritura a un elemento (v[i]=x, x=v[i])
   //T & operator [] (const unsigned i) ;

   // acceso de solo lectura a un elemento ( x=v(i) )
   const T & operator () (const unsigned i) const ;

   // acceso de lectura-escritura a un elemento ( v(i)=x )
   T & operator () (const unsigned i) ;

   // conversion a un puntero de lectura/escritura de tipo T* ( T* p = tupla )
   operator  T * ()  ;

   // conversion a un puntero de solo lectura de tipo: const  T* ( const T* p = tupla )
   operator  const T * ()  const ;

   // suma componente a componente ( v1=v2+v3 )
   VectorTempl<T,n> operator + ( const VectorTempl & der ) const ;

   // resta componente a componente ( v1=v2-v3 )
   VectorTempl<T,n> operator - ( const VectorTempl & der ) const ;

   // devuelve tupla negada ( v1 = -v2 )
   VectorTempl<T,n> operator - (  ) const ;

   // mult. por escalar por la derecha ( v1=v2*a )
   VectorTempl<T,n> operator * ( const T & a ) const ;

   // division por escalar ( v1=v2/a )
   VectorTempl<T,n> operator / ( const T & a ) const ;

   // producto escalar (dot)  a = v1.dot(v2)
   T dot( const VectorTempl<T,n> & v2 ) const ;

   // operador binario para producto escalar a = v1|v2 ;
   T operator | ( const VectorTempl & der ) const ;

   // obtener longitud al cuadrado
   T lengthSq( ) const ;

   // obtener longitud
   T length( ) const ;

   // obtener una copia normalizada
   VectorTempl<T,n> normalized() const ;
} ;

//----------------------------------------------------------------------
// operadores y funciones no miembros

// mult. por escalar por la izquierda ( v1=a*v2 )
template< class T, unsigned n >
inline VectorTempl<T,n> operator *  ( const T & a, const  VectorTempl<T,n> & der ) ;

// escritura de un vector en un ostream
template< class T, unsigned n >
inline std::ostream & operator <<  ( std::ostream & os, const VectorTempl<T,n> & der ) ;

// *********************************************************************
//
// Plantilla de clase: VectorTempl2
// especialización parcial para tuplas de 2 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class VectorTempl2 : public VectorTempl<T,2>
{
   public:

   // constructores: por defecto
   VectorTempl2() ;
   VectorTempl2( const T & c0, const T & c1 ) ;
   VectorTempl2( const VectorTempl<T,2> & ini );
   void operator = ( const VectorTempl<T,2> & der ) ;
} ;



// *********************************************************************
//
// Plantilla de clase: VectorTempl3
// especialización parcial para tuplas de 3 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class VectorTempl3 : public VectorTempl<T,3>
{
   public:

   // constructores: por defecto
   VectorTempl3() ;
   VectorTempl3( const T & c0, const T & c1, const T & c2 ) ;
   VectorTempl3( const VectorTempl<T,3> & ini );
   void operator = ( const VectorTempl<T,3> & der ) ;
   void operator = ( const VectorTempl<T,4> & der ) ; // asignar ignorando ultimo

   // producto vectorial (cross)  a = v1.cross(v2)
   VectorTempl3<T> cross( const VectorTempl3<T> & v2 ) const ;
} ;


// *********************************************************************
//
// Plantilla de clase: VectorTempl4
// especialización parcial para tuplas de 4 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class VectorTempl4 : public VectorTempl<T,4>
{
   public:

   // constructores: por defecto
   VectorTempl4() ;
   VectorTempl4( const T & c0, const T & c1, const T & c2, const T & c3 ) ;
   VectorTempl4( const VectorTempl<T,4> & ini );
   void operator = ( const VectorTempl<T,4> & der ) ;
} ;



// *********************************************************************
// tipos concretos:

typedef VectorTempl2<float>  Vec2f ;
typedef VectorTempl2<double> Vec2d ;
typedef VectorTempl2<uint>   Vec2u ;
typedef VectorTempl2<int>    Vec2i ;

typedef VectorTempl3<float>  Vec3f ;
typedef VectorTempl3<double> Vec3d ;
typedef VectorTempl3<uint>   Vec3u ;
typedef VectorTempl3<int>    Vec3i ;

typedef VectorTempl4<float>  Vec4f ;
typedef VectorTempl4<double> Vec4d ;
typedef VectorTempl4<int>    Vec4i ;
typedef VectorTempl4<uint>   Vec4u ;





// *********************************************************************
// incluir implementaciones 'inline'

#include "vector_templates_impl.hpp"


// *********************************************************************
#endif
