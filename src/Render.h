//
// Created by Yixiang Tu on 2019/5/24.
//

#ifndef LAB2_RENDER_H
#define LAB2_RENDER_H

#include "Basics.h"
#include "config.h"
#include "object/Object.h"


struct Image {
  struct Pixel {
    double r = 0;
    double g = 0;
    double b = 0;
    int count = 0;
    bool canSkip = true;
    double maxDiff = 1;

    Pixel& operator+=(const Vec& v);

    bool skip() const {
      return canSkip and (count > 200 and rnd() > maxDiff * 20);// or rnd() > diff.length() * 5));
    }

    void getValue(unsigned char c[]) const;

    Pixel operator+(const Pixel& other) const {
      Pixel sum;
      sum.r = r + other.r;
      sum.g = g + other.g;
      sum.b = b + other.b;
      sum.count = count + other.count;
      sum.maxDiff = max(maxDiff, other.maxDiff);
      return sum;
    }
  };

  Pixel* pixel[Width];

  Image() {
    for (auto& p: pixel) {
      p = new Pixel[Height];
    }
  }

  void clear() {
    for (auto& p: pixel) {
      memset(p, 0, sizeof(Pixel) * Height);
    }
  }

  void load() {
    auto f = fopen("state.save", "rb");
    if (f) {
      for (auto x = 0; x < Width; x += 1) {
        fread(pixel[x], Height * sizeof(Pixel), 1, f);
      }
      fclose(f);
    }
  }

  void save(bool dump_state = true, std::string filename = "out.ppm") {
    std::cout << "save" << std::flush;
#ifndef USE_PHONG
    if (dump_state) {
      auto s = fopen("state.save", "wb");
      for (auto x = 0; x < Width; x += 1) {
        fwrite(pixel[x], Height * sizeof(Pixel), 1, s);
      }
      fclose(s);
    }
#endif
    auto f = fopen(filename.data(), "wb");
    fprintf(f, "P6\n%d %d\n255\n", Width, Height);
    for (auto y = Height - 1; y >= 0; y -= 1) {
      for (auto x = 0; x < Width; x += 1) {
        unsigned char c[3];
        pixel[x][y].getValue(c);
        fwrite(c, 1, 3, f);
      }
    }
    fclose(f);
    std::cout << "d" << std::endl;
  }

  Image operator+(const Image& other) const {
    Image sum;
    for (auto x = 0; x < Width; x += 1) {
      for (auto y = 0; y < Height; y += 1) {
        sum.pixel[x][y] = pixel[x][y] + other.pixel[x][y];
      }
    }
    return sum;
  }

  Image& operator+=(const Image& other) {
    for (auto x = 0; x < Width; x += 1) {
      for (auto y = 0; y < Height; y += 1) {
        pixel[x][y] = pixel[x][y] + other.pixel[x][y];
      }
    }
    return *this;
  }

  ~Image() {
    for (auto p: pixel) {
      delete[] p;
    }
  }
};

class Render {

public:
  // perspective
  double fov = 72;
  Vec source = {0, 0, -1};
  double elevation_cos = 1;
  double elevation_sin = 0;
  double pan_cos = 1;
  double pan_sin = 0;

  // field depth
  double blur_radius = 0;
  double focus = 1;

  Image image;
  std::vector<Object*> objects;

  Render() = default;

  Render(std::vector<Object*> objects);

  void setElevation(double delta) {
    elevation_cos = cos(delta);
    elevation_sin = sin(delta);
  }

  void setPan(double delta) {
    pan_cos = cos(delta);
    pan_sin = sin(delta);
  }

  virtual void scan();

  // remove unnecessary objects
  void prune(int count) {
    std::vector<Object*> useful_objects;
    for (auto* object: objects) {
      if (object->prune(count)) {
        // delete object;
      } else {
        useful_objects.push_back(object);
      }
    }
    objects.swap(useful_objects);
  }

  virtual SolveContext findIntersection(const Vec& source, const Vec& direction);

  virtual Vec cast(const Vec& vec, Vec source);

};

#endif // LAB2_RENDER_H
