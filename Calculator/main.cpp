//
//  main.cpp
//  Calculator
//
//  Created by Fev1L on 28.10.2025.
//

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct Button {
    SDL_FRect rect;
    SDL_Color color;
    const char* label;
};

struct Text {
    SDL_FRect rect;
    SDL_Color color;
    const char* label;
    string textIn;
};

void drawButton(SDL_Renderer* renderer, const Button& button) {
    SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, 255);
    SDL_RenderFillRect(renderer, &button.rect);
}

string currentInput; // те, що зараз вводиться
double firstNumber = 0;
double secondNumber = 0;
string op;


void handleButtonClick(const string& label , double* sum) {
    if (isdigit(label[0])) {
        currentInput += label;
        *sum = stod(currentInput);
    }else if (label == "AC") {
        currentInput = "";
        firstNumber = 0;
        secondNumber = 0;
        op = "";
        *sum = 0;
    }else if (label == "DEL") {
        if (!currentInput.empty()) {
            currentInput.pop_back();
            if (!currentInput.empty())
                *sum = stod(currentInput);
            else
                *sum = 0;
        }
    }else if (label == "Arithmetic") {
        if (!currentInput.empty()) {
            *sum *= -1;
            currentInput = to_string(*sum);
        }
    }else if (label == "Comma") {
        if (currentInput.find('.') == std::string::npos) {
            if (currentInput.empty()) currentInput = "0";
            currentInput += ".";
            *sum = stod(currentInput);
        }
    }else if (label == "Plus" || label == "Minus" || label == "Multiplication" || label == "Divide" || label == "Module") {
        if(!currentInput.empty()){
            firstNumber = stod(currentInput);
            op = label;
            currentInput = "";
        }
    }else if (label == "Equals") {
        secondNumber = stod(currentInput);
        if(op == "Plus"){
            *sum = firstNumber + secondNumber;
        }else if(op == "Minus"){
            *sum = firstNumber - secondNumber;
        }else if(op == "Multiplication"){
            *sum = firstNumber * secondNumber;
        }else if(op == "Divide"){
            *sum = firstNumber / secondNumber;
        }else if(op == "Module"){
            *sum = (int)firstNumber % (int)secondNumber;
        }
        currentInput = to_string(*sum);
        firstNumber = *sum;
        secondNumber = 0;
        op = "";
    }
}

bool isButtonClicked(const Button& button, float x, float y) {
    return (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
            y >= button.rect.y && y <= button.rect.y + button.rect.h);
}

void drawText(SDL_Renderer* renderer,TTF_Font* font, const Text& text){
    float textW, textH;
    SDL_Color color = {text.color.r, text.color.g, text.color.b, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.textIn.c_str() , 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    SDL_GetTextureSize(texture, &textW, &textH);
    SDL_FRect dst = { text.rect.x, text.rect.y, textW, textH };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);
}

