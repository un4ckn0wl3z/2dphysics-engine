#include "Application.h"
#include "Constants.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();
    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;
    m_particles.push_back(smallBall);

    Particle* bigBall = new Particle(50, 200, 3.0);
    bigBall->radius = 12;
    m_particles.push_back(bigBall);
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
    //m_particle->acceleration = Vec2(2.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    // apply some force to particle
    for (auto particle : m_particles) {
        Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        particle->AddForce(wind);
    }
    // apply weight
    for (auto particle : m_particles) {
        Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    for (auto particle : m_particles) {
        // apply acceleration and velocity
        particle->Integrate(deltaTime);

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