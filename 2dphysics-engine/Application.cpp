#include "Application.h"
#include "Constants.h"

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

    // frame-rate lock
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }

    // calculate deltatime
    float deltatime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;

    // deltatime protection
    //if (deltatime > 0.016) {
    //    deltatime = 0.016;
    //}

    timePreviousFrame = SDL_GetTicks();

    // update logic
    m_particle->acceleration = Vec2(0.0, 9.8 * PIXELS_PER_METER);

    // apply acceleration and velocity
    m_particle->velocity += m_particle->acceleration * deltatime;
    m_particle->position += m_particle->velocity * deltatime;
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