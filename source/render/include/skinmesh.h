#pragma once

#include "render_core.h"
#include "texture.h"
#include "shader.h"

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ZERO_MEM_VAR(var) memset(&var, 0, sizeof(var))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))
#define SAFE_DELETE(p) \
    if (p)             \
    {                  \
        delete p;      \
        p = NULL;      \
    }

class SkinnedMesh {
public:
    SkinnedMesh();

    ~SkinnedMesh();

    bool LoadMesh(const string &Filename);

    void Render();

    uint NumBones() const {
        return m_NumBones;
    }

    void BoneTransform(Shader shader, float TimeInSeconds, vector<mat4> &Transforms);

private:
#define NUM_BONES_PER_VEREX 4

    struct BoneInfo {
        mat4 BoneOffset;
        mat4 FinalTransformation;

        BoneInfo() : BoneOffset(1.0f), FinalTransformation(1.0f) {}
    };

    struct VertexBoneData {
        uint IDs[NUM_BONES_PER_VEREX]{};
        float Weights[NUM_BONES_PER_VEREX]{};

        VertexBoneData() {
            Reset();
        };

        void Reset() {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);
        }

        void AddBoneData(uint BoneID, float Weight);
    };

    void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);

    uint FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);

    const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation, const string NodeName);

    void ReadNodeHeirarchy(Shader shader, float AnimationTime, const aiNode *pNode, const mat4 &ParentTransform);

    bool InitFromScene(const aiScene *pScene, const string &Filename);

    void InitMesh(uint MeshIndex,
                  const aiMesh *paiMesh,
                  vector<vec3> &Positions,
                  vector<vec3> &Normals,
                  vector<vec2> &TexCoords,
                  vector<VertexBoneData> &Bones,
                  vector<unsigned int> &Indices);

    void LoadBones(uint MeshIndex, const aiMesh *paiMesh, vector<VertexBoneData> &Bones);

    bool InitMaterials(const aiScene *pScene, const string &Filename);

    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs
    };

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs]{};

    struct MeshEntry {
        MeshEntry() {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    vector<MeshEntry> m_Entries;
    vector<Texture *> m_Textures;

    map<string, uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    vector<BoneInfo> m_BoneInfo;
    mat4 m_GlobalInverseTransform{};

    const aiScene *m_pScene;
    Assimp::Importer m_Importer;

    static glm::mat4 convertMatrix(const aiMatrix4x4 &aiMat) {
        return {
                aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4};
    }
};
