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
#include "image.h"
#include "font.h"
#include "skydome.h"
#include "plane.h"
#include "model_ms3d.h"

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
Image*		image = NULL;
Font*		font = NULL;
Skydome*    skydome = NULL;
Plane*      plane = NULL;
ModelMS3D*  model = NULL;

static float angle = 0.0f;
static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;

#ifdef ANDROID
static int  i_time = 0;
static int  is_changed = false;
static struct timeval timeNow;
#elif (defined(WIN32) || defined(_WIN32_WCE))
static DWORD i_time = 0;
static int	width = 640;
static int	height = 480;
static int  is_initialized = false;
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
#if DEBUG
        TCHAR szError[32];
        wsprintf (szError, TEXT("WM_KEYDOWN: 0x%2x"), wParam);
        MessageBox (hwnd, szError, TEXT("Debug"), MB_OK);
#endif
        if (wParam == VK_ESCAPE || wParam == 0x86 || wParam == 0x51) {
            is_done = 0;
        }

        useDefWindowProc = 1;
        break;

    case WM_KEYUP:
        useDefWindowProc = 1;
        break;

    case WM_SIZE:
        GetWindowRect(hwnd, &rc);
        width = rc.right;
        height = rc.bottom;
		if (is_initialized) {
			world->resize(width, height);
		}

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
#define	TITLE           TEXT("Skytest")
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
    if (!hwnd) {
#if DEBUG
        MessageBox(hwnd, TEXT("CreateWindow error!"), TEXT("Error"), MB_OK);
#endif // ! DEBUG
        return FALSE;
	}

	ShowWindow(hwnd, cmdShow);

#ifdef _WIN32_WCE
    SHFullScreen(hwnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
    MoveWindow(hwnd, 0, 0, width, height, TRUE);
#endif

    UpdateWindow(hwnd);
#else
//android main entry
int main(int argc, char *argv[]) {
#endif
    printf("world->init()...\n");

    world = new World();
    //world->setBgColor(0.5f, 0.5f, 0.5f, 0.0f);

#if (defined(WIN32) || defined(_WIN32_WCE))
    world->setSize(width, height); //before init world, set windows size
    if (!world->init(hwnd)) {
        MessageBox(hwnd, TEXT("Init world error!"), TEXT("Error"), MB_OK);

        return 0;
    }

	//after create world, set is_initialized to true
	is_initialized = true;
#else
    world->init();
#endif

#if (defined(_WIN32_WCE) && defined(DEBUG))
    MessageBox(hwnd, TEXT("Init world OK!"), TEXT("Info"), MB_OK);
#endif

    camera = new Camera();
    camera->setEye(60.0f, 30.0f, 60.0f);
    camera->setCenter(0.0f, 15.0f, 0.0f);

    world->setCamera(camera);

    image = new Image();
    Texture* texture0 = image->loadTexture("clouds.bmp");
    Texture* texture1 = image->loadTexture("floor.bmp");
    Texture* texture2 = image->loadTexture("DM_Base.bmp");
	Texture* texture3 = image->loadTexture("DM_Face.bmp");

    plane = new Plane(4, 4, 64.0f);
    if (texture1 != NULL)
        plane->setTextureId(texture1->textureId);
    plane->setPosition(-128.0f, 0.0f, -128.0f);

    skydome = new Skydome(128, 30.0f, 10.0f);
    if (texture0 != NULL)
        skydome->setTextureId(texture0->textureId);
    //skydome->setPosition(0.0f, (float)(-128 * sinf(DTOR * 10.0f)), 0.0f);
    printf("%.2f\n", (float)(-128 * sinf(DTOR * 10.0f)));

    model = new ModelMS3D();
    model->loadModel("run.ms3d");

    model->setScale(0.5f, 0.5f, 0.5f);
    model->setPosition(20.0f, 20.0f, -40.0f);

    if (texture2 != NULL)
        model->setTextureId(texture2->textureId);
    if (texture3 != NULL)
        model->setTextureId(texture3->textureId, 1);

    font = new Font(16, 16, 24, 36, "font.bmp");

    printf("start loop...\n");
#if ((defined(_WIN32_WCE) || defined(WIN32)) && defined(DEBUG))
	MessageBox(hwnd, TEXT("start loop!"), TEXT("Info"), MB_OK);
#endif

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

        plane->renderModel();

        //glTranslatef(0.0f, -30.0f, 0.0f);
        skydome->setRotate(-90.0f, 0.0f, angle);
        skydome->renderModel();

		model->setRotate(0.0f, angle * 5.0f, 0.0f);
        model->renderModel();

        //printf("strFps: %s\n", strFps);
        font->drawString(4, 4, strFps);

        world->finishRender();

        fps++;
        angle += 0.05f;
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
    delete plane;
    delete skydome;
    delete model;
    delete image;
    delete world;

    return 0;
}
