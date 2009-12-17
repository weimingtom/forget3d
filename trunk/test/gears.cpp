/*****************************************************************************
 * Copyright (C) 2009 The Forget3D Project by Martin Foo (i25ffz@hotmail.com)
 * ALL RIGHTS RESERVED
 *
 * License I
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITHOUT a fee is granted under following requirements:
 * - You make no money using this software.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * Licence II
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITH a fee is granted under following requirements:
 * - As soon as you make money using this software, you have to pay a
 *   licence fee. Until this point of time, you can use this software
 *   without a fee.
 * 	 Please contact Martin Foo (i25ffz@hotmail.com) for further details.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS OR
 * REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT THE AUTHORS HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/

#ifdef ANDROID
#include <sys/time.h>
#elif defined(_WIN32_WCE)
#include <aygshell.h>
#endif

#include "f3d.h"
#include "world.h"
#include "camera.h"
#include "font.h"


typedef struct {
    GLfloat pos[3];
    GLfloat norm[3];
} vertex_t;

typedef struct {
    vertex_t *vertices;
    GLshort *indices;
    int nvertices;
    int nindices;
} gear_t;

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
Image*		image = NULL;
Font*		font = NULL;
Light*      light = NULL;

static int is_done = 0;
static int fps = 0;
static char	strFps[16];
static int  interval = 0;
static GLfloat rotx = 20.0f, roty = 30.0f, rotz = 0.0f;
static gear_t *gear1, *gear2, *gear3;
static GLfloat angle = 0.0f;
static GLfloat pos[4] = {5.0f, 5.0f, 10.0f, 0.0f};
static GLfloat red[4] = {0.8f, 0.1f, 0.0f, 1.0f};
static GLfloat green[4] = {0.0f, 0.8f, 0.2f, 1.0f};
static GLfloat blue[4] = {0.2f, 0.2f, 1.0f, 1.0f};

#ifdef ANDROID
static int  i_time = 0;
static struct timeval timeNow;
#elif (defined(WIN32) || defined(_WIN32_WCE))
static DWORD i_time = 0;
#ifdef WIN32
static int	width = 640;
static int	height = 480;
#else
static int	width = 240;
static int	height = 320;
#endif
static HWND	hwnd;
#endif

static gear_t* gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
                    GLint teeth, GLfloat tooth_depth) {
    GLint i, j;
    GLfloat r0, r1, r2;
    GLfloat ta, da;
    GLfloat u1, v1, u2, v2, len;
    GLfloat cos_ta, cos_ta_1da, cos_ta_2da, cos_ta_3da, cos_ta_4da;
    GLfloat sin_ta, sin_ta_1da, sin_ta_2da, sin_ta_3da, sin_ta_4da;
    GLshort ix0, ix1, ix2, ix3, ix4, ix5;
    vertex_t *vt, *nm;
    GLshort *ix;

    gear_t *gear = (gear_t *)malloc(sizeof(gear_t));
    gear->nvertices = teeth * 40;
    gear->nindices = teeth * 22 * 3;
    gear->vertices = (vertex_t *)calloc(gear->nvertices, sizeof(vertex_t));
    gear->indices = (GLshort *)calloc(gear->nindices, sizeof(GLshort));

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth / 2.0f;
    r2 = outer_radius + tooth_depth / 2.0f;
    da = 2.0f * M_PI / teeth / 4.0f;

    vt = gear->vertices;
    nm = gear->vertices;
    ix = gear->indices;

#define VERTEX(x,y,z) ((vt->pos[0] = x),(vt->pos[1] = y),(vt->pos[2] = z), \
                       (vt++ - gear->vertices))
#define NORMAL(x,y,z) ((nm->norm[0] = x),(nm->norm[1] = y),(nm->norm[2] = z), \
                       (nm++ - gear->vertices))
#define INDEX(a,b,c) ((*ix++ = a),(*ix++ = b),(*ix++ = c))

    for (i = 0; i < teeth; i++) {
        ta = i * 2.0f * M_PI / teeth;

        cos_ta = (float)cos(ta);
        cos_ta_1da = (float)cos(ta + da);
        cos_ta_2da = (float)cos(ta + 2 * da);
        cos_ta_3da = (float)cos(ta + 3 * da);
        cos_ta_4da = (float)cos(ta + 4 * da);
        sin_ta = (float)sin(ta);
        sin_ta_1da = (float)sin(ta + da);
        sin_ta_2da = (float)sin(ta + 2 * da);
        sin_ta_3da = (float)sin(ta + 3 * da);
        sin_ta_4da = (float)sin(ta + 4 * da);

        u1 = r2 * cos_ta_1da - r1 * cos_ta;
        v1 = r2 * sin_ta_1da - r1 * sin_ta;
        len = (float)sqrt(u1 * u1 + v1 * v1);
        u1 /= len;
        v1 /= len;
        u2 = r1 * cos_ta_3da - r2 * cos_ta_2da;
        v2 = r1 * sin_ta_3da - r2 * sin_ta_2da;

        /* front face */
        ix0 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          width * 0.5f);
        ix1 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          width * 0.5f);
        ix2 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          width * 0.5f);
        ix3 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      width * 0.5f);
        ix4 = VERTEX(r0 * cos_ta_4da,      r0 * sin_ta_4da,      width * 0.5f);
        ix5 = VERTEX(r1 * cos_ta_4da,      r1 * sin_ta_4da,      width * 0.5f);
        for (j = 0; j < 6; j++) {
            NORMAL(0.0f,                  0.0f,                  1.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
        INDEX(ix2, ix3, ix4);
        INDEX(ix3, ix5, ix4);

        /* front sides of teeth */
        ix0 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          width * 0.5f);
        ix1 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      width * 0.5f);
        ix2 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      width * 0.5f);
        ix3 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(0.0f,                  0.0f,                  1.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);

        /* back face */
        ix0 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          -width * 0.5f);
        ix1 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          -width * 0.5f);
        ix2 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      -width * 0.5f);
        ix3 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          -width * 0.5f);
        ix4 = VERTEX(r1 * cos_ta_4da,      r1 * sin_ta_4da,      -width * 0.5f);
        ix5 = VERTEX(r0 * cos_ta_4da,      r0 * sin_ta_4da,      -width * 0.5f);
        for (j = 0; j < 6; j++) {
            NORMAL(0.0f,                  0.0f,                  -1.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
        INDEX(ix2, ix3, ix4);
        INDEX(ix3, ix5, ix4);

        /* back sides of teeth */
        ix0 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      -width * 0.5f);
        ix1 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      -width * 0.5f);
        ix2 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          -width * 0.5f);
        ix3 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      -width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(0.0f,                  0.0f,                  -1.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);

        /* draw outward faces of teeth */
        ix0 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          width * 0.5f);
        ix1 = VERTEX(r1 * cos_ta,          r1 * sin_ta,          -width * 0.5f);
        ix2 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      width * 0.5f);
        ix3 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      -width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(v1,                   -u1,                  0.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
        ix0 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      width * 0.5f);
        ix1 = VERTEX(r2 * cos_ta_1da,      r2 * sin_ta_1da,      -width * 0.5f);
        ix2 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      width * 0.5f);
        ix3 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      -width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(cos_ta,               sin_ta,               0.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
        ix0 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      width * 0.5f);
        ix1 = VERTEX(r2 * cos_ta_2da,      r2 * sin_ta_2da,      -width * 0.5f);
        ix2 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      width * 0.5f);
        ix3 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      -width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(v2,                   -u2,                  0.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
        ix0 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      width * 0.5f);
        ix1 = VERTEX(r1 * cos_ta_3da,      r1 * sin_ta_3da,      -width * 0.5f);
        ix2 = VERTEX(r1 * cos_ta_4da,      r1 * sin_ta_4da,      width * 0.5f);
        ix3 = VERTEX(r1 * cos_ta_4da,      r1 * sin_ta_4da,      -width * 0.5f);
        for (j = 0; j < 4; j++) {
            NORMAL(cos_ta,               sin_ta,               0.0f);
        }
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);

        /* draw inside radius cylinder */
        ix0 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          -width * 0.5f);
        ix1 = VERTEX(r0 * cos_ta,          r0 * sin_ta,          width * 0.5f);
        ix2 = VERTEX(r0 * cos_ta_4da,      r0 * sin_ta_4da,      -width * 0.5f);
        ix3 = VERTEX(r0 * cos_ta_4da,      r0 * sin_ta_4da,      width * 0.5f);
        NORMAL(-cos_ta,              -sin_ta,              0.0f);
        NORMAL(-cos_ta,              -sin_ta,              0.0f);
        NORMAL(-cos_ta_4da,          -sin_ta_4da,          0.0f);
        NORMAL(-cos_ta_4da,          -sin_ta_4da,          0.0f);
        INDEX(ix0, ix1, ix2);
        INDEX(ix1, ix3, ix2);
    }

    return gear;
}

