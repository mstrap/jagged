package org.libgit2.jagged;

import java.io.File;

import org.junit.Test;

public class IndexTest
    extends GitTest
{
    @Test
    public void testAdd()
    {
        final File repoPath = setupRepository("testrepo");
        System.out.println(repoPath);
    }
}
