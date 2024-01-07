#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <memory>

class Entity {
  std::string m_tag;
  size_t m_id;
  bool m_alive = true;

public:
  Entity() {}
  Entity(std::string tag, size_t id) {
    this->m_tag = tag;
    this->m_id = id;
  }

  std::string tag() {
    return m_tag;
  }

  bool alive() {
    return m_alive;
  }
};

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {
  EntityVec m_entities;
  EntityMap m_entityMap;
  EntityVec m_toAdd;
  size_t m_totalEntities = 0;

public:
  EntityManager() {}
  
  void update() {

    for (auto e : m_toAdd) {
      m_entities.push_back(e);
      m_entityMap[e->tag()].push_back(e);
    }

    EntityVec to_r;
    for (auto e : m_entities) {
      if (!e->alive()) to_r.push_back(e);
    }

    for (auto e : to_r) {
      m_entities.erase(m_entities.begin());
    }
    m_toAdd.clear();
  }

  std::shared_ptr<Entity> addEntity(const std::string &tag) {
    auto e = std::make_shared<Entity>(tag, m_totalEntities++);
    m_toAdd.push_back(e);
    return e;
  };

  EntityVec &getEntities();
  EntityVec &getEntities(const std::string &tag);
};

class Vec2 {
public:
  float x = 0;
  float y = 0;

  Vec2() {}

  Vec2(float xin, float yin) : x(xin), y(yin) {}

  Vec2 operator+(const Vec2 &rh) const { return Vec2(x + rh.x, y + rh.y); }

  Vec2 operator*(const float hr) const { return Vec2(x * hr, y * hr); }

  // Chaining: By returning a reference to itself we can
  // enable this method to be chained onto others
  Vec2 &add(const Vec2 &v) {
    x += v.x;
    y += v.y;
    return *this;
  }

  void multiply(const float v) {
    x *= v;
    y *= v;
  }
};

int main() {
  int width = 800, height = 300, x = width / 2, y = height / 2, speed = 1;

  sf::RenderWindow window(sf::VideoMode(width, height), "Pong!");
  sf::CircleShape shape(50.f);
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(x, y);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (x > width || x < 0 || y > height || y < 0)
      speed *= -1;

    x += speed;
    y += speed;

    shape.setPosition(x, y);

    window.clear();
    window.draw(shape);
    window.display();
  }

  return 0;
}
