#include "../simpleCL/simpleCL.h"
#include <bits/stdc++.h>

using namespace std;

ifstream f("data.in");
ofstream g("data.out");

int main()
{
    size_t global_size[2], local_size[2]; // Arguments dimension setters. global_size for kernel and local_size for host
    int found, matA_row, matA_col, matB_row, matB_col;
    sclHard hardware;
    sclSoft software;

    f>>matA_row;
    f>>matA_col;

    f>>matB_row;
    f>>matB_col;

    int matA[matA_row][matA_col],matB[matB_row][matB_col];

    if(matA_col == matB_row)
    {
        int matC[matA_row][matB_col];

        for(int i = 0; i < matA_row; i++)
           for(int j = 0; j < matA_col; j++)
                f>>matA[i][j];

        for(int i = 0; i < matB_row; i++)
           for(int j = 0; j < matB_col; j++)
                f>>matB[i][j];

        // Get the hardware
        hardware = sclGetGPUHardware(0, &found);
        // Get the software
        software = sclGetCLSoftware( "matrix_mul.cl", "matrix_mul", hardware );

        // Set NDRange dimensions
        global_size[0] = matA_row; global_size[1] = matB_col;
        local_size[0] = global_size[0]; local_size[1] = global_size[1];

        cl_mem mem1, mem2, mem3; // declare memory for the kernel arguments.

        mem1 = sclMalloc(hardware, CL_MEM_READ_ONLY, sizeof(matA)); // Allocate memory for the kernel arguments

        mem2 = sclMalloc(hardware, CL_MEM_READ_ONLY, sizeof(matB));

        mem3 = sclMalloc(hardware, CL_MEM_WRITE_ONLY, sizeof(matC));

        sclSetKernelArg(software, 0, sizeof(mem1), &mem1);   // Set mem1, mem2 as buffers of matA and matB arguments in kernel.

        sclSetKernelArg(software, 1, sizeof(mem2), &mem2);

        sclSetKernelArg(software, 2, sizeof(mem3), &mem3);

        sclSetKernelArg(software, 3, sizeof(int), &matA_col);

        sclSetKernelArg(software, 4, sizeof(int), &matB_col);

        sclWrite(hardware, sizeof(matA), mem1, matA);             // Write content of input arrays to mem1 and mem2 and queue it to be used by kernel.

        sclWrite(hardware, sizeof(matB), mem2, matB);

        sclEnqueueKernel(hardware, software, global_size, local_size);  // Add the kernel to the queue.

        sclRead(hardware, sizeof(matC), mem3, matC);             // Add the output array to the queue.

        sclFinish(hardware);                                      // Run the kernel code.

        sclReleaseMemObject(mem1); sclReleaseMemObject(mem2); sclReleaseMemObject(mem3);             // Free the allocated memories.

        sclReleaseClHard(hardware);
        sclReleaseClSoft(software);

        for(int i = 0; i < matA_row; i++)
        {
            for(int j = 0; j < matB_col; j++)
                g<<matC[i][j]<<" ";

            g<<"\n";
        }
    }
    else
    {
        printf("Cannot multiply matrix, matA column and matB row don't match. \n");
    }


}
