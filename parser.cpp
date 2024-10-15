#include "Scop.hpp"

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

void CheckLineVertice(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789.-") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void CheckLineFace(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789/") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void GetVertice(char *line, std::vector<GLfloat> *vertices)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;

    while(std::getline(streamLine, buffer, ' '))
    {
        vertices->push_back(std::stof(buffer.c_str()));
    }
}

void GetFace(char *line, std::vector<GLuint> *faces)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    std::vector<int> dividedLine;

    while(std::getline(streamLine, buffer, ' '))
    {
        dividedLine.push_back(atoi(buffer.c_str()));
        faces->push_back(atoi(buffer.c_str()) - 1);
    }
    if (dividedLine.size() == 4)
    {
        faces->pop_back();
        dividedLine.erase(dividedLine.begin() + 1);
        for (size_t i = 0; i < 3; i++)
        {
            faces->push_back(dividedLine[i] - 1);
        }
    }
}

void Parser(std::string fileName, std::vector<GLfloat> *vertices, std::vector<GLuint> *indices)
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
            if(buffer[0] == 'v' )
            {
                CheckLineVertice(buffer + 2);
                GetVertice(buffer + 2, vertices);
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