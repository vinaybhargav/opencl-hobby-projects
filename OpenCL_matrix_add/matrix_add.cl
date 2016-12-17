__kernel void matrix_add( __global int* matA, __global int* matB , __global int* matC){
       int x = get_global_id(0);      matC[x] = matA[x] + matB[x];

}
