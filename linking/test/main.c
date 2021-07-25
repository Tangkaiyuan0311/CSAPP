#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>

int x[2] = {1,2};
int y[2] = {3,4};
int z[2];

int main(){
  void* handle; //handle to the shared library
  void (*addvec)(int*, int*, int*, int); //to receive the function pointer
  char* error;

  //load the shared library containing addvec
  handle = dlopen("./libvector.so", RTLD_LAZY||RTLD_GLOBAL);
  if (!handle){
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  //receive the symbol address
  addvec = dlsym(handle, "addvec");
  if ((error = dlerror())!= NULL){
    fprintf(stderr, "%s\n", error);
  }

  addvec(x, y, z, 2);
  printf("z = [%d %d]", z[0], z[1]);

  //unload the library
  if (dlclose(handle) < 0){
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  return 0;
}
    
    
    