void drawGear(gear_t* gear, Light* light, GLfloat* color) {
    light->setAmbient(color);
    light->setDiffuse(color);
    light->initLight();

    glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), gear->vertices[0].pos);
    glNormalPointer(GL_FLOAT, sizeof(vertex_t), gear->vertices[0].norm);
    glDrawElements(GL_TRIANGLES, gear->nindices, GL_UNSIGNED_SHORT, gear->indices);
}

#if (defined(WIN32) || defined(_WIN32_WCE))
static LRESULT CALLBACK WndProc(HWND wnd, UINT message,
                                WPARAM wParam, LPARAM lParam) {
    RECT rc;
    int useDefWindowProc = 0;

    switch (message) {
    case WM_CLOSE:
        DestroyWindow(wnd);
        is_done = 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        is_done = 0;
        break;

    case WM_KEYDOWN:
#if 0
        TCHAR szError[32];
        wsprintf (szError, TEXT("WM_KEYDOWN: 0x%2x"), wParam);
        MessageBox (NULL, szError, TEXT("Debug"), MB_OK);
#endif
        if (wParam == VK_ESCAPE) {
            is_done = 0;
        }

        useDefWindowProc = 1;
        break;

    case WM_KEYUP:
        useDefWindowProc = 1;
        break;

    case WM_SIZE:
        GetClientRect(hwnd, &rc);
        width = rc.right;
        height = rc.bottom;
        break;

    default:
        useDefWindowProc = 1;
    }

    if (useDefWindowProc)
        return DefWindowProc(wnd, message, wParam, lParam);

    return 0;
}
#endif

