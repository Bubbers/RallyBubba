
#include <Renderer.h>
#include <cameras/TopDownCamera.h>
#include <constants.h>
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <Scene.h>
#include <Window.h>
#include <ColliderFactory.h>
#include <components/WinOnCollisionComponentAndCheckpointsDone.h>
#include <Logger.h>
#include <StdOutLogHandler.h>
#include <ControlsManager.h>
#include <KeyboardButton.h>
#include <components/PlayerController.h>
#include "controls.h"
#include <fstream>
#include <ui/Menu.h>
#include "components/UnlockCheckpointOnCollisionComponent.h"

Renderer renderer;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool isInMenu = true;
HudRenderer *menuRenderer;
TopDownCamera *camera;
std::shared_ptr<Scene> scene;
Collider *collider = ColliderFactory::getTwoPhaseCollider();
std::shared_ptr<std::vector<std::vector<char>>> tiles;
const float tileWidth = 2.0f;

std::shared_ptr<std::vector<bool>> checkpoints = std::make_shared<std::vector<bool>>();

void loadFloor(const std::shared_ptr<ShaderProgram> &standardShader);
void updateWind();

void createTile(const std::shared_ptr<ShaderProgram> &standardShader, int y, int x, const std::shared_ptr<Mesh> &mesh,
           bool has_wind);

void createCheckpoint(const std::shared_ptr<ShaderProgram> &standardShader, int x, int y);
void createGoal(const std::shared_ptr<ShaderProgram> &standardShader, int x, int y);

void idle(float timeSinceStart, float timeSinceLastCall) {
    scene->update(timeSinceLastCall);
    collider->updateCollision(scene.get());
    updateWind();
}

float lastWindChangeTime = 0;
chag::float3 lastWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);
chag::float3 currentWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);
chag::float3 newWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);

sf::Clock clock1;

void updateWind() {
    float currentTime = clock1.getElapsedTime().asSeconds();
    if (currentTime - lastWindChangeTime > 1.0f) {
        lastWindChangeTime += 1;
        lastWindSpeed = newWindSpeed;
        float x = getRand(0, 1);
        float y = getRand(0, 1);
        float z = getRand(0, 1);

        newWindSpeed.x = fabs(x * 2.0f - 1.0f);
        newWindSpeed.y = fabs(y * 2.0f - 1.0f);
        newWindSpeed.z = fabs(z * 2.0f - 1.0f);

    }

    currentWindSpeed = linearSmoothStep(lastWindSpeed, newWindSpeed, currentTime - lastWindChangeTime);
    scene->setWind(std::make_shared<Wind>(currentWindSpeed));

}


// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    renderer.drawScene(camera, scene.get(), timeSinceStart);
}

// Called when the window gets resized
void resize(int newWidth, int newHeight) {
    if(isInMenu) {
        //menuRenderer->updateLayout();
    }
    renderer.resize(newWidth, newHeight);
}

void createLight() {


    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->diffuseColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->specularColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);

    directionalLight->direction= -chag::make_vector(200.0f,200.0f,0.0f);
    scene->directionalLight = directionalLight;
//    scene->shadowMapCamera = new PerspectiveCamera(
//            chag::make_vector(-200.0f, 200.0f, -200.0f),
//            chag::make_vector(0.0f, 0.0f, 0.0f),
//            chag::make_vector(0.0f, 1.0f, 0.0f),
//            45,
//            1.0f,
//            1.0f,
//            3000.0f
//    );

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    pointLight->specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight->position = chag::make_vector(18.0f, 3.0f, 0.0f);
    scene->pointLights.push_back(pointLight);
}

void createKeyListeners() {
    ControlsManager* cm = ControlsManager::getInstance();
    cm->addBinding(ACCELERATE, new KeyboardButton(sf::Keyboard::W, sf::Keyboard::S));
    cm->addBinding(TURN, new KeyboardButton(sf::Keyboard::D, sf::Keyboard::A));
}

void loadWorld() {
    isInMenu = false;
    scene = std::make_shared<Scene>();

    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");

    // Ground mesh
    loadFloor(standardShader);

    createLight();
    createKeyListeners();
}

void loadMenu() {
    scene = std::make_shared<Scene>();

    auto menuBg = ResourceManager::loadAndFetchTexture("../assets/menu/bubba_menu.png");

    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    menuRenderer = new HudRenderer();
    Menu* menu = new Menu(menuBg.get(), []() { loadWorld(); });
    menuRenderer->setLayout(menu);
    hudObj->addRenderComponent(menuRenderer );
    scene->addTransparentObject(hudObj);
}

std::vector<char> split(const std::string &str) {
    std::vector<char> internal;
    std::stringstream ss(str);
    std::string tok;

    for (char c : str) {
        internal.push_back(c);
    }

    return internal;
}

