#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 4){
		cout << "Invalid arguments. Usage is 'sort <inputFile> <outputFile> <memoryBufferInMB>'" << endl;
		return 1;
	}
	
	char* inputFile = argv[1];
	char* outputFile = argv[2];
	char* memoryBufferInMB = argv[3];	
	
	char * endp;
    long num = strtol(argv[3],&endp,10);
	
	if (*endp){
		cout << "MemoryBufferInMB has to be an int" << endl;
		return -1;
	}
	cout << "InputFile: " << inputFile << " OutputFile: " << outputFile << " MemoryBufferInMB: " << memoryBufferInMB << endl;
}