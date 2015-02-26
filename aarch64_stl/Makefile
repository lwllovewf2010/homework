all:
	/home/merck/Sw/android-ndk-r10d/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-g++ -static -std=c++11 -Wall -g3 -march=armv8-a -U__ANDROID__ -DCOMPILE_ARM64 --sysroot=/home/merck/Sw/android-ndk-r10d/platforms/android-21/arch-arm64 -I/home/merck/Sw/android-ndk-r10d/sources/android/support/include -I/home/merck/Sw/android-ndk-r10d/sources/cxx-stl/llvm-libc++/libcxx/include -o test test.cc /home/merck/Sw/android-ndk-r10d/sources/cxx-stl/llvm-libc++/libs/arm64-v8a/libc++_static.a

clean:
	rm -f test
