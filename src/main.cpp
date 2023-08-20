#include <SDL2/SDL.h>
#include "headers/Vector2.h"
#include <stdio.h>
#include <math.h>

#define screenWidth 1024
#define screenHeight 512
#define FPS 60
#define PI 3.141159
#define PI2 PI/2
#define PI3 3*PI/2

SDL_Window *window;
SDL_Renderer *renderer;
bool running = true;
int lastFrame = 0;

Vector2 pPos = Vector2(200,200);
float pAngle = 0;
float pSpeed = 3;
float pAV = 0.07;
Vector2 pDMove = Vector2(0,0);
bool left,right,up,down;

const int mapWidth = 8, mapHeight = 8;
int worldMap[mapWidth*mapHeight]=
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,1,1,1,1,
    1,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};

int mapBlockWidth = 64, mapBlockHeight  = 64;
void drawMap2D(){
    for(int i=0;i<mapHeight*mapWidth;i++){
        if(worldMap[i] == 0) {
            SDL_Rect rec;
            rec.w = mapBlockWidth-1;
            rec.h = mapBlockHeight-1;
            rec.x = mapBlockWidth*(i%mapWidth);
            rec.y = mapBlockHeight*(i/mapWidth);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rec);
        }
        if(worldMap[i] == 1){
            SDL_Rect rec;
            rec.w = mapBlockWidth-1;
            rec.h = mapBlockHeight-1;
            rec.x = mapBlockWidth*(i%mapWidth);
            rec.y = mapBlockHeight*(i/mapWidth);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rec);
        }
    }
}

int pWidth = 8, pHeight = 8;
void drawPlayer2D(){
    SDL_Rect rec;
    rec.w = pWidth;
    rec.h = pHeight;
    rec.x = pPos.x - pWidth/2;
    rec.y = pPos.y - pHeight/2;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rec);// draw player
}

void drawRays2D(){
    float rAngle = pAngle;
    float rx=0,ry=0,xo=0,yo=0;
    int dof=0,end=mapWidth,mx,my;
    for(int i=0;i<1;i++){
        //---Check Horizontal Lines---
        float disH = 100000;
        Vector2 hVec = pPos;
        //---Looking up---
        if(rAngle > PI) {ry = (((int)pPos.y>>6)<<6) - 0.0001;rx = pPos.x - (pPos.y - ry)/tan(pAngle);yo = -64;xo = -64/tan(pAngle);} 
        //---Looking down---
        if(rAngle < PI) {ry = (((int)pPos.y>>6)<<6) + 64.0001;rx = pPos.x - (pPos.y - ry)/tan(pAngle);yo = 64;xo = 64/tan(pAngle);} 
        //---Looking straight left or right---
        if(rAngle <= 0.0001 || rAngle == PI || rAngle == PI*2) {ry = pPos.y;rx = pPos.x;dof = end;}
        while(dof < end){
            mx = ((int)rx>>6);
            my = ((int)ry>>6);
            int tmp = my*mapWidth+mx;
            if(tmp <= mapHeight*mapWidth && worldMap[tmp] == 1) {dof = end;hVec=Vector2(rx,ry);disH=Vector2::getDist(pPos,hVec);}
            else {rx += xo;ry += yo;}
            dof++;
        }

        //---Check Vertical Lines---
        dof=0;
        float disV = 100000;
        Vector2 vVec = pPos;
        //---Looking left---
        if(rAngle > PI2 && rAngle < PI3) {rx = (((int)pPos.x>>6)<<6) - 0.0001;ry = pPos.y - (pPos.x - rx)*tan(pAngle);xo = -64;yo = -64*tan(pAngle);} 
        //---Looking right---
        if(rAngle < PI2 || rAngle > PI3) {rx = (((int)pPos.x>>6)<<6) + 64.0001;ry = pPos.y - (pPos.x - rx)*tan(pAngle);xo = 64;yo = 64*tan(pAngle);} 
        //---Looking straight up or down---
        if(rAngle == PI2|| rAngle == PI3) {ry = pPos.y;rx = pPos.x;dof = end;}
        while(dof < end){
            mx = ((int)rx>>6);
            my = ((int)ry>>6);
            int tmp = my*mapWidth+mx;
            if(tmp <= mapHeight*mapWidth && worldMap[tmp] == 1) {dof = end;vVec=Vector2(rx,ry);disV=Vector2::getDist(pPos,vVec);}
            else {rx += xo;ry += yo;}
            dof++;
        }
        // draw ray
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if(disH < disV) SDL_RenderDrawLine(renderer, pPos.x, pPos.y, hVec.x, hVec.y);
        if(disH >= disV) SDL_RenderDrawLine(renderer, pPos.x, pPos.y, vVec.x, vVec.y);
    }

}


void handleEvent(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.scancode == SDL_SCANCODE_A) left = true;
                if(event.key.keysym.scancode == SDL_SCANCODE_D) right = true;
                if(event.key.keysym.scancode == SDL_SCANCODE_W) up = true;
                if(event.key.keysym.scancode == SDL_SCANCODE_S) down = true;
                break;
            case SDL_KEYUP:
                if(event.key.keysym.scancode == SDL_SCANCODE_A) left = false;
                if(event.key.keysym.scancode == SDL_SCANCODE_D) right = false;
                if(event.key.keysym.scancode == SDL_SCANCODE_W) up = false;
                if(event.key.keysym.scancode == SDL_SCANCODE_S) down = false;
                break;
            default:
                break;
        }
    }
}

void update(){
    if(left) {pAngle -= pAV; if(pAngle < 0) pAngle = 2*PI;}
    if(right) {pAngle += pAV; if(pAngle > 2*PI) pAngle = 0;}
    pDMove.x = pSpeed * cos(pAngle); pDMove.y = pSpeed * sin(pAngle);
    if(up) {pPos = pPos + pDMove;}
    if(down) {pPos = pPos - pDMove;}
}

void render(){
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);

    drawMap2D();
    drawPlayer2D();
    drawRays2D();

    SDL_RenderPresent(renderer);
}

/*  Main  */

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }
    
    while(running){
        handleEvent();
        if(SDL_GetTicks() - lastFrame > 1000/FPS){
            lastFrame = SDL_GetTicks();
            update();
            render();
        }
    }

    return 0;
}