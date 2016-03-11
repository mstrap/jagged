package org.libgit2.jagged.rebase;

import org.libgit2.jagged.AnnotatedCommit;
import org.libgit2.jagged.Repository;
import org.libgit2.jagged.Signature;
import org.libgit2.jagged.core.NativeMethods;
import org.libgit2.jagged.core.NativeObject;

public class Rebase
    extends NativeObject
{
    public static Rebase init(Repository repository, AnnotatedCommit branch, AnnotatedCommit upstream, AnnotatedCommit onto)
    {
        return NativeMethods.rebaseInit(repository, branch, upstream, onto);
    }

    private Rebase(long handle)
    {
        super(handle);
    }

    public RebaseOperation next()
    {
        return NativeMethods.rebaseNext(this);
    }

    public void finish(Signature signature)
    {
        NativeMethods.rebaseFinish(this, signature);
    }


    public RebaseOperation commit(Signature committer)
    {
        return commit(null, committer, null, null);
    }

    public RebaseOperation commit(Signature author, Signature committer, String messageEncoding, String message)
    {
        return NativeMethods.rebaseCommit(this, author, committer, messageEncoding, message);
    }

    @Override
    public void close()
    {
        NativeMethods.rebaseFree(this);
    }
}
