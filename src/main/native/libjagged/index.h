#ifndef GIT_JAVA_INDEX_H
#define GIT_JAVA_INDEX_H

#include <jni.h>

#include "oid.h"
#include "util.h"

#define GIT_JAVA_CLASS_INDEX "org/libgit2/jagged/Index"

GIT_INLINE(jobject) git_java_index_init(JNIEnv *env, git_index *index)
{
	jclass index_class;
	jmethodID index_initmethod;

	if ((index_class = (*env)->FindClass(env, GIT_JAVA_CLASS_INDEX)) == NULL ||
		(index_initmethod = (*env)->GetMethodID(env, index_class, "<init>", "(J)V")) == NULL)
		return NULL;

	return (*env)->NewObject(env, index_class, index_initmethod, git_java_jlong_from_ptr(index));
}

#endif /* GIT_JAVA_INDEX_H */
