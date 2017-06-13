// *********************************************************************
// **
// ** File: vector_templates_impl.hpp
// ** Templates implementations for fixed-size vectors with numeric values.
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


// *********************************************************************
//
// plantilla de clase:
// VectorTempl<T,n>
//
// *********************************************************************

template< class T, unsigned n> inline
VectorTempl<T,n>::VectorTempl()
{

}

// constructor usando un array C++
template< class T, unsigned n> inline
VectorTempl<T,n>::VectorTempl( const T * org )
{
   for( unsigned i = 0 ; i < n ; i++ )
      (*this)[i] = org[i] ;
}

//----------------------------------------------------------------------

//template< class T, unsigned n >
//T & VectorTempl<T,n>::operator [] (const unsigned i)
//{
   //assert( i < n ) ;
   //return coo[i] ;
//}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
const T & VectorTempl<T,n>::operator () (const unsigned i) const
{
   assert( i < n ) ;
   return coo[i] ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
T & VectorTempl<T,n>::operator () (const unsigned i)
{
   assert( i < n ) ;
   return coo[i] ;
}

//----------------------------------------------------------------------
// conversion a un puntero de lectura/escritura de tipo: T* ( T* p = tupla )

template< class T, unsigned n > inline
VectorTempl<T,n>::operator  T * ()
{
   return coo ;
}

//----------------------------------------------------------------------
// conversion a un puntero de solo lectura de tipo T* ( const T* p = tupla )

template< class T, unsigned n > inline
VectorTempl<T,n>::operator  const T * () const
{
   return coo ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
VectorTempl<T,n> VectorTempl<T,n>::operator + ( const VectorTempl<T,n> & der ) const
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)+der(i) ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
VectorTempl<T,n> VectorTempl<T,n>::operator - ( const VectorTempl<T,n> & der ) const
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)-der(i) ;
   return res ;
}

//----------------------------------------------------------------------

// devuelve tupla negada ( v1 = -v2 )
template< class T, unsigned n > inline
VectorTempl<T,n> VectorTempl<T,n>::operator - (  ) const
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = -(*this)(i) ;
   return res ;

}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
VectorTempl<T,n> VectorTempl<T,n>::operator * ( const T & a ) const
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)*a ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n >  inline
VectorTempl<T,n> VectorTempl<T,n>::operator / ( const T & a ) const
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)/a ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
VectorTempl<T,n> operator * ( const T & a, const VectorTempl<T,n> & der )
{
   VectorTempl<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = a*der(i) ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
std::ostream & operator <<  ( std::ostream & os, const VectorTempl<T,n> & der )
{
   os << "(" ;
   for( unsigned i = 0 ; i < n ; i++ )
   {  os << der(i) ;
      if ( i+1 < n )
         os << "," ;
   }
   os << ")" ;
   return os ;
}

//----------------------------------------------------------------------

// producto escalar (dot)  a = v1.dot(v2)
// (usando multiplicaciones y sumas en doble precision)

template< class T, unsigned n > inline
T VectorTempl<T,n>::dot( const VectorTempl<T,n> & v2 ) const
{
   double res = 0.0 ;
   for( unsigned int i = 0 ; i < n ; i++ )
      res += double((*this)(i)) * double(v2(i)) ;
   return T(res) ;
}

//----------------------------------------------------------------------

// obtener longitud al cuadrado
template< class T, unsigned n > inline
T VectorTempl<T,n>::lengthSq( ) const
{
   return T( this->dot( *this ) ) ;
}

//----------------------------------------------------------------------
// obtener longitud (usando raiz en doble precisión)

template< class T, unsigned n > inline
T VectorTempl<T,n>::length( ) const
{
   return T( sqrt( double(this->lengthSq()) ) ) ;
}

//----------------------------------------------------------------------
// operador binario para producto escalar

template< class T, unsigned n > inline
T VectorTempl<T,n>::operator | ( const VectorTempl & der ) const
{
   return this->dot( der ) ;
}

// ---------------------------------------------------------------------

template< class T, unsigned n > inline
VectorTempl<T,n> VectorTempl<T,n>::normalized() const
{
   return (*this)/this->length() ;

}

// *********************************************************************
//
// plantilla de clase:
//
//   VectorTempl2<T>
//
// *********************************************************************


template< class T > inline
VectorTempl2<T>::VectorTempl2(  )
{

}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl2<T>::VectorTempl2( const VectorTempl<T,2> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void VectorTempl2<T>::operator = ( const VectorTempl<T,2> & der )
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl2<T>::VectorTempl2( const T & c0, const T & c1 )
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
}

// *********************************************************************
//
// plantilla de clase:
// VectorTempl3<T>
//
// *********************************************************************


template< class T > inline
VectorTempl3<T>::VectorTempl3(  )
{

}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl3<T>::VectorTempl3( const VectorTempl<T,3> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
   (*this)[2] = ini(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void VectorTempl3<T>::operator = ( const VectorTempl<T,3> & der )
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void VectorTempl3<T>::operator = ( const VectorTempl<T,4> & der )
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl3<T>::VectorTempl3( const T & c0, const T & c1, const T & c2 )
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
   (*this)[2] = c2 ;
}

// ---------------------------------------------------------------------


template< class T > inline
VectorTempl3<T> VectorTempl3<T>::cross( const VectorTempl3<T> & v2 ) const
{
   // cuidado: no hay acceso a 'coo' tal cual, mirar:
   // http://stackoverflow.com/questions/7281072/accessing-public-members-of-base-class-fails

   return VectorTempl3<T>(  (*this)(1)*v2(2) -  (*this)(2)*v2(1),
                       (*this)(2)*v2(0) -  (*this)(0)*v2(2),
                       (*this)(0)*v2(1) -  (*this)(1)*v2(0)
                     );
}

// *********************************************************************
//
// plantilla de clase:
//
//   VectorTempl4<T>
//
// *********************************************************************


template< class T > inline
VectorTempl4<T>::VectorTempl4(  )
{

}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl4<T>::VectorTempl4( const VectorTempl<T,4> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
   (*this)[2] = ini(2) ;
   (*this)[3] = ini(3) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void VectorTempl4<T>::operator = ( const VectorTempl<T,4> & der )
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
   (*this)[3] = der(3) ;
}

// ---------------------------------------------------------------------

template< class T > inline
VectorTempl4<T>::VectorTempl4( const T& c0, const T& c1, const T& c2, const T& c3 )
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
   (*this)[2] = c2 ;
   (*this)[3] = c3 ;
}
