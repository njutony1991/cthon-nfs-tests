/*
 *	@(#)test2.c	1.6	99/08/29 Connectathon Testsuite
 *	1.3 Lachman ONC Test Suite source
 *
 * Test file and directory removal.
 * Builds a tree on the server.
 *
 * Uses the following important system calls against the server:
 *
 *	chdir()
 *	rmdir()		(if removing directories, level > 1)
 *	unlink()
 */

#if defined (DOS) || defined (WIN32)
/* If Dos, Windows or Win32 */
#define DOSorWIN32
#endif

#ifndef DOSorWIN32
#include <sys/param.h>
#endif

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef DOSorWIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "../tests.h"

static int Tflag = 0;	/* print timing */
static int Fflag = 0;	/* test function only;  set count to 1, negate -t */
static int Nflag = 0;	/* Suppress directory operations */

static void
usage()
{
	fprintf(stdout, "usage: %s [-htfn] [levels files dirs fname dname]\n",
	    Myname);
	fprintf(stdout, "  Flags:  h    Help - print this usage info\n");
	fprintf(stdout, "          t    Print execution time statistics\n");
	fprintf(stdout, "          f    Test function only (negate -t)\n");
	fprintf(stdout, "          n    Suppress test directory create operations\n");
}

int
main(int argc, char *argv[])
{
	int files = DFILS;	/* number of files in each dir */
	int totfiles = 0;
	int dirs = DDIRS;	/* directories in each dir */
	int totdirs = 0;
	int levels = DLEVS;	/* levels deep */
	char *fname = FNAME;
	char *dname = DNAME;
	struct timeval time;
	char *opts;
	char str[256];

	setbuf(stdout, NULL);
    fprintf(stdout, "Test file and directory removal.\n"
                    "Builds a tree on the server.\n"
                    "Syscalls:\n"
                    "chdir() ---- ACCESS\n"
                    "rmdir() (if removing directories, level > 1) ---- RMDIR \n"
                    "unlink() ---- REMOVE\n");

	Myname = *argv++;
	argc--;
	while (argc && **argv == '-') {
		for (opts = &argv[0][1]; *opts; opts++) {
			switch (*opts) {
				case 'h':	/* help */
					usage();
					exit(1);
					break;

				case 't':	/* time */
					Tflag++;
					break;

				case 'f':	/* funtionality */
					Fflag++;
					break;

				case 'n':	/* No Test Directory create */
					Nflag++;
					break;

				default:
					error("unknown option '%c'", *opts);
					usage();
					exit(1);
			}
		}
		argc--;
		argv++;
	}

	if (argc) {
		levels = getparm(*argv, 1, "levels");
		argv++;
		argc--;
	}
	if (argc) {
		files = getparm(*argv, 0, "files");
		argv++;
		argc--;
	}
	if (argc) {
		dirs = getparm(*argv, 0, "dirs");
		if (dirs == 0 && levels != 1) {
			error("Illegal dirs parameter, must be at least 1");
			exit(1);
		}
		argv++;
		argc--;
	}
	if (argc) {
		fname = *argv;
		argc--;
		argv++;
	}
	if (argc) {
		dname = *argv;
		argc--;
		argv++;
	}
	if (argc != 0) {
		error("too many parameters");
		usage();
		exit(1);
	}

	if (Fflag) {
		Tflag = 0;
		levels = 2;
		files = 2;
		dirs = 2;
	}

	fprintf(stdout, "%s: File and directory removal test\n", Myname);

	if (mtestdir(NULL)) {
		sprintf(str, "test1 -s %s %d %d %d %s %s",
			Nflag ? "-n" : "",
			levels, files, dirs, fname, dname);
		if (system(str) != 0) {
			error("can't make directroy tree to remove");
			exit(1);
		}
		if (mtestdir(NULL)) {
			error("still can't go to test directory");
			exit(1);
		}
	}

	if (Tflag) {
		starttime();
	}
    fprintf(stdout, "Args | levels: %d, files: %d, dirs: %d\n", levels, files, dirs);
    fprintf(stdout, "Success | All unlink(), chdir(), rmdir() success\n");
    fprintf(stdout, "Fail | Any unlink(), chdir(), rmdir () fail\n");
	rmdirtree(levels, files, dirs, fname, dname, &totfiles, &totdirs, 0);
	if (Tflag) {
		endtime(&time);
	}
	fprintf(stdout,
	    "\tremoved %d files %d directories %d levels deep",
	    totfiles, totdirs, levels);
	if (Tflag) {
		fprintf(stdout, " in %ld.%-2ld seconds",
		    (long)time.tv_sec, (long)time.tv_usec / 10000);
	}
	fprintf(stdout, "\n");
	complete();
	return 0;
}
