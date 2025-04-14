#include "render/lidarrender.hpp"
#include "render/material.hpp"
#include "models/led.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace micrasverse::render {

LidarRender::LidarRender(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading, const Camera& camera)
    : LightObj(), lightColor(glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f), camera(camera), visible(false) {

    this->shader = Shader();
    this->shader.generate("src/render/assets/vertex-core.glsl", "src/render/assets/fragment-led.glsl");
    
    // Initialize with completely invisible model (zero size in all dimensions)
    this->renderModel = Led(
        glm::vec3(0.0f),  // Set to zero color initially
        glm::vec3(0.0f),  // Zero ambient
        glm::vec3(0.0f),  // Zero diffuse
        glm::vec3(0.0f),  // Zero specular
        glm::vec3(0.0f),  // Zero position
        glm::vec3(0.0f)   // Zero size
    );
    
    this->renderModel.init();
    
    // Force an update with the initial position
    this->update(rayMidPoint, rayDirection, reading);
}

void LidarRender::update(const b2Vec2 rayMidPoint, const b2Rot rayDirection, const float reading) {
    // Always show lidar lines
    visible = true;
    
    this->renderModel.setColor(glm::vec3(237.0f, 47.0f, 50.0f) / 255.0f);
    this->renderModel.setPose(glm::vec3(rayMidPoint.x, rayMidPoint.y, 0.02f), rayDirection);
    this->renderModel.setSize(glm::vec3(0.002f, reading, 0.02f));
}

void LidarRender::render(const glm::mat4 view, const glm::mat4 projection) {
    // Skip rendering entirely if not visible
    if (!visible) {
        return;
    }
    
    // Double-check size before rendering (extra safety)
    if (this->renderModel.size.x <= 0.0f || this->renderModel.size.y <= 0.0f) {
        return;
    }
    
    // Set shader uniforms
    this->shader.activate(view, projection);
    this->shader.set3Float("lightPosition", this->renderModel.position);
    this->shader.set3Float("viewPosition", this->camera.position);
    
    this->shader.set3Float("light.ambient", this->renderModel.ambient);
    this->shader.set3Float("light.diffuse", this->renderModel.diffuse);
    this->shader.set3Float("light.specular", this->renderModel.specular);
    
    // Render the model
    this->renderModel.render(shader, true);
}

} // namespace micrasverse::render
