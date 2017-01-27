/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                               Entry Point file
/-------------------------------------------------------------------------------
*/

#define _USE_MATH_DEFINES

#include "Camera.h"
#include "Floor.h"
#include "Hair.h"
#include "HeadersAndDefines.h"
#include "Light.h"
#include "Mannequin.h"

float *Vector3D::arr;
float *Vector4D::arr;

// Global Variables
int currentX, currentY, windowHandle;
unsigned long long iteration;
unsigned int tilt, MEASUREMENT_STEP;
float pc_frequency[MAX_COUNTERS], counter_start[MAX_COUNTERS];
void *dev_vbo;
struct cudaGraphicsResource *cudaVBO;
Camera *cam;
Floor *ground;
Light *light;
Mannequin *mannequin;
Hair *hair;
Vector3D focus, hairColor;
std::string frameTime, measurementOverheadTime, cpuPhysicsTime, gpuPhysicsTime,
    cudaTime, gravityText, windText, numberingText, RUNS_ON, HAIR_OFF,
    measurementStep;
cudaError_t cudaStatus;
CUresult cudaResult;
Vector3D *dev_vertices;
bool textShouldBeDrawn;

void
initScene();
cudaError_t
kernelWrapper(Vector3D *dev_vertices,
              unsigned int vertices_size,
              unsigned int hairLength,
              unsigned int gravity,
              Vector3D centerOfGravity,
              Vector3D upsizedCenterOfGravity,
              Vector3D downsizedCenterOfGravity,
              Vector3D WindDirection,
              bool wind,
              float fallSpeed,
              float windSpeed,
              float &time,
              float timeSinceLastSimulation);
cudaError_t
tiltKernelWrapper(Vector3D *dev_vertices,
                  unsigned int vertices_size,
                  unsigned int tiltDirection);

