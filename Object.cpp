#include "Object.hpp"

class ParsingException : public std::exception {
 private:
    std::string msg;
 public:
    ParsingException(std::string str) : msg(str) {}
    const char *what() const throw()
    {
        return msg.c_str();
    };
};

Object::Object(std::string str)
{
    Parser(str);
}

Object::~Object()
{
}

void Object::CheckLineVertice(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789.-") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void Object::CheckLineFace(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789/") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void Object::GetVertice(char *line)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;

    while(std::getline(streamLine, buffer, ' '))
    {
        vertices.push_back(std::stof(buffer.c_str()));
    }
}

void Object::GetFace(char *line)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    std::vector<std::string> dividedLine;
    std::vector<std::string> dividedFaces;

    while(std::getline(streamLine, buffer, ' '))
    {
        dividedLine.push_back(buffer);
        faces.push_back(atoi(buffer.c_str()) - 1);
    }
    
    for (size_t i = 0; i < dividedLine.size(); i++)
    {
        std::stringstream tmp(dividedLine[i]);
        while (std::getline(tmp, buffer, '/'))
        {
            dividedFaces.push_back(buffer);
            faces.push_back(atoi(buffer.c_str()) - 1);
        }
        if (dividedLine.size() == 4)
        {
            faces.pop_back();
            dividedLine.erase(dividedLine.begin() + 1);
            for (size_t i = 0; i < 3; i++)
            {
                faces.push_back(dividedLine[i] - 1);
            }
        }
    }

    for (size_t i = 0; i < dividedFaces.size(); i++)
    {
        /* code */
    }
    
    
    
    if (dividedLine.size() == 4)
    {
        faces.pop_back();
        dividedLine.erase(dividedLine.begin() + 1);
        for (size_t i = 0; i < 3; i++)
        {
            faces.push_back(dividedLine[i] - 1);
        }
    }
}

void Object::Parser(std::string fileName)
{
    std::ifstream file;
    char buffer[500];
    file.exceptions(std::ifstream::failbit);
    try
    {
        file.open(fileName);
        file.exceptions(std::ios_base::goodbit);
        while (file.good())
        {
            file.getline(buffer, 500);
            if(buffer[0] == 'v' && buffer[1] == ' ')
            {
                CheckLineVertice(buffer + 2);
                GetVertice(buffer + 2);
            }
            if (buffer[0] == 'f')
            {
                CheckLineFace(buffer + 2);
                GetFace(buffer + 2, indices);
            }
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << e.what() << std::endl;
    }  
}