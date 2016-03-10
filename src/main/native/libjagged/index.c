
#include <assert.h>

#include <jni.h>
#include <git2.h>

#include "index.h"
#include "reference.h"
#include "util.h"

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_indexOpen(
	JNIEnv *env,
	jclass class,
	jobject index_java,
	jstring path_java)
{
	assert(env);
	assert(class);
	assert(index_java);
	assert(path_java);

	const char *path = NULL;
	if ((path = git_java_jstring_to_utf8(env, path_java)) == NULL)
		return;

	int error = 0;
	git_index *index = NULL;
	if ((error = git_index_open(&index, path)) < 0)
		git_java_exception_throw_giterr(env, error);
	else
		git_java_handle_set(env, index_java, index);

	git_java_utf8_free(env, path_java, path);
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_indexFree(
	JNIEnv *env,
	jclass class,
	jobject index_java)
{
	assert(env);
	assert(class);
	assert(index_java);

	git_index *index = git_java_handle_get(env, index_java);
	if (index == NULL)
		return;

	git_index_free(index);
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_indexAddByPath(
	JNIEnv *env,
	jclass class,
	jobject index_java,
	jstring path_java)
{
	assert(env);
	assert(class);
	assert(index_java);
	assert(path_java);

	git_index *index = git_java_handle_get(env, index_java);
	if (index == NULL)
		return;

	const char *path = NULL;
	if ((path = git_java_jstring_to_utf8(env, path_java)) == NULL)
		return;

	int error;
	if ((error = git_index_add_bypath(index, path)) < 0) {
		git_java_exception_throw_giterr(env, error);
		return;
	}

	git_java_utf8_free(env, path_java, path);
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_indexRemoveByPath(
	JNIEnv *env,
	jclass class,
	jobject index_java,
	jstring path_java)
{
	assert(env);
	assert(class);
	assert(index_java);
	assert(path_java);

	git_index *index = git_java_handle_get(env, index_java);
	if (index == NULL)
		return;

	const char *path = NULL;
	if ((path = git_java_jstring_to_utf8(env, path_java)) == NULL)
		return;

	int error;
	if ((error = git_index_remove_bypath(index, path)) < 0) {
		git_java_exception_throw_giterr(env, error);
		return;
	}

	git_java_utf8_free(env, path_java, path);
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_indexWrite(
	JNIEnv *env,
	jclass class,
	jobject index_java)
{
	assert(env);
	assert(class);
	assert(index_java);

	git_index *index = git_java_handle_get(env, index_java);
	if (index == NULL)
		return;

	int error;
	if ((error = git_index_write(index)) < 0) {
		git_java_exception_throw_giterr(env, error);
		return;
	}
}
