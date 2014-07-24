//
//  RenderMatrixStack.cpp
//  SFMLTest
//
//  Created by Luke Powell on 3/12/14.
//
//

#include "RenderMatrixStack.h"


ElysiumEngine::RenderMatrixStack::RenderMatrixStack()
{
    Matrix I;
    I.Identity();
    stack.push_back(I);
    //Set up our view and project defaults
    pushMatrix(I);
    pushMatrix(I);
}

void ElysiumEngine::RenderMatrixStack::pushMatrix(Matrix m)
{
    Matrix temp = m * stack.back();
    stack.push_back(temp);
}

void ElysiumEngine::RenderMatrixStack::popMatrix()
{
    stack.pop_back();
}

void ElysiumEngine::RenderMatrixStack::pushState()
{
    popStates.push_back(stack.size());
}

void ElysiumEngine::RenderMatrixStack::popState()
{
    int value = popStates.back();
    popStates.pop_back();
    stack.resize(value);
}

void ElysiumEngine::RenderMatrixStack::popMatrices(int n)
{
    while(n > 0)
    {
        n--;
        stack.pop_back();
    }
}

const Matrix &ElysiumEngine::RenderMatrixStack::getProjection() const
{
    return stack[1];
}

const Matrix &ElysiumEngine::RenderMatrixStack::getView() const
{
    return stack[2];
}

void ElysiumEngine::RenderMatrixStack::setProjection(const Matrix &m)
{
    stack[1] = m;//stack[1] * m;
}

void ElysiumEngine::RenderMatrixStack::setView(const Matrix &m)
{
    stack[2] = m;//* stack[1];
}

const Matrix &ElysiumEngine::RenderMatrixStack::getTop() const
{
    return  stack.back();
}