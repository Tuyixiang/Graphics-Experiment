//
// Created by Yixiang Tu on 2019/6/25.
//

#ifndef LAB2_PHOTONTEXTURE_H
#define LAB2_PHOTONTEXTURE_H

#include "../object/Triangle.h"
#include "Photon.h"
#include "Context.h"
#include "../config.h"


/**
 * 存光子的贴图
 */
class PhotonTexture {

protected:
  /**
   * 当接受到光子的时候，把光子均匀在一个圆中，圆的半径随着接受更多光子会减小至 1
   * 为了减少计算实际上就用正方形了
   * 因为有些面收到的光子总数很少，所以用此法均匀化
   */
  //double radius = 5;

  /**
   * Pixel per Area，单位面积的像素数量，决定了每个光子打上来时记录的亮度
   */
  double ppa;

  virtual void _receive(double u, double v, const Vec& vec) = 0;

public:
  int samples = 0;

  explicit PhotonTexture(double ppa) : ppa((100. / PM_BATCH_SIZE) * ppa) {}

  void receive(double u, double v, const Vec& vec) {
    _receive(u, v, vec * ppa);
    //radius = .0009765625 + radius * .9990234375; // 收敛至 1
  }

  virtual Vec& dat(double, double) = 0;

  virtual Vec get(double x, double y) {
    return dat(x, y) / samples;
  }

  /**
   * 结束 PM，除以采样量形成亮度贴图
   */
  virtual void complete(int sample_count) {};

  void _test_save(int sample_count, std::string filename, int resolution = 2000) {
    auto f = fopen(filename.data(), "wb");
    fprintf(f, "P6\n%d %d\n255\n", resolution, resolution);
    for (int j = 0; j < resolution; j += 1) {
      for (int i = 0; i < resolution; i += 1) {
        Pix p = dat(i / (double) resolution, j / (double) resolution) / sample_count;
        fwrite(p.v, 1, 3, f);
      }
    }
    fclose(f);
  }

};

/**
 * 物体单独用一个三角形贴图来存光子
 */
class TrianglePhotonTexture : public PhotonTexture {

protected:
  std::vector<Vec> data;
  int size;

public:
  /**
   * 通常而言三角形不会非常细长（或者也不知道哪一边细长）
   * 因此参数只给一个，形成一个等腰直角三角形的贴图
   */
  explicit TrianglePhotonTexture(double area, unsigned size)
    : PhotonTexture(((size + 1) * size / 2) / area), data((size + 1) * size / 2), size(size) {}

  void _receive(double u, double v, const Vec& vec) override {
    dat(u, v) += vec;
  }

  Vec& at(int x, int y) {
    return data[(x + y) * (x + y + 1) / 2 + x];
  }

  /**
   * 数据从角落开始斜着排布
   */
  Vec& dat(double x, double y) override {
    assert(x >= 0 and y >= 0 and x + y <= 1);
    auto u = static_cast<int>((1 - x + EMACH) * size);
    auto v = static_cast<int>((y - EMACH) * size);
    return at(u, v);
  }


};

class RectPhotonTexture : public PhotonTexture {

protected:
  Mat<Vec> data;

public:
  RectPhotonTexture(double area, unsigned width, unsigned height)
    : PhotonTexture((width * height) / area), data(width, height) {}

  void _receive(double u, double v, const Vec& vec) override;

  Vec& at(int x, int y) {
    return data.at(x, y);
  }

  Vec& dat(double x, double y) override {
    assert(x >= 0 and x <= 1 and y >= 0 and y <= 1);
    return at(static_cast<int>((x - EMACH) * data.width()), static_cast<int>((y - EMACH) * data.height()));
  }

  Vec get(double x, double y) override;

  void load(const std::string& filename);

  void save(const std::string& filename);

};

//class SharedBurnTexture : public PhotonTexture {
//
//protected:
//  Mat<Vec>& mat;
//
//public:
//  explicit SharedBurnTexture(Mat<Vec>& mat) : mat(mat) {}
//
//};


#endif //LAB2_PHOTONTEXTURE_H
