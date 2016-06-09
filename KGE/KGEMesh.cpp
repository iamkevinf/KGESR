#include "stdafx.h"
#include "KGEMesh.h"

#include "KGEVertex.h"
#include "KGETriangle.h"
#include "KGEMaterial.h"

#include <assert.h>

#include "util.h"

namespace KGE
{

    KGEMesh::KGEMesh()
    {
    }


    KGEMesh::~KGEMesh()
    {
    }

    KGEVertex KGEMesh::GetVertex(int index)
    {
        assert(index < positionList.size());
        KGEVertex v;
        v.pos = positionList[index];
        v.norm = normList[index];
        v.color = colorList[index];
        v.isCullBack = cullBackList[index];
        v.materialID = materialIDList[index];
        v.textureID = textureIDList[index];
        return v;
    }

    void KGEMesh::SetVertex(int index, const KGEVertex & vertex)
    {
        assert(index < positionList.size());
        positionList[index] = vertex.pos;
        normList[index] = vertex.norm;
        colorList[index] = vertex.color;
        cullBackList[index] = vertex.isCullBack;
        materialIDList[index] = vertex.materialID;
        textureIDList[index] = vertex.textureID;
    }

    void KGEMesh::AddVertex(const KGEVertex & vertex)
    {
        positionList.push_back(vertex.pos);
        normList.push_back(vertex.norm);
        colorList.push_back(vertex.color);
        cullBackList.push_back(vertex.isCullBack);
        materialIDList.push_back(vertex.materialID);
        textureIDList.push_back(vertex.textureID);
    }

