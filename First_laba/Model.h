#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "funcs.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Model
{
public:
    
    vector<Mesh> meshes;
        
    Model(string const& path) 
    {
        loadModel(path);
    }

    void Draw()
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw();
        }
    }

    void Draw(GLuint shader,glm::mat4 transformOX1, glm::mat4 transformOX2, glm::mat4 transformOX3)
    {
        glm::mat4 transformOX1orig = transformOX1;
        glm::mat4 transformOX2orig = transformOX2;
        glm::mat4 transformOX3orig = transformOX3;
        glm::mat4 res3 = glm::mat4(1.0f);
        for (unsigned int i = 0; i < meshes.size(); i++) 
        {
            switch (i)
            {
                case 2: 
                {
                    settingMat4(shader, "model", transformOX1);   
                }
                break;

                case 3:
                {
                    glm::mat4 minusTr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -1.69387, -0.584537));
                    glm::mat4 plusTr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 1.69387, 0.584537));
                    glm::mat4 result = transformOX1 *  plusTr * transformOX2 * minusTr;
                    res3 = result;
                    settingMat4(shader, "model", result);

                }
                break;

                case 4:
                {
                    glm::mat4 minusTr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -3.69, -0.771778));
                    glm::mat4 plusTr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 3.69, 0.771778));
                    glm::mat4 result =  res3 * plusTr * transformOX3 * minusTr;
                    settingMat4(shader, "model", result);
                }
                break;

            default:
                settingMat4(shader, "model", glm::mat4(1.0f));
                break;
            }
            meshes[i].Draw();
        }
    }

 
private:
    
    void loadModel(string const& path)
    {
        // Create an instance of the Importer class
        Assimp::Importer importer;

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        // If the import failed, report it
        if (nullptr == scene) {
            cout << "ASSSIMP ERROR: " <<  importer.GetErrorString() << endl;
            return;
        }

        // Now we can access the file's contents.
        processNode(scene->mRootNode,scene);

        // We're done. Everything will be cleaned up by the importer destructor
    }
        
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) 
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh* mesh)
    {
        vector <Vertex>       vertices;
        vector <unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->HasNormals()) 
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) 
            {
                indices.push_back(face.mIndices[j]);
            }
        }


        return Mesh(vertices, indices);
    }        
};


