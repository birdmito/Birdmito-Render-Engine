
#ifndef Mesh_h
#define Mesh_h

// MARK: - Library
// -----------------
// OpenGL API
#include "glad/glad.h"

// glm library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// own library
#include "Shader.h"

// standard library
#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

// MARK: - Structure
// ------------------
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    
    string path;
};

// MARK: - Class
// ------------------
class Mesh {
public:
    // Mesh properties
    // ----------
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;
    
    // Functions
    // ----------
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void draw(Shader &shader);
    
private:
    // Mesh properties
    // ----------
    unsigned int VBO, EBO;
    
    // Functions
    // ----------
    void setupMesh();
    
};

// MARK: - Function realization
// --------------------
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    
    setupMesh();
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // Vertex Position
    // ----------
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Vertex Normal
    // ----------
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    // Vertex Texcoord
    // ----------
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    // Vertex Tangent
    // ----------
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    
    // Vertex Bitangent
    // ----------
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    
    // Bone IDs
    // ----------
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
    
    // Bone weights
    // ----------
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    
    glBindVertexArray(0);
}

//define texture: texture_categoryN(e.g. texture_diffuse1)
void Mesh::draw(Shader &shader){
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    
    for(unsigned int i = 0; i < textures.size(); i++){
        //active texture units
        glActiveTexture(GL_TEXTURE0 + i);
        
        //get texture number
        string number;
        string name = textures[i].type;
        
        if(name == "texture_diffuse"){
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular"){
            number = std::to_string(specularNr++);
        }
        else if(name == "texture_normal"){
            number = std::to_string(normalNr++); // transfer unsigned int to string
        }
        else if(name == "texture_height"){
            number = std::to_string(heightNr++); // transfer unsigned int to string
        }
        
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    
    //draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
    
}
#endif /* Mesh_h */
