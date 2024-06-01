//
// Created by Nihad Ahmetovic on 1. 6. 2024..
//

#include "InstanceHandler.h"

namespace androidresampler {
    std::map<long, std::vector<jobject>> InstanceHandler::gRefs{};

    jfieldID InstanceHandler::getHandleField(JNIEnv *env, jobject obj) {
        jclass c = env->GetObjectClass(obj);
        // J is the type signature for long:
        return env->GetFieldID(c, "nativeHandle", "J");

    }

    jlong InstanceHandler::getHandleFieldValue(JNIEnv *env, jobject obj) {
        return env->GetLongField(obj, getHandleField(env, obj));
    }

    void InstanceHandler::setHandleFieldValue(JNIEnv *env, jobject obj, jlong value) {
        env->SetLongField(obj, getHandleField(env, obj), value);
    }

    jobject InstanceHandler::createGlobalReference(JNIEnv *env, jobject thiz, jobject obj) {
        jlong handle = env->GetLongField(thiz, getHandleField(env, thiz));
        auto gRef = env->NewGlobalRef(obj);

        if (gRefs.find(handle) == gRefs.end()) {
            auto vector = std::vector<jobject>();
            vector.push_back(gRef);
            gRefs.insert(std::pair<jlong, std::vector<jobject>>(handle, vector));
        } else {
            gRefs[handle].push_back(gRef);
        }
        return gRef;
    }

    void InstanceHandler::clearGlobalReferences(JNIEnv *env, jobject obj) {
        jlong handle = env->GetLongField(obj, getHandleField(env, obj));

        for (int i = 0; i < gRefs[handle].size(); ++i) {
            env->DeleteGlobalRef(gRefs[handle][i]);
        }
        gRefs.erase(handle);
    }

} // androidresampler