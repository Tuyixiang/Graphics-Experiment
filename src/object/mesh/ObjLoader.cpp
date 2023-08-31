//
// Created by Yixiang Tu on 2019/5/28.
//

#include "ObjLoader.h"
#include "../Object.h"
#include "../Triangle.h"

Face readFace(char** s, Texture* texture = nullptr) {
  Face face;

  int v[3], t[3], n[3];
  if (sscanf(s[0], "%d/%d/%d", v, t, n) == 3) {
    for (auto i = 0; i < 3; i += 1) {
      sscanf(s[i], "%d/%d/%d", v + i, t + i, n + i);
      v[i] -= 1;
      t[i] -= 1;
      n[i] -= 1;
    }
    face.addV(v);
    face.addN(n);
    if (texture) {
      face.addT(t, texture);
    }
  } else if (sscanf(s[0], "%d//%d", v, t) == 2) {
    for (auto i = 0; i < 3; i += 1) {
      sscanf(s[i], "%d//%d", v + i, n + i);
      v[i] -= 1;
      n[i] -= 1;
    }
    face.addV(v);
    face.addN(n);
  } else if (sscanf(s[0], "%d/%d", v, t) == 2) {
    for (auto i = 0; i < 3; i += 1) {
      sscanf(s[i], "%d/%d", v + i, t + i);
      v[i] -= 1;
      t[i] -= 1;
    }
    face.addV(v);
    if (texture) {
      face.addT(t, texture);
    }
  } else {
    for (auto i = 0; i < 3; i += 1) {
      v[i] = std::stoi(s[i]);
      v[i] -= 1;
    }
    face.addV(v);
  }

  return face;
}

ObjLoader::ObjLoader(const std::string& filename, std::initializer_list<std::string> textureFiles) {
  static char* buffer[] = {new char[100], new char[100], new char[100]};

  for (auto& tf: textureFiles) {
    textures.push_back(Texture::load(tf));
  }
  int currentTexture = -1; // wait for "usemtl"
  printf("loading obj file \"%s\"\n", filename.data());
  char* line = nullptr;
  size_t len = 0;
  auto f = open(filename);
  while (getline(&line, &len, f) != -1) {
    if (line[0] == 'v' and line[1] == ' ') {
      double x, y, z;
      sscanf(line + 2, "%lf %lf %lf", &x, &y, &z);
      vertices.emplace_back(-x, y, z);
    } else if (line[0] == 'v' and line[1] == 'n' and line[2] == ' ') {
      double x, y, z;
      sscanf(line + 3, "%lf %lf %lf", &x, &y, &z);
      normals.push_back(normalized(Vec{x, -y, -z}));
    } else if (line[0] == 'v' and line[1] == 't' and line[2] == ' ') {
      double x, y;
      sscanf(line + 3, "%lf %lf", &x, &y);
      anchors.emplace_back(x, 1 - y);
    } else if (line[0] == 'f' and line[1] == ' ') {
      if (sscanf(line + 2, "%s %s %s", buffer[0], buffer[2], buffer[1]) == 3) {
        if (currentTexture >= 0 and currentTexture < textures.size()) {
          faces.push_back(readFace(buffer, textures[currentTexture]));
        } else {
          faces.push_back(readFace(buffer));
        }
      }
      if (sscanf(line + 2, "%s %*s %s %s", buffer[0], buffer[2], buffer[1]) == 3) {
        if (currentTexture >= 0 and currentTexture < textures.size()) {
          faces.push_back(readFace(buffer, textures[currentTexture]));
        } else {
          faces.push_back(readFace(buffer));
        }
      }
    } else {
      if (std::string(line, line + 7) == "usemtl " and currentTexture + 1 < textures.size()) {
        currentTexture += 1;
      }
    }
  }
  printf("loaded %lu vertices, %lu faces\n", vertices.size(), faces.size());
}

void ObjLoader::normalize(double xc, double yc, double zc) {
  Range x, y, z;
  for (auto& v: vertices) {
    x += v.x;
    y += v.y;
    z += v.z;
  }
  auto r = max(x.length(), y.length(), z.length());
  for (auto& v: vertices) {
    v.x = (v.x - x.left - x.length() * xc) / r;
    v.y = (v.y - y.left - y.length() * yc) / r;
    v.z = (v.z - z.left - z.length() * zc) / r;
  }
}

void ObjLoader::scale(double size) {
  for (auto& v: vertices) {
    v *= size;
  }
}

void ObjLoader::translate(const Vec& translation) {
  for (auto& v: vertices) {
    v += translation;
  }
}

void ObjLoader::rotateX(double theta) {
  for (auto& v: vertices) {
    auto ny = v.y * cos(theta) + v.z * sin(theta);
    auto nz = -v.y * sin(theta) + v.z * cos(theta);
    v.y = ny;
    v.z = nz;
  }
  for (auto& v: normals) {
    auto ny = v.y * cos(theta) + v.z * sin(theta);
    auto nz = -v.y * sin(theta) + v.z * cos(theta);
    v.y = ny;
    v.z = nz;
  }
}

