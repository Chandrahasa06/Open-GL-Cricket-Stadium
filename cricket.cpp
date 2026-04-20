#include <GL/glut.h>
#include <cmath>
#include <cstring>
GLuint grassTexOutfield;
GLuint grassTexInfield;
float angleX = 20, angleY = 30;
float zoom = -60;
float posX = 0.0f;
float posY = 0.0f;
float posZ = 0.0f;
bool lightsOn = true;

// Draw circle (for field)
void drawCircle(float radius, float height) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.1416f / 180.0f;
        glVertex3f(radius * cos(theta), height, radius * sin(theta));
    }
    glEnd();
}

void drawPoly(float radius, float height,int sides) {
    float angleStep = 2 * 3.1416f / sides;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < sides; i++) {
        float theta = i * angleStep;
        glVertex3f(radius * cos(theta), height, radius * sin(theta));
    }
    glEnd();
}

void drawGround() {
    glColor3f(0.1f, 0.6f, 0.1f); 
    drawCircle(25,0);
}
void drawinnerFeild() {
    glColor3f(0.1f, 0.8f, 0.1f);
    drawCircle(8,0.01);
}

void drawPitch() {
    glColor3f(0.8f, 0.7f, 0.5f); 
    glBegin(GL_QUADS);
    glVertex3f(-0.6, 0.03, -2.5);
    glVertex3f(0.6, 0.03, -2.5);
    glVertex3f(0.6, 0.03, 2.5);
    glVertex3f(-0.6, 0.03, 2.5);
    glEnd();
}

// Draw cylinder (stadium base)
void drawCylinder(float radius, float height) {
    int slices = 100;

    // Side
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float theta = 2 * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);

        glVertex3f(radius * x, 0, radius * z);
        glVertex3f(radius * x, height, radius * z);
    }
    glEnd();
}

// Seating tiers
void drawSeating(float inner, float outer, float height) {
    int slices = 100;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float t = 2 * M_PI * i / slices;

        glVertex3f(inner * cos(t), height, inner * sin(t));
        glVertex3f(outer * cos(t), height, outer * sin(t));
    }
    glEnd();
}

#define PI 3.14159265359

void drawSlantedRing(float outerR, float innerR, float topH, float bottomH, int segments) {
    float thetaStep = 2 * PI / segments;

    for (int i = 0; i < segments; i++) {
        float t1 = i * thetaStep;
        float t2 = (i + 1) * thetaStep;

        // Outer circle (top)
        float x1o = outerR * cos(t1);
        float z1o = outerR * sin(t1);

        float x2o = outerR * cos(t2);
        float z2o = outerR * sin(t2);

        // Inner circle (bottom)
        float x1i = innerR * cos(t1);
        float z1i = innerR * sin(t1);

        float x2i = innerR * cos(t2);
        float z2i = innerR * sin(t2);

        // Draw slanted quad
        glBegin(GL_QUADS);

        //glColor3f(0.7f, 0.7f, 0.7f); // grey wall

        // Top outer edge
        glVertex3f(x1o, topH, z1o);
        glVertex3f(x2o, topH, z2o);

        // Bottom inner edge (lower height)
        glVertex3f(x2i, bottomH, z2i);
        glVertex3f(x1i, bottomH, z1i);

        glEnd();
    }
}

// Floodlight
void drawFloodlight() {
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 7.5, 0);

    // pole
    glPushMatrix();
    glScalef(0.2, 15, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    // light box (4 wide X, 6 tall Y, 1 deep Z)
    glPushMatrix();
    glTranslatef(0, 7, 0);
    glScalef(4, 6, 1);
    glutSolidCube(1);
    glPopMatrix();

    // 6 circles on front face — 2 cols along X, 3 rows along Y
    glColor3f(1.0, 1.0, 0.8);
    float colX[2] = { -1.0f, 1.0f };
    float rowY[3] = { -2.0f, 0.0f, 2.0f };
    float faceZ   =  -0.6f;   // just past box front face (half-depth = 0.5)
    float radius  =  0.4f;

    for (int col = 0; col < 2; col++) {
        for (int row = 0; row < 3; row++) {
            glPushMatrix();
            glTranslatef(colX[col], 7 + rowY[row], faceZ);
            glBegin(GL_POLYGON);
            for (int k = 0; k < 36; k++) {
                float a = k * 2 * 3.1416f / 36;
                glVertex3f(radius * cos(a), radius * sin(a), 0.0f);
            }
            glEnd();
            glPopMatrix();
        }
    }
}

