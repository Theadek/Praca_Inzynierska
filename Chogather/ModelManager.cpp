#include "ModelManager.h"
#include "TextureManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

Model::Model() {

}
bool Model::loadobj(string path)
{
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec2> temp_uvs;
    vector<glm::vec3> temp_normals;
    ifstream file(path);
    string line, mtl_path, prefix;
    stringstream ss;
    Mesh* mesh = nullptr;
    while (getline(file, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;
        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } 
        else if (prefix == "vt") {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "mtllib") {
            ss >> mtl_path;
            mtl_path = "Models/" + mtl_path;
            this->loadmtl(mtl_path);
        }
        else if (prefix == "f") {
            line.erase(std::remove(line.begin(), line.end(), 'f'), line.end());
            // model without textures
            if (line.find("//") != string::npos) { 
                replace(line.begin(), line.end(), '/', ' ');
                stringstream ss_face(line);
                int temp;
                while (ss_face) {
                    ss_face >> temp;
                    vertexIndices.push_back(temp);
                    ss_face >> temp;
                    normalIndices.push_back(temp);
                }
            }
            else { 
                //model with textures
                replace(line.begin(), line.end(), '/', ' ');
                stringstream ss_face(line);
                int temp;
                while (!ss_face.eof()) {
                    ss_face >> temp;
                    vertexIndices.push_back(temp);
                    ss_face >> temp;
                    uvIndices.push_back(temp);
                    ss_face >> temp;
                    normalIndices.push_back(temp);
                }
            }
        }
        else if (prefix == "usemtl") {
            string material_name;
            ss >> material_name;

            for (std::vector<Material>::iterator it = this->materials.begin(); it != this->materials.end(); it++) {
                if (it->name == material_name) {
                    mesh->material = *it;
                }
            }
        }
        else if (prefix == "o") {
            if (mesh == nullptr) {
                mesh = new Mesh();
            }
            else {
                for (unsigned int i = 0; i < vertexIndices.size(); i++) {
                    unsigned int vertexIndex = vertexIndices[i];
                    if (vertexIndex > temp_vertices.size()) {
                        printf("");
                    }
                    glm::vec3 vertex = temp_vertices[vertexIndex - 1];
                    mesh->vertices.push_back(vertex);
                }
                for (unsigned int i = 0; i < uvIndices.size(); i++) {
                    unsigned int uvIndex = uvIndices[i];
                    glm::vec2 uv = temp_uvs[uvIndex - 1];
                    mesh->texture_coordinates.push_back(uv);
                }
                for (unsigned int i = 0; i < normalIndices.size(); i++) {
                    unsigned int normalIndex = normalIndices[i];
                    glm::vec3 normal = temp_normals[normalIndex - 1];
                    mesh->normals.push_back(normal);
                }
                this->meshes.push_back(*mesh);
                vertexIndices.clear();
                uvIndices.clear();
                normalIndices.clear();
                /*temp_vertices.clear();
                temp_uvs.clear();
                temp_normals.clear();*/
                mesh = new Mesh();
            }

        }
    }

    /*for (unsigned int i = 0; i < vertexIndices.size(); i++) {
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
    }*/
    file.close();
    return true;
}

void Model::loadmtl(string path) {
    ifstream mtlfile(path);
    string line, prefix, material_name, texture_name;
    stringstream ss;
    Material *material = nullptr;
    while (getline(mtlfile, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;
        if (prefix == "newmtl") {
            ss >> material_name;
            material = new Material();
            material->name = material_name;

        }
        else if (prefix == "Ka") {
            glm::vec3 ambient;
            ss >> ambient.x >> ambient.y >> ambient.z;
            material->ambient = ambient;
        }
        else if (prefix == "Kd") {
            glm::vec3 diffuse;
            ss >> diffuse.x >> diffuse.y >> diffuse.z;
            material->diffuse = diffuse;
        }
        else if (prefix == "Ks") {
            glm::vec3 specular;
            ss >> specular.x >> specular.y >> specular.z;
            material->specular = specular;
        }
        else if (prefix == "map_Kd") {
            ss >> texture_name;
            texture_name = "Models/" + texture_name;
            Texture2D tex = Texture2D();
            tex.Generate(texture_name.c_str());
            material->diffuseTexture = tex;
        }
        else if (prefix == "map_Ks") {
            ss >> texture_name;
            texture_name = "Models/" + texture_name;
            Texture2D tex = Texture2D();
            tex.Generate(texture_name.c_str());
            material->specularTexture = tex;
            this->materials.push_back(*material);
            material = nullptr;
            prefix = "";
        }
    }
    mtlfile.close();
}