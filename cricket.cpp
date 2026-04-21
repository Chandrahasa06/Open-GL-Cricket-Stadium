#include <GL/glut.h>
#include <cmath>
#include <cstring>

GLuint grassTexOutfield;
GLuint grassTexInfield;
GLuint wallTex;

float angleX = 20, angleY = 30;
float zoom = -60;
float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
bool lightsOn = true;

#define PI 3.14159265359

//  GEOMETRY HELPERS 
void drawCircle(float radius, float height) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1416f / 180.0f;
        glVertex3f(radius * cos(theta), height, radius * sin(theta));
    }
    glEnd();
}

void drawPoly(float radius, float height, int sides) {
    float angleStep = 2 * 3.1416f / sides;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < sides; i++) {
        float theta = i * angleStep;
        glVertex3f(radius * cos(theta), height, radius * sin(theta));
    }
    glEnd();
}

void drawCylinder(float radius, float height) {
    int slices = 100;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float theta = 2 * M_PI * i / slices;
        float x = cos(theta), z = sin(theta);
        glVertex3f(radius * x, 0, radius * z);
        glVertex3f(radius * x, height, radius * z);
    }
    glEnd();
}

void drawSlantedRing(float outerR, float innerR, float topH, float bottomH, int segments) {
    float thetaStep = 2 * PI / segments;
    for (int i = 0; i < segments; i++) {
        float t1 = i * thetaStep, t2 = (i + 1) * thetaStep;
        float x1o = outerR*cos(t1), z1o = outerR*sin(t1);
        float x2o = outerR*cos(t2), z2o = outerR*sin(t2);
        float x1i = innerR*cos(t1), z1i = innerR*sin(t1);
        float x2i = innerR*cos(t2), z2i = innerR*sin(t2);
        glBegin(GL_QUADS);
            glVertex3f(x1o, topH,    z1o);
            glVertex3f(x2o, topH,    z2o);
            glVertex3f(x2i, bottomH, z2i);
            glVertex3f(x1i, bottomH, z1i);
        glEnd();
    }
}

//  PITCH 

void drawPitch() {
    glColor3f(0.8f, 0.7f, 0.5f);
    glBegin(GL_QUADS);
        glVertex3f(-0.6f, 0.03f, -2.5f);
        glVertex3f( 0.6f, 0.03f, -2.5f);
        glVertex3f( 0.6f, 0.03f,  2.5f);
        glVertex3f(-0.6f, 0.03f,  2.5f);
    glEnd();
}

//  WICKETS 
void drawStump(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f);
    // shaft
    glPushMatrix();
        glTranslatef(0, 0.16f, 0);
        glScalef(0.033f, 0.33f, 0.033f);
        glutSolidCube(1);
    glPopMatrix();
    // bail
    glPushMatrix();
        glTranslatef(0, 0.34f, 0);
        glScalef(0.10f, 0.022f, 0.033f);
        glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

void drawWickets() {
    float stumpOffsets[3] = { -0.12f, 0.0f, 0.12f };
    float groundY = 0.0f;
    for (int s = 0; s < 3; s++) drawStump(stumpOffsets[s], groundY,  2.5f);
    glColor3f(1,1,1);
    glPushMatrix(); 
        glTranslatef(0,0.34f,2.5f);  
        glScalef(0.28f,0.022f,0.033f); 
        glutSolidCube(1); 
    glPopMatrix();
    for (int s = 0; s < 3; s++) drawStump(stumpOffsets[s], groundY, -2.5f);
    glColor3f(1,1,1);
    glPushMatrix(); 
        glTranslatef(0,0.34f,-2.5f); 
        glScalef(0.28f,0.022f,0.033f); 
        glutSolidCube(1); 
    glPopMatrix();
}

//  GRASS TEXTURES 
void generateGrassTextures() {
    srand(42);
    {
        const int W=128,H=128; unsigned char pixels[W*H*3];
        for(int y=0;y<H;y++) for(int x=0;x<W;x++){
            int idx=(y*W+x)*3, noise=(rand()%25)-12, blade=(rand()%100<6)?30:0;
            int r=25+noise/3, g=110+noise+blade, b=25+noise/4;
            pixels[idx+0]=(unsigned char)(r<0?0:r>255?255:r);
            pixels[idx+1]=(unsigned char)(g<0?0:g>255?255:g);
            pixels[idx+2]=(unsigned char)(b<0?0:b>255?255:b);
        }
        glGenTextures(1,&grassTexOutfield); glBindTexture(GL_TEXTURE_2D,grassTexOutfield);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,W,H,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    }
    {
        const int W=128,H=128; unsigned char pixels[W*H*3];
        for(int y=0;y<H;y++) for(int x=0;x<W;x++){
            int idx=(y*W+x)*3, noise=(rand()%25)-12, blade=(rand()%100<6)?35:0;
            int r=20+noise/3, g=150+noise+blade, b=20+noise/4;
            pixels[idx+0]=(unsigned char)(r<0?0:r>255?255:r);
            pixels[idx+1]=(unsigned char)(g<0?0:g>255?255:g);
            pixels[idx+2]=(unsigned char)(b<0?0:b>255?255:b);
        }
        glGenTextures(1,&grassTexInfield); glBindTexture(GL_TEXTURE_2D,grassTexInfield);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,W,H,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    }
}