void drawFloodlightX() {
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 7.5, 0);

    // pole
    glPushMatrix();
    glScalef(0.2, 15, 0.2);
    glutSolidCube(1);
    glPopMatrix();

    // light box (1 wide X, 6 tall Y, 4 deep Z)
    glPushMatrix();
    glTranslatef(0, 7, 0);
    glScalef(1, 6, 4);
    glutSolidCube(1);
    glPopMatrix();

    // 6 circles on front face — 2 cols along Z, 3 rows along Y
    glColor3f(1.0, 1.0, 0.8);
    float colZ[2] = { -1.0f, 1.0f };
    float rowY[3] = { -2.0f, 0.0f, 2.0f };
    float faceX   =  -0.6f;   // just past box front face (half-depth = 0.5)
    float radius  =  0.4f;

    for (int col = 0; col < 2; col++) {
        for (int row = 0; row < 3; row++) {
            glPushMatrix();
            glTranslatef(faceX, 7 + rowY[row], colZ[col]);
            glBegin(GL_POLYGON);
            for (int k = 0; k < 36; k++) {
                float a = k * 2 * 3.1416f / 36;
                glVertex3f(0.0f, radius * cos(a), radius * sin(a));
            }
            glEnd();
            glPopMatrix();
        }
    }
}
//seatingvoid 
void drawSeatingRows(float innerR, float outerR, float bottomY, float topY, int rows, int cols,int up) {
    int   sectionCount   = 30;
    int   colsPerSection = cols / sectionCount;
    float gapFraction    = 0.12f;

    float seatFrac = 0.72f;  // fraction of each cell actually filled (rest is gap)

    for (int col = 0; col < cols; col++) {
        int   secIdx   = col / colsPerSection;
        float posInSec = (float)(col % colsPerSection) / colsPerSection;
        if (posInSec < gapFraction) continue;

        bool isRed = ((secIdx+up) % 2 == 0);
        if (isRed) glColor3f(0.90f, 0.08f, 0.08f);
        else       glColor3f(0.95f, 0.85f, 0.05f);

        // angular span of this column cell
        float a_start = (col + (1.0f - seatFrac) * 0.5f)       * 2 * PI / cols;
        float a_end   = (col + (1.0f - seatFrac) * 0.5f + seatFrac) * 2 * PI / cols;

        for (int row = 0; row < rows; row++) {
            // radial span with gap
            float t0 = ((float)row       + (1.0f - seatFrac) * 0.5f) / rows;
            float t1 = ((float)row       + (1.0f - seatFrac) * 0.5f + seatFrac) / rows;

            float r0 = innerR + t0 * (outerR - innerR);
            float r1 = innerR + t1 * (outerR - innerR);

            float y0 = bottomY + t0 * (topY - bottomY) + 0.05f;
            float y1 = bottomY + t1 * (topY - bottomY) + 0.05f;

            glBegin(GL_QUADS);
                glVertex3f(r0 * cos(a_start), y0, r0 * sin(a_start));
                glVertex3f(r0 * cos(a_end),   y0, r0 * sin(a_end));
                glVertex3f(r1 * cos(a_end),   y1, r1 * sin(a_end));
                glVertex3f(r1 * cos(a_start), y1, r1 * sin(a_start));
            glEnd();
        }
    }
}
// Roof segments
void drawRoof(float inner, float outer, float height) {
    int slices = 40;

    glColor3f(1, 1, 1);

    for (int i = 0; i < slices; i++) {
        float t1 = 2 * M_PI * i / slices;
        float t2 = 2 * M_PI * (i + 1) / slices;

        glBegin(GL_QUADS);
        glVertex3f(inner * cos(t1), height, inner * sin(t1));
        glVertex3f(outer * cos(t1), height + 2, outer * sin(t1));
        glVertex3f(outer * cos(t2), height + 2, outer * sin(t2));
        glVertex3f(inner * cos(t2), height, inner * sin(t2));
        glEnd();
    }
}
void drawBoundaryBoxes() {
    int count = 10;
    float wallR = 25.0f;   

    for (int i = 0; i < count; i++) {
        float angle = i * 2 * PI / count;

        // position flush against outer wall
        float cx = wallR * cos(angle);
        float cz = wallR * sin(angle);

        glPushMatrix();
        glTranslatef(cx, 0, cz);
        // rotate so LONG face is tangent to wall (parallel to wall surface)
        glRotatef(-angle * 180.0f / PI + 90.0f, 0, 1, 0);

        float px[6] = { -3.0f,  3.0f,  3.0f,  1.0f, -1.0f, -3.0f };
        float py[6] = {  0.0f,  0.0f,  4.0f,  5.0f,  5.0f,  4.0f };
        float sx = 0.55f, sy = 0.45f;
        float depth = 2.2f;

        // front face (faces outward from stadium)
        glColor3f(0.92f, 0.92f, 0.92f);
        glBegin(GL_POLYGON);
        for (int v = 0; v < 6; v++)
            glVertex3f(px[v]*sx, py[v]*sy, -depth*0.5f);
        glEnd();

        // back face
        glColor3f(0.80f, 0.80f, 0.80f);
        glBegin(GL_POLYGON);
        for (int v = 5; v >= 0; v--)
            glVertex3f(px[v]*sx, py[v]*sy, depth*0.5f);
        glEnd();

        // side quads
        for (int v = 0; v < 6; v++) {
            int next = (v + 1) % 6;
            float shade = (v == 3 || v == 4) ? 0.85f : 0.68f;
            glColor3f(shade, shade, shade);
            glBegin(GL_QUADS);
                glVertex3f(px[v]   *sx, py[v]   *sy, -depth*0.5f);
                glVertex3f(px[next]*sx, py[next]*sy, -depth*0.5f);
                glVertex3f(px[next]*sx, py[next]*sy,  depth*0.5f);
                glVertex3f(px[v]   *sx, py[v]   *sy,  depth*0.5f);
            glEnd();
        }

        // outline
        glColor3f(0.3f, 0.3f, 0.3f);
        glLineWidth(1.2f);
        glBegin(GL_LINE_LOOP);
        for (int v = 0; v < 6; v++)
            glVertex3f(px[v]*sx, py[v]*sy, -depth*0.5f);
        glEnd();
        glBegin(GL_LINE_LOOP);
        for (int v = 0; v < 6; v++)
            glVertex3f(px[v]*sx, py[v]*sy,  depth*0.5f);
        glEnd();
        glBegin(GL_LINES);
        for (int v = 0; v < 6; v++) {
            glVertex3f(px[v]*sx, py[v]*sy, -depth*0.5f);
            glVertex3f(px[v]*sx, py[v]*sy,  depth*0.5f);
        }
        glEnd();
        glLineWidth(1.0f);

        glPopMatrix();
    }
}
void drawAdvertisements() {
    const char* ads[] = {
        "PEPSI", "NIKE", "SAMSUNG", "ADIDAS",
        "COCA COLA", "TOYOTA", "AMAZON", "RELIANCE"
    };
    float adColors[8][3] = {
        {1.0f, 0.1f, 0.1f},
        {1.0f, 1.0f, 0.0f},
        {0.1f, 0.8f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, 0.4f, 0.0f},
        {0.9f, 0.1f, 0.1f},
        {1.0f, 0.6f, 0.0f},
        {0.2f, 1.0f, 0.4f},
    };

    int   adCount       = 8;
    float r             = 18.05f;
    float y             = 3.5f;
    float charAngleStep = 0.055f;

    // get current modelview matrix to find camera position in world space
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);

    // camera position in world space (inverse of translation part of modelview)
    // for a pure rotate+translate matrix, camera world pos = -(R^T * t)
    float camX = -(mat[0]*mat[12] + mat[1]*mat[13] + mat[2]*mat[14]);
    float camZ = -(mat[8]*mat[12] + mat[9]*mat[13] + mat[10]*mat[14]);

    glDepthFunc(GL_LEQUAL);

    for (int i = 0; i < adCount; i++) {
        glColor3f(adColors[i][0], adColors[i][1], adColors[i][2]);

        const char* str = ads[i];
        int   len       = strlen(str);

        float startAngle = i * (2.0f * PI / adCount);
        startAngle -= (len * charAngleStep) * 0.5f;

        for (int c = 0; c < len; c++) {
            if (str[c] == ' ') continue;

            float a = startAngle + c * charAngleStep;
            float x = r * cos(a);
            float z = r * sin(a);

            // outward normal of cylinder at this point = (cos(a), 0, sin(a))
            // vector from surface point to camera
            float toCamX = camX - x;
            float toCamZ = camZ - z;

            // dot product of normal with toCam vector
            // if negative, surface faces away from camera — skip
            float dot = cos(a) * toCamX + sin(a) * toCamZ;
            if (dot < 0.0f) continue;   // back-facing, don't draw

            glRasterPos3f(x, y, z);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[c]);
        }
    }

    glDepthFunc(GL_LESS);
}

