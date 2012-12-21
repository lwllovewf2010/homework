////////////////////////////////////////////////////////////////////////////////
// Simple OpenCL kernel that squares an input array.
// This code is stored in a file called mykernel.cl.
//


//
// A standard C code
//
//aFunction() {
//
//  int i;
//
//  for( i = 0 ; i < 8 ; i++ ) {
//
//      output[ i ] = input[ i ] * input[ i ];
//  }
//}
//


//
// OpenMP Parallelized Code (with Polly & Polyhedral Mathematical Method)
// (Not accurate, just an anology)
// (Find the better Data Locality in both Stoages and Caches with
// Polyhedral Method, and suitable for OpenMP, Many CPU Cores)
//
//
//aFunction() {
//
//  int i, j, k, m;
//
//#prgama XXX
//  for( i = 0 ; i < 2 ; i++ )
//      output[ i ] = input[ i ] * input[ i ];
//
//#prgama YYY
//  for( j = 2 ; j < 4 ; j++ )
//      output[ j ] = input[ j ] * input[ j ];
//
//#prgama ZZZ
//  for( k = 4 ; k < 6 ; k++ )
//      output[ k ] = input[ k ] * input[ k ];
//
//#prgama QQQ
//  for( m = 6 ; m < 8 ; m++ )
//      output[ m ] = input[ m ] * input[ m ];
//
//}
//


//
// OpenCL Parallelized Code
//
// GPU Core0 : output[ 0 ] = input[ 0 ] * input[ 0 ];
// GPU Core1 : output[ 1 ] = input[ 1 ] * input[ 1 ];
// GPU Core2 : output[ 2 ] = input[ 2 ] * input[ 2 ];
// GPU Core3 : output[ 3 ] = input[ 3 ] * input[ 3 ];
// GPU Core4 : output[ 4 ] = input[ 4 ] * input[ 4 ];
// GPU Core5 : output[ 5 ] = input[ 5 ] * input[ 5 ];
// GPU Core6 : output[ 6 ] = input[ 6 ] * input[ 6 ];
// GPU Core7 : output[ 7 ] = input[ 7 ] * input[ 7 ];
//
//
// WorkGroup & WorkItem to divide the Array Dimensions
//
// int ARRAY[ 10 ][ 8 ] = { ... };
// 10 WorkGroups : WorkGroups are running parallelly.
// 8  WorkItems  : WorkItems  are running simultaneously.
//
kernel void square( global float* input, global float* output ) {
    
    // Obtain the NO. of the dispatched OpenCL GPU Core
    size_t k = get_global_id( 0 );
    
    // For each OpenCL kernel
    output[ k ] = input[ k ] * input[ k ];
}


//
// Tends to make it looks like this form
//
//UtilizingPresentPollyPlusOpenCLModifications() {
//
//  int i, j, k, m;
//  size_t id = get_global_id( 0 );
//
//  if( id == 0 )
//      for( i = 0 ; i < 2 ; i++ )
//          output[ i ] = input[ i ] * input[ i ];
//
//  if( id == 1 )
//      for( j = 2 ; j < 4 ; j++ )
//          output[ j ] = input[ j ] * input[ j ];
//
//  if( id == 2 )
//      for( k = 4 ; k < 6 ; k++ )
//          output[ k ] = input[ k ] * input[ k ];
//
//  if( id == 3 )
//      for( m = 6 ; m < 8 ; m++ )
//          output[ m ] = input[ m ] * input[ m ];
//
//}
//


