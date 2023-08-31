//
// Created by Yixiang Tu on 2019/6/18.
//

#ifndef LAB2_SURFACE_H
#define LAB2_SURFACE_H


#include "../Object.h"
#include "../mesh/Mesh.h"

class Surface : public Object {

  friend class SurfaceTriangleSpacial;
  friend class SurfaceTriangle;

protected:
  struct Vertex {
    Vec position = {};
    Vec normal = {};
    double u = 0;
    double v = 0;

    Vertex() = default;

    Vertex(Vec position, Vec normal, double u, double v) : position(position), normal(normal), u(u), v(v) {}
  };

  // function and derivatives
  const std::function<Vec(double u, double v)> func;
  const std::function<Vec(double u, double v)> df_du;
  const std::function<Vec(double u, double v)> df_dv;

  const Range u_domain;
  const Range v_domain;
  Mesh* mesh = nullptr;

  // texture
  Vec color = {};
  Vec illumination = {};
  Texture* texture = nullptr;
  const std::function<Point(double u, double v)> anchor_func;

  // use mesh only
  bool mesh_only = false;

public:
  Surface(const std::function<Vec(double u, double v)>& func,
          const std::function<Vec(double u, double v)>& df_du,
          const std::function<Vec(double u, double v)>& df_dv,
          Range u_domain, Range v_domain,
          Vec color,
          Vec illumination,
          Vec position = {0},
          Deflection deflection = {1, 1, 1},
          unsigned long u_steps = 100, unsigned long v_steps = 100,
          bool mesh_only = false
  );

  Surface(std::function<Vec(double u, double v)> func,
          std::function<Vec(double u, double v)> df_du,
          std::function<Vec(double u, double v)> df_dv,
          Range u_domain, Range v_domain,
          const std::string& texture_filename,
          std::function<Point(double u, double v)> anchor_func,
          Vec illumination,
          Vec position = {0},
          Deflection deflection = {1, 1, 1},
          unsigned long u_steps = 100, unsigned long v_steps = 100,
          bool mesh_only = false
  );

  void createBasicMesh(unsigned long u_steps, unsigned long v_steps);

  void createMesh(unsigned long u_steps, unsigned long v_steps);

  bool findPrecise(SolveContext& ctx, double epsilon, bool require_found);

  bool solvePosition(SolveContext& ctx) override;

  void solveNormal(SolveContext& ctx) override;

  void solveColor(SolveContext& ctx) override;

  void solveIllumination(SolveContext& ctx) override;


};


#endif //LAB2_SURFACE_H
