# Datenbanksysteme und moderne CPU-Architekturen SS2015
[Lecture webpage] (www-db.in.tum.de/teaching/ss15/moderndbs/index.shtml)

Exercise implementation by 

* Martin Schrimpf <martin.schrimpf@tum.de> and 
* Daniel Langerenken <daniel.langerenken@gmail.com>

C++ Code style according to the [Google Styleguide] (https://google-styleguide.googlecode.com/svn/trunk/cppguide.html)


## Assignment 1: External Sort
To run the test sorting 5 GB of data:

    # inside dbimpl directory
    # generate data
    cd lib/data_generator
    make
    ./gen ../../resources/test_data 83886080 # 5 GB (5 * 1024 * 1024 * 1024 / 64)
    
    # build executable
    cd ../..
    cd src/assignment1
    cmake .
    
    # run
    ./assignment1 ../../resources/test_data ../../resources/test_data_sorted 10

## Assignment 2: Buffer Manager
To run the buffertest:

    # inside dbimpl directory
    cd testing/buffer
    cmake .
    make
    # ./buffertest *pagesOnDisk* *pagesInRam* *threadCount*, e.g.
    ./buffertest 100 10 4

## Assignment 3: Relations and Slotted Pages
To run the unit tests:

    # inside dbimpl directory
    cmake .
    
    cd testing/relations
    make -C ../.. relations_tests
    ./relations_tests
    cd ../..
    
    cd testing/slotted_pages
    make -C ../.. slotted_pages_unit_tests
    ./slotted_pages_unit_tests

## Assignment 4: B+ Tree
To run the unit tests and given integration test:

    # inside dbimpl directory
    cmake .
    cd testing/btree
    
    make -C ../.. btree_unit_tests
    ./btree_unit_tests
    
    make -C ../.. BTreeTest
    ./BTreeTest
