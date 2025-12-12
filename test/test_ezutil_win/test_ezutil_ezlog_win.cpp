#include <stdio.h>
#include <stdlib.h>
#include "../../src/ez_log.h"

 
int main(int argc, char *argv[])
{
	int i;
	char *fname = argc>1?argv[1]:"test_ezutil_ezlog_nameofabc.log";

	ezlog_structure(fname, /*"def.log"*/2, 1024*1024*2);
	printf("ezlog_structure:[%s]\n", fname);

	for (i=0;i<1000000;i++)
	{
		ez_log("This is a Log %04d from FILE:%s LINE:%d\n", i, __FILE__, __LINE__);
	}

	ezlog_destructor();

   
   return 0;
}
