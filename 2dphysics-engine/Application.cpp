#include "Application.h"
#include "Constants.h"
#include "Force.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();
    //Particle* smallPlanet = new Particle(200, 200, 1.0);
    //smallPlanet->radius = 6;
    //m_particles.push_back(smallPlanet);

    //Particle* bigPlanet = new Particle(500, 500, 20.0);
    //bigPlanet->radius = 20;
    //m_particles.push_back(bigPlanet);

    //m_liquid.x = 0;
    //m_liquid.y = Graphics::Height() / 2;
    //m_liquid.w = Graphics::Width();
    //m_liquid.h = Graphics::Height() / 2;


    //Particle* bob = new Particle(Graphics::Width() / 2.0, Graphics::Height() / 2.0, 2.0);
    //bob->radius = 10;
    //m_particles.push_back(bob);

    //m_anchor = Vec2(Graphics::Width() / 2.0, 30.0);

    //for (int i = 0; i < m_NUM_PARTICLES; i++) {
    //    Particle* bob = new Particle(m_anchor.x, m_anchor.y + (i * m_restLength), 2.0);
    //    bob->radius = 6;
    //    m_particles.push_back(bob);
    //}


    Particle* a = new Particle(100, 100, 1.0);
    Particle* b = new Particle(300, 100, 1.0);
    Particle* c = new Particle(300, 300, 1.0);
    Particle* d = new Particle(100, 300, 1.0);

    a->radius = 6;
    b->radius = 6;
    c->radius = 6;
    d->radius = 6;

    m_particles.push_back(a);
    m_particles.push_back(b);
    m_particles.push_back(c);
    m_particles.push_back(d);


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
            //case SDL_MOUSEBUTTONDOWN:
            //    if (event.button.button == SDL_BUTTON_LEFT) {
            //        int x, y;
            //        SDL_GetMouseState(&x, &y);
            //        Particle* particle = new Particle(x, y, 1.0);
            //        particle->radius = 5;
            //        m_particles.push_back(particle);
            //    }
            //    break;
            case SDL_MOUSEMOTION:
                m_mouseCursor.x = event.motion.x;
                m_mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!m_leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    m_leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    m_mouseCursor.x = x;
                    m_mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (m_leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    m_leftMouseButtonDown = false;
                    Vec2 impulseDirection = (m_particles[m_NUM_PARTICLES-1]->position - m_mouseCursor).UnitVector();
                    float impulseMagnitude = (m_particles[m_NUM_PARTICLES - 1]->position - m_mouseCursor).Magnitude() * 5.0;
                    m_particles[m_NUM_PARTICLES - 1]->velocity = impulseDirection * impulseMagnitude;
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

    m_particles[0]->AddForce(m_pushForce);


    // apply some force to particle
    for (auto particle : m_particles) {
        //Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0 * PIXELS_PER_METER);
        //particle->AddForce(wind);

        //Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, particle->mass * 9.8 * PIXELS_PER_METER);
        //particle->AddForce(weight);

       //particle->AddForce(m_pushForce);

        //// if we are inside liquid, apply drag force
        //if (particle->position.y >= m_liquid.y) {
        //    Vec2 drag = Force::GenerateDragForce(*particle, 0.03);
        //    particle->AddForce(drag);

        //}

        //Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        //particle->AddForce(friction);

        //Vec2 friction = Force::GenerateFrictionForce(*particle, 5.0);
        //particle->AddForce(friction);

        Vec2 drag = Force::GenerateDragForce(*particle, 0.003);
        particle->AddForce(drag);

        Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);
    }

    // apply spring force
    //Vec2 springForce = Force::GenerateSpringForce(*m_particles[0], m_anchor, m_restLength, m_spring_k);
    //m_particles[0]->AddForce(springForce);
    // simulate gravitional attraction force
    //Vec2 attraction = Force::GenerateGravitationalForce(
    //    *m_particles[0],
    //    *m_particles[1],
    //    1000.0,
    //    5,
    //    100
    //);

    //m_particles[0]->AddForce(attraction);
    //m_particles[1]->AddForce(-attraction);

    //Vec2 springForce = Force::GenerateSpringForce(*m_particles[0], m_anchor, m_restLength, m_spring_k);
    //m_particles[0]->AddForce(springForce);

    //for (int i = 1; i < m_NUM_PARTICLES; i++)
    //{
    //    int currentParticle = i;
    //    int previousParticle = i - 1;
    //    Vec2 springForce = Force::GenerateSpringForce(*m_particles[currentParticle], *m_particles[previousParticle], m_restLength, m_spring_k);
    //    m_particles[currentParticle]->AddForce(springForce);
    //    m_particles[previousParticle]->AddForce(-springForce);
    //}

    // Attach particles with springs
    Vec2 ab = Force::GenerateSpringForce(*m_particles[0], *m_particles[1], m_restLength, m_spring_k); // a <-> b
    m_particles[0]->AddForce(ab);
    m_particles[1]->AddForce(-ab);

    Vec2 bc = Force::GenerateSpringForce(*m_particles[1], *m_particles[2], m_restLength, m_spring_k); // b <-> c
    m_particles[1]->AddForce(bc);
    m_particles[2]->AddForce(-bc);

    Vec2 cd = Force::GenerateSpringForce(*m_particles[2], *m_particles[3], m_restLength, m_spring_k); // c <-> d
    m_particles[2]->AddForce(cd);
    m_particles[3]->AddForce(-cd);

    Vec2 da = Force::GenerateSpringForce(*m_particles[3], *m_particles[0], m_restLength, m_spring_k); // d <-> a
    m_particles[3]->AddForce(da);
    m_particles[0]->AddForce(-da);

    Vec2 ac = Force::GenerateSpringForce(*m_particles[0], *m_particles[2], m_restLength, m_spring_k); // a <-> c
    m_particles[0]->AddForce(ac);
    m_particles[2]->AddForce(-ac);

    Vec2 bd = Force::GenerateSpringForce(*m_particles[1], *m_particles[3], m_restLength, m_spring_k); // b <-> d
    m_particles[1]->AddForce(bd);
    m_particles[3]->AddForce(-bd);


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
    //Graphics::DrawFillRect(
    //    m_liquid.x + m_liquid.w / 2,
    //    m_liquid.y + m_liquid.h / 2,
    //    m_liquid.w,
    //    m_liquid.h,
    //    0xFF6E3713
    //    );

    // draw anchor
    // Graphics::DrawFillCircle(m_anchor.x, m_anchor.y, 5, 0xFF001155);
    // Graphics::DrawLine(m_anchor.x, m_anchor.y, m_particles[0]->position.x, m_particles[0]->position.y, 0xFF0000FF);


    if (m_leftMouseButtonDown) {
        Graphics::DrawLine(m_particles[m_NUM_PARTICLES - 1]->position.x, m_particles[m_NUM_PARTICLES - 1]->position.y, m_mouseCursor.x, m_mouseCursor.y, 0xFF0000FF);
    }


    //for (int i = 0; i < m_NUM_PARTICLES -1; i++)
    //{
    //    int currentParticle = i;
    //    int nexParticle = i + 1;
    //    Graphics::DrawLine(m_particles[currentParticle]->position.x, m_particles[currentParticle]->position.y, m_particles[nexParticle]->position.x, m_particles[nexParticle]->position.y, 0xFF0000FF);

    //}


    //for (auto particle : m_particles) {
    //    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);

    //}


    Graphics::DrawLine(
        m_particles[0]->position.x, 
        m_particles[0]->position.y,
        m_particles[1]->position.x,
        m_particles[1]->position.y,
        0xFF0000FF
        );

    Graphics::DrawLine(
        m_particles[1]->position.x,
        m_particles[1]->position.y,
        m_particles[2]->position.x,
        m_particles[2]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_particles[2]->position.x,
        m_particles[2]->position.y,
        m_particles[3]->position.x,
        m_particles[3]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_particles[3]->position.x,
        m_particles[3]->position.y,
        m_particles[0]->position.x,
        m_particles[0]->position.y,
        0xFF0000FF
    );

    Graphics::DrawLine(
        m_particles[0]->position.x,
        m_particles[0]->position.y,
        m_particles[2]->position.x,
        m_particles[2]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_particles[1]->position.x,
        m_particles[1]->position.y,
        m_particles[3]->position.x,
        m_particles[3]->position.y,
        0xFF0000FF
    );

    Graphics::DrawFillCircle(
        m_particles[0]->position.x,
        m_particles[0]->position.y,
        m_particles[0]->radius,
        0xFFFFFFFF
    );


    Graphics::DrawFillCircle(
        m_particles[1]->position.x,
        m_particles[1]->position.y,
        m_particles[1]->radius,
        0xFFFFFFFF
    );


    Graphics::DrawFillCircle(
        m_particles[2]->position.x,
        m_particles[2]->position.y,
        m_particles[2]->radius,
        0xFFFFFFFF
    );

    Graphics::DrawFillCircle(
        m_particles[3]->position.x,
        m_particles[3]->position.y,
        m_particles[3]->radius,
        0xFFFFFFFF
    );

    Graphics::RenderFrame();
}


void Application::Destroy() {

    for (auto particle : m_particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}