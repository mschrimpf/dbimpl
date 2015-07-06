#include <iostream>
#include <cstdlib>
#include <atomic>
#include <tbb/tbb.h>
#include <unordered_map>
#include <utility>
#include <ext/mt_allocator.h>
#include "../../src/assignment6_parallelhashjoin/ChainingBloomHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/ChainingLockingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/LinearProbingHT.cpp"
#include "../../src/assignment6_parallelhashjoin/MatrixHT.cpp"

using namespace tbb;
//using namespace std;


/**
 * Notes:
 *  > map without locking: split ordered list
 *  > insert from the front
 *
 *  Constraints:
 *  > 6 cores Sandy Bridge
 *  > every relation up to 100 million tuples
 *  > max memory: 48 byte / tuple (build side)
 *  > huge pages = ON (2 MB)
 *  > ranking
 *    - best algorithm of all three
 *    - 2 to 3 different sizes but the biggest size will be ranked
 */

int main(int argc,char** argv) {
  uint64_t sizeR = atoi(argv[1]);
  uint64_t sizeS = atoi(argv[2]);
  unsigned threadCount = atoi(argv[3]);

  task_scheduler_init init(threadCount);

  // Init build-side relation R with random data
  uint64_t* R=static_cast<uint64_t*>(malloc(sizeR*sizeof(uint64_t)));
  parallel_for(blocked_range<size_t>(0, sizeR), [&](const blocked_range<size_t>& range) {
    unsigned int seed=range.begin();
    for (size_t i=range.begin(); i!=range.end(); ++i)
      R[i]=rand_r(&seed)%sizeR;
  });

  // Init probe-side relation S with random data
  uint64_t* S=static_cast<uint64_t*>(malloc(sizeS*sizeof(uint64_t)));
  parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
    unsigned int seed=range.begin();
    for (size_t i=range.begin(); i!=range.end(); ++i)
      S[i]=rand_r(&seed)%sizeR;
  });

  // STL
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();
    std::unordered_multimap<uint64_t,uint64_t, std::hash<uint64_t >, std::equal_to<uint64_t>,
            __gnu_cxx::__mt_alloc<std::pair<const uint64_t, uint64_t >>> ht(sizeR);
    for (uint64_t i=0; i<sizeR; i++)
      ht.emplace(R[i],0);
    tick_count probeTS=tick_count::now();
    std::cout << "STL      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
      uint64_t localHitCounter=0;
      for (size_t i=range.begin(); i!=range.end(); ++i) {
        auto range=ht.equal_range(S[i]);
        for (std::unordered_multimap<uint64_t,uint64_t>::iterator it=range.first; it!=range.second; ++it)
          localHitCounter++;
      }
      hitCounter+=localHitCounter;
    });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }

  // Test you implementation here... (like the STL test above)

  // ChainingBloomHT
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();

    ChainingBloomHT ht(sizeR);
    ChainingBloomHT::Entry* data = new ChainingBloomHT::Entry[sizeR];
    for (uint64_t i=0; i<sizeR; i++){
      data[i].key = R[i];
      data[i].next = nullptr;
      ht.insert(&data[i]);
    }
    tick_count probeTS=tick_count::now();
    std::cout << "ChainingBloomHT      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
        for (size_t i=range.begin(); i!=range.end(); ++i) {
          hitCounter+=ht.lookup(S[i]);
        }
    });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }

  // ChainingHT
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();

    ChainingHT ht(sizeR);
    ChainingHT::Entry * data = new ChainingHT::Entry[sizeR];
    for (uint64_t i=0; i<sizeR; i++){
      data[i].key = R[i];
      data[i].next = nullptr;
      ht.insert(&data[i]);
    }
    tick_count probeTS=tick_count::now();
    std::cout << "ChainingHT      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
        for (size_t i=range.begin(); i!=range.end(); ++i) {
          hitCounter+=ht.lookup(S[i]);
        }
    });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }

  // ChainingLocking
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();
    ChainingLockingHT ht(sizeR);
    ChainingLockingHT::Entry * data = new ChainingLockingHT::Entry [sizeR];
    for (uint64_t i=0; i<sizeR; i++){
      data[i].key = R[i];
      data[i].next = nullptr;
      ht.insert(&data[i]);
    }
    tick_count probeTS=tick_count::now();
    std::cout << "ChainingLockingHT      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
        for (size_t i=range.begin(); i!=range.end(); ++i) {
          hitCounter += ht.lookup(S[i]);
        }
        });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }

  // LinearProbingHT
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();

    LinearProbingHT ht(sizeR);
    LinearProbingHT::Entry * data  = new LinearProbingHT::Entry[sizeR];
    for (uint64_t i=0; i<sizeR; i++){
      data[i].key = R[i];
      data[i].marker = false;

      ht.insert(&data[i]);
    }

    tick_count probeTS=tick_count::now();
    std::cout << "LinearProbingHT      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
        for (size_t i=range.begin(); i!=range.end(); ++i) {
          hitCounter+=ht.lookup(S[i]);
        }
    });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }
  // MatrixHT
  {
    // Build hash table (single threaded)
    tick_count buildTS=tick_count::now();
    MatrixHT ht(sizeR);
    MatrixHT::Entry * data = new MatrixHT::Entry[sizeR];
    for (uint64_t i=0; i<sizeR; i++){
      data[i].key = R[i];
      data[i].next = nullptr;
      ht.insert(&data[i]);
    }

    tick_count probeTS=tick_count::now();
    std::cout << "MatrixHT      build:" << (sizeR/1e6)/(probeTS-buildTS).seconds() << "MT/s " << std::endl;

    // Probe hash table and count number of hits
    std::atomic<uint64_t> hitCounter;
    hitCounter=0;
    parallel_for(blocked_range<size_t>(0, sizeS), [&](const blocked_range<size_t>& range) {
        for (size_t i=range.begin(); i!=range.end(); ++i) {
          hitCounter+=ht.lookup(S[i]);
        }
    });
    tick_count stopTS=tick_count::now();
    std::cout << "probe: " << (sizeS/1e6)/(stopTS-probeTS).seconds() << "MT/s "
    << "total: " << ((sizeR+sizeS)/1e6)/(stopTS-buildTS).seconds() << "MT/s "
    << "count: " << hitCounter << std::endl;
  }
  return 0;
}
