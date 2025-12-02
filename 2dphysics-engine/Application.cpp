#include "Application.h"
#include "Constants.h"
#include "Force.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();
    //Particle* smallBall = new Particle(50, 100, 1.0);
    //smallBall->radius = 4;
    //m_particles.push_back(smallBall);

    //Particle* bigBall = new Particle(50, 100, 3.0);
    //bigBall->radius = 12;
    //m_particles.push_back(bigBall);

    m_liquid.x = 0;
    m_liquid.y = Graphics::Height() / 2;
    m_liquid.w = Graphics::Width();
    m_liquid.h = Graphics::Height() / 2;

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
                if (event.key.keysym.sym == SDLK_w)
                    m_pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_d)
                    m_pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_s)
                    m_pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_a)
                    m_pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_w)
                    m_pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_d)
                    m_pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_s)
                    m_pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_a)
                    m_pushForce.x = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Particle* particle = new Particle(x, y, 1.0);
                    particle->radius = 5;
                    m_particles.push_back(particle);
                }
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
    //m_particle->acceleration = Vec2(2.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    // apply some force to particle
    for (auto particle : m_particles) {
        //Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        //particle->AddForce(wind);

        Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);

        particle->AddForce(m_pushForce);

        // if we are inside liquid, apply drag force
        if (particle->position.y >= m_liquid.y) {
            Vec2 drag = Force::GenerateDragForce(*particle, 0.04);
            particle->AddForce(drag);

        }

    }
    

    for (auto particle : m_particles) {
        // apply acceleration and velocity
        particle->Integrate(deltaTime);
    }

    for (auto particle : m_particles) {
        // limit boundaries
        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        }
        else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        }

        if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        }
        else if (particle->position.y + particle->radius >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }

    }

}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    // draw liquid
    Graphics::DrawFillRect(
        m_liquid.x + m_liquid.w / 2,
        m_liquid.y + m_liquid.h / 2,
        m_liquid.w,
        m_liquid.h,
        0xFF6E3713
        );

    for (auto particle : m_particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}


void Application::Destroy() {

    for (auto particle : m_particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}