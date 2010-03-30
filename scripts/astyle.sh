#!/bin/sh

astyle --indent=spaces=4 --brackets=linux --indent-labels --pad-oper --pad-header --add-brackets \
	--one-line=keep-statements --convert-tabs --indent-preprocessor --pad-header \
	--pad-oper --break-blocks `find -type f -name '*.cpp'` `find -type f -name '*.c'` `find -type f -name '*.h'`
