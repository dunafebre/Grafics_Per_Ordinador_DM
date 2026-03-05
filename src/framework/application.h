/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "entity.h"
#include "mesh.h"
#include "shader.h"

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;
    std::vector<Button> buttons;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
    Vector2 mouse_start; // Mouse position in the first click
    int figura; //0 = line; 1 = rectangle; 2 = triangle
    Color current_color;
    bool pencil_active = false;
    bool eraser_active = false;
    bool is_clicking_toolbar = false;
    bool paint_active = false;
    bool animation_active = false;
    bool fill_pressed = false;
    int current_border = 1;
    ParticleSystem ps;
    
    //lab02
    Entity* entity1;
    Entity* entity2;
    Entity* entity3;
    Camera* camera;
    CameraProperty current_property;
    int scene_mode = 1;
    
    //lab03
    bool useTexture = true;
    bool useZBuffer = true;
    bool useInterpolatedUV = true;
    eRenderMode mode = eRenderMode::TRIANGLES_INTERPOLATED;
    
    //lab04
    Mesh* mesh;
    Shader* shader;
    int currentTask = 1;
    char currentSubtask = 'a';
    Texture* myTexture;
    int currentLab = 4;

    //lab05
    sUniformData uniformData;
    Vector3 ka = Vector3(0.1f, 0.1f, 0.1f);
    Vector3 kd = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 ks = Vector3(1.0f, 1.0f, 1.0f);
    float shininess = 32.0f;
    
	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);
    void HandleButton(ButtonType type);

	// CPU Global framebuffer
	Image framebuffer;
    
    // Z-buffer
    FloatImage zBuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
        this->zBuffer.Resize(width, height);
        
        if (camera)
        {
            camera->SetAspectRatio(width / (float)height);
            camera->UpdateProjectionMatrix();
            camera->UpdateViewProjectionMatrix();
        }
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};
