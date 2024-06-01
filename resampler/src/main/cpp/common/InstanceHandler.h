//
// Created by Nihad Ahmetovic on 1. 6. 2024..
//

#ifndef RESAMPLER_APP_INSTANCEHANDLER_H
#define RESAMPLER_APP_INSTANCEHANDLER_H

#include <jni.h>
#include <map>
#include <vector>
#include <string>

namespace androidresampler {

    class InstanceHandler {
    public:
        static std::map<long, std::vector<jobject>> gRefs;
        static jfieldID getHandleField(JNIEnv *env, jobject obj);
        static jlong getHandleFieldValue(JNIEnv *env, jobject obj);
        static void setHandleFieldValue(JNIEnv *env, jobject obj, jlong value);
        template<typename T>
        static T *getHandle(JNIEnv *env, jobject obj);
        template<typename T>
        static void setHandle(JNIEnv *env, jobject obj, T *t);
        static jobject createGlobalReference(JNIEnv *env, jobject thiz, jobject obj);
        static void clearGlobalReferences(JNIEnv *env, jobject obj);
    };

} // androidresampler

#include "InstanceHandler.tpp" // Include the implementation file for templates


#endif //RESAMPLER_APP_INSTANCEHANDLER_H
