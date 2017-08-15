# SQLite VFS demo [![Build Status](https://travis-ci.org/Metalnem/sqlite-vfsdemo.svg?branch=master)](https://travis-ci.org/Metalnem/sqlite-vfsdemo) [![license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://raw.githubusercontent.com/metalnem/sqlite-vfsdemo/master/LICENSE)

VFS implementation for SQLite that creates the database file if it
doesn't exist, and fails otherwise. It does that by forcing the
**SQLITE_OPEN_EXCLUSIVE** flag, which is normally stripped by
SQLite for some reason.
