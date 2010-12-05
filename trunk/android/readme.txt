Testing steps:
1. Install the NDK r4b;
2. Go in android/jni dir and:
	ndk-build V=1
3. If you have ant, call: ant clean install;
4. If you install eclipse, import this project to Eclipse to run || debug it.

BTW, before testing JNI, you should open projects\forget3d.workspace, and select "forget3d" project "NDK" target in CodeBlocks, after compiling, you will get "libf3d_ndk.a" in "lib" dir.