void drawTexturedGround() {
    int segments=360; float scale=0.08f;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,grassTexOutfield); glColor3f(1,1,1);
    for(int i=0;i<segments;i++){
        float t1=i*2*PI/segments, t2=(i+1)*2*PI/segments;
        float x1i=11*cos(t1),z1i=11*sin(t1), x2i=11*cos(t2),z2i=11*sin(t2);
        float x1o=24.8f*cos(t1),z1o=24.8f*sin(t1), x2o=24.8f*cos(t2),z2o=24.8f*sin(t2);
        glBegin(GL_QUADS);
            glTexCoord2f(x1i*scale+0.5f,z1i*scale+0.5f); glVertex3f(x1i,0,z1i);
            glTexCoord2f(x2i*scale+0.5f,z2i*scale+0.5f); glVertex3f(x2i,0,z2i);
            glTexCoord2f(x2o*scale+0.5f,z2o*scale+0.5f); glVertex3f(x2o,0,z2o);
            glTexCoord2f(x1o*scale+0.5f,z1o*scale+0.5f); glVertex3f(x1o,0,z1o);
        glEnd();
    }
    glBindTexture(GL_TEXTURE_2D,grassTexInfield);
    for(int i=0;i<segments;i++){
        float t1=i*2*PI/segments, t2=(i+1)*2*PI/segments;
        float x1=11*cos(t1),z1=11*sin(t1), x2=11*cos(t2),z2=11*sin(t2);
        glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f,0.5f);               glVertex3f(0,0.01f,0);
            glTexCoord2f(x1*scale+0.5f,z1*scale+0.5f); glVertex3f(x1,0.01f,z1);
            glTexCoord2f(x2*scale+0.5f,z2*scale+0.5f); glVertex3f(x2,0.01f,z2);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glColor3f(1,1,1); drawPoly(22.5f,0.03f,360);
}

//  WALL TEXTURE 

void generateWallTexture() {
    const int W=128,H=128; unsigned char pixels[W*H*3];
    srand(77);
    for(int y=0;y<H;y++) for(int x=0;x<W;x++){
        int idx=(y*W+x)*3, noise=(rand()%30)-15, streak=(rand()%100<4)?-25:0;
        int val=128+noise+streak; val=val<0?0:val>255?255:val;
        pixels[idx+0]=(unsigned char)val;
        pixels[idx+1]=(unsigned char)(val-3<0?0:val-3);
        pixels[idx+2]=(unsigned char)(val-6<0?0:val-6);
    }
    glGenTextures(1,&wallTex); glBindTexture(GL_TEXTURE_2D,wallTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,W,H,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
}

void drawTexturedWall() {
    int slices=100; float radius=25,wallHeight=8,texRepeatS=8,texRepeatT=2;
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,wallTex);
    glColor3f(0.85f,0.85f,0.85f);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=slices;i++){
        float theta=2*M_PI*i/slices, x=radius*cos(theta), z=radius*sin(theta);
        float s=(float)i/slices*texRepeatS;
        glTexCoord2f(s,0);           glVertex3f(x,0,z);
        glTexCoord2f(s,texRepeatT);  glVertex3f(x,wallHeight,z);
    }
    glEnd(); glDisable(GL_TEXTURE_2D);
}

//  BOUNDARY BOXES 

