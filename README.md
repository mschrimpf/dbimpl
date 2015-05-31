# Datenbanksysteme und moderne CPU-Architekturen SS2015
[Lecture webpage] (www-db.in.tum.de/teaching/ss15/moderndbs/index.shtml)

Exercise implementation by 

* Martin Schrimpf <martin.schrimpf@tum.de> and 
* Daniel Langerenken <daniel.langerenken@gmail.com>

C++ Code style according to the [Google Styleguide] (https://google-styleguide.googlecode.com/svn/trunk/cppguide.html)


## Assignment 1
Please follow these shell commands.

    # assuming we are in dbimpl directory
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

## Assignment 3
To run the unit tests:

    # in dbimpl directory
    cmake .
    
    cd testing/relations
    make -C ../.. relations_tests
    ./relations_tests
    cd ../..
    
    cd testing/slotted_pages
    make -C ../.. slotted_pages_unit_tests
    ./slotted_pages_unit_tests
