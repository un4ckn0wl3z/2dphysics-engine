#pragma once

#include "./Graphics.h"
#include "Particle.h"

class Application {
    private:
        bool m_running = false;
        Particle* m_particle;

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