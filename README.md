# svg-gen
C++ code to generate SVG files

Various classes for objects which can be written to a SVG file. Most of the classes are subclasses of 'Polygon', which is a sequence of 3D vertex positions. There are classes from spherical and cylindrical projections of arbitrary 3 polygons.

Once a polygon is created, it is projected to 2D by using a camera (parallel for now), and then written to a SVG. All polygon derived classes are output as a 'path' tag in the SVG file.