void drawBoundaryBoxes() {
    int count=10; float wallR=25;
    for(int i=0;i<count;i++){
        float angle=i*2*PI/count, cx=wallR*cos(angle), cz=wallR*sin(angle);
        glPushMatrix(); 
        glTranslatef(cx,0,cz);
        glRotatef(-angle*180.0f/PI+90,0,1,0);
        float px[6]={-3,3,3,1,-1,-3}, py[6]={0,0,4,5,5,4}, sx=0.55f,sy=0.45f,depth=2.2f;
        glColor3f(0.92f,0.92f,0.92f);
        glBegin(GL_POLYGON); for(int v=0;v<6;v++) 
        glVertex3f(px[v]*sx,py[v]*sy,-depth*0.5f); 
        glEnd();
        glColor3f(0.80f,0.80f,0.80f);
        glBegin(GL_POLYGON); for(int v=5;v>=0;v--) 
        glVertex3f(px[v]*sx,py[v]*sy,depth*0.5f); 
        glEnd();
        for(int v=0;v<6;v++){
            int next=(v+1)%6; float shade=(v==3||v==4)?0.85f:0.68f;
            glColor3f(shade,shade,shade);
            glBegin(GL_QUADS);
                glVertex3f(px[v]*sx,py[v]*sy,-depth*0.5f); glVertex3f(px[next]*sx,py[next]*sy,-depth*0.5f);
                glVertex3f(px[next]*sx,py[next]*sy,depth*0.5f); glVertex3f(px[v]*sx,py[v]*sy,depth*0.5f);
            glEnd();
        }
        glColor3f(0.3f,0.3f,0.3f); glLineWidth(1.2f);
        glBegin(GL_LINE_LOOP); for(int v=0;v<6;v++) glVertex3f(px[v]*sx,py[v]*sy,-depth*0.5f); glEnd();
        glBegin(GL_LINE_LOOP); for(int v=0;v<6;v++) glVertex3f(px[v]*sx,py[v]*sy, depth*0.5f); glEnd();
        glBegin(GL_LINES); for(int v=0;v<6;v++){ glVertex3f(px[v]*sx,py[v]*sy,-depth*0.5f); glVertex3f(px[v]*sx,py[v]*sy,depth*0.5f); } glEnd();
        glLineWidth(1); glPopMatrix();
    }
}

//  ADVERTISEMENTS 

void drawAdvertisements() {
    const char* ads[]={"PEPSI","NIKE","SAMSUNG","ADIDAS","COCA COLA","TOYOTA","AMAZON","RELIANCE"};
    float adColors[8][3]={{1,0.1f,0.1f},{1,1,0},{0.1f,0.8f,1},{1,1,0},{1,0.4f,0},{0.9f,0.1f,0.1f},{1,0.6f,0},{0.2f,1,0.4f}};
    int adCount=8; float r=18.05f,y=3.5f,charAngleStep=0.055f;
    GLfloat mat[16]; glGetFloatv(GL_MODELVIEW_MATRIX,mat);
    float camX=-(mat[0]*mat[12]+mat[1]*mat[13]+mat[2]*mat[14]);
    float camZ=-(mat[8]*mat[12]+mat[9]*mat[13]+mat[10]*mat[14]);
    glDepthFunc(GL_LEQUAL);
    for(int i=0;i<adCount;i++){
        glColor3f(adColors[i][0],adColors[i][1],adColors[i][2]);
        const char* str=ads[i]; int len=strlen(str);
        float startAngle=i*(2*PI/adCount)-(len*charAngleStep)*0.5f;
        for(int c=0;c<len;c++){
            if(str[c]==' ') continue;
            float a=startAngle+c*charAngleStep, x=r*cos(a), z=r*sin(a);
            if(cos(a)*(camX-x)+sin(a)*(camZ-z)<0) continue;
            glRasterPos3f(x,y,z); glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[c]);
        }
    }
    glDepthFunc(GL_LESS);
}

//  FLAG POLES 

void drawFlagPoles() {
    int sides=18; float polyR=25,baseY=8.01f;
    for(int i=0;i<sides;i++){
        float angle=i*2*PI/sides, px=polyR*cos(angle), pz=polyR*sin(angle);
        glPushMatrix(); glTranslatef(px,baseY,pz);
        glColor3f(0.8f,0.8f,0.8f);
        glPushMatrix(); glTranslatef(0,0.75f,0); glScalef(0.08f,1.5f,0.08f); glutSolidCube(1); glPopMatrix();
        if((i/3)%3==0) glColor3f(1,0.75f,0);
        else if((i/3)%3==1) glColor3f(1,1,1);
        else glColor3f(0,1,0);
        float poleTop=1.5f,flagW=0.6f,flagH=0.4f,tx=-sin(angle),tz=cos(angle);
        glBegin(GL_QUADS);
            glVertex3f(0,poleTop,0); glVertex3f(tx*flagW,poleTop,tz*flagW);
            glVertex3f(tx*flagW,poleTop-flagH,tz*flagW); glVertex3f(0,poleTop-flagH,0);
        glEnd();
        glColor3f(0,0,0); glLineWidth(0.8f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0,poleTop,0); glVertex3f(tx*flagW,poleTop,tz*flagW);
            glVertex3f(tx*flagW,poleTop-flagH,tz*flagW); glVertex3f(0,poleTop-flagH,0);
        glEnd();
        glLineWidth(1); glPopMatrix();
    }
}

//  SEATING ROWS  

