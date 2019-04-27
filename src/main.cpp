
#include <Renderer.h>
#include <cameras/TopDownCamera.h>
#include <constants.h>
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <Scene.h>
#include <Window.h>
#include <ColliderFactory.h>
#include <components/WinOnCollisionComponent.h>
#include <Logger.h>
#include <StdOutLogHandler.h>
#include <ControlsManager.h>
#include <KeyboardButton.h>
#include <components/PlayerController.h>
#include "controls.h"
#include <fstream>

Renderer renderer;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TopDownCamera *camera;
std::shared_ptr<Scene> scene;
Collider *collider = ColliderFactory::getTwoPhaseCollider();
std::shared_ptr<std::vector<std::vector<int>>> tiles;
const float tileWidth = 2.0f;

void loadFloor(const std::shared_ptr<ShaderProgram> &standardShader);

void idle(float timeSinceStart, float timeSinceLastCall) {
    scene->update(timeSinceLastCall);
    collider->updateCollision(scene.get());
}

// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    renderer.drawScene(camera, scene.get(), timeSinceStart);
}

// Called when the window gets resized
void resize(int newWidth, int newHeight) {
    renderer.resize(newWidth, newHeight);
}

void createLight() {

    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight->specularColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight->ambientColor= chag::make_vector(0.50f,0.50f,0.50f);

    directionalLight->direction= -chag::make_vector(0.0f,-10.0f,0.0f);
    scene->directionalLight = directionalLight;

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    pointLight->specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight->position = chag::make_vector(18.0f, 3.0f, 0.0f);
    scene->pointLights.push_back(pointLight);
}

void loadWorld() {
    scene = std::make_shared<Scene>();

    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");

    // Ground mesh
    loadFloor(standardShader);

    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/rally_car.obj");
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(playerMesh);
    gameObject->addComponent(new PlayerController(tiles, tileWidth));
    gameObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    //gameObject->setScale(chag::make_vector(0.1f, 3.0f, 0.1f));
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    gameObject->addRenderComponent(stdrenderer);
    gameObject->setDynamic(true);
    gameObject->setIdentifier(2);
    gameObject->addCollidesWith(1);

    scene->addShadowCaster(gameObject);
}

std::vector<int> split(const std::string &str) {
    std::vector<int> internal;
    std::stringstream ss(str);
    std::string tok;

    for (char c : str) {
        internal.push_back(c);
    }

    return internal;
}

void loadFloor(const std::shared_ptr<ShaderProgram> &standardShader) {
    tiles = std::make_shared<std::vector<std::vector<int>>>();
    std::ifstream file("../assets/map/map.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            tiles.get()->push_back(split(line));
        }
        file.close();
    }

    std::shared_ptr<Mesh> asphaltMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");
    std::shared_ptr<Mesh> grassMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/grass.obj");

    for (int y = 0; y < tiles.get()->size(); y++) {
        for (int x = 0; x < ((*tiles.get())[y]).size(); x++) {

            std::shared_ptr<Mesh> mesh;

            if ((*tiles.get())[y][x] == '_' ) {
                mesh = grassMesh;
            } else {
                mesh = asphaltMesh;
            }

            std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>(mesh);
            StandardRenderer *stdFloorRenderer = new StandardRenderer(mesh, standardShader);

            // Placement offset is needed to get the corner of the tile to match the tiles grid coordinates
            const chag::SmallVector3<float> &placementOffset = chag::make_vector(-tileWidth / 2, 0.0f, -tileWidth / 2);
            // The minus for x is needed as the x axis is flipped. For y we need it as we traverse top to down
            floorObject->setLocation(placementOffset + chag::make_vector(-tileWidth * x, 0.0f, -tileWidth * y));
            floorObject->setScale(chag::make_vector(1.0f, 1.0f, 1.0f));
            floorObject->addRenderComponent(stdFloorRenderer);

            scene->addShadowCaster(floorObject);
        }
    }
}

void createKeyListeners() {
    ControlsManager* cm = ControlsManager::getInstance();
    cm->addBinding(ACCELERATE, new KeyboardButton(sf::Keyboard::W, sf::Keyboard::S));
    cm->addBinding(TURN, new KeyboardButton(sf::Keyboard::D, sf::Keyboard::A));
}

int main() {

    Logger::addLogHandler(new StdOutLogHandler());
    Logger::setLogLevel(LogLevel::INFO);

    Window* win = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubba Fing Racer");

    // Setup callbacks for window events
    win->setResizeMethod(resize);
    win->setIdleMethod(idle);
    win->setDisplayMethod(display);

    renderer.setBackgroundColor(chag::make_vector(0.0f,0.0f,0.0f));
    renderer.initRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);

    camera = new TopDownCamera(chag::make_vector(-25.0f, 0.0f, -25.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

    loadWorld();
    createLight();
    createKeyListeners();

    win->start(60);

    return 0;
}
