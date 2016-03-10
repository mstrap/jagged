package org.libgit2.jagged;

import org.libgit2.jagged.Reference.SymbolicReference;
import org.libgit2.jagged.core.Ensure;
import org.libgit2.jagged.core.GitException;
import org.libgit2.jagged.core.NativeHandle;
import org.libgit2.jagged.core.NativeMethods;

public class Index
{
    private final NativeHandle handle = new NativeHandle();

    public Index(final String path)
    {
        Ensure.argumentNotNullOrEmpty(path, "path");

        NativeMethods.indexOpen(this, path);

        Ensure.nativeNotNull(handle);
    }

    @SuppressWarnings("unused")
    private Index(final long handle)
    {
        this.handle.set(handle);
    }

    @SuppressWarnings("unused")
    private void setHandle(long handle)
    {
        this.handle.set(handle);
    }

    @SuppressWarnings("unused")
    private long getHandle()
    {
        return handle.get();
    }

    public void addByPath(String path) {
       NativeMethods.indexAddByPath(this, path);
    }

    public void removeByPath(String path) {
       NativeMethods.indexRemoveByPath(this, path);
    }

    public void write() {
       NativeMethods.indexWrite(this);
    }

    /**
     * Disposes the underlying Index object.
     */
    public void dispose()
    {
        NativeMethods.indexFree(this);
    }
}
