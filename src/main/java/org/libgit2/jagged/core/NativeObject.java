package org.libgit2.jagged.core;

import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public abstract class NativeObject
    implements Closeable
{
    private final NativeHandle handle = new NativeHandle();

    protected NativeObject(long handle)
    {
        this.handle.set(handle);
    }

    protected final long getHandle() {
        return handle.get();
    }
}
