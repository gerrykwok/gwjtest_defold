\#include "${out_file}.hpp"
#if $macro_judgement
$macro_judgement
#end if 
#for header in $headers
\#include "$header"
#end for
\#include "external_include.h"
