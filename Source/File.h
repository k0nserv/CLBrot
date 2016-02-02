#ifndef FILE_H
#define FILE_H
#include <fstream>

using std::ifstream;

class File {
public:
 template <typename T>
    static T read(ifstream* f,bool bigEndian=true){
        unsigned char t[sizeof(T)];
        char temp;
        for (int i = 0, j = sizeof (T) - 1; i < sizeof (T); i++, j--) {
            f->read(&temp, 1);
            t[ bigEndian ? j : i] = temp;
        }
        return *reinterpret_cast<T*>(t);
    }
private:
};



#endif