pushd %~dp0
make --eval=CC=tcc --eval=CFLAGS+=-I..\jsmn
popd
