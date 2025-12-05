#include "Application.h"
#include "Constants.h"
#include "Force.h"
#include "Shape.h"

bool Application::IsRunning() {
    return m_running;
}


void Application::Setup() {
    m_running = Graphics::OpenWindow();

    Body* box = new Body(BoxShape(200, 100), Graphics::Width() / 2, Graphics::Height() / 2, 1.0);
    Body* ball = new Body(CircleShape(50), Graphics::Width() / 2, Graphics::Height() / 2, 1.0);

    m_bodies.push_back(box);
    m_bodies.push_back(ball);

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

    // apply some force to body
    for (auto body : m_bodies) {

        body->AddForce(m_pushForce);

        Vec2 drag = Force::GenerateDragForce(*body, 0.003);
        body->AddForce(drag);

        //Vec2 weight = Vec2(0.0 * PIXELS_PER_METER, body->mass * 9.8 * PIXELS_PER_METER);
        //body->AddForce(weight);

        float torque = 200;
        body->AddTorque(torque);
    }


    for (auto body : m_bodies) {
        // apply acceleration and velocity
        body->IntegrateLinear(deltaTime);
        body->IntegrateAngular(deltaTime);
        bool isPolygon =
            body->shape->GetType() == POLYGON ||
            body->shape->GetType() == BOX;

        if (isPolygon) {
            PolygonShape* polygon = dynamic_cast<PolygonShape*>(body->shape);
            polygon->UpdateVertices(body->rotation, body->position);
        }


    }

    for (auto body : m_bodies) {
        // limit boundaries

        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = dynamic_cast<CircleShape*>(body->shape);
            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9;
            }
            else if (body->position.x + circleShape->radius >= Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9;
            }

            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9;
            }
            else if (body->position.y + circleShape->radius >= Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9;
            }
        }

    }

}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);



    if (m_leftMouseButtonDown) {
        Graphics::DrawLine(m_bodies[m_NUM_BODIES - 1]->position.x, m_bodies[m_NUM_BODIES - 1]->position.y, m_mouseCursor.x, m_mouseCursor.y, 0xFF0000FF);
    }

    for (auto body : m_bodies) {
        if (body->shape->GetType() == CIRCLE) {
            Graphics::DrawCircle(
                body->position.x,
                body->position.y,
                dynamic_cast<CircleShape*>(body->shape)->radius,
                body->rotation,
                0xFFFFFFFF
            );
        }
        else if (body->shape->GetType() == BOX) {
            Graphics::DrawPolygon(
                body->position.x,
                body->position.y,
                dynamic_cast<BoxShape*>(body->shape)->worldVertices,
                0xFFFFFFFF
            );
        }
    }

    Graphics::RenderFrame();
}


void Application::Destroy() {

    for (auto body : m_bodies) {
        delete body;
    }

    Graphics::CloseWindow();
}