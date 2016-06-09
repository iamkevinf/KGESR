#ifndef __KGE_RANGE_H__
#define __KGE_RANGE_H__

#include <iostream> 
#include <assert.h>


class Range
{
public:
    Range()
    {
        initMembers();
    }

    Range(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
    {
        initMembers();
        init(xmin, xmax, ymin, ymax, zmin, zmax);
    }

    virtual ~Range(){};

    bool init(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
    {
        assert(xmax >= xmin);
        assert(ymax >= ymin);
        assert(zmax >= zmin);

        m_xmin = xmin;
        m_xmax = xmax;
        m_ymin = ymin;
        m_ymax = ymax;
        m_zmin = zmin;
        m_zmax = zmax;

        return true;
    }

    float getMinX()const{ return m_xmin; } 
    float getMaxX()const{ return m_xmax; } 
    float getMinY()const{ return m_ymin; } 
    float getMaxY()const{ return m_ymax; } 
    float getMinZ()const{ return m_zmin; } 
    float getMaxZ()const{ return m_zmax; } 
    float getSpanX()const{ return m_xmax - m_xmin; } 
    float getSpanY()const{ return m_ymax - m_ymin; } 
    float getSpanZ()const{ return m_zmax - m_zmin; }

protected:
    void initMembers(){
        m_xmin = m_xmax = 0; 
        m_ymin = m_ymax = 0; 
        m_zmin = m_zmax = 0;
    }

protected:
    float m_xmin, m_xmax;
    float m_ymin, m_ymax;
    float m_zmin, m_zmax;
};


#endif // __KGE_RANGE_H__