void drawSeatingRows(float innerR,float outerR,float bottomY,float topY,int rows,int cols,int up){
    int sectionCount=30, colsPerSection=cols/sectionCount;
    float gapFraction=0.12f, seatFrac=0.72f;
    for(int col=0;col<cols;col++){
        int secIdx=col/colsPerSection;
        float posInSec=(float)(col%colsPerSection)/colsPerSection;
        if(posInSec<gapFraction) continue;
        bool isRed=((secIdx+up)%2==0);
        if(isRed) glColor3f(0.90f,0.08f,0.08f); else glColor3f(0.95f,0.85f,0.05f);
        float a_start=(col+(1-seatFrac)*0.5f)*2*PI/cols;
        float a_end  =(col+(1-seatFrac)*0.5f+seatFrac)*2*PI/cols;
        for(int row=0;row<rows;row++){
            float t0=((float)row+(1-seatFrac)*0.5f)/rows;
            float t1=((float)row+(1-seatFrac)*0.5f+seatFrac)/rows;
            float r0=innerR+t0*(outerR-innerR), r1=innerR+t1*(outerR-innerR);
            float y0=bottomY+t0*(topY-bottomY)+0.05f, y1=bottomY+t1*(topY-bottomY)+0.05f;
            glBegin(GL_QUADS);
                glVertex3f(r0*cos(a_start),y0,r0*sin(a_start));
                glVertex3f(r0*cos(a_end),  y0,r0*sin(a_end));
                glVertex3f(r1*cos(a_end),  y1,r1*sin(a_end));
                glVertex3f(r1*cos(a_start),y1,r1*sin(a_start));
            glEnd();
        }
    }
}

//  CROWD FIGURES    

void drawCrowd() {
    float colors[6][3]={{0.1f,0.7f,0.1f},{1,0.5f,0},{0.9f,0.9f,0.1f},{0.9f,0.1f,0.7f},{0.9f,0.1f,0.1f},{0.1f,0.4f,0.9f}};
    struct TierInfo{ float innerR,outerR,bottomY,topY; int rows,cols,up; };
    TierInfo tiers[2]={ {15,25,0,3,10,480,0},{18,25,4,6,8,640,1} };
    float s=0.10f;
    for(int t=0;t<2;t++){
        TierInfo& T=tiers[t];
        int sectionCount=30, colsPerSection=T.cols/sectionCount;
        float gapFraction=0.12f, seatFrac=0.72f;
        for(int col=0;col<T.cols;col++){
            int secIdx=col/colsPerSection;
            float posInSec=(float)(col%colsPerSection)/colsPerSection;
            if(posInSec<gapFraction) continue;
            float midAngle=(col+0.5f)*2*PI/T.cols;
            float cosA=cos(midAngle), sinA=sin(midAngle);
            for(int row=0;row<T.rows;row++){
                float t0=((float)row+(1-seatFrac)*0.5f)/T.rows;
                float r0=T.innerR+t0*(T.outerR-T.innerR);
                float y0=T.bottomY+t0*(T.topY-T.bottomY)+0.05f;
                float x=r0*cosA, z=r0*sinA, y=y0+0.05f;
                int colorIdx=(col*3+row*7+t*11)%6;
                glPushMatrix(); glTranslatef(x,y,z);
                glRotatef(-midAngle*180/PI+180,0,1,0);
                glColor3f(colors[colorIdx][0],colors[colorIdx][1],colors[colorIdx][2]);
                glPushMatrix(); glTranslatef(0,s*2,0); glScalef(s*0.65f,s*0.65f,s*0.35f); glutSolidCube(1); glPopMatrix();
                glPushMatrix(); glTranslatef(0,s*1,0); glScalef(s*0.8f,s*1.0f,s*0.35f);  glutSolidCube(1); glPopMatrix();
                glPopMatrix();
            }
        }
    }
}
//  ON-FIELD FIGURE HELPERS: torso colour, skin colour, optional scale
static void drawPerson(float tR,float tG,float tB,
                       float hR,float hG,float hB, float sc=1.0f){
    // legs
    glColor3f(0.15f,0.15f,0.15f);
    for(int leg=-1;leg<=1;leg+=2){
        glPushMatrix();
            glTranslatef(leg*0.045f*sc,0.11f*sc,0);
            glScalef(0.07f*sc,0.22f*sc,0.07f*sc);
            glutSolidCube(1);
        glPopMatrix();
    }
    // torso
    glColor3f(tR,tG,tB);
    glPushMatrix();
        glTranslatef(0,0.38f*sc,0);
        glScalef(0.16f*sc,0.22f*sc,0.10f*sc);
        glutSolidCube(1);
    glPopMatrix();
    // arms
    glColor3f(tR,tG,tB);
    for(int arm=-1;arm<=1;arm+=2){
        glPushMatrix();
            glTranslatef(arm*0.12f*sc,0.40f*sc,0);
            glScalef(0.06f*sc,0.18f*sc,0.06f*sc);
            glutSolidCube(1);
        glPopMatrix();
    }
    // head (skin)
    glColor3f(hR,hG,hB);
    glPushMatrix();
        glTranslatef(0,0.58f*sc,0);
        glutSolidSphere(0.07f*sc,8,6);
    glPopMatrix();
}