    bool KGEMesh::LoadFromFile(const std::string & filename)
    {
        FILE * fp = nullptr;
        ::fopen_s(&fp, filename.c_str(), "r");
        if (fp == nullptr)
            return false;

        std::vector<KGETriangle> faceList;
        int materialRef = 0;
        std::vector<KGETriangle> tfaceList;
        std::vector<Vector4> faceNormList;
        std::vector<Vector4> faceV0NormList;
        std::vector<Vector4> faceV1NormList;
        std::vector<Vector4> faceV2NormList;
        std::vector<Vector4> ambientList;
        std::vector<Vector4> diffuseList;
        std::vector<float> shineStrengthList;
        std::vector<float> shineList;
        std::vector<Vector4> specularList;
        std::vector<std::string> texFileNameList;
        {
            fscanStr(fp, "*3DSMAX_ASCIIEXPORT");
            fscanInt(fp);
            fscanStr(fp, "*COMMENT");
            fscanOneQuotation(fp);
            fscanStr(fp, "*SCENE");
            fscanStr(fp, "{");
            fscanStr(fp, "*SCENE_FILENAME");
            fscanOneQuotation(fp);
            fscanStr(fp, "*SCENE_FIRSTFRAME");
            fscanInt(fp);
            fscanStr(fp, "*SCENE_LASTFRAME");
            fscanInt(fp);
            fscanStr(fp, "*SCENE_FRAMESPEED");
            fscanInt(fp);
            fscanStr(fp, "*SCENE_TICKSPERFRAME");
            fscanInt(fp);
            fscanStr(fp, "*SCENE_BACKGROUND_STATIC");
            fscanVector3(fp);
            fscanStr(fp, "*SCENE_AMBIENT_STATIC");
            fscanVector3(fp);
            fscanStr(fp, "}");
            fscanStr(fp, "*MATERIAL_LIST");
            fscanStr(fp, "{");
            fscanStr(fp, "*MATERIAL_COUNT");
            int materialCount = fscanInt(fp);
            for (int materialIndex = 0; materialIndex < materialCount; materialIndex++){
                fscanStr(fp, "*MATERIAL");
                int materialID = fscanInt(fp);
                assert(materialID == materialIndex);
                fscanStr(fp, "{");
                fscanStr(fp, "*MATERIAL_NAME"); fscanOneQuotation(fp);
                fscanStr(fp, "*MATERIAL_CLASS"); fscanOneQuotation(fp);
                fscanStr(fp, "*MATERIAL_AMBIENT"); Vector4 ambient = fscanVector3(fp); ambient.w = 1;
                fscanStr(fp, "*MATERIAL_DIFFUSE"); Vector4 diffuse = fscanVector3(fp); diffuse.w = 1;
                fscanStr(fp, "*MATERIAL_SPECULAR"); Vector4 specular = fscanVector3(fp); specular.w = 1;
                fscanStr(fp, "*MATERIAL_SHINE"); float shine = fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_SHINESTRENGTH"); float shineStrength = fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_TRANSPARENCY"); fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_WIRESIZE"); fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_SHADING"); fscanStr(fp);
                fscanStr(fp, "*MATERIAL_XP_FALLOFF"); fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_SELFILLUM"); fscanFloat(fp);
                fscanStr(fp, "*MATERIAL_FALLOFF"); fscanStr(fp);
                fscanStr(fp, "*MATERIAL_XP_TYPE"); fscanStr(fp);
                fscanStr(fp, "*MAP_DIFFUSE"); fscanStr(fp, "{");
                fscanStr(fp, "*MAP_NAME"); fscanOneQuotation(fp);
                fscanStr(fp, "*MAP_CLASS"); fscanOneQuotation(fp);
                fscanStr(fp, "*MAP_SUBNO"); fscanInt(fp);
                fscanStr(fp, "*MAP_AMOUNT"); fscanFloat(fp);
                fscanStr(fp, "*BITMAP"); std::string texFilePathWithQuotation = fscanOneQuotation(fp);
                std::string texFilePath = texFilePathWithQuotation.substr(1, (int)texFilePathWithQuotation.size() - 2);
                std::string dividerChars;
                dividerChars.push_back('/');
                dividerChars.push_back('\\');
                assert((int)dividerChars.size() == 2);
                std::string texFileName = divideStr(texFilePath, dividerChars).back();
                fscanStr(fp, "*MAP_TYPE"); fscanStr(fp);
                fscanStr(fp, "*UVW_U_OFFSET"); fscanFloat(fp);
                fscanStr(fp, "*UVW_V_OFFSET"); fscanFloat(fp);
                fscanStr(fp, "*UVW_U_TILING"); fscanFloat(fp);
                fscanStr(fp, "*UVW_V_TILING"); fscanFloat(fp);
                fscanStr(fp, "*UVW_ANGLE"); fscanFloat(fp);
                fscanStr(fp, "*UVW_BLUR"); fscanFloat(fp);
                fscanStr(fp, "*UVW_BLUR_OFFSET"); fscanFloat(fp);
                fscanStr(fp, "*UVW_NOUSE_AMT"); fscanFloat(fp);
                fscanStr(fp, "*UVW_NOISE_SIZE"); fscanFloat(fp);
                fscanStr(fp, "*UVW_NOISE_LEVEL"); fscanInt(fp);
                fscanStr(fp, "*UVW_NOISE_PHASE"); fscanFloat(fp);
                fscanStr(fp, "*BITMAP_FILTER"); fscanStr(fp);
                fscanStr(fp, "}");
                fscanStr(fp, "}");

                ambientList.push_back(ambient);
                diffuseList.push_back(diffuse);
                shineStrengthList.push_back(shineStrength);
                shineList.push_back(shine);
                specularList.push_back(specular);
                texFileNameList.push_back(texFileName);

            }
            fscanStr(fp, "}");

