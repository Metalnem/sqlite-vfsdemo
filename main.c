#include "sqlite3.h"
#include "stdio.h"

int vfsexclusive_open(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFile, int flags, int *pOutFlags)
{
	sqlite3_vfs *pRoot = (sqlite3_vfs *)pVfs->pAppData;

	if (flags & (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MAIN_DB))
	{
		flags |= SQLITE_OPEN_EXCLUSIVE;
	}

	return pRoot->xOpen(pRoot, zName, pFile, flags, pOutFlags);
}

int vfsexclusive_register()
{
	sqlite3_vfs *pRoot;
	sqlite3_vfs *pNew;

	if ((pRoot = sqlite3_vfs_find(NULL)) == 0)
	{
		return SQLITE_NOTFOUND;
	}

	if ((pNew = sqlite3_malloc(sizeof(*pNew))) == 0)
	{
		return SQLITE_NOMEM;
	}

	*pNew = *pRoot;

	pNew->pAppData = pRoot;
	pNew->xOpen = vfsexclusive_open;

	return sqlite3_vfs_register(pNew, 1);
}

int main()
{
	sqlite3 *pDb;
	sqlite3_vfs *pVfs;
	char *sql, *errmsg;
	int flags, rc;

	if ((rc = vfsexclusive_register()) > 0)
	{
		printf("%s\n", sqlite3_errstr(rc));
		return rc;
	}

	flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE;

	if ((rc = sqlite3_open_v2("test.db", &pDb, flags, NULL)) > 0)
	{
		printf("%s\n", sqlite3_errstr(rc));
		return rc;
	}

	sql = "CREATE TABLE foo(id PRIMARY KEY);";

	if ((rc = sqlite3_exec(pDb, sql, NULL, NULL, &errmsg)) > 0)
	{
		printf("%s\n", errmsg);
		return rc;
	}

	sql = "INSERT INTO foo(id) VALUES('18cff17188b2fe4c9ee82d5f559ef19d');";

	if ((rc = sqlite3_exec(pDb, sql, NULL, NULL, &errmsg)) > 0)
	{
		printf("%s\n", errmsg);
		return rc;
	}

	if ((rc = sqlite3_close_v2(pDb)) > 0)
	{
		printf("%s\n", sqlite3_errstr(rc));
		return rc;
	}

	return 0;
}
