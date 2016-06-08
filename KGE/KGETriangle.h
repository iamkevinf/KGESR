#ifndef __KGE_TRIANGLE_H__
#define __KGE_TRIANGLE_H__

namespace KGE
{

    class KGETriangle
    {
    public:
        KGETriangle();
        KGETriangle(int ID0, int ID1, int ID2);
        ~KGETriangle();

        short vID(int i)const;
        void SetVID(int i, short vID);

    private:
        short _array[3];
    };

}; // end of namespace KGE

#endif // __KGE_TRIANGLE_H__

