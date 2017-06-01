    #include <iostream>  
    #include <fstream>  
    using namespace std;  
    const char * filename = "test.txt";  
      
    int main () {  
        char * buffer;  
        long size;  
        ifstream in (filename, ios::in|ios::binary|ios::ate);  
        size = in.tellg();  
       in.seekg (0, ios::beg); 
       long l = in.tellg();  
       in.seekg (0, ios::end); 
       long e = in.tellg();
       size = e - l - 1;
       cout << "size = "<< size<<endl;
       in.close();
       
        ifstream in1 (filename, ios::in|ios::binary|ios::ate);  
        buffer = new char [size];  
        in1.read (buffer, size);  
        in1.close();  
          
        cout << "the complete file is in a buffer"<<endl;  
        cout << buffer<<endl;
        delete[] buffer;  
        return 0;  
    } 
