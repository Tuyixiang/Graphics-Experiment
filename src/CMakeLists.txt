project(lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -fopenmp")

add_executable(lab2
        main.cpp
        object/Object.h
        Basics.h
        object/Triangle.cpp
        object/Triangle.h
        object/triangle/TriangleSpacial.cpp
        object/triangle/TriangleSpacial.h
        object/triangle/TriangleColor.h
        object/triangle/TriangleBase.h
        object/triangle/TriangleIllumination.h
        Render.cpp
        Render.h
        object/triangle/ParallelogramSpacial.cpp
        object/triangle/ParallelogramSpacial.h
        object/Parallelogram.cpp
        object/Parallelogram.h
        Scene.cpp
        Scene.h
        object/mesh/Mesh.cpp
        object/mesh/Mesh.h
        object/mesh/ObjLoader.cpp
        object/mesh/ObjLoader.h
        Basic.cpp
        object/Object.cpp
        object/other/SkyBox.cpp
        object/other/SkyBox.h
        object/other/Sphere.cpp
        object/other/Sphere.h
        config.h
        object/surface/Surface.cpp
        object/surface/Surface.h
        pm/PMRender.cpp
        pm/PMRender.h pm/PMObject.cpp pm/PMObject.h pm/Photon.cpp pm/Photon.h pm/Context.h pm/PhotonTexture.cpp pm/PhotonTexture.h pm/PMRect.h pm/PMScene.cpp pm/PMScene.h)
