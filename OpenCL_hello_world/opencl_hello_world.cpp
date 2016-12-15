#include "../simpleCL/simpleCL.h"
#define DEVICE 0

int main()
{
   char host_buf_input[]="Hello, World!";
   size_t global_size[2], local_size[2]; // Arguments dimension setters. global_size for kernel and local_size for host
   int found, worksize;
   sclHard hardware;
   sclSoft software;

   // Target buffer just so we show we got the data from OpenCL
   worksize = strlen(host_buf_input);
   char host_buf_output[worksize];
   host_buf_output[0]='?';
   host_buf_output[worksize]=0;

   // Get the hardware
   hardware = sclGetGPUHardware(DEVICE, &found);
   // Get the software
   software = sclGetCLSoftware( "example.cl", "example", hardware );

   if(software.kernel == NULL)
        printf("Error \n");
   // Set NDRange dimensions
   global_size[0] = strlen(host_buf_input); global_size[1] = 1;
   local_size[0] = global_size[0]; local_size[1] = 1;

   cl_mem mem1, mem2; // declare memory for the kernel arguments.

   mem1 = sclMalloc(hardware, CL_MEM_READ_ONLY, worksize); // Allocate memory for the kernel arguments

   mem2 = sclMalloc(hardware, CL_MEM_WRITE_ONLY, worksize);

   sclSetKernelArg(software, 0, sizeof(mem1), &mem1);   // Set mem1 and mem2 as buffers of buf and buf2 arguments in kernel.

   sclSetKernelArg(software, 1, sizeof(mem2), &mem2);

   sclWrite(hardware, worksize, mem1, host_buf_input);             // Write content of input buffer to mem1 and queue it to be used by kernel.

   sclEnqueueKernel(hardware, software, global_size, local_size);  // Add the kernel to the queue.

   sclRead(hardware, worksize, mem2, host_buf_output);             // Add the output buffer to the queue.

   sclFinish(hardware);                                            // Run the kernel code.

   // Finally, output out happy message.
   printf("\n");
   puts(host_buf_output);

   sclReleaseMemObject(mem1); sclReleaseMemObject(mem2);           // Free the allocated mem1 and mem2 after use.
   getchar();                                                      // wait for user input to exit.

}
