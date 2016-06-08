#include "stdafx.h"
#include "util.h"

#include <assert.h>

namespace KGE
{
    static char buffer[1024] = { 0 };//used for fscan string

    void numberToStr(int num, std::string & str)
    {
        char numbuf[128];
        ::sprintf_s(numbuf, "%i", num);
        str = numbuf;
    }
    
    int charToNumber(char c){
        assert('0'<'9');
        assert(c >= '0');
        assert(c <= '9');
        return (c - '0');
    }

    int strToNumber(const std::string&numStr){
        int number = 0;
        int len = (int)numStr.size();
        for (int i = 0; i<len; i++){
            char c = numStr[i];
            int num = charToNumber(c);
            number *= 10;
            number += num;
        }//got number
        return number;
    }

    std::vector<std::string> divideStr(const std::string&str, const std::string&dividerChars)
    {
        const int dividerCharCount = (int)dividerChars.size();
        std::vector<std::string> strList;
        std::string t_str;
        int len = (int)str.size();
        for (int i = 0; i < len; i++){
            char c = str[i];
            bool cIsDividerCh = false;
            for (int j = 0; j < dividerCharCount; j++){
                char divierCh = dividerChars[j];
                if (c == divierCh){
                    cIsDividerCh = true;
                    break;
                }
            }//got cIsDividerCh
            if (cIsDividerCh == false){
                t_str += c;
            }
            else{
                strList.push_back(t_str);
                t_str.clear();
            }
        }
        if (t_str.empty() == false){
            strList.push_back(t_str);
        }//got strList;
        return strList;

    }

    Vector4 fscanVector4(FILE * fp){
        assert(fp);
        float arr[4];
        for (int i = 0; i < 4; i++){
            fscanf(fp, "%f", &arr[i]);
        }
        return Vector4(arr[0], arr[1], arr[2], arr[3]);
    }
    Vector4 fscanVector3(FILE * fp){
        assert(fp);
        float arr[3];
        for (int i = 0; i < 3; i++){
            fscanf(fp, "%f", &arr[i]);
        }
        return Vector4(arr[0], arr[1], arr[2], 1);
    }
    Vector2 fscanVector2(FILE * fp){
        assert(fp);
        float arr[2];
        for (int i = 0; i < 2; i++){
            fscanf(fp, "%f", &arr[i]);
        }
        return Vector2(arr[0], arr[1]);
    }
    float fscanFloat(FILE * fp){
        assert(fp);
        float temp = 0;
        fscanf(fp, "%f", &temp);
        return temp;

    }
    int fscanInt(FILE * fp){
        assert(fp);
        int temp = 0;
        fscanf(fp, "%i", &temp);
        return temp;
    }
    std::string fscanStr(FILE*fp, const std::string&expectedStr){
        assert(fp);
        fscanf(fp, "%s", buffer);
        std::string str = buffer;
        if (expectedStr.empty() == false){
            assert(str == expectedStr);
        }
        return str;
    }
    std::string fscanOneQuotation(FILE * fp){
        assert(fp);
        std::string quoatation = "";
        fscanf(fp, "%s", buffer);
        std::string beginStr = buffer;
        assert(beginStr[0] == '\"');
        quoatation += beginStr;
        while (1){
            char endCh = quoatation[(int)quoatation.size() - 1];
            if (endCh == '\"'){
                break;
            }
            fscanf(fp, "%s", buffer);
            std::string str = buffer;
            quoatation += str;

        }//got quoatation
        return quoatation;

    }
}
