#include <iostream>  
#include <fstream> 
using namespace std;
    
const char * filename = "test.txt";  
static char *buffer;  
long Getfilelength(const char *filename);
int main () 
{  
//	char * buffer;  
	long size;
	size = Getfilelength(filename);
	ifstream in (filename, ios::in|ios::binary);  
	buffer = new char [size];  
	in.read (buffer, size);  
	in.close();  
				  
	cout << "the complete file is in a buffer"<<endl;  
	cout << "buffer = "<< buffer<<endl;          
	cout << "size = " << size << endl; 
//	delete[] buffer;  
	return 0;  
}
     
     
long Getfilelength(const char *filename)
{
	int size;
    ifstream in(filename, ios::in|ios::binary|ios::ate);   // need include namespace std;  ios::ate 放到文件末尾
    size = in.tellg();  // 取得文件长度
    in.seekg(0, ios::beg);
    in.close();
    return size;
}
