PHP_ARG_ENABLE(bartlby,whether to enable bartlby,
[  --enable-bartlby            Enable bartlby])

if test "$PHP_BARTLBY" != "no"; then
  PHP_EXTENSION(bartlby, bartlby.c, $ext_shared)
fi