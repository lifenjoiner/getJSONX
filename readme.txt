getJSONX

A simple JSON querier.

Usage: getJSONX <operations> [json-file]
operation examples:
    .a.b.c[0].d
    a.b.c.length()
    .
examples:
    type test.json | getJSONX .
    getJSONX . < test.json

Home:
https://github.com/lifenjoiner/getJSONX
