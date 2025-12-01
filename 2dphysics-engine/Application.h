#pragma once

#include "./Graphics.h"
#include "Particle.h"
#include <vector>

class Application {
    private:
        bool m_running = false;
        std::vector<Particle*> m_particles;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};