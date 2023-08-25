#include <stdio.h>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <cstdlib>

#define PART_WIDTH 10
#define PART_HEIGHT 10
#define SHADOW_SIZE 5
#define WIDTH 1920
#define HEIGHT 1080
#define FRICTION 1.017f
#define GRAVITY 0.4f

struct Particle {
  float x;
  float y;
  float vx;
  float vy;

  float ry;
  float rz;
  float vry;
  float vrz;

  Color c;
  float sparkle;
};

float windMap[WIDTH][HEIGHT];

std::vector<Particle> parts;

int t=0;

Particle newPart() {
  int side=rand()%2;
  float angle=float(rand())/float(RAND_MAX);
  angle*=-PI/2-(float(side)*PI/2);
  float force=float(rand())/float(RAND_MAX);
  force*=40;
  force+=2;

  unsigned char r=rand()/(RAND_MAX/255);
  unsigned char g=rand()/(RAND_MAX/255);
  unsigned char b=rand()/(RAND_MAX/255);

  return Particle {
    WIDTH*float(side),
    HEIGHT-300,
    cosf(angle)*force,
    sinf(angle)*force,

    PI,0,
    float(rand())/(RAND_MAX),float(rand())/(RAND_MAX),

    Color{r,g,b,200},
    // float(GetRandomValue(0,100))/100,
    0,
  };
}

float easeIn(float x) {
  return pow(x,3);
}

int main() {
  printf("Hello World!\n");

  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  // SetTraceLogLevel(0);
  InitWindow(WIDTH, HEIGHT, "ribbit");
  SetWindowState(FLAG_WINDOW_UNDECORATED);
  // SetWindowState(FLAG_WINDOW_RESIZABLE);
  // SetWindowState(FLAG_WINDOW_MAXIMIZED);
  ToggleFullscreen();
  SetWindowState(FLAG_WINDOW_MOUSE_PASSTHROUGH);
  SetWindowState(FLAG_WINDOW_HIGHDPI);
  SetWindowState(FLAG_WINDOW_UNFOCUSED);
  SetWindowState(FLAG_WINDOW_TOPMOST);
  SetTargetFPS(60);
  // SetWindowPosition(0,0);

  for (int y=0;y<HEIGHT;y++) {
    for (int x=0;x<WIDTH;x++) {
      windMap[y][x]=float(rand())/float(RAND_MAX)*2.f-1;
    }
  }

  for (int i=0;i<200;i++) {
    parts.push_back(newPart());
  }

  while (!WindowShouldClose()) {
    float frameTime=GetFrameTime()/(1.f/60.f);
    // printf("%f\n",frameTime);
    if (t<10) {
      for (int i=0;i<80;i++) {
        parts.push_back(newPart());
      }
    }
    for (size_t i=0;i<parts.size();i++) {
      Particle p=parts.at(i);
      p.x+=p.vx/1.5;
      p.y+=p.vy/1.5;

      p.ry+=(p.vry*sqrt(p.vx*p.vx + p.vy*p.vy)/15)*frameTime / 5;
      p.rz+=p.vrz*frameTime;
      // p.rz=atan2(p.vy,p.vx);

      p.vy+=GRAVITY;
      p.vx/=FRICTION;
      p.vy/=FRICTION;
      if (p.x>=0 && p.x<WIDTH && p.y>=0 && p.y<HEIGHT) {
        float wind=windMap[int(p.y)][int(p.x)];
        p.vx+=wind*frameTime;
      }
      parts.at(i)=p;
      if (p.y>HEIGHT+10) {
        parts.erase(std::next(parts.begin(),i));
      }
    }
    if (parts.size()==0) {
      break;
    }

    BeginDrawing();

    ClearBackground(BLANK);
    //DrawText("yo soy raisin", 10, 10, 20, GRAY);
    //DrawText(TextFormat("%dx%d",GetScreenWidth(),GetScreenHeight()),10,55,20,WHITE);

    for (size_t i=0;i<parts.size();i++) {
      Particle p=parts.at(i);
      float sparkle=easeIn(p.sparkle * sin(p.ry));
      Color c={
        p.c.r + sparkle*(255-p.c.r),
        p.c.g + sparkle*(255-p.c.g),
        p.c.b + sparkle*(240-p.c.b),
        p.c.a
      };
      Color sparkleColor={
        sparkle*230,
        sparkle*230,
        sparkle*200,
        50
      };
      // DrawRectangle(p.x-SHADOW_SIZE,p.y-SHADOW_SIZE,PART_WIDTH+SHADOW_SIZE*2,PART_HEIGHT+SHADOW_SIZE*2,{0,0,0,16});
      // DrawRectangle(p.x,p.y,PART_WIDTH,PART_HEIGHT,p.c);
      DrawRectanglePro({p.x-4,p.y-4,abs(PART_WIDTH*sin(p.ry))+1+8,PART_HEIGHT+8},{PART_WIDTH/2,PART_HEIGHT/2},p.rz,sparkleColor);
      DrawRectanglePro({p.x-3,p.y-3,abs(PART_WIDTH*sin(p.ry))+1+6,PART_HEIGHT+6},{PART_WIDTH/2,PART_HEIGHT/2},p.rz,sparkleColor);
      DrawRectanglePro({p.x-2,p.y-2,abs(PART_WIDTH*sin(p.ry))+1+4,PART_HEIGHT+4},{PART_WIDTH/2,PART_HEIGHT/2},p.rz,sparkleColor);
      DrawRectanglePro({p.x,p.y,abs(PART_WIDTH*sin(p.ry))+1,PART_HEIGHT},{PART_WIDTH/2,PART_HEIGHT/2},p.rz,c);
    }

    //DrawFPS(10,10);

    //DrawRectangle(0,0,10,10,WHITE);
    
    EndDrawing();
    t++;
  }

  CloseWindow();

  return 0;
}
