

kernel void square( global float *input, global float *output ) {
    
    size_t k = get_global_id( 0 );
    output[ k ] = input[ k ] * input[ k ];
}