#if (defined(WIN32) || defined(_WIN32_WCE))
#define	WINDOW_CLASS    TEXT("F3D")
#define	TITLE           TEXT("Gears")
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance,
                   LPTSTR cmdLine, int cmdShow) {
    MSG msg;
    WNDCLASS wc;
    DWORD windowStyle;
    int windowX, windowY;

    // register class
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = NULL;
    wc.hCursor = 0;
#ifdef WIN32
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
#else
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
#endif
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS;

    if (!RegisterClass(&wc)) {
        //GetLastError()
        return FALSE;
    }

    // init instance
    windowStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE;
#ifdef _WIN32_WCE
    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);
    windowX = windowY = 0;
#else
    windowStyle |= WS_OVERLAPPEDWINDOW;
    windowX = CW_USEDEFAULT;
    windowY = 0;
#endif
    hwnd = CreateWindow(WINDOW_CLASS,
                        TITLE,
                        windowStyle,
                        windowX,
                        windowY,
                        width,
                        height,
                        NULL,
                        NULL,
                        instance,
                        NULL);
    if (!hwnd)
        return FALSE;

    ShowWindow(hwnd, cmdShow);

#ifdef _WIN32_WCE
    SHFullScreen(hwnd,
                 SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
    MoveWindow(hwnd, 0, 0, width, height, TRUE);
#endif

    UpdateWindow(hwnd);
#else
//android main entry
int main(int argc, char *argv[]) {
#endif
    printf("world->init()...\n");

    world = new World();
#if (defined(WIN32) || defined(_WIN32_WCE))
    if (!world->init(hwnd)) {
#ifdef _WIN32_WCE
        MessageBox(hwnd, _T("Init world error!"), _T("Error"), MB_OK);
#else
        MessageBox(hwnd, "Init world error!", "Error", MB_OK);
#endif
        return 0;
    }
    world->setSize(width, height);
#else
    world->init();
    printf("world->init() OK!\n");
#endif

    camera = new Camera();
#if (defined(_WIN32_WCE) || defined(ANDROID))
    camera->setEye(3.0f, 0.0f, 18.0f);
#else
	camera->setEye(-2.0f, 0.0f, 15.0f);
#endif

    light = new Light();
    light->setPosition(pos);
    //set light & camera
    world->setCamera(camera);
    world->setLight(light);

    font = new Font(16, 16, 12, 18, "font.bmp");

    /* make the gears */
    gear1 = gear(1.0f, 4.0f, 1.0f, 20, 0.7f);
    gear2 = gear(0.5f, 2.0f, 2.0f, 10, 0.7f);
    gear3 = gear(1.3f, 2.0f, 0.5f, 10, 0.7f);

    printf("start loop...\n");
    is_done = 1;
#ifdef ANDROID
    gettimeofday(&timeNow, NULL);
    i_time = CLOCK(timeNow);
#elif (defined(WIN32) || defined(_WIN32_WCE))
    i_time = GetTickCount();
#endif
    sprintf(strFps, "Fps:%.2f", 0.0f);
    printf("strFps: %s\n", strFps);

    while (is_done) {
#if (defined(WIN32) || defined(_WIN32_WCE))
        while (PeekMessage(&msg, hwnd, 0, 0, PM_NOREMOVE)) {
            if (GetMessage(&msg, hwnd, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                is_done = 0;
                break;
            }
        }
#endif
        world->prepareRender();

        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glRotatef(rotx, 1.0f, 0.0f, 0.0f);
        glRotatef(roty, 0.0f, 1.0f, 0.0f);
        glRotatef(rotz, 0.0f, 0.0f, 1.0f);

        glPushMatrix();
        glTranslatef(-3.0f, -2.0f, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        drawGear(gear1, light, &red[0]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3.1f, -2.0f, 0.0f);
        glRotatef(-2.0f * angle - 9.0f, 0.0f, 0.0f, 1.0f);
        drawGear(gear2, light, &green[0]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-3.1f, 4.2f, 0.0f);
        glRotatef(-2.0f * angle - 25.0f, 0.0f, 0.0f, 1.0f);
        drawGear(gear3, light, &blue[0]);
        glPopMatrix();

        glDisable(GL_LIGHTING);

        font->drawString(4, 4, strFps);

        world->finishRender();

        angle += 3.0f;
        fps++;
#ifdef ANDROID
        gettimeofday(&timeNow, NULL);
        interval = CLOCK(timeNow) - i_time;
        if (interval >= 20000) {
            is_done = 0;
            printf("fps\t\t: %.2f\n", fps * 1000.0f / interval);
        }
        if (((CLOCK(timeNow) - i_time) / 1000) % 2 == 0 && interval > 0)
            sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
#elif (defined(WIN32) || defined(_WIN32_WCE))
        interval = GetTickCount() - i_time;

        if (interval >= 500) {
            sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
            //reset all time variables after get strFps
            interval = 0;
            i_time = GetTickCount();
            fps = 0;
        }
#endif
    }

    delete font;
    delete image;
    delete light;
    delete world;

    return 0;
}
