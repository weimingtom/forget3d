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
#include "model.h"
#include "font.h"

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
Model*      model = NULL;
Font*       font = NULL;

static int  rotation = 0;
static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;

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

#define	WINDOW_CLASS    TEXT("F3D_Triangle")
#define	WINDOW_TITLE           TEXT("Triangle")
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
#endif

    camera = new Camera();
    camera->setEye(0.0f, 0.0f, 100.0f);

    world->setCamera(camera);

    model = new Model();

    GLfloat vertexArray[9] = {
        -25.0f, -25.0f, 0.0f,
        25.0f, -25.0f, 0.0f,
        0.0f, 25.0f, 0.0f
    };
    GLubyte colorArray[12] = {
        255, 0, 0, 0,
        0, 255, 0, 0,
        0, 0, 255 ,0
    };
    GLfloat *vertices = new GLfloat[9];
    memcpy(vertices, vertexArray, 9 * sizeof(GLfloat));
    GLubyte *colors = new GLubyte[12];
    memcpy(colors, colorArray, 12 * sizeof(GLubyte));

    model->setVertices(vertices);
    model->setColors(colors);
    model->setTriangleNums(1);

    font = new Font(16, 16, 12, 18, "font.bmp");

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

        glRotatex(FIXED(rotation++), 0, ONE,0);

        model->renderModel();

        //printf("strFps: %s\n", strFps);
        font->drawString(4, 4, strFps);

        world->finishRender();

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
    delete model;
    delete world;

    return 0;
}
