//
// Created by Yixiang Tu on 2019/6/18.
//

#include "Surface.h"


Surface::Surface(const std::function<Vec(double u, double v)>& func,
                 const std::function<Vec(double u, double v)>& df_du,
                 const std::function<Vec(double u, double v)>& df_dv,
                 Range u_domain, Range v_domain,
                 Vec color, Vec illumination, Vec position, Deflection deflection,
                 unsigned long u_steps, unsigned long v_steps, bool mesh_only)
  : Object(position, {}, deflection),
    func(func), df_du(df_du), df_dv(df_dv),
    u_domain(u_domain), v_domain(v_domain),
    color(color), illumination(illumination),
    mesh_only(mesh_only) {

  if (mesh_only) {
    createBasicMesh(u_steps, v_steps);
  } else {
    createMesh(u_steps, v_steps);
  }
}

Surface::Surface(std::function<Vec(double u, double v)> func,
                 std::function<Vec(double u, double v)> df_du,
                 std::function<Vec(double u, double v)> df_dv, Range u_domain, Range v_domain,
                 const std::string& texture_filename, std::function<Point(double u, double v)> anchor_func,
                 Vec illumination, Vec position, Deflection deflection, unsigned long u_steps, unsigned long v_steps,
                 bool mesh_only)
  : Object(position, {}, deflection),
    func(std::move(func)), df_du(std::move(df_du)), df_dv(std::move(df_dv)),
    u_domain(u_domain), v_domain(v_domain),
    color(color), illumination(illumination),
    texture(Texture::load(texture_filename)),
    anchor_func(std::move(anchor_func)),
    mesh_only(mesh_only) {

  if (mesh_only) {
    createBasicMesh(u_steps, v_steps);
  } else {
    createMesh(u_steps, v_steps);
  }
}

void Surface::createMesh(unsigned long u_steps, unsigned long v_steps) {
  std::vector<Triangle*> triangles;

  for (auto i = 0; i < u_steps; i += 1) {
    for (auto j = 0; j < v_steps; j += 1) {
      auto u1 = u_domain.left + i * u_domain.length() / u_steps;
      auto v1 = v_domain.left + j * v_domain.length() / v_steps;
      auto u2 = u_domain.left + (i + 1) * u_domain.length() / u_steps;
      auto v2 = v_domain.left + (j + 1) * v_domain.length() / v_steps;
      triangles.push_back(new SurfaceTriangle(
        this,
        {u1, v1}, {u1, v2}, {u2, v1}
      ));
      triangles.push_back(new SurfaceTriangle(
        this,
        {u2, v1}, {u1, v2}, {u2, v2}
      ));
    }
  }

  mesh = new Mesh(triangles);
}

void Surface::createBasicMesh(unsigned long u_steps, unsigned long v_steps) {
  std::vector<std::vector<Vertex>> vertices = {u_steps + 1, std::vector<Vertex>(v_steps + 1, Vertex())};
  std::vector<Triangle*> triangles;

  for (auto i = 0; i <= u_steps; i += 1) {
    for (auto j = 0; j <= v_steps; j += 1) {
      auto u = u_domain.left + i * u_domain.length() / u_steps;
      auto v = v_domain.left + j * v_domain.length() / v_steps;
      auto vertex_position = func(u, v) + position;
      vertices[i][j] = {vertex_position, normalized(df_dv(u, v) % df_du(u, v)), u, v};
      //aabb += vertex_position;
    }
  }

  for (auto i = 0; i < u_steps; i += 1) {
    for (auto j = 0; j < v_steps; j += 1) {
      triangles.push_back(new MeshSurfaceTriangle(
        // vertex positions
        vertices[i][j].position, vertices[i][j + 1].position, vertices[i + 1][j].position,
        // vertex normals
        vertices[i][j].normal, vertices[i][j + 1].normal, vertices[i + 1][j].normal,
        // texture file
        {},
        // anchors
        {vertices[i][j].u, vertices[i][j].v},
        {vertices[i][j + 1].u, vertices[i][j + 1].v},
        {vertices[i + 1][j].u, vertices[i + 1][j].v},
        illumination, deflection
      ));
      triangles.push_back(new MeshSurfaceTriangle(
        // vertex positions
        vertices[i + 1][j].position, vertices[i][j + 1].position, vertices[i + 1][j + 1].position,
        // vertex normals
        vertices[i + 1][j].normal, vertices[i][j + 1].normal, vertices[i + 1][j + 1].normal,
        // texture file
        {},
        // anchors
        {vertices[i + 1][j].u, vertices[i + 1][j].v},
        {vertices[i][j + 1].u, vertices[i][j + 1].v},
        {vertices[i + 1][j + 1].u, vertices[i + 1][j + 1].v},
        illumination, deflection
      ));
    }
  }

  mesh = new Mesh(triangles);
}

