////////////////////////////////////////////////////////////////////
//File: basic_environ.c
//Description: base file for environment exercises with openCL
//
// 
////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif
  
// check error, in such a case, it exits

void cl_error(cl_int code, const char *string){
	if (code != CL_SUCCESS){
		printf("%d - %s\n", code, string);
	    exit(-1);
	}
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  int err;                            	// error code returned from api calls
  size_t t_buf = 50;			// size of str_buffer
  char str_buffer[t_buf];		// auxiliary buffer	
  size_t e_buf;				// effective size of str_buffer in use
  
  size_t global_size;                      	// global domain size for our calculation
  size_t local_size;                       	// local domain size for our calculation

  const cl_uint num_platforms_ids = 10;				// max of allocatable platforms
  cl_platform_id platforms_ids[num_platforms_ids];		// array of platforms
  cl_uint n_platforms;						// effective number of platforms in use
  const cl_uint num_devices_ids = 10;				// max of allocatable devices
  cl_device_id devices_ids[num_platforms_ids][num_devices_ids];	// array of devices
  cl_uint n_devices[num_platforms_ids];				// effective number of devices in use for each platform
	
  cl_device_id device_id;             				// compute device id 
  cl_context context;                 				// compute context
  cl_command_queue command_queue;     				// compute command queue
    

  // 1. Scan the available platforms:
  err = clGetPlatformIDs (num_platforms_ids, platforms_ids, &n_platforms);
  cl_error(err, "Error: Failed to Scan for Platforms IDs");
  printf("Number of available platforms: %d\n\n", n_platforms);

  for (int i = 0; i < n_platforms; i++ ){
	// ***Task***: print on the screen the name, host_timer_resolution, vendor, version, ...

    err = clGetPlatformInfo(platforms_ids[i], CL_PLATFORM_NAME, sizeof(str_buffer), &str_buffer, NULL);
    cl_error (err, "Error: Failed to get info of the platform\n");
    printf( "\t[%d]-Platform Name: %s\n", i, str_buffer);
    
    err = clGetPlatformInfo(platforms_ids[i], CL_PLATFORM_VERSION, sizeof(str_buffer), &str_buffer, NULL);
    cl_error (err, "Error: Failed to get info of the platform\n");
    printf( "\t[%d]-Platform Version: %s\n", i, str_buffer);
    
    err = clGetPlatformInfo(platforms_ids[i], CL_PLATFORM_VENDOR, sizeof(str_buffer), &str_buffer, NULL);
    cl_error (err, "Error: Failed to get info of the platform\n");
    printf( "\t[%d]-Platform Vendor: %s\n", i, str_buffer);
    
    cl_ulong host_timer_res;
    err = clGetPlatformInfo(platforms_ids[i], CL_PLATFORM_HOST_TIMER_RESOLUTION, sizeof(host_timer_res), &host_timer_res, NULL);
    cl_error (err, "Error: Failed to get info of the platform\n");
    printf( "\t[%d]-Platform Host Timer Resolution: %d\n", i, host_timer_res);
    
  }
  printf("\n");


   // ***Task***: print on the screen the cache size, global mem size, local memsize, max work group size, profiling timer resolution and ... of each device
  // 2. Scan for devices in each platform
  for (int i = 0; i < n_platforms; i++ ){
    err = clGetDeviceIDs( platforms_ids[i], CL_DEVICE_TYPE_ALL , num_devices_ids, devices_ids[i], &(n_devices[i]));
    cl_error(err, "Error: Failed to Scan for Devices IDs");
    printf("\t[%d]-Platform. Number of available devices: %d\n", i, n_devices[i]);

    for(int j = 0; j < n_devices[i]; j++){
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_NAME, sizeof(str_buffer), &str_buffer, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device name");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_NAME: %s\n", i, j,str_buffer);

      cl_uint max_compute_units_available;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units_available), &max_compute_units_available, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device max compute units available");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_MAX_COMPUTE_UNITS: %i\n", i, j, max_compute_units_available);
      
	  // Global cache size:
	  cl_ulong global_cache_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(global_cache_size), &global_cache_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting CL_DEVICE_GLOBAL_MEM_CACHE_SIZE");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_GLOBAL_MEM_CACHE_SIZE: %lu\n", i, j,global_cache_size);
      
      // Global mem size:
      cl_ulong global_mem_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(global_mem_size), &global_mem_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting CL_DEVICE_GLOBAL_MEM_SIZE");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_GLOBAL_MEM_SIZE: %lu\n", i, j,global_mem_size);
      
      // Local mem size:
      cl_ulong local_mem_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_mem_size), &local_mem_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting CL_DEVICE_LOCAL_MEM_SIZE");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_LOCAL_MEM_SIZE: %lu\n", i, j,local_mem_size);
      
      // MAX_WORK_GROUP_SIZE:
      size_t max_work_group_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting MAX_WORK_GROUP_SIZE");
      printf("\t\t [%d]-Platform [%d]-Device MAX_WORK_GROUP_SIZE: %i\n", i, j,max_work_group_size);
      
      // Profiling timing resolution
      size_t prof_timing_res;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(prof_timing_res), &prof_timing_res, NULL);
      cl_error(err, "clGetDeviceInfo: Getting CL_DEVICE_PROFILING_TIMER_RESOLUTION");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_PROFILING_TIMER_RESOLUTION: %i\n", i, j,prof_timing_res);
    }
  }	
  


  // 3. Create a context, with a device
  size_t index_platform_context = 1;
  cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms_ids[index_platform_context], 0};
  //~ context = clCreateContext(properties, /***???***/, NULL, NULL, &err);
  context = clCreateContext(properties, n_devices[index_platform_context], devices_ids[index_platform_context], NULL, NULL, &err);
  cl_error(err, "Failed to create a compute context\n");
	
  printf("\n\nCreated a context with the Platform [%d] \n", index_platform_context);


  // 4. Create a command queue
  cl_command_queue_properties proprt[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
  //~ command_queue = clCreateCommandQueueWithProperties( /***???***/, proprt, &err);
  command_queue = clCreateCommandQueueWithProperties(context, devices_ids[index_platform_context][0], proprt, &err);
  cl_error(err, "Failed to create a command queue\n");



  //____________________________________________________________________________________________________________________________


  return 0;
}

