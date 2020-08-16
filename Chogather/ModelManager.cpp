#include "ModelManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

Model::Model() {

}
bool Model::loadobj(const char * path)
{
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec2> temp_uvs;
    vector<glm::vec3> temp_normals;
    ifstream file(path);
    string line;
    while (getline(file, line)) {
        if (line.substr(0, line.find(" ")) == "v") {
            line.erase(std::remove(line.begin(), line.end(), 'v'), line.end());
            stringstream ss(line);
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } 
        else if (line.substr(0, line.find(" ")) == "vt") {
            line.erase(std::remove(line.begin(), line.end(), 'v'), line.end());
            line.erase(std::remove(line.begin(), line.end(), 't'), line.end());
            stringstream ss(line);
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (line.substr(0, line.find(" ")) == "vn") {
            line.erase(std::remove(line.begin(), line.end(), 'v'), line.end());
            line.erase(std::remove(line.begin(), line.end(), 'n'), line.end());
            stringstream ss(line);
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (line.substr(0, line.find(" ")) == "f") {
            line.erase(std::remove(line.begin(), line.end(), 'f'), line.end());
            if (line.find("//") != string::npos) { // model without textures
                replace(line.begin(), line.end(), '/', ' ');
                stringstream ss(line);
                int temp;
                while (ss) {
                    ss >> temp;
                    vertexIndices.push_back(temp);
                    ss >> temp;
                    normalIndices.push_back(temp);
                }
            }
            else { //model with textures
                replace(line.begin(), line.end(), '/', ' ');
                stringstream ss(line);
                int temp;
                while (!ss.eof()) {
                    ss >> temp;
                    vertexIndices.push_back(temp);
                    ss >> temp;
                    uvIndices.push_back(temp);
                    ss >> temp;
                    normalIndices.push_back(temp);
                }
            }
        }


    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        this->vertices.push_back(vertex);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        this->texture_coordinates.push_back(uv);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        this->normals.push_back(normal);
    }
    return true;
}