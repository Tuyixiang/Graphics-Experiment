//
// Created by Yixiang Tu on 2019/5/28.
//

#ifndef LAB2_OBJLOADER_H
#define LAB2_OBJLOADER_H


#include "../../Basics.h"
#include <fstream>

struct Texture;
struct Triangle;
struct Deflection;

struct Face {
  bool hasV = false;
  bool hasN = false;
  Texture* texture = nullptr;
  int v[3] = {};
  int n[3] = {};
  int t[3] = {};

  Face() = default;

  void addV(int v1, int v2, int v3);

  void addV(int v[]) {
    addV(v[0], v[1], v[2]);
  }

  void addN(int n1, int n2, int n3);

  void addN(int n[]) {
    addN(n[0], n[1], n[2]);
  }

  void addT(int t1, int t2, int t3, Texture* tex);

  void addT(int t[], Texture* tex) {
    addT(t[0], t[1], t[2], tex);
  }
};

class ObjLoader {
public:
  std::vector<Vec> vertices;
  std::vector<Vec> normals;
  std::vector<Point> anchors;
  std::vector<Face> faces;
  std::vector<Texture*> textures;

public:
  explicit ObjLoader(const std::string& filename, std::initializer_list<std::string> textureFiles = {});

  void normalize(double xc = 0, double yc = 0, double zc = 0);

  void scale(double size);

  void translate(const Vec& translation);

  void rotateX(double theta);

  void rotateY(double theta);

  void rotateZ(double theta);

  // compute the normal for each vertex
  void _computeNormal();

  std::vector<Triangle*>
  constructB(const Vec& color, const Vec& illumination, const Deflection& deflection);

  std::vector<Triangle*>
  constructI(const Vec& color, const Vec& illumination, const Deflection& deflection);

};


#endif //LAB2_OBJLOADER_H
