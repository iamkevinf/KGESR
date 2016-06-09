#ifndef __KGE_MESH_H__
#define __KGE_MESH_H__

#include "KGEMath.h"
#include "KGETriangle.h"

#include <vector>

namespace KGE
{
    class KGEVertex;
    class KGEMaterial;
    class KGETexture;

    class KGEMesh
    {
    public:
        KGEMesh();
        ~KGEMesh();

        void Clear();
        KGEVertex GetVertex(int index);
        void SetVertex(int index, const KGEVertex & vertex);
        void AddVertex(const KGEVertex & vertex);

        bool LoadFromFile(const std::string & filepath, const std::string & filename);

    public:
        std::vector<Vector4> positionList;
        std::vector<Vector4> normList;
        std::vector<int> textureIDList;
        std::vector<int> materialIDList;
        std::vector<Vector4> colorList;
        std::vector<Vector2> uvList;
        std::vector<bool> cullBackList;

        std::vector<KGETriangle> triList;

        std::vector<KGEMaterial*> materialList;
        std::vector<KGETexture*> textureList;
    };

}; // end of namespace KGE

#endif // __KGE_MESH_H__

