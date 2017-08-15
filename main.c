#include "sqlite3.h"
#include "stdio.h"

int vfsdemo_open(sqlite3_vfs *vfs, const char *name, sqlite3_file *file, int flags, int *outFlags)
{
	sqlite3_vfs *root = (sqlite3_vfs *)vfs->pAppData;

	if (flags & (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MAIN_DB))
	{
		flags |= SQLITE_OPEN_EXCLUSIVE;
	}

	return root->xOpen(root, name, file, flags, outFlags);
}

int vfsdemo_register()
{
	sqlite3_vfs *root;
	sqlite3_vfs *vfs;

	if ((root = sqlite3_vfs_find(NULL)) == NULL)
	{
		return SQLITE_NOTFOUND;
	}

	if ((vfs = sqlite3_malloc(sizeof(*vfs))) == NULL)
	{
		return SQLITE_NOMEM;
	}

	vfs->iVersion = root->iVersion;
	vfs->szOsFile = root->szOsFile;
	vfs->mxPathname = root->mxPathname;
	vfs->zName = "vfsdemo";
	vfs->pAppData = root;
	vfs->xOpen = vfsdemo_open;
	vfs->xDelete = root->xDelete;
	vfs->xAccess = root->xAccess;
	vfs->xFullPathname = root->xFullPathname;
	vfs->xDlOpen = root->xDlOpen;
	vfs->xDlError = root->xDlError;
	vfs->xDlSym = root->xDlSym;
	vfs->xDlClose = root->xDlClose;
	vfs->xRandomness = root->xRandomness;
	vfs->xSleep = root->xSleep;
	vfs->xCurrentTime = root->xCurrentTime;
	vfs->xGetLastError = root->xGetLastError;
	vfs->xCurrentTimeInt64 = root->xCurrentTimeInt64;

	return sqlite3_vfs_register(vfs, 1);
}

int main()
{
	sqlite3 *db;
	char *sql, *err;
	int flags, rc;

	if ((rc = vfsdemo_register()) > 0)
	{
		printf("%s\n", sqlite3_errstr(rc));
		return rc;
	}

	flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

	if ((rc = sqlite3_open_v2("test.db", &db, flags, NULL)) > 0)
	{
		printf("%s\n", sqlite3_errstr(rc));
		return rc;
	}

	sql = "CREATE TABLE IF NOT EXISTS foo(value); \
		INSERT INTO foo(value) VALUES('value');";

	if ((rc = sqlite3_exec(db, sql, NULL, NULL, &err)) > 0)
	{
		printf("%s\n", err);
		return rc;
	}

	return 0;
}