// Helmet on head
static void drawHelmet(float sc=1.0f){
    glColor3f(0.12f,0.12f,0.12f);
    glPushMatrix();
        glTranslatef(0,0.66f*sc,0);
        glScalef(0.13f*sc,0.07f*sc,0.13f*sc);
        glutSolidCube(1);
    glPopMatrix();
}

// Cricket bat
static void drawBat(float sc=1.0f){
    glColor3f(0.75f,0.58f,0.28f);
    glPushMatrix();
        glTranslatef(0.18f*sc,0.35f*sc,0);
        glRotatef(25,0,0,1);
        glScalef(0.04f*sc,0.32f*sc,0.035f*sc);
        glutSolidCube(1);
    glPopMatrix();
}

// Camera tripod + body
static void drawCamera(){
    glColor3f(0.22f,0.22f,0.22f);
    // three legs spread 120 deg
    for(int i=0;i<3;i++){
        glPushMatrix();
            glRotatef(i*120.0f,0,1,0);
            glTranslatef(0.10f,0.14f,0);
            glRotatef(-32,0,0,1);
            glScalef(0.022f,0.30f,0.022f);
            glutSolidCube(1);
        glPopMatrix();
    }
    // camera body
    glColor3f(0.08f,0.08f,0.08f);
    glPushMatrix();
        glTranslatef(0,0.32f,0);
        glScalef(0.20f,0.12f,0.12f);
        glutSolidCube(1);
    glPopMatrix();
    // lens
    glColor3f(0.35f,0.35f,0.35f);
    glPushMatrix();
        glTranslatef(-0.13f,0.32f,0);
        glutSolidSphere(0.042f,8,6);
    glPopMatrix();
}

// Cameraman 
static void drawCameraman(){
    drawPerson(0.25f,0.25f,0.25f, 0.80f,0.65f,0.50f, 0.88f);
}

// Bench (plank + 4 legs)
static void drawBench(){
    glColor3f(0.52f,0.33f,0.13f);
    glPushMatrix();
        glTranslatef(0,0.22f,0);
        glScalef(0.80f,0.04f,0.20f);
        glutSolidCube(1);
    glPopMatrix();
    glColor3f(0.38f,0.22f,0.08f);
    float lx[2]={-0.35f,0.35f}, lz2[2]={-0.07f,0.07f};
    for(int i=0;i<2;i++) for(int j=0;j<2;j++){
        glPushMatrix();
            glTranslatef(lx[i],0.11f,lz2[j]);
            glScalef(0.04f,0.22f,0.04f);
            glutSolidCube(1);
        glPopMatrix();
    }
}

// Drink cart 
static void drawDrinkCart(){
    glColor3f(0.88f,0.88f,0.88f);
    glPushMatrix();
        glTranslatef(0,0.26f,0);
        glScalef(0.34f,0.28f,0.20f);
        glutSolidCube(1);
    glPopMatrix();
    // blue cross on face
    glColor3f(0.10f,0.38f,0.90f);
    glPushMatrix(); glTranslatef(-0.17f,0.26f,0); glScalef(0.01f,0.20f,0.03f); glutSolidCube(1); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.17f,0.26f,0); glScalef(0.01f,0.03f,0.16f); glutSolidCube(1); glPopMatrix();
    // wheels
    glColor3f(0.18f,0.18f,0.18f);
    for(int s=-1;s<=1;s+=2){
        glPushMatrix();
            glTranslatef(s*0.18f,0.09f,0);
            glRotatef(90,0,1,0);
            glutSolidTorus(0.022f,0.075f,8,12);
        glPopMatrix();
    }
    // handle bar
    glColor3f(0.38f,0.38f,0.38f);
    glPushMatrix();
        glTranslatef(0.19f,0.36f,0);
        glScalef(0.035f,0.14f,0.035f);
        glutSolidCube(1);
    glPopMatrix();
}

