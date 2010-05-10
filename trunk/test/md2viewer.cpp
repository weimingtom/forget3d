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
#include "model_md2.h"
#include "font.h"
#include "plane.h"
#include "skydome.h"
#include "fog.h"

using namespace F3D;

//F3D variables
World*		world = NULL;
Camera*		camera = NULL;
Image*		image = NULL;
ModelMD2*	model = NULL;
ModelMD2*	weapon = NULL;
Font*		font = NULL;
Plane*      plane = NULL;
Skydome*    skydome = NULL;
Fog*        fog = NULL;

static char	strFps[16];
static char	strAction[32];
static int	fps = 0;
static int  is_done = 0;
static int	action_idx = 0;
static int  interval = 0;
static float angle = 0.0f;

#ifdef ANDROID
static int  i_time = 0;
static int  is_changed = false;
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
static int  is_foged = true;
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

	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
        DestroyWindow(wnd);
        is_done = 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        is_done = 0;
        break;

    case WM_KEYDOWN:
#ifdef DEBUG
		TCHAR szError[32];
		wsprintf (szError, TEXT("WM_KEYDOWN: 0x%2x"), wParam);
		MessageBox (NULL, szError, TEXT("Debug"), MB_OK);
#endif
        if (wParam == VK_ESCAPE || wParam == 0x51 || wParam == 0x86) { //press "ESC" or "Q" then exit
            is_done = 0;
		} else if (wParam == VK_UP) {
			action_idx--;
			if (action_idx < 0)
				action_idx =  model->getActionCount() - 1;
			//go to prior action
			model->setActionIndex(action_idx);
			weapon->setActionIndex(action_idx);
		} else if (wParam == VK_DOWN) {
			action_idx++;
			if (action_idx >= (int)model->getActionCount())
				action_idx = 0;
			//go to next action
			model->setActionIndex(action_idx);
			weapon->setActionIndex(action_idx);
		} else if (wParam == 0x46) { //if press "F" then enable/disable fog
			is_foged = !is_foged;
			if (is_foged)
				world->setFog(fog);
			else
				world->setFog(NULL);
		}

        break;

    case WM_SIZE:
        GetWindowRect(hwnd, &rc);
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
#endif

#if (defined(WIN32) || defined(_WIN32_WCE))
#define	WINDOW_CLASS    TEXT("F3D")
#define	TITLE           TEXT("Md2Viewer")
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
    world->setBgColor(0.5f, 0.5f, 0.5f, 0.0f);

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

	//after create world, set is_initialized to true
	is_initialized = true;
#else
    world->init();
#endif

    camera = new Camera();
    camera->setEye(60.0f, 15.0f, 60.0f);

    float fogColor[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    fog = new Fog();
    fog->setFogColor(fogColor);
    fog->setFogStart(-5.0f);
    fog->setFogEnd(5.0f);
    fog->setFogDensity(0.004f);

    world->setCamera(camera);
    world->setFog(fog);

    image = new Image();
    Texture* texture0 = image->loadTexture("tris.bmp");
    Texture* texture1 = image->loadTexture("weapon.bmp");
    Texture* texture2 = image->loadTexture("floor.bmp");
    Texture* texture3 = image->loadTexture("clouds.bmp");

    model = new ModelMD2();
    model->loadModel("tris.md2");
    model->setActionIndex(action_idx);
    if (texture0 != NULL)
        model->setTextureId(texture0->textureId);

    weapon = new ModelMD2();
    weapon->loadModel("weapon.md2");
    weapon->setActionIndex(action_idx);
    if (texture1 != NULL)
        weapon->setTextureId(texture1->textureId);

    plane = new Plane(4, 4, 128.0f);
    if (texture2 != NULL)
        plane->setTextureId(texture2->textureId);
    plane->setPosition(-256.0f, -28.0f, -256.0f);

    skydome = new Skydome(256, 30.0f, 10.0f);
    if (texture3 != NULL)
        skydome->setTextureId(texture3->textureId);
    skydome->setPosition(0.0f, (float)(-256 * sinf(DTOR * 10.0f)) - 28.0f, 0.0f);

    font = new Font(16, 16, 24, 36, "font.bmp");

	printf("model->getActionName(%d): %s\n", action_idx, model->getActionName(action_idx));

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

    sprintf(strAction, "Action[%d]:%s", action_idx, model->getActionName(action_idx));
    printf("%s\n", strAction);

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

        model->renderModel();
        weapon->renderModel();

        //printf("strFps: %s\n", strFps);
        font->drawString(4, 4, strFps);

        //draw action info
        int w_height = world->getHeight();
        int f_height = font->getFonHeight();
        font->drawString(4, w_height - f_height - 4, strAction);

        world->finishRender();

        fps++;
		angle += 0.1f;
#ifdef ANDROID
        gettimeofday(&timeNow, NULL);
        interval = CLOCK(timeNow) - i_time;
        if (interval >= 40000) {
            is_done = 0;
            printf("fps\t\t: %.2f\n", fps * 1.0f / 40);
        } else if (interval >= 20000) {
            if (!is_changed) {
				action_idx++;
                model->setActionIndex(action_idx);
                weapon->setActionIndex(action_idx);
                printf("model->getActionName(%d): %s\n", action_idx, model->getActionName(action_idx));
                is_changed = true;
            }
        }
        //refresh strFps per second
        if (((CLOCK(timeNow) - i_time) / 1000) % 2 == 0 && interval > 0) {
            sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
            sprintf(strAction, "Action[%d]:%s", action_idx, model->getActionName(action_idx));
        }
#elif (defined(WIN32) || defined(_WIN32_WCE))
        interval = GetTickCount() - i_time;
        //refresh strFps per 0.5 second
        if (interval >= 500) {
            sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
            sprintf(strAction, "Action[%d]:%s", action_idx, model->getActionName(action_idx));

            //reset all time variables after get strFps
            interval = 0;
			i_time = GetTickCount();
            fps = 0;
        }
#endif
    }

    delete world;
    delete model;
    delete weapon;
    delete image;
    delete font;
	delete plane;
	delete skydome;

    return 0;
}