void
updateWindText() {
    if (hair->wind) {
        if (hair->windDirection == X) {
            windText =
                std::string("Wind:                         Blowing from X
");
            return;
        } else if (hair->windDirection == MinusX) {
            windText = std::string(
                "Wind:                         Blowing from MinusX
");
            return;
        } else if (hair->windDirection == Z) {
            windText =
                std::string("Wind:                         Blowing from Z
");
            return;
        } else if (hair->windDirection == MinusZ) {
            windText = std::string(
                "Wind:                         Blowing from MinusZ
");
            return;
        }
    } else {
        windText = std::string("Wind:                         Off
");
        return;
    }
}

void
updateGravityText() {
    if (hair->gravity == GRAVITY_OFF) {
        gravityText = std::string("Gravity:                      Off
");
    } else if (hair->gravity == NORMAL_GRAVITY) {
        gravityText = std::string("Gravity:                      Normal
");
    } else if (hair->gravity == NEGATIVE_GRAVITY) {
        gravityText = std::string("Gravity:                      Negative
");
    }
}

// Start QueryPerformanceCounter.
void
startCounter(unsigned int i) {
    /*
        // Declarations.
        LARGE_INTEGER li;
        BOOL success;

        success = QueryPerformanceFrequency( &li );
        if( !success )
            errorHandler( ERROR_CODE4, "ERROR 4: QueryPerformanceFrequency
       failed!", GetLastErrorAsString() );

        pc_frequency[i] = (float)li.QuadPart;

        success = QueryPerformanceCounter( &li );
        if( !success )
            errorHandler( ERROR_CODE7, "ERROR 7: QueryPerformanceCounter
       failed!", GetLastErrorAsString() );

        counter_start[i] = (float)li.QuadPart;
    */
}

// End QueryPerformanceCounter and get result.
float
getCounter(unsigned int i) {
    /*
        // Declarations.
        LARGE_INTEGER li;
        BOOL success;

        success = QueryPerformanceCounter( &li );
        if( !success )
            errorHandler( ERROR_CODE7, "ERROR 7: QueryPerformanceCounter
       failed!", GetLastErrorAsString() );

        return float( li.QuadPart - counter_start[i] ) / pc_frequency[i];
    */
}

// Transforms long floating point numbers in 2 decimal floating point numbers.
std::string
limitDecimalsTo2(std::string text) {
    std::size_t whereAt;
    whereAt = text.find(".");
    if (whereAt == std::string::npos)
        return text;
    else
        return text.substr(0, whereAt + 3);
}

// Draw text using orthogonal projection.
void
drawText(std::string textToBeDisplayed) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, (GLfloat)WINDOW_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, (GLfloat)WINDOW_HEIGHT - 30.0f, 0.0f);
    glScalef(0.1f, 0.1f, 1.0f);
    glColor3f(1, 1, 1);
    glutStrokeString(GLUT_STROKE_MONO_ROMAN,
                     (const unsigned char *)textToBeDisplayed.c_str());
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// Draw objects and render camera.
void
drawScene() {

    // Draw objects.
    ground->draw();
    mannequin->draw();
    hair->draw(hairColor);

    // Render camera.
    cam->render();
}

// Window reshape handler.
void
reshape(int width, int height) {

    glViewport((GLint)0, (GLint)0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW_ANGLE, (GLdouble)width / (GLdouble)height,
                   Z_NEAR, Z_FAR);
}

// Mouse passive motion handler.
void
passiveMotion(int x, int y) {

    int dx, dy;

    dx = x - currentX;
    dy = y - currentY;

    // if( abs( dx ) < 30 && abs( dy ) < 30 ){
    cam->rotateFPS_OX(dy * ROTATION_SPEED);
    cam->rotateFPS_OY(dx * ROTATION_SPEED);
    //}
    // Update the saved mouse position.
    if (x != WINDOW_WIDTH / 2 || y != WINDOW_HEIGHT / 2) {
        glutWarpPointer(currentX, currentY);
    }
}

// Keyboard Handler.
void
keyboard(unsigned char key, int x, int y) {

    std::ostringstream stream_measurementStep;
    switch (key) {
    case KEY_W_CAPITAL:
    case KEY_W_LOWER:
        cam->moveForwardGrounded(SPEED);
        break;
    case KEY_A_CAPITAL:
    case KEY_A_LOWER:
        cam->moveRightGrounded(-SPEED);
        break;
    case KEY_S_CAPITAL:
    case KEY_S_LOWER:
        cam->moveForwardGrounded(-SPEED);
        break;
    case KEY_D_CAPITAL:
    case KEY_D_LOWER:
        cam->moveRightGrounded(SPEED);
        break;
    case KEY_Q_CAPITAL:
    case KEY_Q_LOWER:
        cam->lookAt(focus);
        cam->rotateTPS_OY(CAMERA_ROTATION_SPEED, cam->position.Distance(focus));
        break;
    case KEY_E_CAPITAL:
    case KEY_E_LOWER:
        cam->lookAt(focus);
        cam->rotateTPS_OY(-CAMERA_ROTATION_SPEED,
                          cam->position.Distance(focus));
        break;
    case KEY_R_CAPITAL:
    case KEY_R_LOWER:
        cam->moveUpward(SPEED);
        break;
    case KEY_C_CAPITAL:
    case KEY_C_LOWER:
        cam->moveUpward(-SPEED);
        break;
    case KEY_F_CAPITAL:
    case KEY_F_LOWER:
        hair->wind = !hair->wind;
        updateWindText();
        break;
    case KEY_G_CAPITAL:
    case KEY_G_LOWER:
        hair->gravity =
            (N_GRAVITY_TYPES - hair->gravity) %
            N_GRAVITY_TYPES;  // Does not work for N_GRAVITY_TYPES > 3.
        updateGravityText();
        break;
    case KEY_H_CAPITAL:
    case KEY_H_LOWER:
        if (hair->gravity == GRAVITY_OFF)
            hair->gravity = NORMAL_GRAVITY;
        else
            hair->gravity = GRAVITY_OFF;
        updateGravityText();
        break;
    case KEY_X_CAPITAL:
    case KEY_X_LOWER:
        if (hair->windDirection == X)
            hair->windDirection = MinusX;
        else
            hair->windDirection = X;
        updateWindText();
        break;
    case KEY_Z_CAPITAL:
    case KEY_Z_LOWER:
        if (hair->windDirection == Z)
            hair->windDirection = MinusZ;
        else
            hair->windDirection = Z;
        updateWindText();
        break;
    case KEY_0:
    case KEY_1:
    case KEY_2:
    case KEY_3:
    case KEY_4:
    case KEY_5:
    case KEY_6:
    case KEY_7:
    case KEY_8:
    case KEY_9:
        HAIR_LENGTH = key - 0x30;
        initScene();
        break;
    case KEY_SPACE:
        if (RUNS_ON == "CPU")
            RUNS_ON = "GPU";
        else
            RUNS_ON = "CPU";
        initScene();
        break;
    case KEY_MINUS:
        if (MEASUREMENT_STEP > 1)
            MEASUREMENT_STEP >>= 1;
        stream_measurementStep << MEASUREMENT_STEP;
        measurementStep = stream_measurementStep.str();
        break;
    case KEY_EQUALS:
    case KEY_PLUS:
        if (MEASUREMENT_STEP < 10000)
            MEASUREMENT_STEP <<= 1;
        stream_measurementStep << MEASUREMENT_STEP;
        measurementStep = stream_measurementStep.str();
        break;
    case KEY_O_CAPITAL:
    case KEY_O_LOWER:
        HAIR_OFF = "OFF/humanHair500k.off";
        initScene();
        break;
    case KEY_P_CAPITAL:
    case KEY_P_LOWER:
        textShouldBeDrawn = !textShouldBeDrawn;
        break;
    case KEY_L_CAPITAL:
    case KEY_L_LOWER:
        if (glIsEnabled(GL_LINE_SMOOTH))
            glDisable(GL_LINE_SMOOTH);
        else
            glEnable(GL_LINE_SMOOTH);
        break;
    case KEY_ESC:
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}

// Special keyboard Handler.
void
special(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_UP:
        mannequin->tiltForward(TILT_SPEED);
        if (RUNS_ON == "CPU") {
            hair->tiltForward(TILT_SPEED);
        } else if (RUNS_ON == "GPU") {
            tilt = 1 << TILT_FORWARD;
        }
        break;
    case GLUT_KEY_LEFT:
        mannequin->tiltSideways(TILT_SPEED);
        if (RUNS_ON == "CPU") {
            hair->tiltSideways(TILT_SPEED);
        } else if (RUNS_ON == "GPU") {
            tilt = 1 << TILT_LEFT;
        }
        break;
    case GLUT_KEY_DOWN:
        mannequin->tiltForward(-TILT_SPEED);
        if (RUNS_ON == "CPU") {
            hair->tiltForward(-TILT_SPEED);
        } else if (RUNS_ON == "GPU") {
            tilt = 1 << TILT_BACKWARDS;
        }
        break;
    case GLUT_KEY_RIGHT:
        mannequin->tiltSideways(-TILT_SPEED);
        if (RUNS_ON == "CPU") {
            hair->tiltSideways(-TILT_SPEED);
        } else if (RUNS_ON == "GPU") {
            tilt = 1 << TILT_RIGHT;
        }
        break;
    case GLUT_KEY_F1:
        HAIR_OFF = HAIR_OFF1;
        initScene();
        break;
    case GLUT_KEY_F2:
        HAIR_OFF = HAIR_OFF2;
        initScene();
        break;
    case GLUT_KEY_F3:
        HAIR_OFF = HAIR_OFF3;
        initScene();
        break;
    case GLUT_KEY_F4:
        HAIR_OFF = HAIR_OFF4;
        initScene();
        break;
    case GLUT_KEY_F5:
        HAIR_OFF = HAIR_OFF5;
        initScene();
        break;
    case GLUT_KEY_F6:
        HAIR_OFF = HAIR_OFF6;
        initScene();
        break;
    case GLUT_KEY_F7:
        HAIR_OFF = HAIR_OFF7;
        initScene();
        break;
    case GLUT_KEY_F8:
        HAIR_OFF = HAIR_OFF8;
        initScene();
        break;
    case GLUT_KEY_F9:
        HAIR_OFF = HAIR_OFF9;
        initScene();
        break;
    case GLUT_KEY_F10:
        HAIR_OFF = HAIR_OFF10;
        initScene();
        break;
    case GLUT_KEY_F11:
        HAIR_OFF = HAIR_OFF11;
        initScene();
        break;
    case GLUT_KEY_F12:
        HAIR_OFF = HAIR_OFF12;
        initScene();
        break;
    default:
        break;
    }
}

// Idle callback function.
void
idle() {

    // Declarations.
    float finish, flops, kernelTime, kernelFlops;
    std::ostringstream stream_finish, stream_flops, stream_kernelTime,
        stream_kernelFlops;
    size_t size;

    // std::cout << "Collisions: " << collision( hair, mannequin,
    // hair->centerOfGravity ).size() << std::endl;

    // Measurement overhead.
    startCounter(MEASUREMENT_OVERHEAD_TIME);
    finish = getCounter(MEASUREMENT_OVERHEAD_TIME);
    if (iteration % MEASUREMENT_STEP == 0) {
        finish *= 1000000.0f;  // Modify here for different order of magnitude.
                               // Current: us
        stream_finish << finish;
        measurementOverheadTime = "Measurement overhead time:    " +
                                  limitDecimalsTo2(stream_finish.str()) +
                                  " us
";
    }

    // On CPU:
    if (RUNS_ON == "CPU") {
        startCounter(CPU_PHYSICS_TIME);
        hair->timeSinceLastSimulation =
            1000 * getCounter(INTER_SIMULATION_TIME);
        hair->simulateAnimation();
        startCounter(INTER_SIMULATION_TIME);
        if (iteration % MEASUREMENT_STEP == 0) {
            finish = getCounter(CPU_PHYSICS_TIME);
            finish *= 1000.0f;  // Modify here for different order of magnitude.
                                // Current: ms
            flops = hair->vertices.size() *
                    (4 + 8 * hair->length / (hair->length + 1)) / finish /
                    1000.0f;  // Modify here for different order of magnitude.
                              // Current: GFLOPS
            stream_finish << finish;
            stream_flops << flops;
            cpuPhysicsTime = "CPU physics time:             " +
                             limitDecimalsTo2(stream_finish.str()) +
                             " ms
Raw performance:              " +
                             limitDecimalsTo2(stream_flops.str()) + " GFLOPS
";
        }
    }

    // On GPU:
    else if (RUNS_ON == "GPU") {
        startCounter(GPU_PHYSICS_TIME);

        cudaStatus = cudaGraphicsMapResources(1, &cudaVBO, 0);
        if (cudaStatus != cudaSuccess)
            errorHandler(cudaStatus, "cudaGraphicsMapResources() failed.",
                         cudaGetErrorString(cudaStatus));

        cudaStatus = cudaGraphicsResourceGetMappedPointer(
            (void **)&dev_vertices, &size, cudaVBO);
        if (cudaStatus != cudaSuccess)
            errorHandler(cudaStatus,
                         "cudaGraphicsResourceGetMappedPointer() failed.",
                         cudaGetErrorString(cudaStatus));

        size /= sizeof(
            Vector3D);  // Convert from number of bytes to number of elements.
        hair->timeSinceLastSimulation =
            1000 * getCounter(INTER_SIMULATION_TIME);
        cudaStatus = kernelWrapper(
            dev_vertices, size, hair->length, hair->gravity,
            hair->centerOfGravity, hair->upsizedCenterOfGravity,
            hair->downsizedCenterOfGravity, hair->windDirection, hair->wind,
            FALL_SPEED, WIND_SPEED * (float)(rand() % 100) / 100.0f, kernelTime,
            hair->timeSinceLastSimulation);
        startCounter(INTER_SIMULATION_TIME);
        kernelTime = WIND_SPEED * (float)(rand() % 100) / 100.0f;
        if (cudaStatus != cudaSuccess)
            errorHandler(cudaStatus, "kernelWrapper() failed.",
                         cudaGetErrorString(cudaStatus));
        cudaStatus = tiltKernelWrapper(dev_vertices, size, tilt);
        if (cudaStatus != cudaSuccess)
            errorHandler(cudaStatus, "tiltKernelWrapper() failed.",
                         cudaGetErrorString(cudaStatus));
        tilt = 0;

        cudaStatus = cudaGraphicsUnmapResources(1, &cudaVBO, 0);
        if (cudaStatus != cudaSuccess)
            errorHandler(cudaStatus, "cudaGraphicsUnmapResources() failed.",
                         cudaGetErrorString(cudaStatus));

        finish = getCounter(GPU_PHYSICS_TIME);
        if (iteration % MEASUREMENT_STEP == 0) {
            // CUDA Measurement Time.
            kernelTime *= 1.00f;  // Modify here for different order of
                                  // magnitude. Current: ms
            kernelFlops = hair->vertices.size() *
                          (4 + 8 * hair->length / (hair->length + 1)) /
                          kernelTime / 1000.0f;  // Modify here for different
                                                 // order of magnitude. Current:
                                                 // GFLOPS
            stream_kernelTime << kernelTime;
            stream_kernelFlops << kernelFlops;
            cudaTime = "CUDA measured physics time:   " +
                       limitDecimalsTo2(stream_kernelTime.str()) +
                       " ms
Raw performance:              " +
                       stream_kernelFlops.str() + " GFLOPS
";
            // GPU Physics Time
            finish *= 1000.0f;  // Modify here for different order of magnitude.
                                // Current: ms
            flops = hair->vertices.size() *
                    (4 + 8 * hair->length / (hair->length + 1)) / finish /
                    1000.0f;  // Modify here for different order of magnitude.
                              // Current: GFLOPS
            stream_finish << finish;
            stream_flops << flops;
            gpuPhysicsTime = "GPU physics time:             " +
                             limitDecimalsTo2(stream_finish.str()) +
                             " ms
Raw performance:              " +
                             limitDecimalsTo2(stream_flops.str()) + " GFLOPS
";
        }
    }

    iteration += 1;
}

// Display function is being called as fast as the video board can.
void
display() {

    // Declarations.
    std::ostringstream stream_fps, stream_finish;
    float fps, finish;

    // Start timer.
    glFinish();
    finish = getCounter(FRAME_TIME);
    glFinish();
    startCounter(FRAME_TIME);

    // Clear screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
    if (textShouldBeDrawn)
        drawText(measurementOverheadTime + "
Measurement Step:             " +
                 measurementStep + "
" + frameTime + "
" + cpuPhysicsTime +
                 "
" + gpuPhysicsTime + cudaTime + "
" + numberingText +
                 "Runs on:                      " + RUNS_ON + "
" +
                 gravityText + windText);

    // Display time.
    if (iteration % MEASUREMENT_STEP == 0) {
        fps = 1 / finish;
        finish *= 1000.0f;  // Modify here for different order of magnitude.
                            // Current: ms
        stream_finish << finish;
        stream_fps << fps;
        frameTime = "Frame time:                   " +
                    limitDecimalsTo2(stream_finish.str()) +
                    " ms
Frame rate:                   " +
                    limitDecimalsTo2(stream_fps.str()) + " fps
";
    }

    // Double buffering.
    glutSwapBuffers();
    glutPostRedisplay();
}

// Just another new scene in OpenGL.
void
initScene() {

    // Declarations.
    std::ostringstream nOfHairVerticesStream, nOfBaseHairVerticesStream,
        hairLengthStream;

    cudaStatus = cudaGraphicsUnregisterResource(cudaVBO);
    //    if( cudaStatus != cudaSuccess )
    //        errorHandler( cudaStatus, "cudaGraphicsUnregisterResource()
    //        failed.", cudaGetErrorString( cudaStatus ) );

    // Mannequin & hair.
    mannequin->rotation = Vector3D(0, 0, 0);
    hair = new Hair(HumanHair, None, HAIR_LENGTH, RUNS_ON, HAIR_OFF);
    updateGravityText();
    updateWindText();

    if (dev_vertices != NULL)
        cudaFree(dev_vertices);
    cudaMalloc((void **)&dev_vertices,
               hair->vertices.size() * sizeof(Vector3D));

    cudaStatus = cudaGraphicsGLRegisterBuffer(&cudaVBO, hair->vbo,
                                              cudaGraphicsRegisterFlagsNone);
    if (cudaStatus != cudaSuccess)
        errorHandler(cudaStatus, "cudaGraphicsGLRegisterBuffer() failed.",
                     cudaGetErrorString(cudaStatus));

    nOfHairVerticesStream << hair->vertices.size();
    if (hair->length == 0)
        nOfBaseHairVerticesStream << 0;
    else
        nOfBaseHairVerticesStream << hair->vertices.size() / hair->length;
    hairLengthStream << hair->length;
    numberingText =
        "Mobile Hair Density:          " + nOfHairVerticesStream.str() +
        "
Base Hair Density:            " + nOfBaseHairVerticesStream.str() +
        "
Hair length:                  " + hairLengthStream.str() + "
";
}

void
menu(int value) {
    switch (value) {
    case MENU_AMBER:
        hairColor = Vector3D(AMBER_HAIR);
        break;
    case MENU_BLACK:
        hairColor = Vector3D(BLACK_HAIR);
        break;
    case MENU_BLONDE:
        hairColor = Vector3D(BLONDE_HAIR);
        break;
    case MENU_BROWN:
        hairColor = Vector3D(BROWN_HAIR);
        break;
    case MENU_DARK_BROWN:
        hairColor = Vector3D(DARK_BROWN_HAIR);
        break;
    case MENU_RED:
        hairColor = Vector3D(RED_HAIR);
        break;
    case MENU_AUBURN:
        hairColor = Vector3D(AUBURN_HAIR);
        break;
    }
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

void
initGL() {

    // Declarations.
    CUdevice cudaDevice;
    // CUcontext cudaContext;
    int count, i, driverVersion;
    char deviceName[100];
    GLenum err;
    int fakeargc;
    char *fakeargv[2];

    // Display CUDA-capabile devices.
    cudaStatus = cudaGetDeviceCount(&count);
    if (cudaStatus != cudaSuccess)
        errorHandler(cudaStatus, "cudaGetDeviceCount() failed. Do you have a "
                                 "CUDA-capable GPU installed?",
                     cudaGetErrorString(cudaStatus));
    std::cout << count << " CUDA device" << (count == 1 ? "" : "s")
              << " available." << std::endl;
    for (i = 0; i < count; i += 1) {
        cudaResult = cuDeviceGetName(deviceName, 100, i);
        if (cudaResult != CUDA_SUCCESS)
            errorHandler(cudaResult, "cuDeviceGetName() failed.",
                         cuGetErrorString(cudaResult));
        std::cout << i << ": " << deviceName << std::endl;
    }

    // Set CUDA device.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
        errorHandler(cudaStatus, "cudaSetDevice() failed.",
                     cudaGetErrorString(cudaStatus));

    // Display CUDA driver version.
    cudaResult = cuDeviceGet(&cudaDevice, 0);
    if (cudaResult != CUDA_SUCCESS)
        errorHandler(cudaResult, "cuDeviceGet() failed.",
                     cuGetErrorString(cudaResult));

    // Initialize CUDA driver API.
    cudaResult = cuInit(0);
    if (cudaResult != CUDA_SUCCESS)
        errorHandler(cudaResult, "cuInit() failed.",
                     cuGetErrorString(cudaResult));

    // Display CUDA driver version.
    cudaResult = cuDriverGetVersion(&driverVersion);
    if (cudaResult != CUDA_SUCCESS)
        errorHandler(cudaResult, "cuDriverGetVersion() failed.",
                     cuGetErrorString(cudaResult));
    std::cout << "CUDA Version: " << driverVersion << std::endl;

    // GLUT initializations.
    fakeargc = 1;
    fakeargv[0] = "full";
    fakeargv[1] = NULL;
    glutInit(&fakeargc, fakeargv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowPosition(WINDOW_STARTING_X, WINDOW_STARTING_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Window creation.
    windowHandle = glutCreateWindow("Rapunzel");  // Main window.
    if (windowHandle != 1)
        errorHandler(NULL, "glutCreateWindow() failed.", "");

    // Menu.
    glutCreateMenu(menu);
    glutAddMenuEntry("Black", MENU_BLACK);
    glutAddMenuEntry("Brown", MENU_BROWN);
    glutAddMenuEntry("Dark Brown", MENU_DARK_BROWN);
    glutAddMenuEntry("Amber", MENU_AMBER);
    glutAddMenuEntry("Blonde", MENU_BLONDE);
    glutAddMenuEntry("Red", MENU_RED);
    glutAddMenuEntry("Auburn", MENU_AUBURN);
    glutAttachMenu(GLUT_LEFT_BUTTON);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // GLEW Initialization.
    // glewExperimental = true;
    // err = glewInit();
    // if( err != GLEW_OK )
    //     errorHandler( err, "glewInit() failed.", std::string( (char
    //     *)glewGetErrorString( err ) ) );
    // if( !glewIsSupported( "GL_VERSION_2_0 " ) )
    //     errorHandler( NULL, "Support for necessary OpenGL extensions
    //     missing.", "" );

    // Prepare CUDA device.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess)
        errorHandler(cudaStatus, "cudaDeviceReset() failed.",
                     cudaGetErrorString(cudaStatus));
    // Record the curent thread as CUDA-interoperable.
    cudaStatus = cudaGLSetGLDevice(0);
    if (cudaStatus != cudaSuccess)
        errorHandler(cudaStatus, "cudaGLSetGLDevice() failed.",
                     cudaGetErrorString(cudaStatus));

    // Callback functions.
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(passiveMotion);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);

    // Callnever functions.
    glutButtonBoxFunc(NULL);
    glutDialsFunc(NULL);
    glutEntryFunc(NULL);
    glutMenuStateFunc(NULL);
    glutMenuStatusFunc(NULL);
    glutMotionFunc(NULL);
    glutMouseFunc(NULL);
    glutOverlayDisplayFunc(NULL);
    glutSpaceballButtonFunc(NULL);
    glutSpaceballMotionFunc(NULL);
    glutSpaceballRotateFunc(NULL);
    glutTabletButtonFunc(NULL);
    glutTabletMotionFunc(NULL);
    glutVisibilityFunc(NULL);

    // When window is closed, glutMainLoop immediately returns.
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Displaying GL information.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language Version: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    // glGetString( GL_EXTENSIONS );

    /* Create CUDA context and associate it with current thread.
    cudaResult = cuGLCtxCreate( &cudaContext, CU_CTX_LMEM_RESIZE_TO_MAX,
    cudaDevice );
    if( cudaResult != CUDA_SUCCESS )
        errorHandler( cudaResult, "cuGLCtxCreate() failed.", cuGetErrorString(
    cudaResult ) );
    //*/

    // OpenGL miscellaneous.
    glClearColor(GRAY_COLOR, 0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    glViewport((GLint)0, (GLint)0, (GLsizei)WINDOW_WIDTH,
               (GLsizei)WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW_ANGLE,
                   (GLdouble)WINDOW_WIDTH / (GLdouble)WINDOW_HEIGHT, Z_NEAR,
                   Z_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Floor.
    ground = new Floor();
    ground->translation = Vector3D(O);

    // Camera.
    cam = new Camera();
    cam->setPosition(Vector3D(32, 32, 256));
    cam->rotateFPS_OY((float)M_PI);

    // Mannequin.
    mannequin = new Mannequin(Human);
    focus = mannequin->centerOfGravity + Vector3D(32, 32, 32);

    // Lights.
    light = new Light();
    light->setLightType(Spot);
    light->translation = Vector3D(0.0f, 1.0f, 0.2f);
    light->render(light->translation, -light->translation);

    // Mouse cursor.
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

// Trivial initializations.
void
init() {

    // Declarations.
    // HANDLE hStdout;
    // BOOL rv;

    // Initializing vector arrays.
    Vector3D::arr = new float[3];
    Vector4D::arr = new float[4];

    // Console colors.
    // hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    // rv = SetConsoleTextAttribute( hStdout, FOREGROUND_RED | FOREGROUND_GREEN
    // | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
    // if( !rv )
    //    errorHandler( NULL, "ERROR: SetConsoleTextAttribute() failed.", NULL
    //    );

    // Counter initialization.
    startCounter(FRAME_TIME);

    // Window properties.
    currentX = WINDOW_WIDTH / 2;
    currentY = WINDOW_HEIGHT / 2;

    RUNS_ON = "CPU";
    hairColor = Vector3D(RED_HAIR);
    HAIR_OFF = HAIR_OFF4;
    MEASUREMENT_STEP = 1;
    measurementStep = "1";
    textShouldBeDrawn = true;

    startCounter(INTER_SIMULATION_TIME);

    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        errorHandler(cudaStatus, "main()/cudaDeviceSynchronize() failed.",
                     cudaGetErrorString(cudaStatus));
    }
}

int
main(int argc, char **argv) {
    init();
    initGL();
    initScene();
    glutMainLoop();
    return EXIT_SUCCESS;
}