void drawFlagPoles() {
    // white poly has 18 sides at radius 25, height 8.01
    // vertices are at angle = i * 2*PI/18 for i=0..17
    int sides = 18;
    float polyR = 25.0f;
    float baseY = 8.01f;

    for (int i = 0; i < sides; i++) {
        float angle = i * 2 * PI / sides;
        float px = polyR * cos(angle);
        float pz = polyR * sin(angle);

        glPushMatrix();
        glTranslatef(px, baseY, pz);

        // ── small pole ──────────────────────────────────
        glColor3f(0.8f, 0.8f, 0.8f);
        glPushMatrix();
            glTranslatef(0, 0.75f, 0);  // centre of pole
            glScalef(0.08f, 1.5f, 0.08f);
            glutSolidCube(1);
        glPopMatrix();

        // ── flag (small quad, hangs from pole top) ───────
        // alternate colors around the roof for variety
        if ((i / 3) % 3 == 0)      glColor3f(1.0f, 0.75f, 0.0f);   // orange
        else if ((i / 3) % 3 == 1) glColor3f(1.0f, 1.0f, 1.0f);   // white
        else if ((i / 3) % 3 == 2) glColor3f(0.0f, 1.0f, 0.0f);   //green

        float poleTop = 1.5f;
        float flagW   = 0.6f;   // width along tangent
        float flagH   = 0.4f;   // height downward

        // tangent direction so flag faces along the roof edge
        float tx = -sin(angle);
        float tz =  cos(angle);

        glBegin(GL_QUADS);
            // top-left (at pole)
            glVertex3f(0,        poleTop,        0);
            // top-right (along tangent)
            glVertex3f(tx*flagW, poleTop,        tz*flagW);
            // bottom-right
            glVertex3f(tx*flagW, poleTop-flagH,  tz*flagW);
            // bottom-left
            glVertex3f(0,        poleTop-flagH,  0);
        glEnd();

        // flag outline
        glColor3f(0.0f, 0.0f, 0.0f);
        glLineWidth(0.8f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(0,        poleTop,        0);
            glVertex3f(tx*flagW, poleTop,        tz*flagW);
            glVertex3f(tx*flagW, poleTop-flagH,  tz*flagW);
            glVertex3f(0,        poleTop-flagH,  0);
        glEnd();
        glLineWidth(1.0f);

        glPopMatrix();
    }
}


void generateGrassTextures() {
    srand(42);

    // ── OUTFIELD TEXTURE — darker green with noise ───────────
    {
        const int W = 128, H = 128;
        unsigned char pixels[W * H * 3];

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int idx = (y * W + x) * 3;
                int noise = (rand() % 25) - 12;
                int blade = (rand() % 100 < 6) ? 30 : 0;

                int r = 25  + noise/3;
                int g = 110 + noise + blade;
                int b = 25  + noise/4;

                pixels[idx+0] = (unsigned char)(r < 0 ? 0 : r > 255 ? 255 : r);
                pixels[idx+1] = (unsigned char)(g < 0 ? 0 : g > 255 ? 255 : g);
                pixels[idx+2] = (unsigned char)(b < 0 ? 0 : b > 255 ? 255 : b);
            }
        }

        glGenTextures(1, &grassTexOutfield);
        glBindTexture(GL_TEXTURE_2D, grassTexOutfield);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }

    // ── INFIELD TEXTURE — brighter green with noise ──────────
    {
        const int W = 128, H = 128;
        unsigned char pixels[W * H * 3];

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                int idx = (y * W + x) * 3;
                int noise = (rand() % 25) - 12;
                int blade = (rand() % 100 < 6) ? 35 : 0;

                int r = 20  + noise/3;
                int g = 150 + noise + blade;   // brighter than outfield
                int b = 20  + noise/4;

                pixels[idx+0] = (unsigned char)(r < 0 ? 0 : r > 255 ? 255 : r);
                pixels[idx+1] = (unsigned char)(g < 0 ? 0 : g > 255 ? 255 : g);
                pixels[idx+2] = (unsigned char)(b < 0 ? 0 : b > 255 ? 255 : b);
            }
        }

        glGenTextures(1, &grassTexInfield);
        glBindTexture(GL_TEXTURE_2D, grassTexInfield);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, pixels);
    }
}

