//
// Created by Daniel on 30.04.2015.
//

#ifndef PROJECT_FILEIOUTIL_H
#define PROJECT_FILEIOUTIL_H

#include <unordered_map>

class FileIOUtil {
private:
	std::unordered_map<int, int> fdMap;
public:
	void read(int pageId, int segmentId, void * data, int len);
	void write(int pageId, int segmentId, void * data);
	~FileIOUtil();
	FileIOUtil();
};


#endif //PROJECT_FILEIOUTIL_H
