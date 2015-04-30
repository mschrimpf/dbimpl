//
// Created by Daniel on 30.04.2015.
//

#ifndef PROJECT_FILEIOUTIL_H
#define PROJECT_FILEIOUTIL_H

#include <unordered_map>
#include "../buffer/IPageIO.h"

class PageIOUtil : public IPageIO {
private:
	std::unordered_map<int, int> fdMap;
public:
	void read(int pageId, int segmentId, void * data, int len);
	void write(int pageId, int segmentId, void * data, int len);
	~PageIOUtil();
	PageIOUtil();
};


#endif //PROJECT_FILEIOUTIL_H
