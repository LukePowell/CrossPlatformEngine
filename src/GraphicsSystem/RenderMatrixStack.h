//
//  RenderMatrixStack.h
//  SFMLTest
//
//  Created by Luke Powell on 3/12/14.
//
//

#ifndef __SFMLTest__RenderMatrixStack__
#define __SFMLTest__RenderMatrixStack__

#include <deque>
#include <list>
#include "Maths/MathLib.h"

namespace ElysiumEngine
{
    typedef std::list<Matrix>::iterator MatStackIterator;
    
    //4: USER:
    //2: PROJECTION
    //1: VIEW
    //0: I
    class RenderMatrixStack
    {
    public:
        RenderMatrixStack();
        void pushState();//Create state to pop back to if needed
        void popState();//Return to saved state
        void pushMatrix(Matrix m);//Add a new matrix to the statck
        void popMatrix();//Pop the top most matrix
        void popMatrices(int n);//Pop the n top matrices
        
        const Matrix &getProjection() const;
        const Matrix &getView() const;
    
        const Matrix &getTop() const;
        
        void setProjection(const Matrix &m);
        void setView(const Matrix &m);
    private:
        std::list<int> popStates;
        std::deque<Matrix> stack;
    };
}

#endif /* defined(__SFMLTest__RenderMatrixStack__) */
