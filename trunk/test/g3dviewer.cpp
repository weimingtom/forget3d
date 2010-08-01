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
#include "model_g3d.h"
#include "font.h"
#include "plane.h"
#include "skydome.h"

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
ModelG3D*   model = NULL;
Font*		font = NULL;
Plane*      plane = NULL;
Skydome*    skydome = NULL;

static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;
static float angle = 0.0f;

#ifdef ANDROID
static int  i_time = 0;
static struct timeval timeNow;
#elif (defined(WIN32) || defined(_WIN32_WCE))
static DWORD i_time = 0;
static int	width = 480;
static int	height = 640;
static HWND	hwnd;
static int  is_initialized = false;
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
#define	TITLE           TEXT("G3dViewer")
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

    world = World::getInstance();
    world->setBgColor(0.5f, 0.5f, 0.5f, 0.0f);

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

    camera = world->getActiveCamera();
    camera->setEye(30.0f, 15.0f, 30.0f);
    camera->setCenter(0.0f, 5.0f, 0.0f);

    Texture* texture0 = Image::loadTexture("floor.bmp");
    Texture* texture1 = Image::loadTexture("clouds.bmp");
    Texture* texture2 = Image::loadTexture("guard.bmp");

    plane = new Plane(4, 4, 128.0f);
    if (texture0 != NULL)
        plane->setTextureId(texture0->textureId);
    plane->setPosition(-256.0f, -28.0f, -256.0f);

    skydome = new Skydome(256, 30.0f, 10.0f);
    if (texture1 != NULL)
        skydome->setTextureId(texture1->textureId);
    skydome->setPosition(0.0f, (float)(-256 * sinf(DTOR * 10.0f)) - 28.0f, 0.0f);

    model = new ModelG3D();
    model->loadModel("guard.g3d");
    model->setScale(10.0f, 10.0f, 10.0f);
    if (texture2 != NULL)
        model->setTextureId(texture2->textureId);

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

		skydome->setRotate(-90.0f, 0.0f, angle);
        skydome->renderModel();
        plane->renderModel();

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
            printf("fps\t\t: %.2f\n", fps * 1.0f / 20);
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

    delete model;
    delete font;
	delete plane;
	delete skydome;
	//release world instance
    World::release();

    return 0;
}