            fscanStr(fp, "*GEOMOBJECT");
            fscanStr(fp, "{");
            fscanStr(fp, "*NODE_NAME"); std::string nodeName = fscanOneQuotation(fp);
            fscanStr(fp, "*NODE_TM");
            fscanStr(fp, "{");
            fscanStr(fp, "*NODE_NAME"); fscanOneQuotation(fp);
            fscanStr(fp, "*INHERIT_POS"); fscanVector3(fp);
            fscanStr(fp, "*INHERIT_ROT"); fscanVector3(fp);
            fscanStr(fp, "*INHERIT_SCL"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROW0"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROW1"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROW2"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROW3"); fscanVector3(fp);
            fscanStr(fp, "*TM_POS"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROTAXIS"); fscanVector3(fp);
            fscanStr(fp, "*TM_ROTANGLE"); fscanFloat(fp);
            fscanStr(fp, "*TM_SCALE"); fscanVector3(fp);
            fscanStr(fp, "*TM_SCALEAXIS"); fscanVector3(fp);
            fscanStr(fp, "*TM_SCALEAXISANG"); fscanFloat(fp);


            fscanStr(fp, "}");
            fscanStr(fp, "*MESH");
            fscanStr(fp, "{");
            fscanStr(fp, "*TIMEVALUE"); fscanInt(fp);
            fscanStr(fp, "*MESH_NUMVERTEX"); int vertexCount = fscanInt(fp);
            fscanStr(fp, "*MESH_NUMFACES"); int faceCount = fscanInt(fp);
            fscanStr(fp, "*MESH_VERTEX_LIST");
            fscanStr(fp, "{");
            for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++){
                fscanStr(fp, "*MESH_VERTEX");    int vID = fscanInt(fp);	Vector4 pos = fscanVector3(fp); pos.w = 1;
                assert(vertexIndex == vID);
                positionList.push_back(pos);
            }
            fscanStr(fp, "}");

