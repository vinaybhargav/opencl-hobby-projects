#include "../simpleCL/simpleCL.h"
#include <bits/stdc++.h>

using namespace std;

ifstream f("data.in");
ofstream g("data.out");

int main()
{
    size_t global_size[2], local_size[2]; // Arguments dimension setters. global_size for kernel and local_size for host
    int found, mat_width, mat_height;
    sclHard hardware;
    sclSoft software;

    f>>mat_width;
    f>>mat_height;

    int matA[mat_width][mat_height],matB[mat_width][mat_height],matC[mat_width][mat_height];

    for(int i = 0; i < mat_height; i++)
       for(int j = 0; j < mat_width; j++)
            f>>matA[i][j];

    for(int i = 0; i < mat_height; i++)
       for(int j = 0; j < mat_width; j++)
            f>>matB[i][j];

    // Get the hardware
    hardware = sclGetGPUHardware(0, &found);
    // Get the software
    software = sclGetCLSoftware( "matrix_add.cl", "matrix_add", hardware );

    // Set NDRange dimensions
    global_size[0] = mat_width*mat_height; global_size[1] = 1;
    local_size[0] = global_size[0]; local_size[1] = 1;

    cl_mem mem1, mem2, mem3; // declare memory for the kernel arguments.

    mem1 = sclMalloc(hardware, CL_MEM_READ_ONLY, sizeof(matA)); // Allocate memory for the kernel arguments

    mem2 = sclMalloc(hardware, CL_MEM_READ_ONLY, sizeof(matA));

    mem3 = sclMalloc(hardware, CL_MEM_WRITE_ONLY, sizeof(matA));

    sclSetKernelArg(software, 0, sizeof(mem1), &mem1);   // Set mem1, mem2 as buffers of matA and matB arguments in kernel.

    sclSetKernelArg(software, 1, sizeof(mem2), &mem2);

    sclSetKernelArg(software, 2, sizeof(mem3), &mem3);

    sclWrite(hardware, sizeof(matA), mem1, matA);             // Write content of input arrays to mem1 and mem2 and queue it to be used by kernel.

    sclWrite(hardware, sizeof(matA), mem2, matB);

    sclEnqueueKernel(hardware, software, global_size, local_size);  // Add the kernel to the queue.

    sclRead(hardware, sizeof(matA), mem3, matC);             // Add the output array to the queue.

    sclFinish(hardware);                                      // Run the kernel code.

    sclReleaseMemObject(mem1); sclReleaseMemObject(mem2); sclReleaseMemObject(mem3);             // Free the allocated memories.

    sclReleaseClHard(hardware);
    sclReleaseClSoft(software);

    for(int i = 0; i < mat_height; i++)
    {
        for(int j = 0; j < mat_width; j++)
            g<<matC[i][j]<<" ";

        g<<"\n";
    }

}
