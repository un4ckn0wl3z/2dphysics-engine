#include "Application.h"
#include "Constants.h"
#include "Force.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();



    Body* a = new Body(100, 100, 1.0);
    Body* b = new Body(300, 100, 1.0);
    Body* c = new Body(300, 300, 1.0);
    Body* d = new Body(100, 300, 1.0);

    a->radius = 6;
    b->radius = 6;
    c->radius = 6;
    d->radius = 6;

    m_bodies.push_back(a);
    m_bodies.push_back(b);
    m_bodies.push_back(c);
    m_bodies.push_back(d);


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
                    Vec2 impulseDirection = (m_bodies[m_NUM_BODIES-1]->position - m_mouseCursor).UnitVector();
                    float impulseMagnitude = (m_bodies[m_NUM_BODIES - 1]->position - m_mouseCursor).Magnitude() * 5.0;
                    m_bodies[m_NUM_BODIES - 1]->velocity = impulseDirection * impulseMagnitude;
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

    m_bodies[0]->AddForce(m_pushForce);


    // apply some force to body
    for (auto body : m_bodies) {

        Vec2 drag = Force::GenerateDragForce(*body, 0.003);
        body->AddForce(drag);

        Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, body->mass * 9.8 * PIXELS_PER_METER);
        body->AddForce(weight);
    }


    // Attach bodies with springs
    Vec2 ab = Force::GenerateSpringForce(*m_bodies[0], *m_bodies[1], m_restLength, m_spring_k); // a <-> b
    m_bodies[0]->AddForce(ab);
    m_bodies[1]->AddForce(-ab);

    Vec2 bc = Force::GenerateSpringForce(*m_bodies[1], *m_bodies[2], m_restLength, m_spring_k); // b <-> c
    m_bodies[1]->AddForce(bc);
    m_bodies[2]->AddForce(-bc);

    Vec2 cd = Force::GenerateSpringForce(*m_bodies[2], *m_bodies[3], m_restLength, m_spring_k); // c <-> d
    m_bodies[2]->AddForce(cd);
    m_bodies[3]->AddForce(-cd);

    Vec2 da = Force::GenerateSpringForce(*m_bodies[3], *m_bodies[0], m_restLength, m_spring_k); // d <-> a
    m_bodies[3]->AddForce(da);
    m_bodies[0]->AddForce(-da);

    Vec2 ac = Force::GenerateSpringForce(*m_bodies[0], *m_bodies[2], m_restLength, m_spring_k); // a <-> c
    m_bodies[0]->AddForce(ac);
    m_bodies[2]->AddForce(-ac);

    Vec2 bd = Force::GenerateSpringForce(*m_bodies[1], *m_bodies[3], m_restLength, m_spring_k); // b <-> d
    m_bodies[1]->AddForce(bd);
    m_bodies[3]->AddForce(-bd);


    for (auto body : m_bodies) {
        // apply acceleration and velocity
        body->Integrate(deltaTime);
    }

    for (auto body : m_bodies) {
        // limit boundaries
        if (body->position.x - body->radius <= 0) {
            body->position.x = body->radius;
            body->velocity.x *= -0.9;
        }
        else if (body->position.x + body->radius >= Graphics::Width()) {
            body->position.x = Graphics::Width() - body->radius;
            body->velocity.x *= -0.9;
        }

        if (body->position.y - body->radius <= 0) {
            body->position.y = body->radius;
            body->velocity.y *= -0.9;
        }
        else if (body->position.y + body->radius >= Graphics::Height()) {
            body->position.y = Graphics::Height() - body->radius;
            body->velocity.y *= -0.9;
        }

    }

}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);



    if (m_leftMouseButtonDown) {
        Graphics::DrawLine(m_bodies[m_NUM_BODIES - 1]->position.x, m_bodies[m_NUM_BODIES - 1]->position.y, m_mouseCursor.x, m_mouseCursor.y, 0xFF0000FF);
    }


    Graphics::DrawLine(
        m_bodies[0]->position.x, 
        m_bodies[0]->position.y,
        m_bodies[1]->position.x,
        m_bodies[1]->position.y,
        0xFF0000FF
        );

    Graphics::DrawLine(
        m_bodies[1]->position.x,
        m_bodies[1]->position.y,
        m_bodies[2]->position.x,
        m_bodies[2]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_bodies[2]->position.x,
        m_bodies[2]->position.y,
        m_bodies[3]->position.x,
        m_bodies[3]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_bodies[3]->position.x,
        m_bodies[3]->position.y,
        m_bodies[0]->position.x,
        m_bodies[0]->position.y,
        0xFF0000FF
    );

    Graphics::DrawLine(
        m_bodies[0]->position.x,
        m_bodies[0]->position.y,
        m_bodies[2]->position.x,
        m_bodies[2]->position.y,
        0xFF0000FF
    );


    Graphics::DrawLine(
        m_bodies[1]->position.x,
        m_bodies[1]->position.y,
        m_bodies[3]->position.x,
        m_bodies[3]->position.y,
        0xFF0000FF
    );

    Graphics::DrawFillCircle(
        m_bodies[0]->position.x,
        m_bodies[0]->position.y,
        m_bodies[0]->radius,
        0xFFFFFFFF
    );


    Graphics::DrawFillCircle(
        m_bodies[1]->position.x,
        m_bodies[1]->position.y,
        m_bodies[1]->radius,
        0xFFFFFFFF
    );


    Graphics::DrawFillCircle(
        m_bodies[2]->position.x,
        m_bodies[2]->position.y,
        m_bodies[2]->radius,
        0xFFFFFFFF
    );

    Graphics::DrawFillCircle(
        m_bodies[3]->position.x,
        m_bodies[3]->position.y,
        m_bodies[3]->radius,
        0xFFFFFFFF
    );

    Graphics::RenderFrame();
}


void Application::Destroy() {

    for (auto body : m_bodies) {
        delete body;
    }

    Graphics::CloseWindow();
}