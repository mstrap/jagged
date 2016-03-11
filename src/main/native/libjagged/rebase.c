#include <assert.h>

#include <jni.h>
#include <git2.h>
#include <git2/rebase.h>

#include "oid.h"

int git_java_to_native_signature(git_signature **signature, jobject signature_java, JNIEnv *env) {
	jobject name_java, email_java;
	if (jni_call_object_method(&name_java, signature_java, "org/libgit2/jagged/Signature", "getName", "()Ljava/lang/String;", env) < 0 ||
	    jni_call_object_method(&email_java, signature_java, "org/libgit2/jagged/Signature", "getEmail", "()Ljava/lang/String;", env) < 0)
		return -1;

	const char *name, *email;
	if ((name = git_java_jstring_to_utf8(env, name_java)) == NULL ||
	    (email = git_java_jstring_to_utf8(env, email_java)) == NULL)
		return -1;

	int error = 0;
	if ((error = git_signature_now(signature, name, email)) < 0) {
		git_java_exception_throw_giterr(env, error);
		return -1;
	}

	return 0;
}

JNIEXPORT jobject JNICALL
Java_org_libgit2_jagged_core_NativeMethods_rebaseInit(
		JNIEnv *env,
		jclass class,
		jobject repo_java,
		jobject branch_java,
		jobject upstream_java,
		jobject onto_java) {
	assert(env);
	assert(class);
	assert(repo_java);

	jobject rebase_java;
	git_repository *repo = git_java_handle_get(env, repo_java);
	git_annotated_commit *branch = NULL;
	git_annotated_commit *upstream = NULL;
	git_annotated_commit *onto = NULL;
	if (branch_java != NULL && (branch = git_java_handle_get(env, branch_java)) == NULL)
		return NULL;
	if (upstream_java != NULL && (upstream = git_java_handle_get(env, upstream_java)) == NULL)
		return NULL;
	if (onto_java != NULL && (onto = git_java_handle_get(env, onto_java)) == NULL)
		return NULL;

	jni_constructor constructor;
	if (!jni_constructor_init(&constructor, "org/libgit2/jagged/rebase/Rebase", "(J)V", env))
		return NULL;

	git_rebase *rebase = NULL;
	int error;
	if ((error = git_rebase_init(&rebase, repo, branch, upstream, onto, NULL)) < 0) {
		git_java_exception_throw_giterr(env, error);
		goto done;
	}

	rebase_java = jni_constructor_invoke(&constructor, env, git_java_jlong_from_ptr(rebase));

	done:
	return rebase_java;
}

JNIEXPORT jobject JNICALL
Java_org_libgit2_jagged_core_NativeMethods_rebaseNext(
		JNIEnv *env,
		jclass class,
		jobject rebase_java) {
	assert(env);
	assert(class);
	assert(rebase_java);

	git_rebase *rebase = NULL;
	jobject operation_java = NULL;
	if ((rebase = git_java_handle_get(env, rebase_java)) == NULL)
		return NULL;

	jni_constructor constructor;
	if (!jni_constructor_init(&constructor, "org/libgit2/jagged/rebase/RebaseOperation", "(ILorg/libgit2/jagged/ObjectId;Ljava/lang/String;)V", env))
		return NULL;

	int error;
	git_rebase_operation *operation = NULL;
	if ((error = git_rebase_next(&operation, rebase)) < 0) {
		if (error != GIT_ITEROVER) {
			git_java_exception_throw_giterr(env, error);
		}
		goto done;
	}

	jobject id_java = NULL;
	if ((id_java = git_java_objectid_init(env, &(operation->id))) == NULL)
		goto done;

	operation_java = jni_constructor_invoke(&constructor, env, (jint) operation->type, id_java, git_java_utf8_to_jstring(env, operation->exec));

	done:
	return operation_java;
}

JNIEXPORT jobject JNICALL
Java_org_libgit2_jagged_core_NativeMethods_rebaseCommit(
		JNIEnv *env,
		jclass class,
		jobject rebase_java,
		jobject author_java,
		jobject committer_java,
		jobject message_encoding_java,
		jobject message_java) {
	assert(env);
	assert(class);
	assert(rebase_java);
	assert(committer_java);

	jobject id_java = NULL;
	git_rebase *rebase = NULL;
	git_signature *author = NULL;
	git_signature *committer = NULL;
	const char *message_encoding = NULL;
	const char *message = NULL;
	if ((rebase = git_java_handle_get(env, rebase_java)) == NULL)
		return NULL;
	if (author_java != NULL && (git_java_to_native_signature(&author, author_java, env) < 0))
		goto done;
	if ((git_java_to_native_signature(&committer, committer_java, env) < 0))
		goto done;
	if (message_encoding_java != NULL && ((message_encoding = git_java_jstring_to_utf8(env, message_encoding_java)) == NULL))
		goto done;
	if (message_java != NULL && ((message = git_java_jstring_to_utf8(env, message_java)) == NULL))
		goto done;

	int error;
	git_oid id;
	if ((error = git_rebase_commit(&id, rebase, author, committer, message_encoding, message) < 0)) {
		git_java_exception_throw_giterr(env, error);
		goto done;
	}

	if (((id_java = git_java_objectid_init(env, &id)) == NULL))
		goto done;

	done:
	if (author != NULL)
		git_signature_free(author);
	if (committer != NULL)
		git_signature_free(author);
	if (message_encoding != NULL)
		git_java_utf8_free(env, message_encoding_java, message_encoding);
	if (message != NULL)
		git_java_utf8_free(env, message_java, message);
	return id_java;
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_rebaseFinish(
		JNIEnv *env,
		jclass class,
		jobject rebase_java,
		jobject signature_java) {
	assert(env);
	assert(class);
	assert(rebase_java);

	git_rebase *rebase = NULL;
	if ((rebase = git_java_handle_get(env, rebase_java)) == NULL)
		return;

	git_signature *signature = NULL;
	if (signature_java != NULL && (git_java_to_native_signature(&signature, signature_java, env) < 0))
		goto done;

	int error;
	if ((error = git_rebase_finish(rebase, signature)) < 0) {
		git_java_exception_throw_giterr(env, error);
		goto done;
	}

	done:
	if (signature != NULL)
		git_signature_free(signature);
}

JNIEXPORT void JNICALL
Java_org_libgit2_jagged_core_NativeMethods_rebaseFree(
		JNIEnv *env,
		jclass class,
		jobject rebase_java) {
	assert(env);
	assert(class);
	assert(rebase_java);

	git_rebase *rebase = NULL;
	if ((rebase = git_java_handle_get(env, rebase_java)) == NULL)
		return;

	git_rebase_free(rebase);
}
