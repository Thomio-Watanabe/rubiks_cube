#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Cube{
  Cube();
  ~Cube();
  void updateAuxiliar();

  unsigned *values;
  unsigned *auxiliar;
  unsigned total_elements;
};

Cube::Cube(){
  total_elements = 27;

  // Each number defines a piece of the cube.
  // Couting from the left top of the cube.
  // 13 is the cube center.
  // Indexes and content have equal values.
  values = (unsigned *)malloc( sizeof(unsigned) *total_elements );
  for( size_t i = 0; i < total_elements; ++i)
    values[i] = i;

  auxiliar = (unsigned*)malloc( sizeof(unsigned) * total_elements );
  for( size_t i = 0; i < total_elements; ++i)
    auxiliar[i] = i;
}

Cube::~Cube(){
  free(values);
  free(auxiliar);
}

void Cube::updateAuxiliar(){
  for( size_t i = 0; i < total_elements; ++i )
    auxiliar[i] = values[i];
}


class RubiksCube
{
  public:
  RubiksCube();
  ~RubiksCube();
  void printValues();
  void rotateSlice(const unsigned &axis, const unsigned &slice, const unsigned &rotation);

  private:
  Cube cube;
  const unsigned numPieces;
};


RubiksCube::RubiksCube() : numPieces(3)
{
}

RubiksCube::~RubiksCube()
{
}


void RubiksCube::rotateSlice( const unsigned &axis, const unsigned &slice, const unsigned &rotation )
{
// axis = 0-horizontal, 1-vertical, 2-transverse
// slice = 0,1,2 -> 
// rotation = 0 (CW clockwise), 1 (CC counter-clockwise)
// The left-top piece of the cube is the origin.
// x points right (horizontal axis)
// y points down (vertical axis)
// z points inside screen (transversal axis)
// The rotation "looks" from -inf (left, top, outside screen)

//  const unsigned temp_cube[27] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,
//                                   9, 10, 11, 12, 13, 14, 15, 16, 17,
//                                  18, 19, 20, 21, 22, 23, 24, 25, 26};
// 0 -> 18
// 1 -> 9
// 2 -> 0
// 9 -> 19
// 10 -> 10
// 11 -> 1
// 18 -> 20
// 19 -> 11
// 20 -> 2
  if( axis == 1 ){
    for( size_t depth = 0; depth < numPieces; ++depth ){
      unsigned index = depth * numPieces * numPieces;
      unsigned destination0 = rotation ? (depth + 18): index;
      unsigned destination1 = rotation ? (depth + 9): (index + 1);
      unsigned destination2 = rotation ? depth : (index + 2);
      unsigned origin0 = rotation ? index : (depth + 18);
      unsigned origin1 = rotation ? (index + 1) : (depth + 9);
      unsigned origin2 = rotation ? (index + 2) : depth;

      destination0 += slice * numPieces;
      destination1 += slice * numPieces;
      destination2 += slice * numPieces;
      origin0 += slice * numPieces;
      origin1 += slice * numPieces;
      origin2 += slice * numPieces;

      cube.values[ destination0 ] = cube.auxiliar[ origin0 ];
      cube.values[ destination1 ] = cube.auxiliar[ origin1 ];
      cube.values[ destination2 ] = cube.auxiliar[ origin2 ];
    }
  }
  cube.updateAuxiliar();
}


void RubiksCube::printValues()
{
   // The cube has 3 elements per row, column and depth.
  const unsigned num_pieces = 3;
  printf("Cube content = \n");
  for( size_t depth = 0; depth < num_pieces; ++depth ){
    for( size_t col = 0; col < num_pieces; ++col ){
      for( size_t row = 0; row < num_pieces; ++row){
        const unsigned index = num_pieces*( depth * num_pieces + col ) + row;
        unsigned piece = cube.values[ index ];
        printf( "\t%u,", piece );
      }
      printf("\n");
    }
    printf("\n");
  }
}


int main(){
  // Each action can create 4 new states
  RubiksCube cube;
  cube.printValues();
  cube.rotateSlice(1,2,1);
  cube.printValues();

  return 0;
}

