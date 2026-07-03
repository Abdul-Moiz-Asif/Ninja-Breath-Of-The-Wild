#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float GRAVITY = 300.0f;
const float JUMP_SPEED = -350.0f;
const float SCROLL_SPEED = 100.0f;
const float BOOSTED_SCROLL_SPEED = 200.0f;
const float PLATFORM_SPAWN_INTERVAL = 2.5f;
const float SPIKE_SPAWN_DELAY = 2.0f;
const int COIN_SCORE = 20;

class Ninja {
public:
    Ninja(float x, float y, Texture* texture) {
        sprite.setTexture(*texture);
        sprite.setScale(0.75f, 0.75f);
        sprite.setPosition(x, y);
    }

    void update(float deltaTime, bool isOnGround) {
        if (!isOnGround) {
            velocity.y += GRAVITY * deltaTime;
        } else {
            velocity.y = 0;
        }

        sprite.move(0, velocity.y * deltaTime);

        if (sprite.getPosition().y + sprite.getGlobalBounds().height > WINDOW_HEIGHT) {
            sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - sprite.getGlobalBounds().height);
            onGround = true;
        }
    }

    void jump() {
        if (onGround || canDoubleJump) {
            velocity.y = JUMP_SPEED;
            if (!onGround) canDoubleJump = false;
            onGround = false;
        }
    }

    void setOnGround(bool value) {
        onGround = value;
        if (onGround) canDoubleJump = true;
    }

    bool isOnGround() const {
        return onGround;
    }

    Sprite getSprite() const {
        return sprite;
    }

    Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

private:
    Sprite sprite;
    Vector2f velocity{0, 0};
    bool onGround = false;
    bool canDoubleJump = true;
};

class Platform {
public:
    Platform(float x, float y, float width, float height, Texture* texture) {
        shape.setSize({width, height});
        shape.setTexture(texture);
        shape.setPosition(x, y);
    }

    void moveLeft(float deltaTime, float speed) {
        shape.move(-speed * deltaTime, 0);
    }

    RectangleShape getShape() const {
        return shape;
    }

private:
    RectangleShape shape;
};

class Coin {
public:
    Coin(float x, float y, Texture* texture) {
        sprite.setTexture(*texture);
        sprite.setScale(0.3f, 0.3f);
        sprite.setPosition(x, y);
    }

    void moveLeft(float deltaTime, float speed) {
        sprite.move(-speed * deltaTime, 0);
    }

    Sprite getSprite() const {
        return sprite;
    }

    float getX() const {
        return sprite.getPosition().x;
    }

private:
    Sprite sprite;
};

class Spike {
public:
    Spike(float x, float y, Texture* texture) {
        sprite.setTexture(*texture);
        sprite.setScale(2.0f, 2.0f);
        sprite.setPosition(x, y);
    }

    void moveLeft(float deltaTime, float speed) {
        sprite.move(-speed * deltaTime, 0);
    }

    Sprite getSprite() const {
        return sprite;
    }

