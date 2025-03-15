#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <list> 
#include <memory> //unique_ptr

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
const int max_count = 1;

class Model 
{
public:
    Model(){};
    list<unique_ptr<Model>> children;
    Model* parent = nullptr;
    
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    { 

        loadModel(path);
    }

    // draws the model, and thus all its meshes
    // void Draw(Shader &shader)
    // {
    //     for(unsigned int i = 0; i < meshes.size(); i++)
    //         meshes[i].Draw(shader);
    // }

    void Draw(Shader &shader)
    {

        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
        for(const auto& child : children){
            if(child){
                child->Draw(shader);
            }
        }
    }


private:
    glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& aiMat) {
        return glm::mat4(
            aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
            aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
            aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
            aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
        );
    }

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        // processNode(scene->mRootNode, scene,0);
        processNode(*this,scene->mRootNode, scene,0);
    }

    void processNode(aiNode *node, const aiScene *scene, int currentLevel, int childId = -1, aiMatrix4x4 transform = aiMatrix4x4())
    {
        // 计算当前节点相对于世界坐标系的变换矩阵
        aiMatrix4x4 globalTransform = transform * node->mTransformation;
        auto global_trans = aiMatrix4x4ToGlm(globalTransform);
        cout << "01----当前层级:" << currentLevel << "&& 子节点索引值:" << childId << endl;
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene,global_trans));
            cout << "02----当前层级:" << currentLevel << "&& 子节点索引值:" << childId <<" && mesh数量: " << meshes.size() << endl;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, currentLevel+1, (int)i, globalTransform);
        }
    }


    void processNode(Model& self_model,aiNode *node, const aiScene *scene, int currentLevel, int childId = -1, aiMatrix4x4 transform = aiMatrix4x4())
    {
        // 计算当前节点相对于世界坐标系的变换矩阵
        aiMatrix4x4 globalTransform = transform * node->mTransformation;
        auto global_trans = aiMatrix4x4ToGlm(globalTransform);
        cout << "01----当前层级:" << currentLevel << "&& 子节点索引值:" << childId << endl;
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            self_model.meshes.push_back(processMesh(mesh, scene,global_trans));
            cout << "02----当前层级:" << currentLevel << "&& 子节点索引值:" << childId <<" && mesh数量: " << meshes.size() << endl;
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            children.emplace_back(make_unique<Model>());
            // 获取最后一个元素的引用
            std::unique_ptr<Model>& lastPtr = children.back();
            // 解引用 unique_ptr 以获取 classA 对象
            Model& lastObj = *lastPtr;
            processNode(lastObj, node->mChildren[i], scene, currentLevel+1, (int)i, globalTransform);
        }
    }
    

    Mesh processMesh(aiMesh *mesh, const aiScene *scene, const glm::mat4& transform)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            glm::vec4 trans_vec = transform*glm::vec4(vector, 1.0f);
            vertex.Position = glm::vec3(trans_vec.x,trans_vec.y,trans_vec.z);
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                // vertex.Normal = vector;
                glm::mat4 normalMat = glm::transpose(glm::inverse(transform));
                glm::mat3 new_normal_mat = glm::mat3(normalMat);
                vertex.Normal = glm::normalize(new_normal_mat * vector);
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }
};


unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
#endif
