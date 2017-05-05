#include <jni.h>
#include <string>
#include <cstdio>
#include <string>
#include <memory>
#include <cassert>
#include "utils/file.h"
#include "mips.h"

std::unique_ptr<mips::CPU> cpu;

extern "C" JNIEXPORT jboolean JNICALL Java_info_czekanski_avocado_Avocado_init(JNIEnv* env, jobject) {
    cpu = std::make_unique<mips::CPU>();
    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL Java_info_czekanski_avocado_Avocado_loadBios(JNIEnv* env, jobject, jbyteArray bios) {
    jbyte* biosArray = env->GetByteArrayElements(bios, 0);
    cpu->loadBios((const char*)biosArray, (size_t)env->GetArrayLength(bios));
    env->ReleaseByteArrayElements(bios, biosArray, JNI_ABORT);
    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL Java_info_czekanski_avocado_Avocado_emulateFrame(JNIEnv* env, jobject) {
    if (cpu->state != mips::CPU::State::run) return JNI_FALSE;
    cpu->emulateFrame();
    return JNI_TRUE;
}

extern "C" JNIEXPORT jint JNICALL Java_info_czekanski_avocado_Avocado_getPc(JNIEnv* env, jobject) { return cpu->PC; }

extern "C" JNIEXPORT jstring JNICALL Java_info_czekanski_avocado_Avocado_getLogs(JNIEnv* env, jobject) {
    jstring result = env->NewStringUTF(cpu->logs.c_str());
    cpu->logs.clear();
    return result;
}