void createPlayer(std::shared_ptr<ShaderProgram> standardShader, int x, int y){
    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/rally_car.obj");
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(playerMesh);
    gameObject->addComponent(new PlayerController(tiles, tileWidth));
    gameObject->setLocation(chag::make_vector(-tileWidth * x, 0.0f, -tileWidth * y));
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    gameObject->addRenderComponent(stdrenderer);
    gameObject->setDynamic(true);
    gameObject->setIdentifier(2);
    gameObject->addCollidesWith(7);

    scene->addShadowCaster(gameObject);
}

void loadFloor(const std::shared_ptr<ShaderProgram> &standardShader) {
    tiles = std::make_shared<std::vector<std::vector<char>>>();
    std::ifstream file("../assets/map/map.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            tiles.get()->push_back(split(line));
        }
        file.close();
    }

    std::shared_ptr<Mesh> grassMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/grass.obj");
    std::shared_ptr<Mesh> asphaltMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");
    std::shared_ptr<Mesh> mountainMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/mountain.obj");
    std::shared_ptr<Mesh> treeMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/tree.obj");

    for (int y = 0; y < tiles.get()->size(); y++) {
        for (int x = 0; x < ((*tiles.get())[y]).size(); x++) {

            std::shared_ptr<Mesh> mesh;

            bool has_wind = false;
            char &tile = (*tiles.get())[y][x];
            if (tile == '_' ) {
                createTile(standardShader, y, x, grassMesh, has_wind);
            } else if (tile == '#'){
                createTile(standardShader, y, x, asphaltMesh, has_wind);
            } else if (tile == '^'){
                createTile(standardShader, y, x, mountainMesh, has_wind);
            } else if (tile == '$'){
                has_wind = true;
                createTile(standardShader, y, x, treeMesh, has_wind);
            } else if (tile == 'S'){
                createPlayer(standardShader, x, y);
                createTile(standardShader, y, x, asphaltMesh, has_wind);
            } else if (tile == 'C'){
                createCheckpoint(standardShader, x, y);
                createTile(standardShader, y, x, asphaltMesh, has_wind);
            } else if (tile == 'E'){
                createGoal(standardShader, x, y);
                createTile(standardShader, y, x, asphaltMesh, has_wind);
            }
        }
    }
}

void createCheckpoint(const std::shared_ptr<ShaderProgram> &standardShader, int x, int y) {
    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/revive.fbx");
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(playerMesh);

    const chag::SmallVector3<float> &placementOffset = chag::make_vector(-tileWidth / 2, 0.0f, -tileWidth / 2);
    gameObject->setLocation(placementOffset + chag::make_vector(-tileWidth * x, 0.0f, -tileWidth * y));
    gameObject->update(0.0f);
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    gameObject->addRenderComponent(stdrenderer);
    gameObject->setIdentifier(7);

    gameObject->addComponent(new UnlockCheckpointOnCollisionComponent(checkpoints->size(), checkpoints));
    checkpoints->push_back(false);

    scene->addShadowCaster(gameObject);
}

void createGoal(const std::shared_ptr<ShaderProgram> &standardShader, int x, int y) {
    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/revive.fbx");
    std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(playerMesh);

    const chag::SmallVector3<float> &placementOffset = chag::make_vector(-tileWidth / 2, 0.0f, -tileWidth / 2);
    gameObject->setLocation(placementOffset + chag::make_vector(-tileWidth * x, 0.0f, -tileWidth * y));
    gameObject->update(0.0f);
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    gameObject->addRenderComponent(stdrenderer);
    gameObject->setIdentifier(7);

    gameObject->addComponent(new WinOnCollisionComponentAndCheckpointsDone(checkpoints));

    scene->addShadowCaster(gameObject);
}

void createTile(const std::shared_ptr<ShaderProgram> &standardShader, int y, int x, const std::shared_ptr<Mesh> &mesh,
           bool has_wind) {
    std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>(mesh);
    StandardRenderer *stdFloorRenderer = new StandardRenderer(mesh, standardShader);

    // Placement offset is needed to get the corner of the tile to match the tiles grid coordinates
    const chag::SmallVector3<float> &placementOffset = chag::make_vector(-tileWidth / 2, 0.0f, -tileWidth / 2);
    // The minus for x is needed as the x axis is flipped. For y we need it as we traverse top to down
    floorObject->setLocation(placementOffset + chag::make_vector(-tileWidth * x, 0.0f, -tileWidth * y));
    floorObject->setScale(chag::make_vector(1.0f, 1.0f, 1.0f));
    floorObject->addRenderComponent(stdFloorRenderer);

    if (has_wind) {
                floorObject->setAffectedByWind(true);
                floorObject->setMainBendiness(0.05);
                floorObject->setBranchAmplitude(0.5);
                floorObject->setLeafAmplitude(0.05);
            }

    scene->addShadowCaster(floorObject);
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

    loadMenu();

    win->start(60);

    return 0;
}
