#include "RenderProject.h"

/* Initialize the Project */
vmml::AABBf aabb;
void RenderProject::init()
{
    bRenderer::loadConfigFile("config.json");	// load custom configurations replacing the default values in Configuration.cpp
    
    // let the renderer create an OpenGL context and the main window
    if(Input::isTouchDevice())
        bRenderer().initRenderer(true);										// full screen on iOS
    else
        bRenderer().initRenderer(1920, 1080, false, "Assignment 6");		// windowed mode on desktop
    //bRenderer().initRenderer(View::getScreenWidth(), View::getScreenHeight(), true);		// full screen using full width and height of the screen
    
    // start main loop
    bRenderer().runRenderer();
}

/* This function is executed when initializing the renderer */
void RenderProject::initFunction()
{
    // get OpenGL and shading language version
    bRenderer::log("OpenGL Version: ", glGetString(GL_VERSION));
    bRenderer::log("Shading Language Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // initialize variables
    _offset = 0.0f;
    _randomOffset = 0.0f;
    _cameraSpeed = 40.0f;
    _running = true; _lastStateSpaceKey = bRenderer::INPUT_UNDEFINED;
    _viewMatrixHUD = Camera::lookAt(vmml::Vector3f(0.f, 0.0f, 0.0f), vmml::Vector3f::ZERO, vmml::Vector3f::UP);
    
    // set shader versions (optional)
    bRenderer().getObjects()->setShaderVersionDesktop("#version 120");
    bRenderer().getObjects()->setShaderVersionES("#version 100");
    
    // load materials and shaders before loading the model
    ShaderPtr guyShader = bRenderer().getObjects()->loadShaderFile("guy", 0, false, false, false, false, false);				// load shader from file without lighting, the number of lights won't ever change during rendering (no variable number of lights)
    
    // create additional properties for a model
    PropertiesPtr guyProperties = bRenderer().getObjects()->createProperties("guyProperties");
    
    // load models
    //bRenderer().getObjects()->loadObjModel("guy.obj", true, true, true, 0, false, false, guyProperties);
    bRenderer().getObjects()->loadObjModel("minecraftcharacter.obj", false, true, guyShader, guyProperties);
    bRenderer().getObjects()->loadObjModel("block.obj", false, true, guyShader, guyProperties);
    bRenderer().getObjects()->loadObjModel("clouds.obj", false, true, guyShader, guyProperties);
    bRenderer().getObjects()->loadObjModel("backgroundPlane.obj", false, true, guyShader, guyProperties);
//    bRenderer().getInput()->singleTapRecognized();
//
   
    
    // automatically generates a shader with a maximum of 4 lights (number of lights may vary between 0 and 4 during rendering without performance loss)
    
    // create camera
    bRenderer().getObjects()->createCamera("camera", vmml::Vector3f(0.0f, 0.0f, 10.0f), vmml::Vector3f(0.f, 0.0f, 0.5f));
    
    
    //Set starting position
    controller.initialize(bRenderer());

    
//    player_character = Player(tempPos);
    
    
    // Update render queue
    updateRenderQueue("camera", 0.0f);
    
    
    
    
}

/* Draw your scene here */
void RenderProject::loopFunction(const double &deltaTime, const double &elapsedTime)
{
    //	bRenderer::log("FPS: " + std::to_string(1 / deltaTime));	// write number of frames per second to the console every frame
    
    //// Draw Scene and do post processing ////
    
    /// Begin post processing ///
    //	GLint defaultFBO;
    //	if (!_running){
    //		bRenderer().getView()->setViewportSize(bRenderer().getView()->getWidth() / 5, bRenderer().getView()->getHeight() / 5);		// reduce viewport size
    //		defaultFBO = Framebuffer::getCurrentFramebuffer();	// get current fbo to bind it again after drawing the scene
    //		bRenderer().getObjects()->getFramebuffer("fbo")->bindTexture(bRenderer().getObjects()->getTexture("fbo_texture1"), false);	// bind the fbo
    //	}
    
    /// Draw scene ///
    
    bRenderer().getModelRenderer()->drawQueue(/*GL_LINES*/);
    bRenderer().getModelRenderer()->clearQueue();
    
    //// Camera Movement ////
    updateCamera("camera", deltaTime);
    
    /// Update render queue ///
    updateRenderQueue("camera", deltaTime);
    
    // Quit renderer when escape is pressed
//    if (bRenderer().getInput()->getKeyState(bRenderer::KEY_ESCAPE) == bRenderer::LEFT_MOUSE_BUTTON)
//        bRenderer().terminateRenderer();
}

/* This function is executed when terminating the renderer */
void RenderProject::terminateFunction()
{
    bRenderer::log("I totally terminated this Renderer :-)");
}

/* Update render queue */
void RenderProject::updateRenderQueue(const std::string &camera, const double &deltaTime)
{
 
    vmml::Matrix4f modelMatrix;
    
    
    // Touch controls TODO: Implement more directions, implement Collision Detection, adjust Camera according to player character
    bool isTouching = false;
    int direction;  // Directions: 1=Right, 2=Left, 3=Jump, 4=Down, 5=No Input
    TouchMap touchMap = bRenderer().getInput()->getTouches();
    
    for (auto t = touchMap.begin(); t != touchMap.end(); ++t)
    {
       elapsedTime = (elapsedTime+deltaTime);
        isTouching=true;
        
        Touch touch = t->second;
        
        // if touch is in right half of the view: move right
        
        if (touch.startPositionX > bRenderer().getView()->getWidth() / 2){
            direction=1;
            
        }
        
        // If touch is in left half of the view: move left
        
        if (touch.startPositionX < bRenderer().getView()->getWidth() / 2){
            direction=2;
            
        }
        
        
        
    }
    if(isTouching==false){
        direction=5;
    }
    
    controller.update(0.04f,direction);
    
    
    
    // ---------> Prints BoundingBox Size of a given model in console
    std::cout << "BB: " << bRenderer().getObjects()->getModel("minecraftcharacter")->getBoundingBoxObjectSpace();
   

    


//    int ButtonPress = ;bRenderer().getInput()->getCursorPositionX()
    
//    printf("%d",ButtonPress);
    //Renderer().getInput()->getTouches().map::
    
//    printf("%d",Renderer().getInput()->doubleTapRecognized());
//
//    if (bRenderer().getInput()->singleTapRecognized())
//{
//        
//        
//        
//        modelMatrix = vmml::create_translation(vmml::Vector3f(elapsedTime,0,0)) * player_character.getPos();
//        
//        
//    }
//    else{
//        
//       modelMatrix = player_character.getPos();
//    }
//
//    
//    /*** solar system ***/
//    
//    // TODO: implement solar system here
//    
//    
//    elapsedTime = elapsedTime+deltaTime;
//    
//    
//    //Assign Block Positions
//    
//    vmml::Matrix4f modelMatrix2 = vmml::create_translation(vmml::Vector3f(5,0,0)) *  player_character.getPos();
//
//    
//    
//    vmml::Matrix4f viewMatrix = bRenderer().getObjects()->getCamera("camera")->getViewMatrix();
//    
//    ShaderPtr shader = bRenderer().getObjects()->getShader("guy");
//    
//    if (shader.get())
//    {
//        shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
//        shader->setUniform("ViewMatrix", viewMatrix);
//        shader->setUniform("ModelMatrix", modelMatrix);
//        
//        vmml::Matrix3f normalMatrix;
//        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix)), normalMatrix);
//        shader->setUniform("NormalMatrix", normalMatrix);
//        
//        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//        shader->setUniform("EyePos", eyePos);
//        
//        shader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
//        shader->setUniform("Ia", vmml::Vector3f(1.f));
//        shader->setUniform("Id", vmml::Vector3f(1.f));
//        shader->setUniform("Is", vmml::Vector3f(1.f));
//    }
//    else
//    {
//        bRenderer::log("No shader available.");
//    }
//     
//    
//
//    
//    //shader->setUniform("NormalMatrix", vmml::Matrix3f(modelMatrix));
//    
//    
//    bRenderer().getModelRenderer()->drawModel("minecraftcharacter", "camera", modelMatrix, std::vector<std::string>({ }));
//    
//
//    
//    shader = bRenderer().getObjects()->getShader("guy");
//    
//    if (shader.get())
//    {
//        shader->setUniform("ProjectionMatrix", vmml::Matrix4f::IDENTITY);
//        shader->setUniform("ViewMatrix", viewMatrix);
//        shader->setUniform("ModelMatrix", modelMatrix2);
//        
//        vmml::Matrix3f normalMatrix;
//        vmml::compute_inverse(vmml::transpose(vmml::Matrix3f(modelMatrix2)), normalMatrix);
//        shader->setUniform("NormalMatrix", normalMatrix);
//        
//        vmml::Vector4f eyePos = vmml::Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
//        shader->setUniform("EyePos", eyePos);
//        
//        shader->setUniform("LightPos", vmml::Vector4f(0.f, 1.f, .5f, 1.f));
//        shader->setUniform("Ia", vmml::Vector3f(1.f));
//        shader->setUniform("Id", vmml::Vector3f(1.f));
//        shader->setUniform("Is", vmml::Vector3f(1.f));
//    }
//    else
//    {
//        bRenderer::log("No shader available.");
//    }
//    
//    
//    bRenderer().getModelRenderer()->drawModel("block", "camera", modelMatrix2, std::vector<std::string>({ }));
//    
//    
    

}

/* Camera movement */
void RenderProject::updateCamera(const std::string &camera, const double &deltaTime)
{
    //// Adjust aspect ratio ////
    bRenderer().getObjects()->getCamera(camera)->setAspectRatio(bRenderer().getView()->getAspectRatio());

    bRenderer().getObjects()->getCamera(camera)->setRotation(vmml::Vector3f(0.0f,0.f,0.f));
    
    bRenderer().getObjects()->getCamera(camera)->setPosition(controller.getPlayerPosition() - vmml::Vector3f(0.0f,0.0f,-10.0f));
}

/* For iOS only: Handle device rotation */
void RenderProject::deviceRotated()
{
    if (bRenderer().isInitialized()){
        // set view to full screen after device rotation
        bRenderer().getView()->setFullscreen(true);
        bRenderer::log("Device rotated");
    }
}

/* For iOS only: Handle app going into background */
void RenderProject::appWillResignActive()
{
    if (bRenderer().isInitialized()){
        // stop the renderer when the app isn't active
        bRenderer().stopRenderer();
    }
}

/* For iOS only: Handle app coming back from background */
void RenderProject::appDidBecomeActive()
{
    if (bRenderer().isInitialized()){
        // run the renderer as soon as the app is active
        bRenderer().runRenderer();
    }
}

/* For iOS only: Handle app being terminated */
void RenderProject::appWillTerminate()
{
    if (bRenderer().isInitialized()){
        // terminate renderer before the app is closed
        bRenderer().terminateRenderer();
    }
}

/* Helper functions */
GLfloat RenderProject::randomNumber(GLfloat min, GLfloat max){
    return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}