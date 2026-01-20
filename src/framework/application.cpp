#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"

Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    this->framebuffer.Resize(w, h);
}

Application::~Application()
{
    //fer
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    
    Image img_pencil;
    img_pencil.LoadPNG("/Users/dunafebrevila/res/images/pencil.png");
    
    Image img_blau_clar;
    img_blau_clar.LoadPNG("res/images/cyan.png");
    
    Image img_blau_fosc;
    img_blau_fosc.LoadPNG("res/images/blue.png");
    
    Image img_vermell;
    img_vermell.LoadPNG("res/images/red.png");
    
    Image img_groc;
    img_groc.LoadPNG("res/images/yellow.png");
    
    Image img_rosa;
    img_rosa.LoadPNG("res/images/pink.png");
    
    Image img_blanc;
    img_blanc.LoadPNG("res/images/white.png");
    
    Image img_negre;
    img_negre.LoadPNG("res/images/black.png");
    
    Image img_green;
    img_green.LoadPNG("res/images/green.png");
    
    Image img_eraser;
    img_eraser.LoadPNG("res/images/eraser.png");
    
    Image img_triangle;
    img_triangle.LoadPNG("res/images/triangle.png");

    Image img_line;
    img_line.LoadPNG("res/images/line.png");
    
    Image img_rectangle;
    img_rectangle.LoadPNG("res/images/rectangle.png");
    
    Image img_clear;
    img_clear.LoadPNG("res/images/clear.png");
    
    Image img_load;
    img_load.LoadPNG("res/images/load.png");
    
    Image img_save;
    img_save.LoadPNG("res/images/save.png");
    
    int x_offset = 10;
    int y_toolbar = 0;
    int spacing = 50;

    buttons.push_back(Button(img_pencil, Vector2(x_offset, y_toolbar), ButtonType::PENCIL));
    x_offset += spacing;
    
    buttons.push_back(Button(img_blau_clar, Vector2(x_offset, y_toolbar), ButtonType::BLAU_CLAR));
    x_offset += spacing;
    
    buttons.push_back(Button(img_blau_fosc, Vector2(x_offset, y_toolbar), ButtonType::BLAU_FOSC));
    x_offset += spacing;
    
    buttons.push_back(Button(img_vermell, Vector2(x_offset, y_toolbar), ButtonType::VERMELL));
    x_offset += spacing;
    
    buttons.push_back(Button(img_groc, Vector2(x_offset, y_toolbar), ButtonType::GROC));
    x_offset += spacing;
    
    buttons.push_back(Button(img_rosa, Vector2(x_offset, y_toolbar), ButtonType::ROSA));
    x_offset += spacing;
    
    buttons.push_back(Button(img_blanc, Vector2(x_offset, y_toolbar), ButtonType::BLANC));
    x_offset += spacing;
    
    buttons.push_back(Button(img_negre, Vector2(x_offset, y_toolbar), ButtonType::NEGRE));
    x_offset += spacing;
    
    buttons.push_back(Button(img_green, Vector2(x_offset, y_toolbar), ButtonType::VERD));
    x_offset += spacing;

    buttons.push_back(Button(img_line, Vector2(x_offset, y_toolbar), ButtonType::LINE));
    x_offset += spacing;

    buttons.push_back(Button(img_rectangle, Vector2(x_offset, y_toolbar), ButtonType::RECTANGLE));
    x_offset += spacing;

    buttons.push_back(Button(img_triangle, Vector2(x_offset, y_toolbar), ButtonType::TRIANGLE));
    x_offset += spacing;

    buttons.push_back(Button(img_eraser, Vector2(x_offset, y_toolbar), ButtonType::ERASER));
    x_offset += spacing;

    buttons.push_back(Button(img_clear, Vector2(x_offset, y_toolbar), ButtonType::CLEAR_IMAGE));
    x_offset += spacing;

    buttons.push_back(Button(img_load, Vector2(x_offset, y_toolbar), ButtonType::LOAD_IMAGE));
    x_offset += spacing;

    buttons.push_back(Button(img_save, Vector2(x_offset, y_toolbar), ButtonType::SAVE_IMAGE));
    x_offset += spacing;

}

