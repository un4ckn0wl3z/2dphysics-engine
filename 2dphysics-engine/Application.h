#pragma once

#include "./Graphics.h"
#include "Body.h"
#include <vector>

class Application {
    private:
        bool m_running = false;
        std::vector<Body*> m_bodies;
        Vec2 m_pushForce = Vec2(0, 0);
        //SDL_Rect m_liquid;
        Vec2 m_mouseCursor = Vec2(0, 0);
        bool m_leftMouseButtonDown = false;
        const int m_NUM_BODIES = 1;


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