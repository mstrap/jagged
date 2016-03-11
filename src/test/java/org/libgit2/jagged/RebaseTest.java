package org.libgit2.jagged;

import org.junit.Before;
import org.junit.Test;
import org.libgit2.jagged.rebase.Rebase;
import org.libgit2.jagged.rebase.RebaseOperation;

public class RebaseTest
    extends GitLocalCommandTest
{
    @Before
    public void before()
    {
        repoPath = setupRepository("rebase");
    }

    @Test
    public void testBasicRebase()
    {
        Repository repository = new Repository(repoPath.getPath());
        final ReferenceCollection references = repository.getReferences();
        final AnnotatedCommit branch = AnnotatedCommit.openFromRef(repository, references.get("refs/heads/beef"));
        final AnnotatedCommit upstream = AnnotatedCommit.openFromRef(repository, references.get("refs/heads/master"));
        final Rebase rebase = Rebase.init(repository, branch, upstream, null);
        final Signature committer = new Signature("T. E. Ster", "tester@domain.com");
        for (;;) {
            final RebaseOperation operation = rebase.next();
            if (operation == null) {
                break;
            }
            rebase.commit(committer);
        }

        rebase.finish(null);
        rebase.close();
        branch.close();
        upstream.close();
    }
}
