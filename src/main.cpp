#include <iostream>
#include <optional>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
std::vector<sf::Vector2f> controlPoints = { {100.0f, 500.0f}, {200.0f, 100.0f}, {600.0f, 100.0f}, {700.0f, 500.0f}};
int selectedPoint = -1;

using Point2D = sf::Vector2f;
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};
//taken from prep

Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
    float tminus = 1.0f - t;
    return pts[0] * std::pow(tminus, 3.0f) + pts[1] * (3.0f * std::pow(tminus, 2.0f) * t) + pts[2] * (3.0f * std::pow(t, 2.0f) * tminus) + pts[3] * (std::pow(t, 3.0f));
}

Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) { 
    float tminus = 1.0f - t;
    return (pts[1] - pts[0]) * (3.0f * tminus * tminus) + (pts[2] - pts[1]) * (6.0f * tminus * t) + (pts[3] - pts[2]) * (3.0f * t * t);
}

//b4(t) = (1 − t)^3 p1 + 3(1 − t)^2t p2 + 3(1 − t)t^2 p3 + t^3 p4 used as reference and 
// derivative (b4(t)) = 3(1 − t)^2 (p2 − p1) + 6(1 − t)t (p3 − p2) + 3t^2 (p4 − p3) for above

void maintainSlope(std::vector<Point2D>& pts, int movedIndex){

    for(size_t joint = 3; joint + 1 < pts.size(); joint +=3){
        int before = joint - 1;
        int after = joint + 1;

        if(movedIndex != before && movedIndex != after){
            continue;
        } 

        int other;
        if(movedIndex == before){
            other = after;
        } else {
            other = before;
        }

        Point2D jointPosition = pts[joint];
        Point2D moveHandlePos = pts[movedIndex];
        Point2D otherHandlePos = pts[other];

        float directionX = jointPosition.x - moveHandlePos.x;
        float directionY = jointPosition.y - moveHandlePos.y;

        float directionLength = std::sqrt(directionX * directionX + directionY * directionY);
        if(directionLength > 0.0001f){
            directionX = directionX / directionLength;
            directionY = directionY / directionLength;
        }

        float offsetX = otherHandlePos.x - jointPosition.x;
        float offsetY = otherHandlePos.y - jointPosition.y;
        float otherHandleDist = std::sqrt(offsetX * offsetX + offsetY * offsetY);

        float newX = jointPosition.x + directionX * otherHandleDist;
        float newY = jointPosition.y + directionY * otherHandleDist;

        pts[other] = Point2D(newX, newY);
    }


}//maintains slope by checking points between 3 and 4 and 4 and 5



void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if(mouse->button == sf::Mouse::Button::Left){
                    Point2D mousePos(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
                    float closest = 1000.0f; //grab a point thats closest, high number allows a higher range within window
                    int index = -1;
                    
                    for(size_t i = 0; i < controlPoints.size(); i++){
                        Point2D distance = controlPoints[i] - mousePos;
                        float distance2 = std::sqrt(distance.x * distance.x + distance.y * distance.y);
                        if(distance2 < closest){
                            closest = distance2;
                            index = static_cast<int>(i);
                        }
                    }
                selectedPoint = index;    
            }
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            if(mouse->button == sf::Mouse::Button::Left){
                selectedPoint = -1;
            }
        } else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {

             if(selectedPoint != -1){
                controlPoints[selectedPoint] = Point2D(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
                maintainSlope(controlPoints, selectedPoint);
             }
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            
            if(key->scancode == sf::Keyboard::Scan::Equal){
                Point2D first = controlPoints.front();
                Point2D last = controlPoints.back();

                controlPoints.push_back(first + (last - first) * 0.25f);
                controlPoints.push_back(first + (last - first) * 0.5f);
                controlPoints.push_back(first + (last - first) * 0.75f);
                
            } else if (key->scancode == sf::Keyboard::Scan::Hyphen){
                if(controlPoints.size() > 4){
                    controlPoints.resize(controlPoints.size() - 3);
                }
            }
            
        }
    }
}

//removed switch statements as they seemed unneccesary(?), release does not need any code and no inputs needed checking for left and middle mouse

void drawControlPoints(sf::RenderWindow& window, const std::vector<sf::Vector2f>& pts){

    for(const auto& p : pts){
        sf::CircleShape point(5.0f);
        point.setOrigin(sf::Vector2f(5.0f, 5.0f));
        point.setPosition(p);
        point.setFillColor(sf::Color::Red);
        window.draw(point);
    }
}

void drawLines(sf::RenderWindow& window, const std::vector<sf::Vector2f>& pts){
    for(size_t i = 0; i + 3 < pts.size(); i += 3){
        
        sf::VertexArray line1(sf::PrimitiveType::Lines, 2);
        line1[0].position = pts[i];
        line1[1].position = pts[i+1];
        line1[0].color = line1[1].color = sf::Color::White;
        window.draw(line1);

        sf::VertexArray line2(sf::PrimitiveType::Lines, 2);
        line2[0].position = pts[i + 2];
        line2[1].position = pts[i + 3];
        line2[0].color = line2[1].color = sf::Color::White;
        window.draw(line2);
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);

    static int frames = 0;
    int framesWanted = FPS_LIMIT;
    float time = static_cast<float>(frames % framesWanted)/framesWanted;
    float x = tween(0.0f, WINDOW_WIDTH, time);
    frames++;
    //same code from prep


    int steps = 100;
    for(size_t j = 0; j + 3 < controlPoints.size(); j += 3){
        std::vector<Point2D> segment(controlPoints.begin() + j, controlPoints.begin() + j + 4);
        //added so more than 4 points can be handled at a time

        sf::VertexArray curve(sf::PrimitiveType::LineStrip, steps + 1);
        for(int i = 0; i <= steps; i++){
            float position = static_cast<float>(i) / steps;
            curve[i].position = getPoint(segment, position);
            curve[i].color = sf::Color::Cyan;
        }

        window.draw(curve);
        
    }//draw curve

    float tween1 = tween(0.0f, 1.0f, time);

    
    int squareSegments = static_cast<int>((controlPoints.size() - 1) / 3);
    //same reasoning as before--segments added so the square can move along more than 4 points
    float scaled = tween1 * squareSegments;
    int segmentIndex = std::min(static_cast<int>(scaled), squareSegments - 1);
    float local = scaled - segmentIndex;
    std::vector<Point2D>segment(controlPoints.begin() + segmentIndex * 3, controlPoints.begin() + segmentIndex * 3 + 4);
    //Added code to update square path when new points are added


    Point2D squarePos = getPoint(segment, local);
    Point2D slope = getSlope(segment, local);
    float rotation = std::atan2(slope.y, slope.x) * 180.0f/3.14159265f;
    //calculate degree

    sf::RectangleShape rect(sf::Vector2f(15.0f, 15.0f));
    rect.setOrigin(sf::Vector2f(7.5f, 7.5f));
    rect.setPosition(squarePos);
    rect.setRotation(sf::degrees(rotation));
    rect.setFillColor(sf::Color::Yellow);
    window.draw(rect);
    //draw rectangle

    drawLines(window, controlPoints);
    drawControlPoints(window, controlPoints);
    


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
