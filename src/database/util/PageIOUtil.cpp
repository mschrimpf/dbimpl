//
// Created by Daniel on 30.04.2015.
//

#include <io.h>
#include <fcntl.h>
#include "PageIOUtil.h.h"


void FileIOUtil::read(int pageId, int segmentId, void *data, int len) {
    int magicCalculation = pageId + segmentId;
    int fd = -1;
    if (fdMap.find(magicCalculation) != fdMap.end()){
        fd = fdMap[magicCalculation];
    }else{
        fd = open("address", O_CREAT);
        fdMap.insert(magicCalculation, fd);
    }
    //doReadStuff here
}

void FileIOUtil::write(int pageId, int segmentId, void *data) {
    //do write stuff here
}

FileIOUtil::FileIOUtil() {

}

FileIOUtil::~FileIOUtil() {
    for (std::pair fd : fdMap){
        close(fd.second);
    }
    fdMap.clear();
}
