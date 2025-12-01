#include "Application.h"
#include "Constants.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();
    m_particle = new Particle(50, 100, 1.0);
    m_particle->radius = 4;
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
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;

    // deltatime protection for debug purpose
    //if (deltatime > 0.016) {
    //    deltatime = 0.016;
    //}

    timePreviousFrame = SDL_GetTicks();

    // update logic
    m_particle->acceleration = Vec2(2.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    // apply acceleration and velocity
    m_particle->Integrate(deltaTime);

    // limit boundaries
    if (m_particle->position.x - m_particle->radius <= 0) {
        m_particle->position.x = m_particle->radius;
        m_particle->velocity.x *= -0.9;
    }
    else if (m_particle->position.x + m_particle->radius >= Graphics::Width()) {
        m_particle->position.x = Graphics::Width() - m_particle->radius;
        m_particle->velocity.x *= -0.9;
    }

    if (m_particle->position.y - m_particle->radius <= 0) {
        m_particle->position.y = m_particle->radius;
        m_particle->velocity.y *= -0.9;
    }
    else if (m_particle->position.y + m_particle->radius >= Graphics::Height()) {
        m_particle->position.y = Graphics::Height() - m_particle->radius;
        m_particle->velocity.y *= -0.9;
    }



}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(m_particle->position.x, m_particle->position.y, m_particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}


void Application::Destroy() {

    delete m_particle;

    Graphics::CloseWindow();
}