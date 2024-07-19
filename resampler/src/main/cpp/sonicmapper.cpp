//
// Created by Nihad Ahmetovic on 1. 6. 2024..
//
#include <jni.h>
#include <string>
#include <android/log.h>
#include <cstdlib>
#include "sonic/sonic.h"
#include "common/InstanceHandler.h"


using namespace androidresampler;

struct sonicInstStruct {
    sonicStream stream;
    short *byteBuf;
    int byteBufSize;
};

typedef struct sonicInstStruct *sonicInst;
#define SONIC_DEBUG true
// For debug messages:
#if SONIC_DEBUG
#define APPNAME "Sonic"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, __VA_ARGS__);
#else
#define LOGV(...)
#endif

extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_initialize(JNIEnv *env, jobject thiz, jint sample_rate, jint channels) {
    auto inst = (sonicInst) calloc(1, sizeof(struct sonicInstStruct));

    if (inst == nullptr) {
        return;
    }
    inst->stream = sonicCreateStream(sample_rate, channels);
    if (inst->stream == nullptr) {
        return;
    }
    inst->byteBufSize = 100;
    inst->byteBuf = (short *) calloc(inst->byteBufSize, sizeof(short));
    if (inst->byteBuf == nullptr) {
        return;
    }

    auto g_object = InstanceHandler::createGlobalReference(env, thiz, thiz);
    InstanceHandler::setHandle(env, g_object, inst);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setSampleRateNative(JNIEnv *env, jobject thiz, jint new_sample_rate) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set sample rate to %d", new_sample_rate);
    sonicSetSampleRate(stream, new_sample_rate);
}
extern "C"
JNIEXPORT jint JNICALL
Java_team_unravel_resampler_Sonic_getSampleRateNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0;
    sonicStream stream = inst->stream;
    LOGV("Reading sample rate");
    return sonicGetSampleRate(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setNumChannelsNative(JNIEnv *env, jobject thiz, jint new_num_channels) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set sample rate to %d", new_num_channels);
    sonicSetNumChannels(stream, new_num_channels);
}
extern "C"
JNIEXPORT jint JNICALL
Java_team_unravel_resampler_Sonic_getNumChannelsNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0;
    sonicStream stream = inst->stream;
    LOGV("Reading num channels");
    return sonicGetNumChannels(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setPitchNative(JNIEnv *env, jobject thiz, jfloat new_pitch) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set pitch to %f", new_pitch);
    sonicSetPitch(stream, new_pitch);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_team_unravel_resampler_Sonic_getPitchNative(JNIEnv *env, jobject thiz) {
    sonicStream stream = InstanceHandler::getHandle<sonicInstStruct>(env, thiz)->stream;
    LOGV("Reading pitch");
    return sonicGetPitch(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setSpeedNative(JNIEnv *env, jobject thiz, jfloat new_speed) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set speed to %f", new_speed);
    sonicSetSpeed(stream, new_speed);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_team_unravel_resampler_Sonic_getSpeedNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0.0;
    sonicStream stream = inst->stream;
    LOGV("Reading speed");
    return sonicGetSpeed(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setRateNative(JNIEnv *env, jobject thiz, jfloat new_rate) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set rate to %f", new_rate);
    sonicSetRate(stream, new_rate);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_team_unravel_resampler_Sonic_getRateNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0.0;
    sonicStream stream = inst->stream;
    LOGV("Reading rate");
    return sonicGetRate(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setChordPitchNative(JNIEnv *env, jobject thiz, jboolean use_chord_pitch) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set chord pitch to %d", use_chord_pitch);
    sonicSetChordPitch(stream, use_chord_pitch);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_team_unravel_resampler_Sonic_getChordPitchNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return false;
    sonicStream stream = inst->stream;
    LOGV("Reading chord pitch");
    return sonicGetChordPitch(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_setVolumeNative(JNIEnv *env, jobject thiz, jfloat new_volume) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;
    LOGV("Set volume to %f", new_volume);
    sonicSetVolume(stream, new_volume);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_team_unravel_resampler_Sonic_getVolumeNative(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0.0;
    sonicStream stream = inst->stream;
    LOGV("Reading volume");
    return sonicGetVolume(stream);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_team_unravel_resampler_Sonic_putBytes(JNIEnv *env, jobject thiz, jbyteArray buffer, jint len_bytes) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0;
    sonicStream stream = inst->stream;

    int samples = len_bytes / (sizeof(short) * sonicGetNumChannels(stream));
    int remainingBytes = len_bytes - samples * sizeof(short) * sonicGetNumChannels(stream);

    if (remainingBytes != 0) {
        LOGV("Remaining bytes == %d!!!", remainingBytes);
    }
    if (len_bytes > inst->byteBufSize * sizeof(short)) {
        inst->byteBufSize = len_bytes * (2 / sizeof(short));
        inst->byteBuf = (short *) realloc(inst->byteBuf, inst->byteBufSize * sizeof(short));
        if (inst->byteBuf == nullptr) {
            return 0;
        }
    }
    LOGV("Writing %d bytes to stream", len_bytes);
    env->GetByteArrayRegion(buffer, 0, len_bytes, (jbyte *) inst->byteBuf);
    return sonicWriteShortToStream(stream, inst->byteBuf, samples);
}
extern "C"
JNIEXPORT jint JNICALL
Java_team_unravel_resampler_Sonic_receiveBytes(JNIEnv *env, jobject thiz, jbyteArray ret, jint len_bytes) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return 0;
    sonicStream stream = inst->stream;

    int available = sonicSamplesAvailable(stream) * sizeof(short) * sonicGetNumChannels(stream);
    int samplesRead, bytesRead;

    LOGV("Reading %d bytes from stream", len_bytes);
    if (len_bytes > available) {
        len_bytes = available;
    }
    if (len_bytes > inst->byteBufSize * sizeof(short)) {
        inst->byteBufSize = len_bytes * (2 / sizeof(short));
        inst->byteBuf = (short *) realloc(inst->byteBuf, inst->byteBufSize * sizeof(short));
        if (inst->byteBuf == NULL) {
            return -1;
        }
    }
    LOGV("Doing read %d", len_bytes);
    samplesRead = sonicReadShortFromStream(stream, inst->byteBuf,
                                           len_bytes / (sizeof(short) * sonicGetNumChannels(stream)));
    bytesRead = samplesRead * sizeof(short) * sonicGetNumChannels(stream);
    LOGV("Returning %d", samplesRead);
    env->SetByteArrayRegion(ret, 0, bytesRead, (jbyte *) inst->byteBuf);
    return bytesRead;
}
extern "C"
JNIEXPORT jint JNICALL
Java_team_unravel_resampler_Sonic_availableBytes(JNIEnv *env, jobject thiz) {
    auto sonicInst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (sonicInst == nullptr) return 0;
    sonicStream stream = sonicInst->stream;
    LOGV("Reading samples available = %d", sonicSamplesAvailable(stream) * sizeof(short) * sonicGetNumChannels(stream));

    return sonicSamplesAvailable(stream) * sizeof(short) * sonicGetNumChannels(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_flush(JNIEnv *env, jobject thiz) {
    auto sonicInst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (sonicInst == nullptr) return;
    sonicStream stream = sonicInst->stream;
    LOGV("Flushing stream");
    sonicFlushStream(stream);
}
extern "C"
JNIEXPORT void JNICALL
Java_team_unravel_resampler_Sonic_close(JNIEnv *env, jobject thiz) {
    auto inst = InstanceHandler::getHandle<sonicInstStruct>(env, thiz);
    if (inst == nullptr) return;
    sonicStream stream = inst->stream;

    sonicDestroyStream(stream);
    free(inst->byteBuf);
    free(inst);
    InstanceHandler::clearGlobalReferences(env, thiz);
    InstanceHandler::setHandleFieldValue(env, thiz, -1);
}