            fscanStr(fp, "*MESH_FACE_LIST");
            fscanStr(fp, "{");
            for (int faceIndex = 0; faceIndex < faceCount; faceIndex++){

                fscanStr(fp, "*MESH_FACE");
                std::string faceIDStr = fscanStr(fp);
                int faceID = strToNumber(faceIDStr.substr(0, (int)faceIDStr.size() - 1));
                fscanStr(fp, "A:"); int vIDA = fscanInt(fp);
                fscanStr(fp, "B:"); int vIDB = fscanInt(fp);
                fscanStr(fp, "C:"); int vIDC = fscanInt(fp);
                fscanStr(fp, "AB:"); fscanInt(fp);
                fscanStr(fp, "BC:"); fscanInt(fp);
                fscanStr(fp, "CA:"); fscanInt(fp);
                fscanStr(fp, "*MESH_SMOOTHING"); fscanInt(fp);
                fscanStr(fp, "*MESH_MTLID"); fscanInt(fp);

                KGETriangle face(vIDA, vIDB, vIDC);
                faceList.push_back(face);

            }
            fscanStr(fp, "}");
            fscanStr(fp, "*MESH_NUMTVERTEX"); int tvertexCount = fscanInt(fp);
            fscanStr(fp, "*MESH_TVERTLIST");
            fscanStr(fp, "{");
            for (int tvertexIndex = 0; tvertexIndex < tvertexCount; tvertexIndex++){
                fscanStr(fp, "*MESH_TVERT"); int tvertexID = fscanInt(fp);	Vector4 texCoord = fscanVector3(fp);
                assert(tvertexIndex == tvertexID);
                uvList.push_back(Vector2(texCoord.x, texCoord.y));
            }
            fscanStr(fp, "}");
            fscanStr(fp, "*MESH_NUMTVFACES"); int tfaceCount = fscanInt(fp);
            fscanStr(fp, "*MESH_TFACELIST");
            fscanStr(fp, "{");
            for (int tfaceIndex = 0; tfaceIndex < tfaceCount; tfaceIndex++){
                fscanStr(fp, "*MESH_TFACE");
                int tfaceID = fscanInt(fp);
                assert(tfaceIndex == tfaceID);
                int tvID0 = fscanInt(fp);
                int tvID1 = fscanInt(fp);
                int tvID2 = fscanInt(fp);
                KGETriangle tface(tvID0, tvID1, tvID2);
                tfaceList.push_back(tface);
            }
            fscanStr(fp, "}");
            fscanStr(fp, "*MESH_NUMCVERTEX"); int cvertexCount = fscanInt(fp);
            if (cvertexCount != 0){
                //currently we only support cvertexCount==0 !
                return false;
            }
            fscanStr(fp, "*MESH_NORMALS");
            fscanStr(fp, "{");
            for (int faceIndex = 0; faceIndex < faceCount; faceIndex++){
                fscanStr(fp, "*MESH_FACENORMAL"); int faceID = fscanInt(fp);	Vector4 faceNorm = fscanVector3(fp);	faceNorm.w = 0;
                assert(faceIndex == faceID);
                fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID0 = fscanInt(fp);	Vector4 norm0 = fscanVector3(fp);  norm0.w = 0;
                fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID1 = fscanInt(fp);	Vector4 norm1 = fscanVector3(fp);  norm1.w = 0;
                fscanStr(fp, "*MESH_VERTEXNORMAL"); int vID2 = fscanInt(fp);	Vector4 norm2 = fscanVector3(fp);  norm2.w = 0;
                faceNormList.push_back(faceNorm);
                faceV0NormList.push_back(norm0);
                faceV1NormList.push_back(norm1);
                faceV2NormList.push_back(norm2);
            }
            fscanStr(fp, "}");
            fscanStr(fp, "}");
            fscanStr(fp, "*PROP_MOTIONBLUR"); fscanInt(fp);
            fscanStr(fp, "*PROP_CASTSHADOW"); fscanInt(fp);
            fscanStr(fp, "*PROP_RECVSHADOW"); fscanInt(fp);
            fscanStr(fp, "*MATERIAL_REF"); materialRef = fscanInt(fp);
            fscanStr(fp, "}");

        }

        //convert data format and fill data to mesh
        {
            //----material and texture
            const int materialIDBase = (int)materialIDList.size();
            const int textureIDBase = (int)textureIDList.size();
            const int materialCount = (int)ambientList.size();
            for (int materialIndex = 0; materialIndex < materialCount; materialIndex++){
                KGEMaterial*material = new KGEMaterial();
                material->init(ambientList[materialIndex], diffuseList[materialIndex], specularList[materialIndex], shineStrengthList[materialIndex] / shineList[materialIndex]);
                materialList.push_back(material);
                Ctexture*texture = new Ctexture();
                string texFilePath = folderPath + "/" + texFileNameList[materialIndex];
                bool initTexSucc = texture->initWithFile(texFilePath);
                assert(initTexSucc);
                m_textureList.push_back(texture);

            }



            //----convert mesh
            std::vector<KGEVertex> vList;
            int faceCount = (int)faceList.size();
            for (int faceIndex = 0; faceIndex < faceCount; faceIndex++){
                const KGETriangle&face = faceList[faceIndex];
                const KGETriangle&tface = tfaceList[faceIndex];
                for (int i = 0; i < 3; i++){
                    const int vID = face.vID(i);
                    const int tvID = tface.vID(i);
                    KGEVertex v;
                    v.pos = positionList[vID];
                    v.norm;
                    if (i == 0){
                        v.norm = faceV0NormList[faceIndex];
                    }
                    else if (i == 1){
                        v.norm = faceV1NormList[faceIndex];
                    }
                    else if (i == 2){
                        v.norm = faceV2NormList[faceIndex];
                    }
                    v.materialID = materialIDBase + materialRef;
                    v.textureID = textureIDBase + materialRef;
                    v.uv = uvList[tvID];
                    AddVertex(v);
                }
                int newVID0 = faceIndex * 3 + 0;
                int newVID1 = faceIndex * 3 + 1;
                int newVID2 = faceIndex * 3 + 2;
                triList.push_back(KGETriangle(newVID0, newVID1, newVID2));
            }//got vList and IDtriList
        }

        //--------------------------close file
        fclose(fp);

        return true;
    }

}; // end of namespace KGE