void drawOnFieldScene(){

    const float SKIN=0.80f, SKING=0.65f, SKINB=0.50f;

    //  2 BATSMEN  (red kit, helmet, bat)
    // Striker — batting end 
    glPushMatrix();
        glTranslatef(0.35f, 0.0f, 2.0f);
        glRotatef(180, 0,1,0);
        drawPerson(0.85f,0.06f,0.06f, SKIN,SKING,SKINB);
        drawHelmet(); drawBat();
    glPopMatrix();

    // Non-striker — beside bowling-end stumps, stands off to side
    glPushMatrix();
        glTranslatef(-0.6f, 0.0f, -2.0f);
        glRotatef(0, 0,1,0);
        drawPerson(0.85f,0.06f,0.06f, SKIN,SKING,SKINB);
        drawHelmet(); drawBat();
    glPopMatrix();

    //  UMPIRE  — white coat, stands right beside
    glPushMatrix();
        glTranslatef(1.2f, 0.0f, -2.5f);
        glRotatef(90, 0,1,0);   // faces down the pitch
        drawPerson(0.93f,0.93f,0.90f, SKIN,SKING,SKINB);
    glPopMatrix();

    //  BLUE TEAM — 11 players
    // [0] Wicketkeeper: just behind batting-end stumps (z=+2.5)
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 3.2f);
        glRotatef(0, 0,1,0);     // faces bowling end
        glScalef(1.0f, 0.80f, 1.0f);
        drawPerson(0.10f,0.22f,0.82f, SKIN,SKING,SKINB);
    glPopMatrix();

    // [1] Bowler: in his delivery stride
    glPushMatrix();
        glTranslatef(0.3f, 0.0f, -5.5f);
        glRotatef(180, 0,1,0);  // faces batting end
        drawPerson(0.10f,0.22f,0.82f, SKIN,SKING,SKINB);
        // raised bowling arm
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f);
            glColor3f(0.10f,0.22f,0.82f);
            glPushMatrix();
                glTranslatef(0.12f, 0.60f, 0);
                glRotatef(-100, 0,0,1);
                glScalef(0.06f,0.22f,0.06f);
                glutSolidCube(1);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

    // 9 fielders 
    struct FPos{ float x,z,face; const char* role; };
    FPos fielders[9]={
        {  1.5f,  4.5f, 185.0f, "1st slip"     },
        {  2.5f,  5.0f, 188.0f, "2nd slip"      },
        {  3.5f,  5.5f, 192.0f, "gully"         },
        {  8.0f,  0.0f, 200.0f, "point"         },
        {  5.5f, -5.0f, 160.0f, "mid-off"       },
        { -5.5f, -5.0f,  20.0f, "mid-on"        },
        { -8.0f,  0.0f, 340.0f, "square leg"    },
        { 12.0f, -7.0f, 175.0f, "deep extra cover"},
        {-12.0f, -7.0f,  15.0f, "deep mid-wicket"},
    };
    for(int i=0;i<9;i++){
        glPushMatrix();
            glTranslatef(fielders[i].x, 0.0f, fielders[i].z);
            glRotatef(fielders[i].face, 0,1,0);
            drawPerson(0.10f,0.22f,0.82f, SKIN,SKING,SKINB);
        glPopMatrix();
    }

    // 3 cameras + cameramen 
    float camAng[3]={ 80.0f, 200.0f, 320.0f };
    for(int i=0;i<3;i++){
        float ang = camAng[i]*PI/180.0f;
        float cx  = 14.0f*cos(ang);
        float cz  = 14.0f*sin(ang);
        float faceInward = camAng[i] + 180.0f;

        // cameraman
        glPushMatrix();
            glTranslatef(cx + 0.55f*cos(ang+0.3f), 0.0f, cz + 0.55f*sin(ang+0.3f));
            glRotatef(faceInward, 0,1,0);
            drawCameraman();
        glPopMatrix();
        // camera tripod
        glPushMatrix();
            glTranslatef(cx, 0.0f, cz);
            glRotatef(faceInward, 0,1,0);
            drawCamera();
        glPopMatrix();
    }

    // Bench 
    {
        float ang = 50.0f*PI/180.0f;
        float bx  = 14.5f*cos(ang);
        float bz  = 14.5f*sin(ang);
        glPushMatrix();
            glTranslatef(bx, 0.0f, bz);
            glRotatef(50.0f, 0,1,0);
            drawBench();
            // 3 substitutes seated
            float sx3[3]={ -0.27f, 0.0f, 0.27f };
            for(int s=0;s<3;s++){
                glPushMatrix();
                    glTranslatef(sx3[s], 0.22f, 0);
                    glColor3f(0.85f, 0.06f, 0.06f);
                    glPushMatrix(); glTranslatef(0,0.10f,0); glScalef(0.11f,0.15f,0.08f); glutSolidCube(1); glPopMatrix();
                    glColor3f(SKIN,SKING,SKINB);
                    glPushMatrix(); glTranslatef(0,0.23f,0); glutSolidSphere(0.052f,7,5); glPopMatrix();
                glPopMatrix();
            }
        glPopMatrix();
    }

    // 2 drink carts 
    float cartAng[2]={ 140.0f, 160.0f };
    for(int i=0;i<2;i++){
        float ang = cartAng[i]*PI/180.0f;
        float cx  = 14.5f*cos(ang);
        float cz  = 14.5f*sin(ang);
        glPushMatrix();
            glTranslatef(cx, 0.0f, cz);
            glRotatef(cartAng[i], 0,1,0);
            drawDrinkCart();
        glPopMatrix();
    }
}