string formatDouble(double value) {
    ostringstream ss;
    ss << fixed << setprecision(10);
    ss << value;
    string result = ss.str();

    result.erase(result.find_last_not_of('0') + 1, string::npos);

    if (!result.empty() && result.back() == '.') {
        result.pop_back();
    }

    return result;
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Failure!");
        return -1;
    }
    
    if (!TTF_Init()) {
        SDL_Log("TTF_Init Failure!");
        SDL_Quit();
        return 1;
    }
    SDL_Window * window;
    window = SDL_CreateWindow("Calculator by Fev1L v.1", 390, 600, SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    
    SDL_Surface* icon = SDL_LoadBMP("Assets/icon.bmp");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_DestroySurface(icon);
    } else {
        SDL_Log("Icon not found: %s", SDL_GetError());
    }

    
    TTF_Font* font = TTF_OpenFont("Assets/Roboto-Regular.ttf", 48);
    if (!font) {
        SDL_Log("FONT Failure!");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    vector<Button> calculatorTableButton = {
        {{10 ,125 , 85, 85}, {166, 166, 166, 255}, "DEL"},
        {{105 ,125 , 85, 85}, {166, 166, 166, 255}, "AC"},
        {{200 ,125 , 85, 85}, {166, 166, 166, 255}, "Module"},
        {{295 ,125 , 85, 85}, {240, 163, 38, 255}, "Divide"},
        {{10 ,220 , 85, 85}, {58, 58, 58, 255}, "1"},
        {{105 ,220 , 85, 85}, {58, 58, 58, 255}, "2"},
        {{200 ,220 , 85, 85}, {58, 58, 58, 255}, "3"},
        {{295 ,220 , 85, 85}, {240, 163, 38, 255}, "Multiplication"},
        {{10 ,315 , 85, 85}, {58, 58, 58, 255}, "4"},
        {{105 ,315 , 85, 85}, {58, 58, 58, 255}, "5"},
        {{200 ,315 , 85, 85}, {58, 58, 58, 255}, "6"},
        {{295 ,315 , 85, 85}, {240, 163, 38, 255}, "Minus"},
        {{10 ,410 , 85, 85}, {58, 58, 58, 255}, "7"},
        {{105 ,410 , 85, 85}, {58, 58, 58, 255}, "8"},
        {{200 ,410 , 85, 85}, {58, 58, 58, 255}, "9"},
        {{295 ,410 , 85, 85}, {240, 163, 38, 255}, "Plus"},
        {{10 ,505 , 85, 85}, {58, 58, 58, 255}, "Arithmetic"},
        {{105 ,505 , 85, 85}, {58, 58, 58, 255}, "0"},
        {{200 ,505 , 85, 85}, {58, 58, 58, 255}, "Comma"},
        {{295 ,505 , 85, 85}, {240, 163, 38, 255}, "Equals"}
    };
    Text calculatorTableText[] = {
        {{15 ,139}, {225, 225, 225, 255}, "DEL", "Del"},
        {{115 ,139}, {225, 225, 225, 255}, "AC", "AC"},
        {{223 ,139}, {225, 225, 225, 255}, "Module", "%"},
        {{329 ,139}, {225, 225, 225, 255}, "Divide", "/"},
        {{41 ,234}, {225, 225, 225, 255}, "1", "1"},
        {{133 ,234}, {225, 225, 225, 255}, "2", "2"},
        {{228 ,234}, {225, 225, 225, 255}, "3", "3"},
        {{329 ,242}, {225, 225, 225, 255}, "Multiplication", "*"},
        {{41 ,329}, {225, 225, 225, 255}, "4", "4"},
        {{133 ,329}, {225, 225, 225, 255}, "5", "5"},
        {{228 ,329}, {225, 225, 225, 255}, "6", "6"},
        {{329 ,329}, {225, 225, 225, 255}, "Minus", "-"},
        {{41 ,424}, {225, 225, 225, 255}, "7", "7"},
        {{133 ,424}, {225, 225, 225, 255}, "8", "8"},
        {{228 ,424}, {225, 225, 225, 255}, "9", "9"},
        {{325 ,424}, {225, 225, 225, 255}, "Plus", "+"},
        {{25 ,519}, {225, 225, 225, 255}, "Arithmetic", "+/-"},
        {{133 ,519}, {225, 225, 225, 255}, "0", "0"},
        {{237 ,519}, {225, 225, 225, 255}, "Comma", ","},
        {{325 ,519}, {225, 225, 225, 255}, "Equals", "="}
        
    };
    double sum = 0;

    bool running = true;
    SDL_Event event;
    while (running) {
        Text enter = {{10 ,29}, {225, 225, 225, 255}, "Click", formatDouble(sum) };
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
            else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                for (auto& btn : calculatorTableButton) {
                    if (isButtonClicked(btn, mouseX, mouseY)) {
                        handleButtonClick(btn.label , &sum);
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        for(Button buttonElement : calculatorTableButton){
            drawButton(renderer, buttonElement);
        }
        for(Text textElement : calculatorTableText){
            drawText(renderer, font, textElement);
        }
        drawText(renderer, font, enter);
        SDL_RenderPresent(renderer);
        SDL_Delay(32);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
