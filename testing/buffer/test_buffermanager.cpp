#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include "../../src/database/buffer/BufferManager.hpp"
#include "../../src/database/buffer/BufferFrame.hpp"

using namespace std;

int pagesOnDisk = 100000;

int testZeroingBuffer(int argc, char** argv){
   BufferManager* bufferManager = new BufferManager(10);
   unsigned totalCountOnDiskZero = 0;
   for (unsigned i=0; i<pagesOnDisk; i++) {
      BufferFrame& bf = bufferManager->fixPage(i,false);
      unsigned int pageValue = reinterpret_cast<unsigned*>(bf.getData())[0];
      cout << "Read value " << pageValue << " from page " << i << endl;
      totalCountOnDiskZero+= pageValue;
      bufferManager->unfixPage(bf, false);
   }
   if (0==totalCountOnDiskZero) {
      cout << "[ZERO TEST] successful" << endl;
      delete(bufferManager);
   } else {
      cerr << "[ZERO TEST] error: expected " << 0 << " but got " << totalCountOnDiskZero << endl;
      delete bufferManager;
      assert(false);
   }
}

int testSingleInsertion(int argc, char** argv){
   BufferManager* bufferManager = new BufferManager(10);
   BufferFrame& bf = bufferManager->fixPage(1, true);
   unsigned * pageValue = reinterpret_cast<unsigned *> (bf.getData());
   *pageValue = 42;
   bufferManager->unfixPage(bf, true);
   delete bufferManager;

   bufferManager = new BufferManager(10);
   BufferFrame& bf2 = bufferManager->fixPage(1, false);
   pageValue = reinterpret_cast<unsigned *> (bf2.getData());
   if (*pageValue == 42){
      cout << "[SingleInsertion TEST] successful" <<endl;
      delete(bufferManager);
   }else{
      cerr << "[SingleInsertion TEST] error: expected " << 42 << " but got " << *pageValue << endl;
      delete bufferManager;
      assert(false);
   }
}

int testSameFrame(int argc, char** argv){
   BufferManager* bufferManager = new BufferManager(10);
   BufferFrame& bf = bufferManager->fixPage(1, true);
   unsigned * pageValue = reinterpret_cast<unsigned *> (bf.getData());
   *pageValue = 42;
   bufferManager->unfixPage(bf, true);
   BufferFrame& bf2 = bufferManager->fixPage(1, false);
   pageValue = reinterpret_cast<unsigned *> (bf2.getData());
   (*pageValue)++;
   bufferManager->unfixPage(bf2, true);
   delete(bufferManager);

   bufferManager = new BufferManager(10);
   BufferFrame& bf3 = bufferManager->fixPage(1, false);
   pageValue = reinterpret_cast<unsigned *> (bf3.getData());
   if (*pageValue == 43){
      cout << "[testSameFrame TEST] successful" <<endl;
      delete(bufferManager);
   }else{
      cerr << "[testSameFrame TEST] error: expected " << 43 << " but got " << *pageValue << endl;
      delete bufferManager;
      assert(false);
   }
}

int main(int argc, char** argv) {
   testZeroingBuffer(argc, argv);
   testSingleInsertion(argc, argv);
   testSameFrame(argc, argv);
}
