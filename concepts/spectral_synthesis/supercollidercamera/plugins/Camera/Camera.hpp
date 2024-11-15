// PluginCamera.hpp
// Evan Murray (emurray49@gatech.edu)

#pragma once

#include "SC_PlugIn.hpp"

namespace SuperColliderCamera {

class Camera : public SCUnit {
public:
    Camera();

    // Destructor
    // ~Camera();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace SuperColliderCamera
