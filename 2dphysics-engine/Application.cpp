#include "Application.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();
    m_particle = new Particle(50, 100, 1.0);
}


void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    m_running = false;
                break;
        }
    }
}

void Application::Update() {
    m_particle->velocity = Vec2(2.0, 0.0);
    m_particle->position + m_particle->velocity;
}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(m_particle->position.x, m_particle->position.y, 4, 0xFFFFFFFF);
    Graphics::RenderFrame();
}


void Application::Destroy() {

    delete m_particle;

    Graphics::CloseWindow();
}