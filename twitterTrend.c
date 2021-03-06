#include "queue.c"
#include "hashmap.c"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#define MAX_PATH_SIZE 100


sem_t mutex;
int counter; /* shared variable */

void readDB()
{
    FILE *fp = fopen("TwitterDB.txt", "r");
    const char s[2] = ",";
    char *token;
    int i;
    if(fp != NULL)
    {
        char line[50];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            token = strtok(line, s);
            char name[20] = "";
            strcpy(name, token);
            char def[100] = "";
            while(token != NULL) {
                token = strtok(NULL,s);
                if(token != NULL) {
                    strcat(def, token);
                    strcat(def, ",");
                }
            }
            def[strlen(def)-1] = 0;
            install(name, def);
        }
        fclose(fp);
    } else {
        perror("TwitterDB.txt");
    }   
}

void push_queue(char inputfile[]) {
    FILE *fp = fopen(inputfile, "r");
    char *token;
    if(fp != NULL)
    {
        char line[50];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            if (line[0] != '\n') {
            
                char *client = (char*)malloc(sizeof(char)*10);
                strcpy(client, line);
                Enqueue(client);
            }
            
        }
        fclose(fp);
    } else {
        perror("client3.in");
    }
}

char* readFile(char* name) {
    FILE *fp = fopen(name, "r");
    char *result = (char*)malloc(100);
    if(fp != NULL)
    {
        char line[50];
        while(fgets(line, sizeof line, fp) != NULL)
        {
            if (line[0] != '\n') {
            
                strcpy(result, line);
            }
            
        }
        fclose(fp);
    } else {
        perror("no such file");
    }
    return result;
}

char* writeFile(char* output, char* content) {
    FILE *f = fopen(output, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    
    /* print some text */
    fprintf(f, "%s", content);
    
    fclose(f);
}

void* func ( void * ptr )
{
    int x = ptr;
    
    while(front != NULL) {
        sem_wait(&mutex);       //Down semaphore
    //begin critical sectin
        char client[20] = ""; 
        strcpy(client, Front());
        client[strlen(client)-1] = 0;
        Dequeue();
        sem_post(&mutex);       // up semaphore 
        printf("Thread %d is handling client %s \n", x, client);
        char city[20] = "";
        strcpy(city, readFile(client));
        city[strlen(city)-1] = 0;
        struct nlist* result = lookup(city);
        char content[50] = "";
        strcpy(content, city);
        strcat(content, " : ");
        strcat(content, result->defn);
        char output[20] = "";
        strcpy(output, client);
        strcat(output, ".result");
        writeFile(output, content);
        printf("Thread %d finished handling client %s \n", x, client);
    //end critical section

    
    }
    pthread_exit(0); /* exit thread */
}

void run(int num_thread) {
    int n; 
    pthread_t *threads;
    
    sem_init(&mutex, 0, 1);      /* initialize mutex to 1 - binary semaphore */
                                 /* second param = 0 - semaphore is local */
    n = num_thread;                         
	/*if (argc != 2)
	{
		printf("Usage: %s n\n  where n is no. of thread\n", argv[0]);
		exit(1);
	}*/
	
	//n = atoi(argv[1]);
	
	threads = (pthread_t *) malloc(n * sizeof(*threads));

	/* Start up thread */
	int i = 0;
	for (i = 0; i < n; i++)
	{	
		pthread_create(&threads[i], NULL, func, i+1);
	}

	/* Synchronize the completion of each thread. */

	for (i = 0; i < n; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	
    sem_destroy(&mutex); /* destroy semaphore */
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf ("Usage: intput_file_path num_threads");     //n = atoi(argv[1]);
        printf ("Usage: intput_file_path num_threads");
        return 1;
    }

    char *cwd;
    cwd = getcwd(0, 0);
    if(!cwd) {
        fprintf(stderr, "getcwd falied: %s \n", strerror (errno));
    }

    char inputfile[MAX_PATH_SIZE];

    strcpy(inputfile, cwd);
    strcat(inputfile, "/");
    strcat(inputfile, argv[1]);

    int num_thread = atoi(argv[2]);
    //Inti semaphore with num_thread items
    //sem_init(&mutex, 0, 1);
    //sem_init(&empty, 0, num_thread);
    //sem_init(&full, 0, 0);
    readDB();
    push_queue(inputfile);
       
    run(num_thread);
    
   // char name1[] = "Minneapolis";
   // char name2[] = "Paris";
   // char name3[] = "London";
   // struct nlist* result = lookup(name1);
   // struct nlist* result2 = lookup(name2);
   // struct nlist* result3 = lookup(name3);
   // printf("%s", result->defn);
   // printf("%s", result2->defn);
   // printf("%s", result3->defn);
}
