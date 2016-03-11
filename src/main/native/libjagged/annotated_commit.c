#include <assert.h>

#include <jni.h>
#include <git2.h>

#include "annotated_commit.h"
#include "reference.h"

JNIEXPORT jobject JNICALL
Java_org_libgit2_jagged_core_NativeMethods_annotatedCommitFromRef(
		JNIEnv *env,
		jclass class,
		jobject repo_java,
        jobject reference_java) {
	jobject reference_name_java;
	jni_constructor constructor;
	if (!jni_get_object_field(&reference_name_java, reference_java, "canonicalName", "Ljava/lang/String;", env) ||
	    !jni_constructor_init(&constructor, GIT_JAVA_CLASS_ANNOTATED_COMMIT, "(Lorg/libgit2/jagged/ObjectId;J)V", env))
		return NULL;

	int error;
	jobject annotated_commit_java = NULL;
	const char *reference_name = git_java_jstring_to_utf8(env, reference_name_java);

	git_repository *repo = git_java_handle_get(env, repo_java);
	git_reference *reference;
	if ((error = git_reference_lookup(&reference, repo, reference_name)) < 0) {
		git_java_exception_throw_giterr(env, error);
		goto done;
	}

	git_annotated_commit *annotated_commit;
	if ((error = git_annotated_commit_from_ref(&annotated_commit, repo, reference)) < 0) {
		git_java_exception_throw_giterr(env, error);
		goto done;
	}

	const git_oid *id = git_annotated_commit_id(annotated_commit);
	jobject id_java = git_java_objectid_init(env, id);
	if (id_java == NULL)
		goto done;

	annotated_commit_java = jni_constructor_invoke(&constructor, env, id_java, git_java_jlong_from_ptr(annotated_commit));
	done:
	if (reference_name != NULL)
		git_java_utf8_free(env, reference_name_java, reference_name);
	return annotated_commit_java;
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_annotatedCommitFree(
		JNIEnv *env,
		jclass class,
		jobject annotated_commit_java) {
	assert(env);
	assert(class);
	assert(annotated_commit_java);

	git_annotated_commit *annotated_commit = NULL;
	if ((annotated_commit = git_java_handle_get(env, annotated_commit_java)) == NULL)
		return;

	git_annotated_commit_free(annotated_commit);
}