void drawTexturedGround() {
    int   segments = 360;
    float scale    = 0.08f;

    glEnable(GL_TEXTURE_2D);

    // ── OUTFIELD — darker green texture ─────────────────────
    glBindTexture(GL_TEXTURE_2D, grassTexOutfield);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < segments; i++) {
        float t1 = i       * 2 * PI / segments;
        float t2 = (i + 1) * 2 * PI / segments;

        float x1i = 11.0f * cos(t1), z1i = 11.0f * sin(t1);
        float x2i = 11.0f * cos(t2), z2i = 11.0f * sin(t2);
        float x1o = 24.8f * cos(t1), z1o = 24.8f * sin(t1);
        float x2o = 24.8f * cos(t2), z2o = 24.8f * sin(t2);

        glBegin(GL_QUADS);
            glTexCoord2f(x1i*scale+0.5f, z1i*scale+0.5f); glVertex3f(x1i, 0.0f, z1i);
            glTexCoord2f(x2i*scale+0.5f, z2i*scale+0.5f); glVertex3f(x2i, 0.0f, z2i);
            glTexCoord2f(x2o*scale+0.5f, z2o*scale+0.5f); glVertex3f(x2o, 0.0f, z2o);
            glTexCoord2f(x1o*scale+0.5f, z1o*scale+0.5f); glVertex3f(x1o, 0.0f, z1o);
        glEnd();
    }

    // ── INFIELD — brighter green texture ────────────────────
    glBindTexture(GL_TEXTURE_2D, grassTexInfield);

    for (int i = 0; i < segments; i++) {
        float t1 = i       * 2 * PI / segments;
        float t2 = (i + 1) * 2 * PI / segments;

        float x1 = 11.0f * cos(t1), z1 = 11.0f * sin(t1);
        float x2 = 11.0f * cos(t2), z2 = 11.0f * sin(t2);

        glBegin(GL_TRIANGLES);
            glTexCoord2f(0.5f,          0.5f);          glVertex3f(0,  0.01f, 0);
            glTexCoord2f(x1*scale+0.5f, z1*scale+0.5f); glVertex3f(x1, 0.01f, z1);
            glTexCoord2f(x2*scale+0.5f, z2*scale+0.5f); glVertex3f(x2, 0.01f, z2);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);

    // ── LINES ────────────────────────────────────────────────
    glColor3f(1.0f, 1.0f, 1.0f);
    drawPoly(22.5f, 0.03f, 360);
}

