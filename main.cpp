
#include "figures.hpp"

int main( int argc, char * argv [] )
{
  using namespace std ;

  if ( argc < 3 )
  { cerr << "please specify figure number and name for .svg output file" << endl << flush ;
    return 1 ;
  }

  int arg1 ;
  try
  { arg1 = std::stoi(std::string(argv[1]));
  }
  catch ( std::exception e )
  { cerr << "cannot convert first argument to integer (" << argv[1] << ")" << endl ;
    return 1 ;
  }

  std::string arg2( argv[2] );

  //test_svg_simple( arg1 );
  //test_figura_estrella( arg1 );
  //test_poligonos_estilos( arg1 );

  Figure * fig = nullptr ;

  switch( arg1 )
  {
    case 1 :
      fig = new Figure1_HatBox(  );
      break ;
    case 2 :
      fig = new Figure2_ParamCil(  );
      break ;
    case 3 :
      fig = new Figure3_ParamConPol(  );
      break ;
    case 4 :
      fig = new Figure4_EllipseSectorZ(  );
      break ;
    case 5 :
      fig = new Figure5_EllipseSectorRadial() ;
      break ;
    case 6 :
      fig = new Figure6_SuplPara();
      break ;
    case 7 :
      fig = new Figure7_PSA_shape();
      break ;
    case 8 :
      fig = new Figure8_PSA_ellipse();
      break ;
    case 9 :
      fig = new Figure9_PSA_ellipse_lune();
      break ;
    case 10 :
      fig = new Figure10_PSA_lune();
      break ;
    default:
      cerr << "first argument is an out of range number (" << argv[1] << ")" << endl ;
      return 1 ;
  }

  fig->drawSVG( arg2 );

  return 0 ;

}
