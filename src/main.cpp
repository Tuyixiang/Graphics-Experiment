#include "Render.h"
#include "Scene.h"
#include "pm/PMRender.h"
#include "pm/PMScene.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void pm() {

  auto& render = *getPMScene();
  render.run();
  exit(0);

  //vector<Triangle*> triangles;
  //for (auto i = 0; i < 100; i += 1) {
  //  auto v = Vec::random();
  //  triangles.push_back(new BBBTriangle(v, v + (v % Vec{0.001, 0, 0}), v + (v % Vec{0, 0, 0.001})));
  //}
  //auto x = Mesh(triangles);
  //exit(0);
}

int main(int argc, char* argv[]) {

  //pm();
  auto t = steady_clock::now();

  cout << "creating objects" << endl;
  int iterations = 0;
  auto& render = *getScene();

  cout << "loading saved state" << endl;
  render.image.load();

  cout << "preparation done within "
       << duration_cast<milliseconds>(steady_clock::now() - t).count() / 1000.0
       << "s" << endl;
  t = steady_clock::now();
  auto last_save_time = t;
  cout << "rendering..." << endl;
  {
    auto quit = false;
    auto pruned = false;
    while (!quit) {
      render.scan();
      bool save = false;
      printf("iterations: %d\n", ++iterations);

      // debug
#ifdef __APPLE__
      if (steady_clock::now() - t > 10800s) {
        render.image.save();
        exit(0);
      }
#endif

      cout << "time elapsed: "
           << duration_cast<milliseconds>(steady_clock::now() - t).count() / 1000.0
           << "s" << endl;
      if (steady_clock::now() - last_save_time > seconds(save_interval) or iterations == 1) {
        save = true;
        last_save_time = steady_clock::now();
      }
      if (not pruned and iterations > 64) {
        pruned = true;
        printf("pruning... ");
        //render.prune(iterations);
        printf("done\n");
      }
      if (save) {
        render.image.save();
      }
#ifdef USE_PHONG
      //break;
#endif
      //temp->clear();
    }
    //delete temp;
  }
  return 0;
}