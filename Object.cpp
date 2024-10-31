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
    this->MeshParser(str);
}

Object::~Object()
{
}

std::vector<GLfloat>    Object::GetVertices()
{
    return vertices;
}

std::vector<GLfloat>    Object::GetMeshVertexArray()
{
    return meshVertexArray;
}

int                     Object::GetComponents()
{
    return components;
}

void Object::MeshCheckLineVertice(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789.-") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
}

void Object::MeshCheckLineFace(char *line)
{
    std::string tmpLine;
    tmpLine = line;
    if (tmpLine.find_first_not_of(" 0123456789/") != std::string::npos)
        throw ParsingException("parsing error: " + tmpLine);
    if (vertices.size() == 0)
        throw ParsingException("parsing error: no vertices");
}

void Object::MeshGetVertice(char *line, int lineNbr)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while(std::getline(streamLine, buffer, ' '))
    {
        vertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 3)
        throw ParsingException("parsing error: vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Object::MeshGetNormalVertice(char *line, int lineNbr)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while(std::getline(streamLine, buffer, ' '))
    {
        normalVertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 3)
        throw ParsingException("parsing error: normal vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Object::MeshGetTextureVertice(char *line, int lineNbr)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    int i = 0;

    while(std::getline(streamLine, buffer, ' '))
    {
        textureVertices.push_back(std::stof(buffer.c_str()));
        i++;
    }
    if (i != 2)
        throw ParsingException("parsing error: texture vertices line: " + std::to_string(lineNbr) + " " + tmpLine);
}

void Object::MeshGetFace(char *line, int lineNbr)
{
    std::string tmpLine = line;
    std::stringstream streamLine(tmpLine);
    std::string buffer;
    std::vector<std::string> dividedLine;
    std::vector<std::string> dividedFaces;

    while(std::getline(streamLine, buffer, ' '))
    {
        dividedLine.push_back(buffer);
        dividedFaces.push_back(buffer);
        faces.push_back(atoi(buffer.c_str()) - 1);
    }

    if (dividedLine.size() == 4)
    {
        faces.pop_back();
        dividedFaces.pop_back();
        dividedLine.erase(dividedLine.begin() + 1);
        for (size_t i = 0; i < 3; i++)
        {
            dividedFaces.push_back(dividedLine[i]);
            faces.push_back(atoi(dividedLine[i].c_str()) - 1);
        }
    }
    for (size_t i = 0; i < dividedFaces.size(); i++)
    {
        std::stringstream tmp(dividedFaces[i]);
        std::vector<std::string> tmpFaces;
        int facesNbrCheck = 0;
        while (std::getline(tmp, buffer, '/'))
        {
            if (buffer[0] != '\0')
                facesNbrCheck++;
            tmpFaces.push_back(buffer);
        }
        
        if (facesNbrCheck > 1 && normalVertices.size() == 0 || facesNbrCheck > 1 && textureVertices.size() == 0)
            throw ParsingException("error: faces indices reference non existant vertex: " + std::to_string(lineNbr) + " " + tmpLine);

        for (size_t j = 0; j < tmpFaces.size(); j++)
        {
            if (j == 2)
            {
                
                meshVertexArray.push_back(textureVertices[(atoi(tmpFaces[j - 1].c_str()) - 1) * 2]);
                meshVertexArray.push_back(textureVertices[(atoi(tmpFaces[j - 1].c_str()) - 1) * 2 + 1]);
            }
            else if (j == 1 && tmpFaces.size() > 2)
            {
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3]);
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3 + 1]);
                meshVertexArray.push_back(normalVertices[(atoi(tmpFaces[j + 1].c_str()) - 1) * 3 + 2]);
            }
            else
            {
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3]);
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3 + 1]);
                meshVertexArray.push_back(vertices[(atoi(tmpFaces[j].c_str()) - 1) * 3 + 2]);
            }
        }
        if (i > 0 && facesNbrCheck != components)
            throw ParsingException("parsing error: faces numbers: " + std::to_string(lineNbr) + " " + tmpLine);
        components = tmpFaces.size();
    }

    /*for (size_t i = 0; i < meshVertexArray.size(); i++)
    {
        std::cout << meshVertexArray[i] << std::endl;
        if ((i + 1) % 8 == 0)
            std::cout << std::endl;
    }*/
}

void Object::MeshParser(std::string fileName)
{
    std::ifstream file;
    char buffer[500];
    int lineNbr = 0;
    try
    {
        file.open(fileName);
        if (!file.good())
            throw ParsingException("parsing error: file " + fileName + " does not exist");
        while (file.good())
        {
            lineNbr++;
            file.getline(buffer, 500);
            std::string strbuffer(buffer);
            if(strbuffer.size() > 3 && buffer[0] == 'v' && buffer[1] == ' ')
            {
                MeshCheckLineVertice(buffer + 2);
                MeshGetVertice(buffer + 2, lineNbr);
            }
            if (strbuffer.size() > 4 && buffer[0] == 'v' && buffer[1] == 'n' && buffer[2] == ' ')
            {
                MeshCheckLineVertice(buffer + 3);
                MeshGetNormalVertice(buffer + 3, lineNbr);
            }
            if (strbuffer.size() > 4 && buffer[0] == 'v' && buffer[1] == 't' && buffer[2] == ' ')
            {
                MeshCheckLineVertice(buffer + 3);
                MeshGetTextureVertice(buffer + 3, lineNbr);
            }
            if (strbuffer.size() > 2 && buffer[0] == 'f')
            {
                MeshCheckLineFace(buffer + 2);
                MeshGetFace(buffer + 2, lineNbr);
            }
        }
        if (meshVertexArray.size() == 0)
            throw ParsingException("parsing error: no faces");
    }
    catch(ParsingException e)
    {
        std::cerr << e.what() << std::endl;
        exit(0);
    }
}