    float getX() const {
        return sprite.getPosition().x;
    }

private:
    Sprite sprite;
};

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ninja, Breath of the Wild");
    window.setFramerateLimit(60);

    // Load textures
    Texture ninjaTexture, platformTexture, spikeTexture, backgroundTexture, coinTexture;
    if (!ninjaTexture.loadFromFile("ninja.png") || !platformTexture.loadFromFile("platform.png") ||
        !spikeTexture.loadFromFile("spike.png") || !backgroundTexture.loadFromFile("background.png") ||
        !coinTexture.loadFromFile("coin.png")) {
        cerr << "Failed to load textures.\n";
        return -1;
    }

    Sprite background(backgroundTexture), background2(backgroundTexture);
    background.setScale((float)WINDOW_WIDTH / backgroundTexture.getSize().x, (float)WINDOW_HEIGHT / backgroundTexture.getSize().y);
    background2.setScale((float)WINDOW_WIDTH / backgroundTexture.getSize().x, (float)WINDOW_HEIGHT / backgroundTexture.getSize().y);
    background2.setPosition(WINDOW_WIDTH, 0);

    Ninja ninja(100.0f, 500.0f, &ninjaTexture);

    vector<Platform> platforms = {
        Platform(0, 550, 400, 40, &platformTexture),
        Platform(400, 450, 300, 40, &platformTexture)
    };
    vector<Spike> spikes;
    vector<Coin> coins;

    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Failed to load font.\n";
        return -1;
    }

    int score = 0;
    Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);

    Text gameOverText("GAME OVER!", font, 48);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 100);

    Clock clock;
    float platformSpawnTimer = 1.5f, coinSpawnTimer = 0.5f, spikeSpawnTimer = SPIKE_SPAWN_DELAY;
    float elapsedTime = 0.0f;

    srand((unsigned)time(nullptr));

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            ninja.jump();
        }

        float currentScrollSpeed = Keyboard::isKeyPressed(Keyboard::Right) ? BOOSTED_SCROLL_SPEED : SCROLL_SPEED;

        background.move(-currentScrollSpeed * deltaTime, 0);
        background2.move(-currentScrollSpeed * deltaTime, 0);
        if (background.getPosition().x + WINDOW_WIDTH <= 0) background.setPosition(background2.getPosition().x + WINDOW_WIDTH, 0);
        if (background2.getPosition().x + WINDOW_WIDTH <= 0) background2.setPosition(background.getPosition().x + WINDOW_WIDTH, 0);

        platformSpawnTimer += deltaTime;
        if (platformSpawnTimer >= PLATFORM_SPAWN_INTERVAL) {
            platformSpawnTimer = 0.0f;
            float platformHeight = 250.0f + rand() % 150;
            platforms.emplace_back(WINDOW_WIDTH - 50.0f, platformHeight, 200.0f, 40.0f, &platformTexture);
        }

        coinSpawnTimer += deltaTime;
        if (coinSpawnTimer >= 1.5f) {
            coinSpawnTimer = 0.0f;
            coins.emplace_back(WINDOW_WIDTH - 50.0f, 75 + rand() % 300, &coinTexture);
        }

        if (elapsedTime >= SPIKE_SPAWN_DELAY) {
            spikeSpawnTimer += deltaTime;
            if (spikeSpawnTimer >= 1.0f) {
                spikeSpawnTimer = 0.0f;
                spikes.emplace_back(WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT - 40, &spikeTexture);
            }
        }

        for (size_t i = 0; i < platforms.size(); ++i) {
            platforms[i].moveLeft(deltaTime, currentScrollSpeed);
        }
        for (size_t i = 0; i < coins.size(); ++i) {
            coins[i].moveLeft(deltaTime, currentScrollSpeed);
        }
        for (size_t i = 0; i < spikes.size(); ++i) {
            spikes[i].moveLeft(deltaTime, currentScrollSpeed);
        }

        ninja.update(deltaTime, ninja.isOnGround());

        bool isOnGround = false;
        for (size_t i = 0; i < platforms.size(); ++i) {
            FloatRect ninjaBounds = ninja.getSprite().getGlobalBounds();
            FloatRect platformBounds = platforms[i].getShape().getGlobalBounds();

            if (ninjaBounds.intersects(platformBounds) && ninjaBounds.top + ninjaBounds.height <= platforms[i].getShape().getPosition().y + 10.0f) {
                isOnGround = true;
                ninja.setPosition(ninja.getPosition().x, platforms[i].getShape().getPosition().y - ninjaBounds.height);
                break;
            }
        }
        ninja.setOnGround(isOnGround);

        for (size_t i = 0; i < coins.size();) {
            if (ninja.getSprite().getGlobalBounds().intersects(coins[i].getSprite().getGlobalBounds())) {
                score += COIN_SCORE;
                coins.erase(coins.begin() + i);
                scoreText.setString("Score: " + to_string(score));
            } else {
                ++i;
            }
        }

        for (size_t i = 0; i < spikes.size(); ++i) {
            if (ninja.getSprite().getGlobalBounds().intersects(spikes[i].getSprite().getGlobalBounds())) {
                window.clear();
                window.draw(gameOverText);
                window.display();
                sleep(seconds(3));
                window.close();
            }
        }

        window.clear();
        window.draw(background);
        window.draw(background2);
        window.draw(scoreText);
        window.draw(ninja.getSprite());
        for (size_t i = 0; i < platforms.size(); ++i) {
            window.draw(platforms[i].getShape());
        }
        for (size_t i = 0; i < coins.size(); ++i) {
            window.draw(coins[i].getSprite());
        }
        for (size_t i = 0; i < spikes.size(); ++i) {
            window.draw(spikes[i].getSprite());
        }
        window.display();
    }

    return 0;
}
