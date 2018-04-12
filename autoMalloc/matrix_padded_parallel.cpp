#include <pthread.h>
#include <stdio.h>
#include "matrix.h"
#include <inttypes.h>
#include <sys/stat.h>
#include <string>

#ifdef PADDING_DISTANCE
#define DISTANCE PADDING_DISTANCE
#else
#define DISTANCE 1
#endif

inline bool file_exists(const std::string& name)
{
  struct stat buffer;
  return ( stat ( name.c_str(), &buffer  ) == 0  );
}

template<class Type>
class SharedVariableWrapper
{
public:
  Type *array;
  int *changedIndex;
  int size;
  int totalSize;
  bool logMode;
  int *threadWriteCounts[CORE_COUNT+1];
  int *threadReadCounts[CORE_COUNT+1];
  SharedVariableWrapper(int sz)
  {
    if ( file_exists("trace.out") )
    {
      logMode = false;
      FILE *fp = fopen("trace.out", "r");
      long unsigned startAddress;
      long unsigned sizeOfType;
      int paddingRequiredCount;
      fscanf(fp, "%lu", &startAddress);
      fscanf(fp, "%lu", &sizeOfType);
      fscanf(fp, "%d", &paddingRequiredCount);
      int *paddingRequiredIndices = new int[paddingRequiredCount];

      for(int i = 0; i < paddingRequiredCount; i++)
      {
        fscanf(fp, "%d", &paddingRequiredIndices[i]);
      }

      size = sz;
      changedIndex = new int[size];
      totalSize = size + (paddingRequiredCount * 63);
      array = new Type[totalSize];
      int currentIndex = 0;
      int nextPadding = 0;
      for(int i = 0; i < size; i++)
      {
        changedIndex[i] = currentIndex;
        if( (nextPadding < paddingRequiredCount) && (paddingRequiredIndices[nextPadding] == i) )
        {
          currentIndex += 64;
          nextPadding += 1;
        }
        else
        {
          currentIndex += 1;
        }
      }

      delete [] paddingRequiredIndices;
    }
    else
    {
      logMode = true;
      size = sz;
      array = new Type[size];
      for(int i=0; i<=CORE_COUNT; i++)
      {
        threadWriteCounts[i]=new int[size];
        threadReadCounts[i]=new int[size];
        for(int j = 0; j<=size;j++)
        {
          threadWriteCounts[i][j]=0;
          threadReadCounts[i][j]=0;
        }
      }
    }
   
  }
  inline Type read(int index, int threadId)
  {
    if(logMode)
    {
      threadReadCounts[threadId][index]+=1;
      return array[index];
    }
    else
    {
      return array[changedIndex[index]];
    }
  }
  inline void write(int index, Type value, int threadId)
  {
    if(logMode)
    {
      threadWriteCounts[threadId][index]+=1;
      array[index] = value;
    }
    else
    {
      array[changedIndex[index]] = value;
    }
  }
  inline void increment(int index, Type value, int threadId)
  {
    if(logMode)
    {
      threadReadCounts[threadId][index]+=1;
      threadWriteCounts[threadId][index]+=1;
      array[index] += value;
    }
    else
    {
      array[changedIndex[index]] += value;
    }
  }
  inline bool isSharedBySameThreads(int index)
  {
    bool returnResult = true;
    for(int i = 0; i < CORE_COUNT+1; i++)
    {
      if(threadWriteCounts[i][index] == 0 && threadWriteCounts[i][index+1] != 0)
      {
        return false;
      }
      if(threadWriteCounts[i][index] != 0 && threadWriteCounts[i][index+1] == 0)
      {
        return false;
      }
    }
    return returnResult;
  }
  inline bool isOnTheSameCacheLine(int index)
  {
    uintptr_t p1 = (uintptr_t) &array[index];
    uintptr_t p2 = (uintptr_t) &array[index+1];
    if ( (p1/64) == (p2/64) ) return true;
    return false;
  }
  ~SharedVariableWrapper()
  {
    if(logMode){
      bool *paddingRequired = new bool[size];
      int paddingRequiredCount = 0;
      for(int i = 0; i < size-1; i++)
      {
        if(isSharedBySameThreads(i))
        {
          paddingRequired[i] = false;
        }
        else
        {
          if(isOnTheSameCacheLine(i))
          {
            paddingRequired[i] = true;
            paddingRequiredCount += 1;
          }
          else
          {
            paddingRequired[i] = false;
          }
        }
      }
      paddingRequired[size-1]=false;
      FILE *fp = fopen("trace.out", "w");
      fprintf(fp,"%" PRIuPTR  "\n",(uintptr_t)array);
      fprintf(fp, "%lu\n", sizeof(Type));
      fprintf(fp, "%d\n", paddingRequiredCount);
      for(int i = 0; i <size; i++)
      {
        if(paddingRequired[i]==true)fprintf(fp, "%d\n", i);
      }
      fclose(fp);
      delete [] paddingRequired;
      for(int i=0; i<=CORE_COUNT; i++)
      {
        delete [] threadWriteCounts[i];
        delete [] threadReadCounts[i]; 
      }
    } else {
      delete [] changedIndex;
    }
    delete [] array;
  }
};

int  a[SIZE][SIZE], b[SIZE][SIZE];
SharedVariableWrapper<int> c(SIZE*SIZE*DISTANCE);

struct padded
{
int value;
int padding[15];
};

void *run(void *index)
{
  int i = *(int *)index;
  int j,k;
  int mod = SIZE % CORE_COUNT;
  int number = SIZE / CORE_COUNT;
  int start;
  if(i < mod)
  {
    number += 1;
    start = i * number;
  }
  else
  {
    start = (i * number)+mod;
  }
  for(int l = start; l - start < number; l++)
  { 
    for(j=0;j<SIZE;j++)
    {
      //int threadLocal = 0;
      for(k=0;k<SIZE;k++)
      {
        //c[i][j].set(a[i][k]*b[k][j]+c[i][j].get());
        //c[i][j].incr(a[i][k]*b[k][j]);
        //c[l][j*DISTANCE]+=a[l][k]*b[k][j];
        //threadLocal += a[l][k]*b[k][j];
        //c.write(l*SIZE+j*DISTANCE,a[l][k]*b[k][j]+c.read(l*SIZE+j*DISTANCE, i+1),i+1);
        c.increment(l*SIZE+j*DISTANCE,a[l][k]*b[k][j], i+1);
      }
      //c.write(l*SIZE+j*DISTANCE, threadLocal, i+1);
      //c[l][j].value = threadLocal; 
    }
  }
}

void printResult()
{
  int i,j;
  FILE *fp = fopen("padded_parallel_result.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%d ",c.read(i*SIZE+j*DISTANCE, 0));
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
}

int main(int argc, char *argv[])
{
  int i,j,k;
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      a[i][j]=i+j;
      b[i][j]=i-j;
      c.write(i*SIZE+j*DISTANCE,0,0);
    }
  }
  //printf("%lu\n%p\n%p\n",sizeof(PaddedInt),&c[0][0],&c[0][1]);
  pthread_t threads[THREAD_SIZE];
  //int num=0;
  struct padded tmp[THREAD_SIZE];
  for(i=0;i<THREAD_SIZE;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i].value=i;
    pthread_create(&threads[i], NULL, run, &tmp[i].value);
  }
  for(i=0;i<THREAD_SIZE;i++)pthread_join(threads[i], NULL);
  if(argc>1)printResult();
  return 0;
}
