#ifndef OBJ_STL_H
#define OBJ_STL_H

#include "j_framework.h"

class Obj_stl
{
public:
    // Constructors
    Obj_stl(const std::string&);
    
    // Public methods
    Mesh get();
    void clear();
    jint error();
    
    // Friend function
    friend std::ostream& operator<<(std::ostream&, const Obj_stl&);

private:
    // Private methods
    Mesh import(const std::string&);
    void parseLine(const std::string&, Mesh&);
    
    // Private members
    std::vector<std::string> _tokens;
    Mesh _mesh;
    jint _error;
};

#endif // OBJ_STL_H
