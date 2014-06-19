/*
Copyright (C) 2008- The University of Notre Dame
This software is distributed under the GNU General Public License.
See the file COPYING for details.
*/

#include "work_queue.h"

#include "cctools.h"
#include "debug.h"
#include "stringtools.h"
#include "xxmalloc.h"
#include "itable.h"
#include "list.h"
#include "get_line.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int submit_tasks(struct work_queue *q, int input_size, int run_time, int output_size, int count )
{
	static int ntasks=0;
	char output_file[128];
	char command[256];
	char gen_input_cmd[256];

	sprintf(gen_input_cmd, "dd if=/dev/zero of=input.0 bs=1M count=%d",input_size);
	system(gen_input_cmd);

	int i;
	for(i=0;i<count;i++) {
		sprintf(output_file, "output.%d",ntasks++);
		sprintf(command, "dd if=/dev/zero of=outfile bs=1M count=%d; sleep %d", output_size, run_time );

		struct work_queue_task *t = work_queue_task_create(command);
		work_queue_task_specify_file(t, "input.0", "infile", WORK_QUEUE_INPUT, WORK_QUEUE_CACHE);
		work_queue_task_specify_file(t, output_file, "outfile", WORK_QUEUE_OUTPUT, WORK_QUEUE_NOCACHE);
		work_queue_task_specify_cores(t,1);
		work_queue_submit(q, t);
	}

	return 1;
}

void wait_for_all_tasks( struct work_queue *q )
{
	struct work_queue_task *t;
	while(!work_queue_empty(q)) {
		t = work_queue_wait(q,5);
		if(t) work_queue_task_delete(t);
	}
}

void work_queue_mainloop( const char *q )
{
	char line[1024];

	int sleep_time, run_time, input_size, output_size, count;

	while(1) {
		printf("work_queue_test > ");
		fflush(stdout);

		if(!fgets(line,sizeof(line),stdin)) break;

		if(line[0]=='#') continue;

		string_chomp(line);

		if(sscanf(line,"sleep %d",&sleep_time)==1) {
			printf("sleeping %d seconds...\n",sleep_time);
			sleep(sleep_time);
		} else if(!strcmp(line,"wait")) {
			printf("waiting for all tasks...\n");
			wait_for_all_tasks(q);
		} else if(sscanf(line, "submit %d %d %d %d",&input_size, &run_time, &output_size, &count)==4) {
			printf("submitting %d tasks...\n",count);
			submit_tasks(q,input_size,run_time,output_size,count);
		} else if(!strcmp(line,"quit") || !strcmp(line,"exit")) {
			break;
		} else if(!strcmp(line,"help")) {
			printf("Available commands are:\n");
			printf("sleep <n>               Sleep for n seconds.\n");
			printf("wait                    Wait for all submitted tasks to finish.\n");
			printf("submit <I> <T> <O> <N>  Submit N tasks that read I MB input,\n");
			printf("                        run for T seconds, and produce O MB of output.\n");
			printf("quit, exit              Wait for all tasks to complete, then exit.\n");
			printf("\n");
		} else {
			fprintf(stderr,"ignoring badly formatted line: %s\n",line);			continue;
		}
	}

	work_queue_delete(q);

	return 0;
}

/* vim: set noexpandtab tabstop=4: */
