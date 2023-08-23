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
#define D2R 0.01745

SDL_Window *window;
SDL_Renderer *renderer;
bool running = true;
int lastFrame = 0;
float deltaFrame = (1/(float)FPS);

Vector2 pPos = Vector2(200,200);
float pAngle = 0;
float pSpeed = 150;
float pAV = 3;
int pPOV = 60;
Vector2 pDMove = Vector2(0,0);
bool left,right,up,down;

const int mapWidth = 8, mapHeight = 8;
int worldMap[mapWidth*mapHeight]=
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,1,0,1,1,
    1,0,0,0,1,0,0,1,
    1,0,0,0,1,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1
};

const int textureSize = 32;
int allTextures[]=
{
    // test wall
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
    1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
};

const int mapBlockSize = 64;
void drawMap2D(){
    for(int i=0;i<mapHeight*mapWidth;i++){
        if(worldMap[i] == 0) {
            SDL_Rect rec;
            rec.w = mapBlockSize-1;
            rec.h = mapBlockSize-1;
            rec.x = mapBlockSize*(i%mapWidth);
            rec.y = mapBlockSize*(i/mapWidth);
            SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
            SDL_RenderFillRect(renderer, &rec);
        }
        if(worldMap[i] == 1){
            SDL_Rect rec;
            rec.w = mapBlockSize-1;
            rec.h = mapBlockSize-1;
            rec.x = mapBlockSize*(i%mapWidth);
            rec.y = mapBlockSize*(i/mapWidth);
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
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
    SDL_SetRenderDrawColor(renderer, 220,220,0,255);
    SDL_RenderFillRect(renderer, &rec);// draw player
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(renderer,pPos.x,pPos.y,pPos.x+pDMove.x*10,pPos.y+pDMove.y*10);// draw dir
}

void drawRay3D(){
    float rAngle = pAngle - (pPOV/2)*D2R;if(rAngle < 0) rAngle += 2*PI;if(rAngle > 2*PI) rAngle -= 2*PI;
    int end = 8;
    float dist = 0;
    for(int i=0;i<pPOV;i++){
        float rx=0,ry=0,xo=0,yo=0;
        int dof=0,mx,my;
        //---Check Horizontal Lines---
        float disH = 100000;
        Vector2 hVec = pPos;
        //Looking up
        if(rAngle > PI) {ry = (((int)pPos.y>>6)<<6) - 0.0001;rx = pPos.x - (pPos.y - ry)/tan(rAngle);yo = -64;xo = -64/tan(rAngle);} 
        //Looking down
        if(rAngle < PI) {ry = (((int)pPos.y>>6)<<6) + 64.0001;rx = pPos.x - (pPos.y - ry)/tan(rAngle);yo = 64;xo = 64/tan(rAngle);} 
        //Looking straight left or right
        if(rAngle <= 0.0001 || rAngle == PI || rAngle == PI*2) {ry = pPos.y;rx = pPos.x;dof = end;}
        while(dof < end){
            mx = ((int)rx>>6);
            my = ((int)ry>>6);
            int tmp = my*mapWidth+mx;
            if(tmp >= 0 && tmp <= mapHeight*mapWidth && worldMap[tmp] == 1) {dof = end;hVec=Vector2(rx,ry);disH=Vector2::getDist(pPos,hVec);}
            else {rx += xo;ry += yo;}
            dof++;
        }

        //---Check Vertical Lines---
        dof=0;
        float disV = 100000;
        Vector2 vVec = pPos;
        //Looking left
        if(rAngle > PI2 && rAngle < PI3) {rx = (((int)pPos.x>>6)<<6) - 0.0001;ry = pPos.y - (pPos.x - rx)*tan(rAngle);xo = -64;yo = -64*tan(rAngle);} 
        //Looking right
        if(rAngle < PI2 || rAngle > PI3) {rx = (((int)pPos.x>>6)<<6) + 64.0001;ry = pPos.y - (pPos.x - rx)*tan(rAngle);xo = 64;yo = 64*tan(rAngle);} 
        //Looking straight up or down
        if(rAngle == PI2 || rAngle == PI3) {ry = pPos.y;rx = pPos.x;dof = end;}
        while(dof < end){
            mx = ((int)rx>>6);
            my = ((int)ry>>6);
            int tmp = my*mapWidth+mx;
            if(tmp >= 0 && tmp <= mapHeight*mapWidth && worldMap[tmp] == 1) {dof = end;vVec=Vector2(rx,ry);disV=Vector2::getDist(pPos,vVec);}
            else {rx += xo;ry += yo;}
            dof++;
        }
        //--Draw 2D ray-- 
        SDL_SetRenderDrawColor(renderer,0,255,0,255);
        float shade = 1;
        if(disH < disV) {dist = disH;rx = hVec.x;ry = hVec.y;}
        if(disH >= disV) {dist = disV;rx = vVec.x;ry = vVec.y;shade=0.8;}
        SDL_RenderDrawLine(renderer, pPos.x, pPos.y, rx, ry);
        //--Draw 3D wall--
        float da = pAngle - rAngle; if(da < 0) {da+=2*PI;} if(da > 2*PI) {da-=2*PI;} dist = dist*cos(da); //fix fish eye
        float lineH = (mapBlockSize*screenHeight/dist); //get vertical line height
        float tyStep = textureSize / lineH;
        float tyOffset = 0;
        if(lineH > screenHeight) {tyOffset = (lineH-screenHeight)/2;lineH = screenHeight;} //clip line height if it's too high
        float lineO = ((screenHeight) - lineH)/2; //get offset of line

        float ty = tyStep*tyOffset;
        float tx = (int)(rx*((float)textureSize/mapBlockSize))%textureSize;if(rAngle < PI) tx = (textureSize-1) - tx;
        if(shade != 1){
            tx = (int)(ry*((float)textureSize/mapBlockSize))%textureSize;if(rAngle > PI2 && rAngle < PI3) tx = (textureSize-1) - tx;
        }
        for(int j=0;j<lineH;j++){
            int c = allTextures[(int)ty*textureSize+(int)tx] * 255 * shade;
            SDL_SetRenderDrawColor(renderer,c,c,c,255);
            SDL_Rect rec;
            rec.w = (screenWidth/2)/pPOV;
            rec.h = 1;
            rec.x = (screenWidth/2)+i*rec.w;
            rec.y = lineO+j;
            SDL_RenderFillRect(renderer, &rec);
            ty += tyStep;
        }

        //--Update ray angle
        rAngle += D2R;if(rAngle > 2*PI) rAngle -= 2*PI;
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
    if(left) {pAngle -= pAV*deltaFrame; if(pAngle < 0) {pAngle = 2*PI;}}
    if(right) {pAngle += pAV*deltaFrame; if(pAngle > 2*PI) {pAngle = 0;}}
    int pDX, pDY;
    pDMove.x = pSpeed * cos(pAngle)*deltaFrame;if(pDMove.x > 0) pDX = pDMove.x + 5;else pDX = pDMove.x - 5;
    pDMove.y = pSpeed * sin(pAngle)*deltaFrame;if(pDMove.y > 0) pDY = pDMove.y + 5;else pDY = pDMove.y - 5;
    if(up) {
        int tmp;
        tmp = (pPos.x+pDX)/64 + mapWidth*(int)(pPos.y/64);
        if(worldMap[tmp] == 0) pPos.x += pDMove.x;
        tmp = pPos.x/64 + mapWidth*(int)((pPos.y+pDY)/64);
        if(worldMap[tmp] == 0) pPos.y += pDMove.y;
    }
    if(down) {
        int tmp;
        tmp = (pPos.x-pDX)/64 + mapWidth*(int)(pPos.y/64);
        if(worldMap[tmp] == 0) pPos.x -= pDMove.x;
        tmp = pPos.x/64 + mapWidth*(int)((pPos.y-pDY)/64);
        if(worldMap[tmp] == 0) pPos.y -= pDMove.y;
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderClear(renderer);

    drawMap2D();
    drawRay3D();
    drawPlayer2D();

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