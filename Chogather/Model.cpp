#include "Model.h"
#include <limits>
using namespace std;

Model::Model() {
    glGenVertexArrays(1, &this->VAO);
}
Model::~Model() {
    glDeleteVertexArrays(1, &this->VAO);
}
bool Model::loadobj(string path)
{
    float maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN, minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;
    ifstream file(path);
    if (!file) {
        std::cout << "Cannot load this model, bad path: " + path << endl;
        return false;
    }
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<glm::vec3> temp_vertices;
    vector<glm::vec2> temp_uvs;
    vector<glm::vec3> temp_normals;
    string line, mtl_path, prefix, mtl_prefix;
    stringstream ss;
    Mesh* mesh = nullptr;
	int pos = path.find_last_of("/");
	mtl_prefix = path.substr(0, pos);
    while (getline(file, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;
        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
            if (vertex.x <= minX)
                minX = vertex.x;
            if (vertex.x >= maxX)
                maxX = vertex.x;
            if (vertex.y <= minY)
                minY = vertex.y;
            if (vertex.y >= maxY)
                maxY = vertex.y;
            if (vertex.z <= minZ)
                minZ = vertex.z;
            if (vertex.z >= maxZ)
                maxZ = vertex.z;
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
            mtl_path = mtl_prefix + '/' + mtl_path;
            if (!this->loadmtl(mtl_path)) {
                return false;
            }
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
                mesh->fillVBO();
                this->meshes.push_back(mesh);
                vertexIndices.clear();
                uvIndices.clear();
                normalIndices.clear();
                mesh = new Mesh();
            }

        }
    }
    if (mesh != nullptr) {
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];
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
        mesh->fillVBO();
        this->meshes.push_back(mesh);
        vertexIndices.clear();
        uvIndices.clear();
        normalIndices.clear();
    }
    file.close();
    this->size = glm::vec3(maxX - minX, maxY - minY, maxZ - minZ);
    this->corner = glm::vec3(minX, minY, minZ);
    this->maxX = maxX;
    this->maxY = maxY;
    return true;
}

bool Model::loadmtl(string path) {
    ifstream mtlfile(path);
    if (!mtlfile) {
        std::cout << "Cannot load this material, bad path: " + path << endl;
        return false;
    }
    string line, prefix, material_name, texture_name, mtl_prefix;
    stringstream ss;
    Material *material = nullptr;
	int pos  = path.find_last_of("/");
	mtl_prefix = path.substr(0, pos);
    while (getline(mtlfile, line)) {
        ss.clear();
        ss.str(line);
        ss >> prefix;
        if (prefix == "newmtl") {
            ss >> material_name;
            if (material == nullptr) {
                material = new Material();
                material->name = material_name;
            }
            else {
                this->materials.push_back(*material);
                material = new Material();
                material->name = material_name;
            }

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
            texture_name = mtl_prefix + "/" + texture_name;
            Texture2D tex = Texture2D();
            tex.Generate(texture_name.c_str());
            material->diffuseTexture = tex;
        }
        else if (prefix == "map_Bump") {
			ss >> texture_name;
			texture_name = mtl_prefix + "/" + texture_name;
			Texture2D tex = Texture2D();
			tex.Generate(texture_name.c_str());
			material->bumpTexture = tex;
        }
        else if (prefix == "map_Ks") {
            ss >> texture_name;
            texture_name = mtl_prefix + "/" + texture_name;
            Texture2D tex = Texture2D();
            tex.Generate(texture_name.c_str());
            material->specularTexture = tex;
        }
    }
    if (material != nullptr) {
        this->materials.push_back(*material);
    }
    mtlfile.close();
    return true;
}
