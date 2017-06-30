#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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
  // unsigned values[27] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,
  //                         9, 10, 11, 12, 13, 14, 15, 16, 17,
  //                        18, 19, 20, 21, 22, 23, 24, 25, 26};
  values = (unsigned *)malloc( sizeof(unsigned) * total_elements );
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
  unsigned countConnexions();

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
// rotation = 1 (CW clockwise), 0 (CC counter-clockwise)
// The left-top piece of the cube is the origin.
// x points right (horizontal axis)
// y points down (vertical axis)
// z points inside screen (transversal axis)
// The rotation "looks" from -inf (left, top, outside screen)

  if( axis == 0 ){
    for( size_t i = 0; i < numPieces; ++i ){
      unsigned index = i * numPieces * numPieces;
      unsigned destination0 = rotation ? (6 - i*numPieces): index;
      unsigned destination1 = rotation ? (15 - i*numPieces): (index + numPieces);
      unsigned destination2 = rotation ? (24 - i*numPieces): (index + 2*numPieces);
      unsigned origin0 = rotation ? (index): (6 - i*numPieces);
      unsigned origin1 = rotation ? (index + numPieces) : (15 - i*numPieces);
      unsigned origin2 = rotation ? (index + 2*numPieces) : (24 - i*numPieces);

      destination0 += slice;
      destination1 += slice;
      destination2 += slice;
      origin0 += slice;
      origin1 += slice;
      origin2 += slice;

      cube.values[ destination0 ] = cube.auxiliar[ origin0 ];
      cube.values[ destination1 ] = cube.auxiliar[ origin1 ];
      cube.values[ destination2 ] = cube.auxiliar[ origin2 ];
    }
  }

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
  if( axis == 2 ){
    for( size_t i = 0; i < numPieces; ++i ){
      unsigned index = i * numPieces;
      unsigned destination0 = rotation ? (2 - i): index;
      unsigned destination1 = rotation ? (5 - i): (index + 1);
      unsigned destination2 = rotation ? (8 - i): (index + 2);
      unsigned origin0 = rotation ? index : (2 - i);
      unsigned origin1 = rotation ? (index + 1) : (5 - i);
      unsigned origin2 = rotation ? (index + 2) : (8 - i);

      destination0 += slice * numPieces * numPieces;
      destination1 += slice * numPieces * numPieces;
      destination2 += slice * numPieces * numPieces;
      origin0 += slice * numPieces * numPieces;
      origin1 += slice * numPieces * numPieces;
      origin2 += slice * numPieces * numPieces;

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


unsigned RubiksCube::countConnexions()
{
  unsigned connexions_counter = 0;
  for( size_t depth = 0; depth < numPieces; ++depth ){
    for( size_t row = 0; row < numPieces; ++row ){
      for( size_t col = 0; col < numPieces; ++col ){
        unsigned index = col + row * numPieces + depth * numPieces * numPieces;
        for( size_t i = 0; i < numPieces; ++i ){
          unsigned step = (unsigned) pow( numPieces, i );

          if( col < numPieces - 1 ){
            if( cube.values[index+1] == cube.values[index] - step )
              ++connexions_counter;
            if( cube.values[index+1] == cube.values[index] + step )
              ++connexions_counter;
          } // col if

          if( row < numPieces - 1 ){
            if( cube.values[index+numPieces] == cube.values[index] - step )
              ++connexions_counter;
            if( cube.values[index+numPieces] == cube.values[index] + step )
              ++connexions_counter;
          } // row if

          if( depth < numPieces - 1 ){
            unsigned inc = (unsigned) pow(numPieces,2);
            if( cube.values[index+inc] == cube.values[index] - step )
              ++connexions_counter;
            if( cube.values[index+inc] == cube.values[index] + step )
              ++connexions_counter;
          }

        } // i if
      }
    }
  }

  return connexions_counter;
}


int main(){
  // Each action can create 4 new states
  RubiksCube cube;
  cube.printValues();

  unsigned con = cube.countConnexions();
  printf("Initial number of connexions = %u \n", con);

  cube.rotateSlice(1,2,1);
  cube.printValues();

  con = cube.countConnexions();
  printf("Initial number of connexions = %u \n", con);

  cube.rotateSlice(1,2,1);
  cube.printValues();

  con = cube.countConnexions();
  printf("Initial number of connexions = %u \n", con);

  cube.rotateSlice(1,2,1);
  cube.printValues();

  con = cube.countConnexions();
  printf("Initial number of connexions = %u \n", con);

  cube.rotateSlice(1,2,1);
  cube.printValues();

  con = cube.countConnexions();
  printf("Initial number of connexions = %u \n", con);

  return 0;
}

