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
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;
    
    Image img_pencil;
    img_pencil.LoadPNG("images/pencil.png");
    
    Image img_blau_clar;
    img_blau_clar.LoadPNG("images/cyan.png");
    
    Image img_blau_fosc;
    img_blau_fosc.LoadPNG("images/blue.png");
    
    Image img_vermell;
    img_vermell.LoadPNG("images/red.png");
    
    Image img_groc;
    img_groc.LoadPNG("images/yellow.png");
    
    Image img_rosa;
    img_rosa.LoadPNG("images/pink.png");
    
    Image img_blanc;
    img_blanc.LoadPNG("images/white.png");
    
    Image img_negre;
    img_negre.LoadPNG("images/black.png");
    
    Image img_green;
    img_green.LoadPNG("images/green.png");
    
    Image img_eraser;
    img_eraser.LoadPNG("images/eraser.png");
    
    Image img_triangle;
    img_triangle.LoadPNG("images/triangle.png");

    Image img_line;
    img_line.LoadPNG("images/line.png");
    
    Image img_rectangle;
    img_rectangle.LoadPNG("images/rectangle.png");
    
    Image img_clear;
    img_clear.LoadPNG("images/clear.png");
    
    Image img_load;
    img_load.LoadPNG("images/load.png");
    
    Image img_save;
    img_save.LoadPNG("images/save.png");
    
    int x_offset = 10; //marge esquerre de 10 pixels per posar la primera imatge
    int y_toolbar = 5; //marge inferior de 5 pixels (distancia de les imatges al "terra")
    int spacing = 50; //separació entre imatges

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
    
    current_color = Color::WHITE;
    figura = 0;
    pencil_active = true;
    eraser_active = false;
    paint_active = true;
    animation_active = false;
    
    ps.Init();

}

// Render one frame
void Application::Render(void)
{
    if(animation_active){
        framebuffer.Fill(Color::BLACK);
        ps.Render(&framebuffer);
    }
    else if(paint_active){
        framebuffer.DrawRect(0, 0, framebuffer.width, 50, Color::GRAY, 0, true, Color::GRAY); //des d'abaix a la esquerra, 50 pixels d'alçada per la toolbar
        
        for (Button& b : buttons)
        {
            framebuffer.DrawImage(b.image, b.position.x, b.position.y);
        }
    }
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    ps.Update(seconds_elapsed);
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
        case SDLK_1:
            paint_active = true;
            animation_active = false;
            framebuffer.Fill(Color::BLACK);
            break;
        case SDLK_2:
            paint_active = false;
            animation_active = true;
            break;
        case SDLK_f:
            fill_pressed = true;
            break;
        case SDLK_PLUS:
            current_border++;
            break;
        case SDLK_MINUS:
            current_border--;
            break;
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state |= SDL_BUTTON(SDL_BUTTON_LEFT); //canviem l'estat del ratolí a button left presionat amb una operació OR

        mouse_start.x = event.x;
        mouse_start.y = framebuffer.height - event.y;
        
        is_clicking_toolbar = (event.y > (framebuffer.height - 50)); //si el ratolí està per sobre de "550", estara tocant la barra
        
        for(Button& b : buttons)
        {
            if(b.IsMouseInside(mouse_start))
            {
                HandleButton(b.type);
                return;
            }
        }
    }
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {
        mouse_state &= ~SDL_BUTTON(SDL_BUTTON_LEFT);
        
        if (pencil_active || eraser_active || is_clicking_toolbar) { //per evitar que faci figures quan s'esta utilitzant el llapis o la goma o quan es clica sobre la toolbar
            return; //no fa res
        }
        
        float y_invertida = framebuffer.height - event.y; //y del ratolí/Xcode -> y de les imatges
        
        if(figura == 0){
            framebuffer.DrawLineDDA(mouse_start.x, mouse_start.y, event.x, y_invertida, current_color);
        }
        else if(figura == 1){
            int x_min = std::min(mouse_start.x, (float)event.x); //agafen la esquina inferior esquerra sigui quina sigui la direcció en que fas el rectangle
            int y_min = std::min(mouse_start.y, y_invertida);
            int width = std::abs((float)event.x - mouse_start.x);
            int height = std::abs(y_invertida - mouse_start.y);
            
            if(current_border < 1){
                current_border = 1;
            }
            
            if(!fill_pressed){
                framebuffer.DrawRect(x_min, y_min, width, height, current_color, current_border, false, current_color);
            }
            else{
                framebuffer.DrawRect(x_min, y_min, width, height, current_color, current_border, true, current_color);
            }
        }
        else if(figura == 2){
            //triangle isòsceles rectangle
            Vector2 p0 = mouse_start;
            Vector2 p1 = Vector2(event.x, y_invertida);
            Vector2 p2 = Vector2(mouse_start.x, y_invertida);
            
            if(!fill_pressed){
                framebuffer.DrawTriangle(p0, p1, p2, current_color, false, current_color);
            }
            else{
                framebuffer.DrawTriangle(p0, p1, p2, current_color, true, current_color);
            }
        }
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    mouse_delta = mouse_position; //guardem la posició anterior abans d'actualitzar-la
    
    mouse_position.x = event.x;
    mouse_position.y = framebuffer.height - event.y; //totes les posicions del ratolí s'han d'invertir perque van al revés que les imatges
    
    if(mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT) && !is_clicking_toolbar){
        if(pencil_active){
            framebuffer.DrawLineDDA(mouse_delta.x, mouse_delta.y, mouse_position.x, mouse_position.y, current_color); //per que els pixels a pintar quedin més units, fem linies
        }
        else if(eraser_active){
            framebuffer.DrawLineDDA(mouse_delta.x, mouse_delta.y, mouse_position.x, mouse_position.y, Color::BLACK);
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
            current_color = Color::PURPLE;
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
            if(loaded.LoadPNG("images/fruits.png")){
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




