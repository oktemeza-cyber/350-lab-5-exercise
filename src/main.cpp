#include <iostream>
#include <optional>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

using Point2D = sf::Vector2f;
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].

Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
    float tminus = 1.0f - t;
    return pts[0] * std::pow(tminus, 3.0f) + pts[1] * (3.0f * std::pow(tminus, 2.0f*t)) + pts[2] * (3.0f * std::pow(tminus, 2.0f*t)) + pts[3] * (std::pow(t, 3.0f));
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) { 
    float tminus = 1.0f - t;
    return (pts[1] - pts[0]) * (3.0f * tminus * tminus) + (pts[2] - pts[1]) * (6.0f * tminus * t) + (pts[3] - pts[2]) * (3.0f * t * t);
}

//b4(t) = (1 − t)^3 p1 + 3(1 − t)^2t p2 + 3(1 − t)t^2 p3 + t^3 p4 used as reference and 
// derivative (b4(t)) = 3(1 − t)^2 (p2 − p1) + 6(1 − t)t (p3 − p2) + 3t^2 (p4 − p3)

// TODO: (Part 1) Store four control points for the curve.
// TODO: (Part 2) Track animation time for the square moving along the curve.
// TODO: (Part 3) Track the index of the control point being dragged.

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            switch (mouse->button){
                case sf::Mouse::Button::Right:
                case sf::Mouse::Button::Left:
                case sf::Mouse::Button::Middle:
                default:
                  break;
            }
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            switch (mouse->button){
                case sf::Mouse::Button::Right:
                case sf::Mouse::Button::Left:
                case sf::Mouse::Button::Middle:
                default:
                  break;
            }
            // TODO: (Part 3) On left-button release, stop dragging.
        } 
        // else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
        //     switch (mouse->button){
        //         case sf::Mouse::Button::Right:
        //             sf::Vector2i pos = sf::Mouse::getPosition();
        //         case sf::Mouse::Button::Left:
        //             sf::Vector2i pos = sf::Mouse::getPosition();
        //         case sf::Mouse::Button::Middle:
        //             sf::Vector2i pos = sf::Mouse::getPosition()	
        //         default:
        //           break;
        //     }
            // TODO: (Part 3) Move the selected control point to mouse->position.
            // TODO: (Part 4) Maintain matching slopes at shared endpoints.
            // When moving point 3, move point 5 without changing its distance
            // from point 4 (point numbers here start at 1).
         else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            // TODO: (Part 4) '+' adds three control points; '-' removes three,
            // keeping at least four points.
        }
    }
}

void drawControlPoints(sf::RenderWindow& window, const std::vector<sf::Vector2f>& pts){

    for(const auto& p : pts){
        sf::CircleShape point(5.0f);
        point.setOrigin(sf::Vector2f(5.0f, 5.0f));
        point.setPosition(p);
        point.setFillColor(sf::Color::Red);
        window.draw(point);
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    static int frames = 0;
    int framesWanted = FPS_LIMIT;
    float time = static_cast<float>(frames % framesWanted)/framesWanted;
    float x = tween(0.0f, WINDOW_WIDTH, time);
    std::vector<sf::Vector2f> controlPoints = { {100.0f, 500.0f}, {200.0f, 100.0f}, {600.0f, 100.0f}, {700.0f, 500.0f}};
    frames++;

    int steps = 100;
    sf::VertexArray curve(sf::PrimitiveType::LineStrip, steps + 1);
    for(int i = 0; i <= steps; i++){
        float position = static_cast<float>(i) / steps;
        curve[i].position = getPoint(controlPoints, position);
        curve[i].color = sf::Color::Cyan;
    }
    window.draw(curve);
    //Code from prep

    drawControlPoints(window, controlPoints);
    //lerping used in the same context
    
    // ====== ====== ======
    // TODO: (Part 1) Sample GetPoint over t in [0, 1] and connect samples using the line-drawing
    // code from your project. Draw all four control points as circles after drawing the curve.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Part 2) Draw a small square moving repeatedly along the curve.
    // Use GetSlope to orient it to the curve at each time step.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Part 3) Draw control handles from point 1 to 2 and point 3 to 4.
    // TODO: (Part 4) Draw all connected cubic Bezier segments and their handles.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Bonus) Support multiple curves, a Galaga screen overlay at a 1:2 ratio, and exporting
    // curve points as C++ code for Project 1b.
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Bezier Curve Editor");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
