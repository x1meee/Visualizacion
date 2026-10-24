
#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

//Clase para implementar cámara orbital
class OrbitCamera
{
private:

    glm::vec3 m_pos;

    // Centro de atención
    glm::vec3 m_coi;

    // Vector perpendicular a la cámara.
    glm::vec3 m_up;

    float m_radius;

    float m_theta;

    // Ángulo vertical
    float m_phi;

    // Calcula ángulos
    glm::vec2 getAngles(glm::vec2 mouseInit, glm::vec2 mouseCurr);

public:
    // Constructor
    OrbitCamera(glm::vec3 pos, glm::vec3 coi, glm::vec3 up)
        : m_pos{pos},
          m_coi{coi},
          m_up{up}
    {
        m_radius = glm::distance(m_pos, m_coi);

        glm::vec3 direction = m_pos - m_coi;

        m_theta = std::atan2(direction.z, direction.x);
        m_phi = std::asin(direction.y / m_radius);
    }

    //Devuelve la matriz de vista
    glm::mat4 getMatrix();

    //Actualiza los ángulos
    void setPos(glm::vec2 mouseInit, glm::vec2 mouseCurr);

    //Rota la cámara
    void rotate(glm::vec2 mouseInit, glm::vec2 mouseCurr);
};

#endif
