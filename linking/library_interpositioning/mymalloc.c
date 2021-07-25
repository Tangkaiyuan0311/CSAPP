#ifdef COMPILETIME

#include<stdio.h>
#include<malloc.h> //standard headerfile

/*malloc wrapper*/
void* mymalloc(size_t size){
  void* ptr = malloc(size);//library malloc
  printf("malloc(%d) = %p\n", (int)size, ptr);
  return ptr;
}

/*free wrapper*/
void myfree(void* ptr){
  free(ptr);//library free
  printf("free(%p)\n", ptr);
}


#endif


#ifdef LINKTIME

#include<stdio.h>

void* __real_malloc(size_t);
void __real_free(void*);

/*malloc wrapper*/
void* __wrap_malloc(size_t size){
  void* ptr = __real_malloc(size);//library malloc
  printf("malloc(%d) = %p\n", (int)size, ptr);
  return ptr;
}

/*free wrapper*/
void __wrap_free(void* ptr){
  __real_free(ptr);//library free
  printf("free(%p)\n", ptr);
}


#endif


#ifdef RUNTIME

#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>

/*malloc warpper function*/
void* malloc(size_t size){
  void* (*mallocp)(size_t);
  char* error;
  mallocp = dlsym(RTLD_NEXT, "malloc"); //find the next occurrence of the disered symbol in the search order after the current object, which is the library malloc in this case
  if ((error = dlerror()) != NULL){
    fputs(error, stderr);
    exit(1);
  }
  char* ptr = mallocp(size);
  //figure out the reason why stdout will trigger segmentation fault
  //printf will allcate a buffer size of 1024 bytes, triggering a call of malloc, which again goes to the malloc wrapper, again tries to use malloc to allocate the buffer
  //printf("malloc(%d) = %p\n", (int)size, ptr);
  fprintf(stderr, "malloc(%d) = %p\n", (int)size, ptr);
  return ptr;
}

/*free wrapper*/
void free(void* ptr){
  void (*freep)(void*);
  char* error;
  freep = dlsym(RTLD_NEXT, "free");//get library free
  if ((error = dlerror()) != NULL){
    fputs(error, stderr);
    exit(1);
  }
  freep(ptr);
  //printf("free(%p)\n", ptr);
  fprintf(stderr, "free(%p)\n", ptr);
}
#endif