//  LIGHT CONES 

void drawLightCones() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    int coneSegs=32; float coneRadius=12;
    for(int p=0;p<6;p++){
        float angle=p*2*PI/6, wx=26.5f*cos(angle), wz=26.5f*sin(angle);
        float dirX=-cos(angle), dirZ=-sin(angle);
        float boxAngleDeg=(-angle*180/PI)+90, boxAngleRad=boxAngleDeg*PI/180;
        float lx=wx-0.6f*sin(boxAngleRad), ly=15, lz=wz+0.6f*cos(boxAngleRad);
        float shift=24, gx=wx+dirX*shift, gz=wz+dirZ*shift, gy=0.05f;
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1,1,0.85f,0.12f); glVertex3f(lx,ly,lz);
            for(int k=0;k<=coneSegs;k++){ float a=k*2*PI/coneSegs; glColor4f(1,1,0.8f,0); glVertex3f(gx+coneRadius*cos(a),gy,gz+coneRadius*sin(a)); }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1,1,0.9f,0.18f); glVertex3f(lx,ly,lz);
            for(int k=0;k<=coneSegs;k++){ float a=k*2*PI/coneSegs; glColor4f(1,1,0.85f,0); glVertex3f(gx+coneRadius*0.4f*cos(a),gy,gz+coneRadius*0.4f*sin(a)); }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1,1,1,0.35f); glVertex3f(lx,ly,lz);
            for(int k=0;k<=24;k++){ float a=k*2*PI/24; glColor4f(1,1,0.8f,0); glVertex3f(lx+1.8f*cos(a),ly+1.8f*sin(a),lz); }
        glEnd();
    }
    glDepthMask(GL_TRUE); glDisable(GL_BLEND);
}