bool Surface::findPrecise(SolveContext& ctx, double epsilon, bool require_found) {
  auto success = false;
  auto initial = ctx.target->solveAnchor(ctx);

  auto u = initial.x;
  auto v = initial.y;

  auto t = ctx.distance;
  auto g = func(u, v) + position - (ctx.lgtSource + t * ctx.lgtDirection);

  auto recursion_count = 0;
  while (++recursion_count < 8) {
    if (maxabs(g) < epsilon) {
      success = true;
      break;
    }
    auto partial_u = df_du(u, v);
    auto partial_v = df_dv(u, v);
    auto A = partial_u % ctx.lgtDirection;
    auto B = g % partial_v;
    auto s = Vec{-(B * ctx.lgtDirection), A * g, -(B * partial_u)} / (A * partial_v);

    double nu;
    double nv;
    double nt;
    Vec ng;
    // damped newton algorithm
    double lambda = 1;
    while (true) {
      nu = u - s.x * lambda;
      nv = v - s.y * lambda;
      nt = t - s.z * lambda;
      ng = func(nu, nv) + position - (ctx.lgtSource + nt * ctx.lgtDirection);
      if (maxabs(ng) < maxabs(g)) {
        break;
      } else if (lambda < 1e-3) {
        recursion_count = 10000; // terminate
        break;
      }
      lambda *= .5;
    }
    u = nu;
    v = nv;
    t = nt;
    g = ng;
  }

  if (!require_found or (success and t > 1e-3)) {
    ctx.distance = t;
    ctx.solverData[2] = u;
    ctx.solverData[3] = v;
    return true;
  } else {
    return false;
  }
}

bool Surface::solvePosition(SolveContext& ctx) {
  return mesh->solvePosition(ctx);
}

void Surface::solveNormal(SolveContext& ctx) {
  if (mesh_only) {
    return ctx.target->solveNormal(ctx);
  }
  //ctx.itsNormal = dynamic_cast<SurfaceTriangle*>(ctx.target)->normal;
  //return;
  // goal: g->0
  findPrecise(ctx, 1e-4, false);
  auto& u = ctx.solverData[2];
  auto& v = ctx.solverData[3];
  ctx.itsNormal = normalized(df_dv(u, v) % df_du(u, v));
}

void Surface::solveColor(SolveContext& ctx) {
  //auto anchor = ctx.target->solveAnchor(ctx);
  //auto v = 1 - (anchor.x + anchor.y) * .5 * M_1_PI - 1. / 6;
  //ctx.itsColor = Vec::spectrum(v - floor(v)) * .4 + Vec{.6};
  //return;
  if (texture) {
    auto anchor = ctx.target->solveAnchor(ctx);
    ctx.itsColor = texture->dat(anchor_func(anchor.x, anchor.y));
  } else {
    ctx.itsColor = color;
  }
}

void Surface::solveIllumination(SolveContext& ctx) {
  ctx.itsIllumination = illumination;
}