void ObjLoader::rotateY(double theta) {
  for (auto& v: vertices) {
    auto nx = v.x * cos(theta) - v.z * sin(theta);
    auto nz = v.x * sin(theta) + v.z * cos(theta);
    v.x = nx;
    v.z = nz;
  }
  for (auto& v: normals) {
    auto nx = v.x * cos(theta) - v.z * sin(theta);
    auto nz = v.x * sin(theta) + v.z * cos(theta);
    v.x = nx;
    v.z = nz;
  }
}

void ObjLoader::rotateZ(double theta) {
  for (auto& v: vertices) {
    auto nx = v.x * cos(theta) - v.y * sin(theta);
    auto ny = v.x * sin(theta) + v.y * cos(theta);
    v.x = nx;
    v.y = ny;
  }
  for (auto& v: normals) {
    auto nx = v.x * cos(theta) - v.y * sin(theta);
    auto ny = v.x * sin(theta) + v.y * cos(theta);
    v.x = nx;
    v.y = ny;
  }
}

void ObjLoader::_computeNormal() {
  std::cout << "generating normal vectors" << std::endl;
  normals.clear();
  //assert(normals.empty());
  normals.assign(vertices.size(), {});
  for (auto& face: faces) {
    auto& v0 = vertices[face.v[0]];
    auto& v1 = vertices[face.v[1]];
    auto& v2 = vertices[face.v[2]];
    auto e1 = v1 - v0;
    auto e2 = v2 - v0;
    auto e3 = v2 - v1;
    auto l1 = normalized(e1);
    auto l2 = normalized(e2);
    auto l3 = normalized(e3);
    auto norm = normalized((v1 - v0) % (v2 - v0));
    normals[face.v[0]] += norm * acos(l1 * l2) * (e1 % e2).length();
    normals[face.v[1]] += norm * acos(-(l1 * l3)) * (e1 % e3).length();
    normals[face.v[2]] += norm * acos(l2 * l3) * (e2 % e3).length();
    face.addN(face.v[0], face.v[1], face.v[2]);
  }
  for (auto& n: normals) {
    n.normalize();
  }
}

std::vector<Triangle*>
ObjLoader::constructB(const Vec& color, const Vec& illumination, const Deflection& deflection) {
  printf("constructing triangles\n");
  std::vector<Triangle*> result;
  result.reserve(faces.size());

  for (auto& face: faces) {
    if (face.texture) {
      if (face.hasN) {
        result.push_back(new IABTriangle{
          vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
          normals[face.n[0]], normals[face.n[1]], normals[face.n[2]],
          *face.texture,
          anchors[face.t[0]], anchors[face.t[1]], anchors[face.t[2]],
          illumination,
          deflection
        });
      } else {
        result.push_back(new BABTriangle{
          vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
          *face.texture,
          anchors[face.t[0]], anchors[face.t[1]], anchors[face.t[2]],
          illumination,
          deflection
        });
      }
    } else {
      if (face.hasN) {
        result.push_back(new IBBTriangle{
          vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
          normals[face.n[0]], normals[face.n[1]], normals[face.n[2]],
          color,
          illumination,
          deflection
        });
      } else {
        result.push_back(new BBBTriangle{
          vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
          color,
          illumination,
          deflection
        });
      }
    }
  }
  return result;
}

std::vector<Triangle*>
ObjLoader::constructI(const Vec& color, const Vec& illumination, const Deflection& deflection) {
  printf("constructing triangles\n");
  std::vector<Triangle*> result;
  result.reserve(faces.size());

  _computeNormal();
  for (auto& face: faces) {
    if (face.texture) {
      result.push_back(new IABTriangle{
        vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
        normals[face.n[0]], normals[face.n[1]], normals[face.n[2]],
        *face.texture,
        anchors[face.t[0]], anchors[face.t[1]], anchors[face.t[2]],
        illumination,
        deflection
      });
    } else {
      result.push_back(new IBBTriangle{
        vertices[face.v[0]], vertices[face.v[1]], vertices[face.v[2]],
        normals[face.n[0]], normals[face.n[1]], normals[face.n[2]],
        color,
        illumination,
        deflection
      });
    }
  }

  return result;
}

void Face::addV(int v1, int v2, int v3) {
  hasV = true;
  v[0] = v1, v[1] = v2, v[2] = v3;
}

void Face::addN(int n1, int n2, int n3) {
  hasN = true;
  n[0] = n1, n[1] = n2, n[2] = n3;
}

void Face::addT(int t1, int t2, int t3, Texture* tex) {
  texture = tex;
  t[0] = t1, t[1] = t2, t[2] = t3;
}