void drawLightboxGrid() {
    // Panel extents
    const float PX0 = -1.5f, PX1 = 1.5f;   
    const float PY0 = -1.8f, PY1 =  1.8f;   
    const float FZ  = -0.56f;              
    const float LZ  =  FZ - 0.01f;          
    const float GAP =  0.07f;              

    // Column X boundaries 
    float colX[2][2] = {
        { PX0,         -GAP * 0.5f },   
        {  GAP * 0.5f,  PX1        }    
    };

    // Row Y boundaries  
    float rowH = (PY1 - PY0) / 3.0f;          // = 1.2
    float rowY[3][2] = {
        { PY0,              PY0 + rowH - GAP * 0.5f },
        { PY0 + rowH + GAP * 0.5f, PY0 + 2*rowH - GAP * 0.5f },
        { PY0 + 2*rowH + GAP * 0.5f, PY1 }
    };

    //  6 cell rectangles 
    if (lightsOn)
        glColor3f(1.00f, 0.97f, 0.78f);   // warm bright white
    else
        glColor3f(0.14f, 0.14f, 0.10f);   // dark off-state

    for (int col = 0; col < 2; col++) {
        for (int row = 0; row < 3; row++) {
            float x0 = colX[col][0], x1 = colX[col][1];
            float y0 = rowY[row][0], y1 = rowY[row][1];
            glBegin(GL_QUADS);
                glVertex3f(x0, y0, FZ);
                glVertex3f(x1, y0, FZ);
                glVertex3f(x1, y1, FZ);
                glVertex3f(x0, y1, FZ);
            glEnd();
        }
    }

    //  Divider lines 
    glColor3f(0.08f, 0.08f, 0.06f);
    glLineWidth(2.2f);

    // 1 vertical centre line
    glBegin(GL_LINES);
        glVertex3f(0.0f, PY0, LZ);
        glVertex3f(0.0f, PY1, LZ);
    glEnd();

    // 2 horizontal dividers
    for (int h = 1; h <= 2; h++) {
        float hy = PY0 + h * rowH;
        glBegin(GL_LINES);
            glVertex3f(PX0, hy, LZ);
            glVertex3f(PX1, hy, LZ);
        glEnd();
    }

    //  Outer border 
    glColor3f(0.22f, 0.22f, 0.18f);
    glLineWidth(1.8f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(PX0, PY0, LZ);
        glVertex3f(PX1, PY0, LZ);
        glVertex3f(PX1, PY1, LZ);
        glVertex3f(PX0, PY1, LZ);
    glEnd();
    glLineWidth(1.0f);
}

//  MAIN DISPLAY 

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(posX, -5+posY, zoom+posZ);
    glRotatef(angleX,1,0,0);
    glRotatef(angleY,0,1,0);

    // Outer wall
    drawTexturedWall();
    glPushMatrix(); 
    glTranslatef(0,3,0); 
    glColor3f(0.5f,0.5f,0.5f); 
    drawCylinder(18,1); 
    glPopMatrix();
    drawBoundaryBoxes();

    // Seating rings
    glEnable(GL_POLYGON_OFFSET_FILL); glPolygonOffset(4,4);
    glColor3f(0.75f,0.75f,0.75f); drawSlantedRing(25,15,3,0,120);
    glColor3f(0.62f,0.62f,0.62f); drawSlantedRing(25,18,6,4,120);
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Roof stripes
    float thetaStep=2*PI/360;
    for(int i=0;i<360;i++){
        float t1=i*thetaStep, t2=(i+1)*thetaStep;
        glColor3f((i%2==0)?0.92f:0.70f,(i%2==0)?0.45f:0.25f,(i%2==0)?0.50f:0.30f);
        glBegin(GL_QUADS);
            glVertex3f(25*cos(t1),8,25*sin(t1)); glVertex3f(25*cos(t2),8,25*sin(t2));
            glVertex3f(18*cos(t2),6,18*sin(t2)); glVertex3f(18*cos(t1),6,18*sin(t1));
        glEnd();
    }
    glColor3f(1,1,1); drawPoly(25,8.01f,18);
    drawFlagPoles();

    // Field
    drawPitch();
    drawTexturedGround();
    drawWickets();
    glColor3f(1,1,1); drawPoly(14,0.04f,360);

    // Lit seating rows
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); 
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    GLfloat lpos[]={0,60,0,1}, ldif[]={1,1,1,1}, lamb[]={0.5f,0.5f,0.5f,1};
    glLightfv(GL_LIGHT0,GL_POSITION,lpos); 
    glLightfv(GL_LIGHT0,GL_DIFFUSE,ldif); 
    glLightfv(GL_LIGHT0,GL_AMBIENT,lamb);
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);
    drawSeatingRows(15,25,0,3,10,480,0);
    drawSeatingRows(18,25,4,6, 8,640,1);
    glDisable(GL_CULL_FACE); glDisable(GL_LIGHTING);

    // Crowd in stands
    drawCrowd();

    // On-field players, officials and boundary equipment
    drawOnFieldScene();

    //  6 Floodlight towers 
    for (int i = 0; i < 6; i++) {
        float angle = i * 2 * PI / 6.0f;
        float wx = 26.5f * cos(angle), wz = 26.5f * sin(angle);
        glPushMatrix();
        glTranslatef(wx, 0, wz);

        // Pole
        glColor3f(0.2f, 0.2f, 0.2f);
        glPushMatrix();
            glTranslatef(0, 7.5f, 0);
            glScalef(0.2f, 15.0f, 0.2f);
            glutSolidCube(1);
        glPopMatrix();

        // Light box + grid panel
        glPushMatrix();
            glTranslatef(0, 15.0f, 0);
            glRotatef((-angle * 180.0f / PI) + 90.0f, 0, 1, 0);

            // Dark housing
            glColor3f(0.15f, 0.15f, 0.15f);
            glPushMatrix();
                glScalef(3.3f, 4.0f, 1.0f);
                glutSolidCube(1);
            glPopMatrix();

            // 6-cell grid face
            drawLightboxGrid();

        glPopMatrix(); // end light box
        glPopMatrix(); // end pole world pos
    }

    // Light cones 
    if (lightsOn) drawLightCones();
    glutSwapBuffers();
}

//  INPUT HANDLERS 

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'w': posY -= 1.0f; break;
        case 's': posY += 1.0f; break;
        case 'a': posX += 1.0f; break;
        case 'd': posX -= 1.0f; break;
        case 'q': zoom+=2;      break;
        case 'e': zoom-=2;      break;
        case 'z': posZ += 1.0f; break;
        case 'x': posZ -= 1.0f; break;
        case 'l': case 'L': lightsOn = !lightsOn; break;
        case 27:  exit(0);       break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if(key==GLUT_KEY_LEFT)  angleY -= 5;
    if(key==GLUT_KEY_RIGHT) angleY += 5;
    if(key==GLUT_KEY_UP)    angleX -= 5;
    if(key==GLUT_KEY_DOWN)  angleX += 5;
    glutPostRedisplay();
}

//  INIT / RESHAPE / MAIN 

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.55f, 0.55f, 0.58f, 1.0f);
    generateGrassTextures();
    generateWallTexture();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)w/h, 1, 200);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Stadium");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}