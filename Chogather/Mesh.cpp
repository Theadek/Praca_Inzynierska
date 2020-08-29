#include "Mesh.h"
Mesh::Mesh() {
	glGenBuffers(1, &this->VBO);
}
Mesh::~Mesh() {
	glDeleteBuffers(1, &this->VBO);
}
void Mesh::RenderMesh(ShaderManager *shader, glm::vec3 position, glm::vec3 size, float rotate) {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    if (this->texture_coordinates.size() != 0) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(this->vertices.size() * sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(this->vertices.size() * sizeof(glm::vec3) + this->texture_coordinates.size() * sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);
        shader->setInt("diffuse_tex", 0);
        glActiveTexture(GL_TEXTURE0);
        this->material.diffuseTexture.Bind();
        shader->setInt("specular_tex", 1);
        glActiveTexture(GL_TEXTURE1);
        this->material.specularTexture.Bind();
		shader->setInt("bump_tex", 2);
		glActiveTexture(GL_TEXTURE2);
		this->material.bumpTexture.Bind();
    }
	else {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(this->vertices.size() * sizeof(glm::vec3)));
		glEnableVertexAttribArray(1);
	}
	shader->setVec3("ambientModel", this->material.ambient);
    shader->setVec3("diffuseModel", this->material.diffuse);
    shader->setVec3("specularModel", this->material.specular);
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::scale(model_matrix, size);
    model_matrix = glm::translate(model_matrix, position);  
    model_matrix = glm::rotate(model_matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    shader->setMat4("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}


void Mesh::fillVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	if (this->texture_coordinates.size() != 0) {
		glBufferData(GL_ARRAY_BUFFER, (this->vertices.size() * sizeof(glm::vec3) + this->texture_coordinates.size() * sizeof(glm::vec2) + this->normals.size() * sizeof(glm::vec3)), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->texture_coordinates.size() * sizeof(glm::vec2), &this->texture_coordinates[0]);
		glBufferSubData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3) + this->texture_coordinates.size() * sizeof(glm::vec2), this->normals.size() * sizeof(glm::vec3), &this->normals[0]);
	}
    else {
		glBufferData(GL_ARRAY_BUFFER, (this->vertices.size() * sizeof(glm::vec3) + this->normals.size() * sizeof(glm::vec3)), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), this->normals.size() * sizeof(glm::vec3), &this->normals[0]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}