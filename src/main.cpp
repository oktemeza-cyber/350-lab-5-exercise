#include <iostream>
#include <optional>
#include <vector>
#include <functional>
#include <cmath>
#include <limits>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

using Point2D = sf::Vector2f;
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

// Point2D derivative(std::vector<sf::Vector2f> f, float x){
//     float h = sqrt(std::numeric_limits<float>::epsilon());
//     float result = ((f(x + h) - f(x - h)) / (2.0f * h));
//     return static_cast<Point2D>(result);
// }

// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].
Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
        float tween1 = tween(0.0f, 1.0f, t);
        Point2D position = sf::Vector2f(std::lerp(50.0f, 50.0f + 650.0f, t), std::lerp(WINDOW_HEIGHT - 50.0f, (WINDOW_HEIGHT - 50.0f) - 350.0f, tween1));
        return position; 
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) { 



    return Point2D{}; 
}

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

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    static int frames = 0;
    int framesWanted = 200;
    float time = static_cast<float>(frames % framesWanted)/framesWanted;
    float x = tween(0.0f, WINDOW_WIDTH, time);
    frames++;

    sf::CircleShape shape;
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(sf::Vector2f(5, 5));
    shape.setPosition(sf::Vector2(x, WINDOW_HEIGHT/3.0f));
    window.draw(shape);
    //draw circle and do animation w/tween

    sf::VertexArray yAxis(sf::PrimitiveType::Lines, 2);
    yAxis[0].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 50.0f);
    yAxis[1].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 350.0f);
    yAxis[0].color = sf::Color::White;
    yAxis[1].color = sf::Color::White;
    window.draw(yAxis);

    sf::VertexArray xAxis(sf::PrimitiveType::Lines, 2);
    xAxis[0].position = sf::Vector2f(50.0f, WINDOW_HEIGHT - 50.0f);
    xAxis[1].position = sf::Vector2f(50.0f + 650.0f, WINDOW_HEIGHT - 50.0f);
    xAxis[0].color = sf::Color::White;
    xAxis[1].color = sf::Color::White;
    window.draw(xAxis);
    //set up the x and y axis for the graph using a vertex array

    int steps = 100;
    sf::VertexArray curve(sf::PrimitiveType::LineStrip, steps + 1);
    for(int i = 0; i <= steps; i++){
        float position = static_cast<float>(i) / steps;
        float tween1 = tween(0.0f, 1.0f, position);
        curve[i].position = sf::Vector2f(std::lerp(50.0f, 50.0f + 650.0f, position), std::lerp(WINDOW_HEIGHT - 50.0f, (WINDOW_HEIGHT - 50.0f) - 350.0f, tween1));
        curve[i].color = sf::Color::Cyan;
    }
    window.draw(curve);
    //Code from prep

    //dot draw
    float current = tween(0.0f, 1.0f, time);
    sf::CircleShape dot(5.0f);
    dot.setOrigin(sf::Vector2f(5.0f, 5.0f));
    dot.setPosition(sf::Vector2f(std::lerp(50.0f, 50.0f + 650.0f, time), std::lerp(WINDOW_HEIGHT - 50.0f, (WINDOW_HEIGHT - 50.0f) - 350.0f, current)));
    dot.setFillColor(sf::Color::Yellow);
    window.draw(dot);
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
