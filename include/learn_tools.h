#pragma once

#include <cassert>
#include <iostream>

#include "mesh.h"

void ExtractVertexsPos(float* vertices, int arraySize, int stride, int count, int offset, vector<Vertex>& vertexs) {
  int groups = arraySize / stride;
  if (vertexs.size() == 0) {
    std::cerr << "未定义顶点size!!! " << std::endl;
    assert(vertexs.size() != 0);
  }
  for (int i = 0; i < groups; i++) {
    int index = i * stride + offset;
    for (int j = 0; j < count; j++) {
      vertexs[i].Position[j] = vertices[index + j];
    }
  }
}

void ExtractVertexsNormal(float* vertices, int arraySize, int stride, int count, int offset, vector<Vertex>& vertexs) {
  int groups = arraySize / stride;
  if (vertexs.size() == 0) {
    std::cerr << "未定义顶点size!!! " << std::endl;
    assert(vertexs.size() != 0);
  }
  for (int i = 0; i < groups; i++) {
    int index = i * stride + offset;
    for (int j = 0; j < count; j++) {
      vertexs[i].Normal[j] = vertices[index + j];
    }
  }
}
