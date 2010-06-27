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
Image*      image = NULL;

static GLfloat rotation = 0.0f;
static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;
static GLfloat alpha = 0.0f;

#ifdef ANDROID
static int  i_time = 0;
static struct timeval timeNow;
#elif (defined(WIN32) || defined(_WIN32_WCE))
static DWORD i_time = 0;
static int	width = 480;
static int	height = 640;
static int  is_initialized = false;
static HWND	hwnd;
static int  camera_idx = 1;
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
        MessageBox (hwnd, szError, TEXT("Debug"), MB_OK);
#endif
        if (wParam == VK_ESCAPE || wParam == 0x51 || wParam == 0x86) {
            is_done = 0;
        } else if (wParam == VK_UP || wParam == VK_DOWN) {
            camera_idx = (camera_idx ? 0 : 1);
            world->setActiveCamera(camera_idx);
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
			world->resize(width, height);
		}
        break;

    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
        printf("%s event, x: %d, y: %d\n", message == WM_LBUTTONUP ? "WM_LBUTTONUP" : "WM_LBUTTONDOWN", LOWORD(lParam), HIWORD(lParam));
        if (message == WM_LBUTTONUP) {
            camera_idx = (camera_idx ? 0 : 1);
            world->setActiveCamera(camera_idx);
        }

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
    world->setSize(width, height);
    if (!world->init(hwnd)) {
        MessageBox(hwnd, TEXT("Init world error!"), TEXT("Error"), MB_OK);

        return 0;
    }

	//after create world, set is_initialized to true
	is_initialized = true;
#else
    world->init();
#endif

    world->setCameraCount(2);

    camera = world->getActiveCamera();
    camera->setEye(0.0f, 0.0f, 20.0f);

    camera = world->getCamera(1);
    camera->setEye(0.0f, 20.0f, 20.0f);

    world->setActiveCamera(1);

    //triangle data
    GLfloat vertices0[9] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    GLfloat vertices1[9] = {
        0.0f, -1.0f, -1.0f,
        0.0f, -1.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };
    GLubyte colors[12] = {
        255, 0, 0, 0,
        0, 255, 0, 0,
        0, 0, 255 ,0
    };

    model = new Model();
    model->setMeshCount(2);
    //init first triangle
    model->setVertices(vertices0, 9 * sizeof(GLfloat));
    model->setColors(colors, 12 * sizeof(GLubyte));
    model->setTriangleNums(1, 0);
    //init second triangle
    colors[2] = colors[6] = 128;
    model->setVertices(vertices1, 9 * sizeof(GLfloat), 1);
    model->setColors(colors, 12 * sizeof(GLubyte), 1);
    model->setTriangleNums(1, 1);

    model->setPosition(0.0f, 0.0f, -5.0f);
    model->setScale(5.0f, 5.0f, 5.0f);

    printf("create triangle OK!\n");

    Color4f color = {1.0f, 1.0f, 1.0f, 0.2f};
    font = new Font(16, 16, "font.bmp");
    font->setFontColor(&color);

    image = new Image("f3d_logo.bmp");
    color.alpha = alpha;
    image->setImageColor(&color);

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

        model->setRotate(0.0f, rotation, 0.0f);
        model->renderModel();

        //printf("strFps: %s\n", strFps);
        color.alpha = 1.0;
        color.blue = 0.0f;
        font->setFontColor(&color);
        font->drawString(4, world->getHeight() - 40, 24, 36, strFps);

        color.blue = 1.0f;
        font->setFontColor(&color);
        font->drawString(4, 4, 24, 36, "Tap screen!");

        //draw f3d logo, at (width - display width - 4)
        image->drawImage(world->getWidth() - 132, 4, 128, 128);

        world->finishRender();

        fps++;
        rotation += 2.0f;

        if (alpha <= 1.0f) {
            alpha += 0.002f;
            color.alpha = alpha;
            image->setImageColor(&color);
        }
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
    delete image;

    return 0;
}