// ── SINGLE STUMP ────────────────────────────────────────────
void drawStump(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // stump shaft
    glColor3f(1.0f, 1.0f, 1.0f);  // wood color
    glPushMatrix();
        glTranslatef(0, 0.3f, 0);
        glScalef(0.06f, 0.6f, 0.06f);
        glutSolidCube(1);
    glPopMatrix();

    // bail on top (small horizontal bar)
     glColor3f(1.0f, 1.0f, 1.0f); 
    glPushMatrix();
        glTranslatef(0, 0.62f, 0);
        glScalef(0.18f, 0.04f, 0.06f);
        glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

void drawWickets() {
    // pitch runs from Z = -2.5 to Z = +2.5, centred at X=0
    // stumps at each end: 3 stumps spaced 0.22 apart along X

    float stumpOffsets[3] = { -0.22f, 0.0f, 0.22f };
    float groundY = 0.0f;

    // ── BATTING END (Z = +2.5) ───────────────────────────────
    for (int s = 0; s < 3; s++) {
        drawStump(stumpOffsets[s], groundY, 2.5f);
    }
    // bail connecting top of outer two stumps
     glColor3f(1.0f, 1.0f, 1.0f); 
    glPushMatrix();
        glTranslatef(0, 0.62f, 2.5f);
        glScalef(0.50f, 0.04f, 0.06f);
        glutSolidCube(1);
    glPopMatrix();

    // ── BOWLING END (Z = -2.5) ───────────────────────────────
    for (int s = 0; s < 3; s++) {
        drawStump(stumpOffsets[s], groundY, -2.5f);
    }
     glColor3f(1.0f, 1.0f, 1.0f); 
    glPushMatrix();
        glTranslatef(0, 0.62f, -2.5f);
        glScalef(0.50f, 0.04f, 0.06f);
        glutSolidCube(1);
    glPopMatrix();
}

void drawLightCones() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    int   coneSegs   = 32;
    float coneRadius = 12.0f;  // wide spread on field

    for (int p = 0; p < 6; p++) {
        float angle       = p * 2 * PI / 6.0f;
        float wx          = 26.5f * cos(angle);
        float wz          = 26.5f * sin(angle);
        float dirX        = -cos(angle);
        float dirZ        = -sin(angle);
        float boxAngleDeg = (-angle * 180.0f / PI) + 90.0f;
        float boxAngleRad = boxAngleDeg * PI / 180.0f;

        // single cone from CENTER of light box
        float lx = wx - 0.6f * sin(boxAngleRad);
        float ly = 15.0f;
        float lz = wz + 0.6f * cos(boxAngleRad);

        // cone base on field toward center
        float shift = 24.0f;
        float gx    = wx + dirX * shift;
        float gz    = wz + dirZ * shift;
        float gy    = 0.05f;

        // outer cone (wide, very transparent)
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1.0f, 1.0f, 0.85f, 0.12f);
            glVertex3f(lx, ly, lz);
            for (int k = 0; k <= coneSegs; k++) {
                float a = k * 2 * PI / coneSegs;
                glColor4f(1.0f, 1.0f, 0.80f, 0.0f);
                glVertex3f(gx + coneRadius * cos(a), gy,
                           gz + coneRadius * sin(a));
            }
        glEnd();

        // inner cone (tight, slightly brighter)
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1.0f, 1.0f, 0.90f, 0.18f);
            glVertex3f(lx, ly, lz);
            for (int k = 0; k <= coneSegs; k++) {
                float a = k * 2 * PI / coneSegs;
                glColor4f(1.0f, 1.0f, 0.85f, 0.0f);
                glVertex3f(gx + (coneRadius * 0.4f) * cos(a), gy,
                           gz + (coneRadius * 0.4f) * sin(a));
            }
        glEnd();

        // glow at box face
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(1.0f, 1.0f, 1.0f, 0.35f);
            glVertex3f(lx, ly, lz);
            for (int k = 0; k <= 24; k++) {
                float a = k * 2 * PI / 24;
                glColor4f(1.0f, 1.0f, 0.8f, 0.0f);
                glVertex3f(lx + 1.8f * cos(a),
                           ly + 1.8f * sin(a), lz);
            }
        glEnd();
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}
GLuint wallTex;
void generateWallTexture() {
    const int W = 128, H = 128;
    unsigned char pixels[W * H * 3];

    srand(77);
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int idx = (y * W + x) * 3;

            // base concrete grey
            int noise  = (rand() % 30) - 15;
            // occasional darker streaks (water stains / shadow)
            int streak = (rand() % 100 < 4) ? -25 : 0;

            int val = 128 + noise + streak;
            val = val < 0 ? 0 : val > 255 ? 255 : val;

            // very slight warm tint
            pixels[idx+0] = (unsigned char)(val);
            pixels[idx+1] = (unsigned char)(val - 3 < 0 ? 0 : val - 3);
            pixels[idx+2] = (unsigned char)(val - 6 < 0 ? 0 : val - 6);
        }
    }

    glGenTextures(1, &wallTex);
    glBindTexture(GL_TEXTURE_2D, wallTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, W, H, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

void drawTexturedWall() {
    int   slices     = 100;
    float radius     = 25.0f;
    float wallHeight = 8.0f;
    float texRepeatS = 8.0f;   // how many times texture wraps around
    float texRepeatT = 2.0f;   // how many times texture repeats vertically

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, wallTex);
    glColor3f(0.85f, 0.85f, 0.85f);   

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        float theta = 2 * M_PI * i / slices;
        float x     = radius * cos(theta);
        float z     = radius * sin(theta);

        float s = (float)i / slices * texRepeatS;

        glTexCoord2f(s, 0.0f);         glVertex3f(x, 0.0f,       z);
        glTexCoord2f(s, texRepeatT);   glVertex3f(x, wallHeight,  z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void drawPerson(float x, float y, float z, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y, z);
    float s = 0.18f;  // scale

    // head
    glPushMatrix();
        glTranslatef(0, s*2.2f, 0);
        glScalef(s*0.7f, s*0.7f, s*0.3f);
        glutSolidCube(1);
    glPopMatrix();

    // body
    glPushMatrix();
        glTranslatef(0, s*1.1f, 0);
        glScalef(s*0.9f, s*1.2f, s*0.3f);
        glutSolidCube(1);
    glPopMatrix();

    // left arm
    glPushMatrix();
        glTranslatef(-s*0.9f, s*1.3f, 0);
        glScalef(s*0.8f, s*0.3f, s*0.3f);
        glutSolidCube(1);
    glPopMatrix();

    // right arm
    glPushMatrix();
        glTranslatef(s*0.9f, s*1.3f, 0);
        glScalef(s*0.8f, s*0.3f, s*0.3f);
        glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}
void drawCrowd() {
    float colors[6][3] = {
        {0.1f, 0.7f, 0.1f},
        {1.0f, 0.5f, 0.0f},
        {0.9f, 0.9f, 0.1f},
        {0.9f, 0.1f, 0.7f},
        {0.9f, 0.1f, 0.1f},
        {0.1f, 0.4f, 0.9f},
    };

    // ── match EXACTLY the seat grid from drawSeatingRows ─────
    // lower tier: innerR=15, outerR=25, bottomY=0, topY=3, rows=10, cols=480
    // upper tier: innerR=18, outerR=25, bottomY=4, topY=6,  rows=8,  cols=640

    struct TierInfo {
        float innerR, outerR, bottomY, topY;
        int   rows, cols, up;
    };

    TierInfo tiers[2] = {
        { 15.0f, 25.0f, 0.0f, 3.0f, 10, 480, 0 },
        { 18.0f, 25.0f, 4.0f, 6.0f,  8, 640, 1 },
    };

    float s = 0.10f;  // person scale — small enough to fit on seat

    for (int t = 0; t < 2; t++) {
        TierInfo& T = tiers[t];

        int   sectionCount   = 30;
        int   colsPerSection = T.cols / sectionCount;
        float gapFraction    = 0.12f;
        float seatFrac       = 0.72f;

        for (int col = 0; col < T.cols; col++) {
            int   secIdx   = col / colsPerSection;
            float posInSec = (float)(col % colsPerSection) / colsPerSection;
            if (posInSec < gapFraction) continue;  // skip walkway

            // mid angle of this seat column
            float midAngle = (col + 0.5f) * 2 * PI / T.cols;
            float cosA     = cos(midAngle);
            float sinA     = sin(midAngle);

            for (int row = 0; row < T.rows; row++) {
                // exact same interpolation as drawSeatingRows
                float t0 = ((float)row + (1.0f - seatFrac) * 0.5f) / T.rows;
                float r0 = T.innerR + t0 * (T.outerR - T.innerR);
                float y0 = T.bottomY + t0 * (T.topY - T.bottomY) + 0.05f;

                float x = r0 * cosA;
                float z = r0 * sinA;
                float y = y0 + 0.05f;  // sit just above seat surface

                int colorIdx = (col * 3 + row * 7 + t * 11) % 6;
                float cr = colors[colorIdx][0];
                float cg = colors[colorIdx][1];
                float cb = colors[colorIdx][2];

                glPushMatrix();
                glTranslatef(x, y, z);
                // face toward field center
                glRotatef(-midAngle * 180.0f / PI + 180.0f, 0, 1, 0);

                glColor3f(cr, cg, cb);

                // head
                glPushMatrix();
                    glTranslatef(0, s*2.0f, 0);
                    glScalef(s*0.65f, s*0.65f, s*0.35f);
                    glutSolidCube(1);
                glPopMatrix();

                // body
                glPushMatrix();
                    glTranslatef(0, s*1.0f, 0);
                    glScalef(s*0.80f, s*1.0f, s*0.35f);
                    glutSolidCube(1);
                glPopMatrix();

                glPopMatrix();
            }
        }
    }
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(posX, -5 + posY, zoom + posZ);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    // ===== OUTER WALL =====
    // glColor3f(0.50f, 0.50f, 0.50f);
    // drawCylinder(25, 8);
    drawTexturedWall();
    glPushMatrix();
    glTranslatef(0, 3, 0);
    glColor3f(0.50f, 0.50f, 0.50f);
    drawCylinder(18, 1);
    glPopMatrix();
    //drawAdvertisements();
    drawBoundaryBoxes();

    // ===== LOWER TIER — white =====
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(4.0f, 4.0f);
    glColor3f(0.75f, 0.75f, 0.75f);
    drawSlantedRing(25, 15, 3, 0, 120);

    // ===== UPPER TIER — light grey =====
    glColor3f(0.62f, 0.62f, 0.62f);
    drawSlantedRing(25, 18, 6, 4, 120);
    glDisable(GL_POLYGON_OFFSET_FILL);

    // ===== ROOF — alternating stripes =====
    float thetaStep = 2 * PI / 360;
    for (int i = 0; i < 360; i++) {
        float t1 = i * thetaStep;
        float t2 = (i + 1) * thetaStep;

        float x1o = 25 * cos(t1), z1o = 25 * sin(t1);
        float x2o = 25 * cos(t2), z2o = 25 * sin(t2);
        float x1i = 18 * cos(t1), z1i = 18 * sin(t1);
        float x2i = 18 * cos(t2), z2i = 18 * sin(t2);

        if (i % 2 == 0)
            glColor3f(0.92f, 0.45f, 0.50f);
        else
            glColor3f(0.70f, 0.25f, 0.30f);

        glBegin(GL_QUADS);
            glVertex3f(x1o, 8, z1o);
            glVertex3f(x2o, 8, z2o);
            glVertex3f(x2i, 6, z2i);
            glVertex3f(x1i, 6, z1i);
        glEnd();
    }
    glColor3f(1, 1, 1);
    drawPoly(25, 8.01f, 18);
    glColor3f(1, 1, 1);
    drawPoly(25, 8.01f, 18);
    drawFlagPoles();              
    // ===== GROUND =====
    // drawGround();
    // drawinnerFeild();
    drawPitch();
    drawTexturedGround();
    drawWickets();
    glColor3f(1, 1, 1);
    drawPoly(14, 0.04f, 360);

    // ===== SEATS — drawn ON TOP of rings with Y offset to avoid z-fight =====
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    GLfloat lpos[] = { 0, 60, 0, 1 };
    GLfloat ldif[] = { 1.0f, 1.0f, 1.0f, 1 };
    GLfloat lamb[] = { 0.50f, 0.50f, 0.50f, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  ldif);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lamb);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    drawSeatingRows(15.0f, 25.0f, 0.0f, 3.0f, 10, 480,0);
    drawSeatingRows(18.0f, 25.0f, 4.0f, 6.0f,  8, 640,1);
    glDisable(GL_CULL_FACE);

    glDisable(GL_LIGHTING);
    drawCrowd();

 // ===== 6 FLOODLIGHTS =====
// ===== 6 FLOODLIGHTS =====
for (int i = 0; i < 6; i++) {
    float angle = i * 2 * PI / 6.0f;
    float wx = 26.5f * cos(angle);
    float wz = 26.5f * sin(angle);

    glPushMatrix();
    glTranslatef(wx, 0, wz);

    // ── POLE ──────────────────────────────────────────────
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
        glTranslatef(0, 7.5f, 0);
        glScalef(0.2f, 15.0f, 0.2f);
        glutSolidCube(1);
    glPopMatrix();

    // ── LIGHT BOX + RECTANGLE PANEL ───────────────────────
    glPushMatrix();
        glTranslatef(0, 15.0f, 0);
        // Rotate so the front face (-Z) points toward stadium center
        glRotatef((-angle * 180.0f / PI) + 90.0f, 0, 1, 0);

        // Dark box housing
        glColor3f(0.15f, 0.15f, 0.15f);
        glPushMatrix();
            glScalef(4.0f, 3.0f, 0.6f);
            glutSolidCube(1);
        glPopMatrix();
if (lightsOn) {
    glColor3f(1.0f, 1.0f, 0.85f);
    glPushMatrix();
        glTranslatef(0, 0, -0.55f);
        glBegin(GL_QUADS);
            glVertex3f(-1.5f, -1.8f, 0);
            glVertex3f( 1.5f, -1.8f, 0);
            glVertex3f( 1.5f,  1.8f, 0);
            glVertex3f(-1.5f,  1.8f, 0);
        glEnd();
    glPopMatrix();
} else {
    // dark off state
    glColor3f(0.15f, 0.15f, 0.10f);
    glPushMatrix();
        glTranslatef(0, 0, -0.55f);
        glBegin(GL_QUADS);
            glVertex3f(-1.5f, -1.8f, 0);
            glVertex3f( 1.5f, -1.8f, 0);
            glVertex3f( 1.5f,  1.8f, 0);
            glVertex3f(-1.5f,  1.8f, 0);
        glEnd();
    glPopMatrix();
}
    glPopMatrix();  // end light box

        // Subtle border around the rectangle
        glColor3f(0.4f, 0.4f, 0.4f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            glVertex3f(-rW,  rH, rZ);
            glVertex3f( rW,  rH, rZ);
            glVertex3f( rW, -rH, rZ);
            glVertex3f(-rW, -rH, rZ);
        glEnd();
        glLineWidth(1.0f);

}  
if (lightsOn) {
    drawLightCones();
}
     glutSwapBuffers();
}
// Controls
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        // zoom in/out
        case 'w': zoom += 2;    break;
        case 's': zoom -= 2;    break;

        // move X
        case 'a': posX -= 1.0f; break;
        case 'd': posX += 1.0f; break;

        // move Y
        case 'q': posY += 1.0f; break;
        case 'e': posY -= 1.0f; break;

        // move Z
        case 'z': posZ += 1.0f; break;
        case 'x': posZ -= 1.0f; break;

        case 'l': lightsOn = !lightsOn; break;  // L key toggles lights
        case 'L': lightsOn = !lightsOn; break;

        // ESC
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    // arrow keys rotate view
    if (key == GLUT_KEY_LEFT)  angleY -= 5;
    if (key == GLUT_KEY_RIGHT) angleY += 5;
    if (key == GLUT_KEY_UP)    angleX -= 5;
    if (key == GLUT_KEY_DOWN)  angleX += 5;

    glutPostRedisplay();
}


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
    gluPerspective(60, (float)w / h, 1, 200);

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