// Render one frame
void Application::Render(void)
{
    //framebuffer.Fill(Color::BLACK);
    //framebuffer.DrawLineDDA(200, 300, 200 + 100 * cos(time), 300 + 100 * sin(time), Color::RED);
    //framebuffer.DrawRect(600, 300, 250, 350, Color::RED, 10, true, Color::WHITE);
    //framebuffer.DrawTriangle(Vector2(250, 250), Vector2(350, 450), Vector2(200, 500), Color::WHITE, true,Color::RED);
    for (Button& b : buttons)
    {
        framebuffer.DrawImage(b.image, b.position.x, framebuffer.height - b.position.y);
    }
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state |= SDL_BUTTON(SDL_BUTTON_LEFT); //canviem l'estat del ratolí a button left presionat amb una operació OR

        mouse_start.x = event.x;
        mouse_start.y = event.y;
        
        for(Button& b : buttons)
        {
            if(b.IsMouseInside(mouse_start))
            {
                HandleButton(b.type);
                break; // solo el primero que coincide
            }
        }
    }
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state &= ~SDL_BUTTON(SDL_BUTTON_LEFT);
        
        if(figura == 0){
            framebuffer.DrawLineDDA(mouse_start.x, mouse_start.y, event.x, event.y, current_color);
        }
        else if(figura == 1){
            int w = event.x - mouse_start.x;
            int h = event.y - mouse_start.y;
            framebuffer.DrawRect(mouse_start.x, mouse_start.y, abs(w), abs(h), current_color, 1, true, current_color);
        }
        else if(figura == 2){
            //triangle isòsceles rectangle
            Vector2 p0 = mouse_start;
            Vector2 p1 = Vector2(event.x, event.y);
            Vector2 p2 = Vector2(mouse_start.x, event.y);

            framebuffer.DrawTriangle(p0, p1, p2, Color::GREEN, true, current_color);
        }
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    mouse_position.x = event.x;
    mouse_position.y = event.y;
    
    if(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
        if(pencil_active){
            framebuffer.SetPixel((int)mouse_position.x, (int)mouse_position.y, current_color);
        }
        else if(eraser_active){
            framebuffer.SetPixel((int)mouse_position.x, (int)mouse_position.y, Color::BLACK); // o Color::WHITE
        }
    }

}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float dy = event.preciseY;

    // ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}

void Application::HandleButton(ButtonType type)
{
    switch(type)
    {
        case ButtonType::PENCIL:
            pencil_active = true;
            eraser_active = false;
            break;
        case ButtonType::LINE:
            pencil_active = false;
            eraser_active = false;
            figura = 0; // línea
            break;
        case ButtonType::RECTANGLE:
            pencil_active = false;
            eraser_active = false;
            figura = 1; // rectangle
            break;
        case ButtonType::TRIANGLE:
            pencil_active = false;
            eraser_active = false;
            figura = 2; // triangle
            break;
        case ButtonType::ERASER:
            pencil_active = false;
            eraser_active = true;
            break;
        case ButtonType::BLAU_CLAR:
            current_color = Color::CYAN;
            break;
        case ButtonType::BLAU_FOSC:
            current_color = Color::BLUE;
            break;
        case ButtonType::VERMELL:
            current_color = Color::RED;
            break;
        case ButtonType::GROC:
            current_color = Color::YELLOW;
            break;
        case ButtonType::ROSA:
            current_color = Color::PINK;
            break;
        case ButtonType::BLANC:
            current_color = Color::WHITE;
            break;
        case ButtonType::NEGRE:
            current_color = Color::BLACK;
            break;
        case ButtonType::VERD:
            current_color = Color::GREEN;
            break;
        case ButtonType::CLEAR_IMAGE:
            framebuffer.Fill(Color::BLACK);
            break;
        case ButtonType::LOAD_IMAGE:
        {
            Image loaded;
            if(loaded.LoadPNG("res/images/test.png")){  // mirar path
                framebuffer = loaded;
            }
        }
            break;
        case ButtonType::SAVE_IMAGE:
            framebuffer.SaveTGA("paint_output.tga");
            break;
        default:
            break;
    }
}




