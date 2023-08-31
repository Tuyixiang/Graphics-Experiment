//
// Created by Yixiang Tu on 2019/5/30.
//

#ifndef LAB2_SKYBOX_H
#define LAB2_SKYBOX_H

#include "../Object.h"

class SkyBox : public Object {

  const Texture& front;
  const Texture& back;
  const Texture& up;
  const Texture& down;
  const Texture& left;
  const Texture& right;
  SkyBox* lowDefChild = nullptr;

  double rotationSin = 0;
  double rotationCos = 1;

public:
  SkyBox(std::vector<Texture*> textures, double theta = 0)
    : Object({}, {}, {}),
      front(*textures[2]),
      back(*textures[4]),
      up(*textures[0]),
      down(*textures[5]),
      left(*textures[1]),
      right(*textures[3]) {
    if (front.width() > 1024) {
      lowDefChild = new SkyBox{
        *front.scaleDown(32),
        *back.scaleDown(32),
        *up.scaleDown(32),
        *down.scaleDown(32),
        *left.scaleDown(32),
        *right.scaleDown(32),
      };
    }
    setRotation(theta);
  }

  SkyBox(const std::string& packName, double theta = 0)
    : Object({}, {}, {}),
      front(*Texture::load(packName + "/front.pbm")),
      back(*Texture::load(packName + "/back.pbm")),
      up(*Texture::load(packName + "/up.pbm")),
      down(*Texture::load(packName + "/down.pbm")),
      left(*Texture::load(packName + "/left.pbm")),
      right(*Texture::load(packName + "/right.pbm")) {
    if (front.width() > 1024) {
      lowDefChild = new SkyBox{
        *front.scaleDown(32),
        *back.scaleDown(32),
        *up.scaleDown(32),
        *down.scaleDown(32),
        *left.scaleDown(32),
        *right.scaleDown(32),
      };
    }
    setRotation(theta);
  }

  SkyBox(const Texture& front,
         const Texture& back,
         const Texture& up,
         const Texture& down,
         const Texture& left,
         const Texture& right,
         double theta = 0) :
    front(front), back(back), up(up), down(down), left(left), right(right) {
    setRotation(theta);
  }

  bool solvePosition(SolveContext& ctx) override;

  void setRotation(double theta) {
    rotationSin = sin(theta);
    rotationCos = cos(theta);
  }

  void solveColor(SolveContext& ctx) override {}

  void solveIllumination(SolveContext& ctx) override {}

  void solveNormal(SolveContext& ctx) override {}

  static SkyBox* loadImage(const std::string& filename, double delta = 0) {
    static int x_offset[] = {1, 0, 1, 2, 3, 1};
    static int y_offset[] = {0, 1, 1, 1, 1, 2};
    auto image = Texture::load(filename);
    assert(image->width() * 3 == image->height() * 4);
    auto block_size = static_cast<int>(image->width() / 4);

    std::vector<Texture*> textures;

    for (auto i = 0; i < 6; i += 1) {
      textures.push_back(new Texture(block_size, block_size));
      for (auto y = 0; y < block_size; y += 1) {
        memcpy((*textures.back())[y].data(),
               &(*image)[y + y_offset[i] * block_size][x_offset[i] * block_size],
               block_size * sizeof(Pix));
      }
    }
    delete image;
    return new SkyBox(textures, delta);
  }

};


#endif //LAB2_SKYBOX_H
