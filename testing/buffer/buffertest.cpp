#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include "../../src/database/buffer/BufferManager.hpp"
#include "../../src/database/buffer/BufferFrame.hpp"

using namespace std;

BufferManager* bm;
unsigned pagesOnDisk;
unsigned pagesInRAM;
unsigned threadCount;
unsigned* threadSeed;
volatile bool stop=false;

unsigned randomPage(unsigned threadNum) {
   // pseudo-gaussian, causes skewed access pattern
   unsigned page=0;
   for (unsigned  i=0; i<20; i++)
      page+=rand_r(&threadSeed[threadNum])%pagesOnDisk;
   return page/20;
}

static void* scan(void *arg) {
   // scan all pages and check if the counters are not decreasing
   vector<unsigned> counters(pagesOnDisk, 0);

   while (!stop) {
      unsigned start = random()%(pagesOnDisk-10);
      for (unsigned page=start; page<start+10; page++) {
         BufferFrame& bf = bm->fixPage(page, false);
         unsigned newcount = reinterpret_cast<unsigned*>(bf.getData())[0];
         assert(counters[page]<=newcount);
         counters[page]=newcount;
         bm->unfixPage(bf, false);
      }
   }

   return NULL;
}

static void* readWrite(void *arg) {
   // read or write random pages
   uintptr_t threadNum = reinterpret_cast<uintptr_t>(arg);

   uintptr_t count = 0;
   for (unsigned i=0; i<100000/threadCount; i++) {
      unsigned int pageId = randomPage(threadNum);
      bool isWrite = rand_r(&threadSeed[threadNum])%128<10;
      cout <<"[Thread#" << threadNum << "] fixing page " << pageId
            << " (" << (isWrite ? "write" : "read") << ")" << endl;
      BufferFrame& bf = bm->fixPage(pageId, isWrite);

      if (isWrite) {
         count++;
         reinterpret_cast<unsigned*>(bf.getData())[0]++;
      }
      bm->unfixPage(bf, isWrite);
   }

   return reinterpret_cast<void*>(count);
}

int main(int argc, char** argv) {
   if (argc==4) {
      pagesOnDisk = atoi(argv[1]);
      pagesInRAM = atoi(argv[2]);
      threadCount = atoi(argv[3]);
   } else {
      cerr << "usage: " << argv[0] << " <pagesOnDisk> <pagesInRAM> <threads>" << endl;
      exit(1);
   }

   cout << "start buffer-test with args: "
   << "pagesOnDisk " << argv[1]
   << " pagesInRam " << argv[2]
   << " threadCount " << argv[3] << endl;

   threadSeed = new unsigned[threadCount];
   for (unsigned i=0; i<threadCount; i++)
      threadSeed[i] = i*97134;

   bm = new BufferManager(pagesInRAM);

   vector<pthread_t> threads(threadCount);
   pthread_attr_t pattr;
   pthread_attr_init(&pattr);

   // set all counters to 0
   cout <<"start: set all counters to 0" << endl;
   for (unsigned i=0; i<pagesOnDisk; i++) {
      BufferFrame& bf = bm->fixPage(i, true);
      reinterpret_cast<unsigned*>(bf.getData())[0]=0;
      bm->unfixPage(bf, true);
      cout <<"zero'ed data " << i << "/" << pagesOnDisk -1 << endl;
   }


   ///////////////////      ADDED     /////////////////////
   cout << "ZERO TEST" << endl;
   delete bm;
   bm = new BufferManager(pagesInRAM);
   unsigned totalCountOnDiskZero = 0;
   for (unsigned i=0; i<pagesOnDisk; i++) {
      BufferFrame& bf = bm->fixPage(i,false);
      unsigned int pageValue = reinterpret_cast<unsigned*>(bf.getData())[0];
      cout << "Read value " << pageValue << " from page " << i << endl;
      totalCountOnDiskZero+= pageValue;
      bm->unfixPage(bf, false);
   }
   if (0==totalCountOnDiskZero) {
      cout << "[ZERO TEST] successful" << endl;
   } else {
      cerr << "[ZERO TEST] error: expected " << 0 << " but got " << totalCountOnDiskZero << endl;
      delete bm;
      return 1;
   }
   ///////////////////      ADDED     ////////////////////


   cout <<"end: set all counters to 0" << endl;

   if (pagesOnDisk < 11){
      cerr << "pages on disk has to be bigger than 10" <<endl;
      exit(1);
   }

   // start scan thread
   pthread_t scanThread;
   pthread_create(&scanThread, &pattr, scan, NULL);

   // start read/write threads
   for (unsigned i=0; i<threadCount; i++)
      pthread_create(&threads[i], &pattr, readWrite, reinterpret_cast<void*>(i));

   cout <<"threads started" << endl;
   // wait for read/write threads
   unsigned totalCount = 0;
   for (unsigned i=0; i<threadCount; i++) {
      void *ret;
      pthread_join(threads[i], &ret);
      totalCount+=reinterpret_cast<uintptr_t>(ret);
   }
   cout <<"threads ended" << endl;

   // wait for scan thread
   stop=true;
   cout << "waiting for scan thread" << endl;
   pthread_join(scanThread, NULL);
   cout <<"scan thread ended" << endl;

   // restart buffer manager
   cout << "restart buffer manager" << endl;
   delete bm;
   bm = new BufferManager(pagesInRAM);

   // check counter
   unsigned totalCountOnDisk = 0;
   for (unsigned i=0; i<pagesOnDisk; i++) {
      BufferFrame& bf = bm->fixPage(i,false);
      totalCountOnDisk+=reinterpret_cast<unsigned*>(bf.getData())[0];
      bm->unfixPage(bf, false);
   }
   if (totalCount==totalCountOnDisk) {
      cout << "test successful" << endl;
      delete bm;
      return 0;
   } else {
      cerr << "error: expected " << totalCount << " but got " << totalCountOnDisk << endl;
      delete bm;
      return 1;
   }
}
