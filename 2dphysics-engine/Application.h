#pragma once

#include "./Graphics.h"
#include "Particle.h"
#include <vector>

class Application {
    private:
        bool m_running = false;
        std::vector<Particle*> m_particles;
        Vec2 m_pushForce = Vec2(0, 0);
        //SDL_Rect m_liquid;
        Vec2 m_mouseCursor = Vec2(0, 0);
        bool m_leftMouseButtonDown = false;
        Vec2 m_anchor = Vec2(0, 0);
        float m_spring_k = 40;
        float m_restLength = 200;


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