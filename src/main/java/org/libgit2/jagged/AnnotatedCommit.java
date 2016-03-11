package org.libgit2.jagged;

import org.libgit2.jagged.core.NativeMethods;
import org.libgit2.jagged.core.NativeObject;

public class AnnotatedCommit
    extends NativeObject
{
    public static AnnotatedCommit openFromRef(Repository repository, Reference ref) {
        return NativeMethods.annotatedCommitFromRef(repository, ref);
    }

    private final ObjectId id;

    private AnnotatedCommit(ObjectId id, long handle)
    {
        super(handle);

        this.id = id;
    }

    public ObjectId getId()
    {
        return id;
    }

    @Override
    public void close()
    {
        NativeMethods.annotatedCommitFree(this);
    }
}
