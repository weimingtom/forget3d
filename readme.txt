==========================================================================
The Forget3D Engine SDK V0.1
==========================================================================

  Welcome the Forget3D Engine SDK.

  Content of this file:
  1. Introduce
  2. How To Build & Run
  3. To Do List
  4. License

==========================================================================
1. Introduce
==========================================================================

  Forget 3D Game Engine

  Forget 3D Game Engine is a OpenGL ES framework(still not a engine, ~_~) for
  Android, Win32, WinCE platform, it simplifies your OpenGL ES program 
  development, support scene management, Texture, Camera, Light, Model loader,
  Font etc.

  Release Date:     23-Jun-2009

  Current Version:  0.1
  Current Authors:  Martin Foo (i25ffz@hotmail.com)

  Features in Version 0.1:
  - Screen management
   Support cross-platform scene management, OpenGL ES display,context init, render;
   OpenGL ES status management, support camera, light, fog features.
  - 3D model loader and management
   Current support (Quake II)MD2 model.
  - Texture management
   Current support 8 & 24 bit bitmap image file.
  - Font support
   Support font, you can simply use drawString function to draw text.
  - Skydome, Plane, Terrain support
   Support skydome, plane, terrain, you can simply create them in your applocation.

==========================================================================
2. How To Build & Run
==========================================================================

  Current Forget3D support Android Native/WinCE/Win32 platforms, for Android
  Native & Win32 platform I provide Code::Blocks(http://www.codeblocks.org) 
  workspace & project files, for WinCE/PPC platform, I provide EVC4 worksapce
  & project file, you can find all project files in "projects" dir.
  
  About OpenGL ES implementation, for Android Native use google implementation, 
  you can find the library(libGLES_CM.so) in Emulator /system/lib dir; For WinCE
  & Win32 platforms, I use Hybrid Rasteroid 3.1 OpenGL ES implementation, cause
  this implementation support GL_OES_draw_texture feature, Font feature need 
  GL_OES_draw_texture to draw string, of course you can use PowerVR & Vincent
  implementation, 
  
  * Android Native
    Make sure you have install Code::Blocks & Android Native Toolchain for Cygwin,
    if not pls see my blog's article to setup your env(in chinese):
    http://blog.sina.com.cn/s/blog_4a0a39c30100d931.html
    After all is prepared, start your android emulator, and make sure "adb shell"
    can work, now you can enter "projects" dir, and open forget3d.workspace, and 
    select menu "Build -> Build Workspace", if no problem, IDE will compile whole
    workspace successfully(include forget3d libarary and test demos), and IDE will
    automatically push the test programs to your emulator, after do this, enter 
    "data" dir, run "push.bat", this script will push all forget3d test programs 
    resources to your emulator "dev/sample" dir, and you can enter your emulator 
    shell env and run them:
    $adb shell
    #cd /dev/sample
    #ls -l
    #./triangle
    #./gears
    #./md2viewer
    #./skytest
    
  * WinCE
    Make sure you have install EVC4 with SP4 & PocketPC 2003 SDK, enter 
    "projects/wince" dir, open forget3d.vcw, build them!
    If you use emulator to test, pls select menu "Tools -> Remote File Viewer" to
    open Remote File Viewer and connect emulator, and push "data" dir all model &
    images to your emulator root dir, and push "lib\wince\emu" dir's libEGL.dll & 
    libGLES_CM_NoE.dll to your emulator "Windows" dir, now you can run them!
    If you use device to test,pls copy "data" dir all models & images to your PPC
    root dir, and copy "lib\wince\emu" dir's libEGL.dll & libGLES_CM_NoE.dll to 
    "Windows" dir, run them! (I don't test it on device, if you test it on your 
    device, pls send me a photo, thks!)

  * Win32
    Make sure you have correctly install Code::Blocks & MinGW on your system, enter
    "projects/win32" dir, open forget3d.workspace, and select menu "Build -> Build 
    Workspace", if no problem, IDE will output the test programs to "data" dir, copy
    "lib\win32" dir's libEGL.dll & libGLES_CM_NoE.dll "data" dir or $WINDOWS\system32
    dir, run them!

==========================================================================
3. To Do List
==========================================================================

  Forget3D still not a real engine, still a framework, so need more featues add to
  it, such as network, audio, particle, effect, more platform support etc.

  * More than one mesh support for Model class
  * More than one camera & light support  
  * More platform support
    1. S60 platform port;
    2. Mac OS(iPhone) platform support.
  * Audio
  * Network
  * Particle & effect support
  * And more features...

==========================================================================
4. License
==========================================================================

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
 *      Please contact Norbert Nopper (i25ffz@hotmail.com) for further details.
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