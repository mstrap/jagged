package org.libgit2.jagged.rebase;

import org.libgit2.jagged.ObjectId;

public class RebaseOperation
{
    private final int type;
    private final ObjectId id;
    private final String exec;

    private RebaseOperation(int type, ObjectId id, String exec)
    {
        this.type = type;
        this.id = id;
        this.exec = exec;
    }

    public int getType()
    {
        return type;
    }

    public ObjectId getId()
    {
        return id;
    }

    public String getExec()
    {
        return exec;
    }
}