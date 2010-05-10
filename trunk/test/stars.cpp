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

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
Font*       font = NULL;
Image*		image = NULL;
Texture*	texture = NULL;

static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;
static int  is_twinkled = 1;
static GLfloat zoom = -15.0f;   /* Viewing Distance Away From Stars */
static GLfloat tilt = 90.0f;    /* Tilt The View */

/* Define the star structure */
typedef struct
{
    int r, g, b;   /* Stars Color */
    GLfloat dist;  /* Stars Distance From Center */
    GLfloat angle; /* Stars Current Angle */
} star;

/* Number of stars */
#define NUM 50
star stars[NUM];       /* Make an array of size 'NUM' of stars */

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
static int  is_initialized = false;
static HWND	hwnd;
#endif

// draw all stars
static void drawScene() {
    GLfloat spin = 0;

    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4] = {1, 0, 2, 3}; /* QUAD to TRIANGLE_STRIP conversion; */

    glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	glClearDepthf(1.0f);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE ); //GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE
    glEnable(GL_BLEND);

    /* Select Our Texture */
    glBindTexture(GL_TEXTURE_2D, texture->textureId);
    glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Loop Through All The Stars */
    for (int loop = 0; loop < NUM; loop++) {
        /* Reset The View Before We Draw Each Star */
        glLoadIdentity();
        /* Zoom Into The Screen (Using The Value In 'zoom') */
        glTranslatef(0.0f, 0.0f, zoom);

        /* Tilt The View (Using The Value In 'tilt') */
        glRotatef(tilt, 1.0f, 0.0f, 0.0f);
        /* Rotate To The Current Stars Angle */
        glRotatef(stars[loop].angle, 0.0f, 1.0f, 0.0f);

        /* Move Forward On The X Plane */
        glTranslatef(stars[loop].dist, 0.0f, 0.0f);

        /* Cancel The Current Stars Angle */
        glRotatef(-stars[loop].angle, 0.0f, 1.0f, 0.0f);
        /* Cancel The Screen Tilt */
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f);

        /* Twinkling Stars Enabled */
        if (is_twinkled) {
            /* Assign A Color Using Bytes */
            glColor4f((GLfloat)stars[(NUM-loop)-1].r/255.0f,
                      (GLfloat)stars[(NUM-loop)-1].g/255.0f,
                      (GLfloat)stars[(NUM-loop)-1].b/255.0f, 0.8f);

            /* Fill texture and vertex arrays */
            texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
            vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=0.0f;
            texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
            vertices[1][0]=1.0f; vertices[1][1]=-1.0f; vertices[1][2]=0.0f;
            texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
            vertices[2][0]=1.0f; vertices[2][1]=1.0f; vertices[2][2]=0.0f;
            texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
            vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=0.0f;

            /* Draw one textured plane using two stripped triangles */
            glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
        }

        /* Rotate The Star On The Z Axis */
        glRotatef(spin, 0.0f, 0.0f, 1.0f);

        /* Assign A Color Using Bytes */
        glColor4f((GLfloat)stars[loop].r/255.0f,
                  (GLfloat)stars[loop].g/255.0f,
                  (GLfloat)stars[loop].b/255.0f, 0.8f);

        /* Fill texture and vertex arrays */
        texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
        vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=0.0f;
        texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
        vertices[1][0]=1.0f; vertices[1][1]=-1.0f; vertices[1][2]=0.0f;
        texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
        vertices[2][0]=1.0f; vertices[2][1]=1.0f; vertices[2][2]=0.0f;
        texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
        vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=0.0f;

        /* Draw one textured plane using two stripped triangles */
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

        /* Used To Spin The Stars */
        spin += 0.01f;
        /* Changes The Angle Of A Star */
        stars[loop].angle+=(float)loop/NUM;
        /* Changes The Distance Of A Star */
        stars[loop].dist-=0.01f;

        /* Is The Star In The Middle Yet */
        if (stars[loop].dist<0.0f) {
            /* Move The Star 5 Units From The Center */
            stars[loop].dist+=5.0f;
            /* Give It A New Red Value */
            stars[loop].r=rand()%256;
            /* Give It A New Green Value */
            stars[loop].g=rand()%256;
            /* Give It A New Blue Value */
            stars[loop].b=rand()%256;
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
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
#if DEBUG
        TCHAR szError[32];
        wsprintf (szError, TEXT("WM_KEYDOWN: 0x%2x"), wParam);
        MessageBox (hwnd, szError, TEXT("Debug"), MB_OK);
#endif
        if (wParam == VK_ESCAPE || wParam == 0x51 || wParam == 0x86) {
            is_done = 0;
		} else if (wParam == 0x54) {
			is_twinkled = !is_twinkled;
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
		if (is_initialized) {
			world->setSize(width, height);
		}
        break;

    default:
        useDefWindowProc = 1;
    }

    if (useDefWindowProc)
        return DefWindowProc(wnd, message, wParam, lParam);

    return 0;
}

#define	WINDOW_CLASS    TEXT("F3D_Stars")
#define	WINDOW_TITLE           TEXT("Stars")
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
                        WINDOW_TITLE,
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
        MessageBox(hwnd, TEXT("Init world error!"), TEXT("Error"), MB_OK);

        return 0;
    }
    world->setSize(width, height);

	//after create world, set is_initialized to true
	is_initialized = true;
#else
    world->init();
#endif

    image = new Image();
    texture = image->loadTexture("star.bmp");

    camera = new Camera();
    camera->setEye(0.0f, 0.0f, 15.0f);

    world->setCamera(camera);

	/* Create A Loop That Goes Through All The Stars */
    for (int loop=0; loop < NUM; loop++) {
        /* Start All The Stars At Angle Zero */
        stars[loop].angle=0.0f;

        /* Calculate Distance From The Center */
        stars[loop].dist=((float)loop/NUM)*5.0f;
        /* Give star[loop] A Random Red Intensity */
        stars[loop].r=rand() % 256;
        /* Give star[loop] A Random Green Intensity */
        stars[loop].g=rand() % 256;
        /* Give star[loop] A Random Blue Intensity */
        stars[loop].b=rand() % 256;
    }

    font = new Font(16, 16, 24, 36, "font.bmp");

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

        drawScene();

        //printf("strFps: %s\n", strFps);
        //font->drawString(4, 4, strFps);

        world->finishRender();

        fps++;
#ifdef ANDROID
        gettimeofday(&timeNow, NULL);
        interval = CLOCK(timeNow) - i_time;
        if (interval >= 180000) {
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
    delete world;

    return 0;
}
