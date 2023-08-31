//
// Created by Yixiang Tu on 2019/6/25.
//

#include "PhotonTexture.h"

Vec RectPhotonTexture::get(double x, double y) {
  auto u = max(0., (x - EMACH) * data.width());
  auto v = max(0., (y - EMACH) * data.height());
  if (u < data.width() - 1) {
    if (v < data.height() - 1) {
      return (at((int) u, (int) v) * ((ceil(u) - u) * (ceil(v) - v)) +
              at((int) u, (int) v + 1) * ((ceil(u) - u) * (v - floor(v))) +
              at((int) u + 1, (int) v) * ((u - floor(u)) * (ceil(v) - v)) +
              at((int) u + 1, (int) v + 1) * ((u - floor(u)) * (v - floor(v)))) / samples;
    } else {
      return (at((int) u, (int) v) * (ceil(u) - u) +
              at((int) u + 1, (int) v) * (u - floor(u))) / samples;
    }
  } else {
    if (v < data.height() - 1) {
      return (at((int) u, (int) v) * (ceil(v) - v) +
              at((int) u, (int) v + 1) * (v - floor(v))) / samples;
    } else {
      return at((int) u, (int) v) / samples;
    }
  }
}

void RectPhotonTexture::_receive(double u, double v, const Vec& vec) {
  if (std::isnan(u) or std::isnan(v)) {
    printf("#");
    return;
  }

  dat(u, v) += vec;

  //auto x_anchor = u * data.width();
  //auto y_anchor = v * data.height();
  //auto x_lo = max(0, static_cast<int>(x_anchor - radius));
  //auto x_hi = min(static_cast<int>(data.width() - 1), static_cast<int>(x_anchor + radius));
  //auto r2 = pow<2>(radius);
  //for (auto x = x_lo; x <= x_hi; x += 1) {
  //  auto y_lo = max(0, static_cast<int>(y_anchor - sqrt(r2 - pow<2>(x + .5 - x_anchor))));
  //  auto y_hi = min(static_cast<int>(data.height() - 1),
  //                  static_cast<int>(y_anchor + sqrt(r2 - pow<2>(x + .5 - x_anchor))));
  //  for (auto y = y_lo; y <= y_hi; y += 1) {
  //    auto d = (r2 - pow<2>(x + .5 - x_anchor) - pow<2>(y + .5 - y_anchor));
  //    if (d < 0) {
  //      continue;
  //    }
  //    at(x, y) += vec * d / pow<2>(r2);
  //  }
  //}

}

void RectPhotonTexture::load(const std::string& filename) {
  auto f = fopen(filename.data(), "rb");

  if (not f) {
    return;
  }

  unsigned width, height;
  fread(&ppa, sizeof(double), 1, f);
  fread(&samples, sizeof(int), 1, f);
  fread(&width, sizeof(unsigned), 1, f);
  fread(&height, sizeof(unsigned), 1, f);

  if (width * height >= 10000) {
    // 太小的贴图就不要丢人了
    std::cout << "loading photon texture \"" << filename << "\"\r" << std::flush;
  }

  data = {width, height};
  for (auto i = 0; i < height; i += 1) {
    fread(data[i].data(), sizeof(Vec), width, f);
  }

  fclose(f);
}

void RectPhotonTexture::save(const std::string& filename) {
  printf("saving photon texture %s\n", filename.data());
  auto f = fopen(filename.data(), "wb");

  auto width = static_cast<unsigned>(data.width()), height = static_cast<unsigned>(data.height());
  printf("%lu %lu\n", data.width(), data.height());
  fwrite(&ppa, sizeof(double), 1, f);
  fwrite(&samples, sizeof(int), 1, f);
  fwrite(&width, sizeof(unsigned), 1, f);
  fwrite(&height, sizeof(unsigned), 1, f);

  for (auto i = 0; i < height; i += 1) {
    fwrite(data[i].data(), sizeof(Vec), width, f);
  }

  fclose(f);
}
