/* spawnprocess.c
 *
 * This program will spawn child process of any binary file, for each file 
 * in config_path provided.  It will look for sigchld and respawn as needed.
 * It will also look for sighup and re-read the directory as needed.
 *
 */

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "childinfo.h"
#include <stdio.h>
#include <string.h>

char binary[50];         // binary file name 
char binary_path[128];   // binary file path for executing the binary file
char config_path[128];   // config file path for executing the binary file
char options[128];       // options for executing the binary file, if any

void usage() 
{
    printf("usage: spawnprocess --binary <binary>  --binary-path <binary_path>  --config-path <config_path>  --options <opt1 opt2...>\n");
}

void hupchild(pid_t child){
	kill(child, SIGTERM);
}


// This runs as the child process for given binary file
void spawnchild(char *file){
        char program[512];

        if (!strstr (file,".conf")) {
                fprintf (stderr,"skipping %s, not a proper configuration file!\n",file);
                return;
        }
        fprintf (stderr,"Starting a child process with %s\n",file);
        close(0);
        close(1);
        close(2);

       	sprintf(program, "sudo %s/%s",binary_path, binary); 
	
        execl(program, binary, options, file, "--syslog", file, (char *)NULL);
}

int main(int argc, char **argv){
	int i;
	int optlen = 0;

  
        if (argc >= 7) {
		if(strlen(argv[2]) >= sizeof(binary)){
			fprintf(stderr, "Error:cannot accept binary name greater than %ld bytes\n", (unsigned long) (sizeof(binary)));
			usage();
			return (-1);	
		} else {
		    strncpy(binary, argv[2], strlen(argv[2]));	
		}

                if(strlen(argv[4]) >= sizeof(binary_path)){
                        fprintf(stderr, "Error:cannot accept binary path greater than %ld bytes\n", (unsigned long) (sizeof(binary_path)));
                        usage();
                        return (-1);
                } else {
                    strncpy(binary_path, argv[4], strlen(argv[4]));
                }

                if(strlen(argv[6]) >= sizeof(config_path)){   
                        fprintf(stderr, "Error:cannot accept config path greater than %ld bytes\n", (unsigned long) (sizeof(config_path)));
                        usage();
                        return (-1);
                } else {
                    strncpy(config_path, argv[6], strlen(argv[6]));
                }
		
		if(argc > 7){
			for(i=8; i<argc; i++){
				printf("argv[%d] => %s\n",i,argv[i]);
				optlen += strlen(argv[i]);
			}
   			printf("optlen => %d\n", optlen);

			if(optlen >= sizeof(options)){ 
				fprintf(stderr, "Error:cannot accept options greater than %ld bytes\n", (unsigned long) (sizeof(options)));
				usage();
				return (-1);	
			} else {
			    for(i=8; i<argc; i++){
				strcat(options, " ");
				strcat(options, argv[i]);
			    }	
  			}
		} else {
		  fprintf(stderr, "Info: no option provided for executing the binary file\n"); 
		}
        } else {
	    usage();
	    return (-1);		
        }
	
	childinfo_init(config_path, &spawnchild, &hupchild);
        while (1){
        	childinfo_sync();
        	childinfo_spawn();
	        sleep(1);
       }	     
}
